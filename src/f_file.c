#include "f_file.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

const char *keywords_to_str[F_CKEYWORDS_COUNT] = {
	"typedef",
	"struct",
	"enum",
};

const char *tokens_to_str[F_CTOKENS_COUNT] = {
	"{", "}", ";", "\n", "\t", " ",
};

int F_get_file_prefix(MRS_String *file_name, MRS_String *dest)
{
	int file_prefix_len = 0;

	char *file_prefix_delimiter =
		MRS_strchr(file_name, FILE_PREFIX_DELIMITER);
	if (file_prefix_delimiter == NULL) {
		return -1;
	}

	file_prefix_len = file_prefix_delimiter - file_name->value;

	if (file_prefix_len > MAX_FILE_PREFIX_LENGTH) {
		return -1;
	}

	for (int i = 0; i < file_prefix_len; i++) {
		if (!isalpha(file_name->value[i]) ||
		    isupper(file_name->value[i])) {
			dest = NULL;
			return -1;
		}
	}

	MRS_strndup(file_name, file_prefix_len, dest);
	return 0;
}

MRS_String *F_get_file_contents(const char *file_name)
{
	FILE *file = fopen(file_name, "rb");
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char buffer[file_size];
	fread(buffer, 1, file_size, file);
	fclose(file);

	MRS_String *contents = MRS_create(file_size);
	MRS_setstrn(contents, buffer, file_size, file_size);

	return contents;
}

// returns 0 if valid surroundings
int F_keyword_validate_surrounding(MRS_String *file_contents,
				   char *struct_start_position,
				   F_CKeywords keyword)
{
	// check character immediatly after struct for no-whitespace no-{
	int after_idx = struct_start_position - file_contents->value +
			strlen(keywords_to_str[keyword]);
	if (MRS_is_whitespace(file_contents, after_idx) != 0 &&
	    MRS_get_char(file_contents, after_idx) !=
		    *tokens_to_str[F_CTOKENS_OPEN_CURLY]) {
		return 1;
	}

	// check character immediatly before struct for no-whitespace no-} no-;
	if (struct_start_position != file_contents->value) {
		size_t before_idx = 0;
		MRS_get_idx(file_contents, struct_start_position - 1,
			    &before_idx);

		if (MRS_is_whitespace(file_contents, before_idx) != 0 &&
		    MRS_get_char(file_contents, before_idx) !=
			    *tokens_to_str[F_CTOKENS_CLOSE_CURLY] &&
		    MRS_get_char(file_contents, before_idx) !=
			    *tokens_to_str[F_CTOKENS_SEMI_COLON]) {
			return 1;
		}
	}
	return 0;
}

// returns 0 if found
int F_get_next_keyword_idx(MRS_String *file_contents, size_t start_position,
			   F_CKeywords keyword, size_t *found_position)
{
	MRS_String keyword_str;
	MRS_init(0, keywords_to_str[keyword], strlen(keywords_to_str[keyword]),
		 &keyword_str);

	while (start_position < file_contents->len) {
		char *x =
			MRS_strstr(file_contents, &keyword_str, start_position);
		if (x == NULL) {
			MRS_free(&keyword_str);
			return -1;
		}

		if (F_keyword_validate_surrounding(file_contents, x, keyword) !=
		    0) {
			start_position += strlen(keywords_to_str[keyword]);
			continue;
		}

		MRS_free(&keyword_str);
		return MRS_get_idx(file_contents, x, found_position);
	}

	MRS_free(&keyword_str);
	return -1;
}

MRS_String *F_get_struct_name(MRS_String *file_contents,
			      size_t struct_start_position)
{
	MRS_String open_curly;
	MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY],
		 strlen(tokens_to_str[F_CTOKENS_OPEN_CURLY]), &open_curly);

	char *next_open_curly_position =
		MRS_strstr(file_contents, &open_curly, struct_start_position);

	MRS_String *name = MRS_create(MAX_STRUCT_NAME_LENGTH);
	const char *start_of_name =
		&file_contents->value[struct_start_position] +
		strlen(keywords_to_str[F_CKEYWORDS_STRUCT]);

	size_t name_length = next_open_curly_position - start_of_name;

	MRS_setstrn(name, start_of_name, name_length, name_length);
	MRS_remove_whitespace(name);
	MRS_free(&open_curly);

	if (name->len == 0) {
		MRS_free(name);
		free(name);
		return NULL;
	}
	return name;
}

MRS_String *F_get_struct_typedef_name(MRS_String *file_contents,
				      size_t struct_start_position)
{
	MRS_String open_curly;
	MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY],
		 strlen(tokens_to_str[F_CTOKENS_OPEN_CURLY]), &open_curly);

	MRS_String semi_colon;
	MRS_init(0, tokens_to_str[F_CTOKENS_SEMI_COLON],
		 strlen(tokens_to_str[F_CTOKENS_SEMI_COLON]), &semi_colon);

	char *struct_open_curly_ptr =
		MRS_strstr(file_contents, &open_curly, struct_start_position);

	size_t search_position =
		struct_open_curly_ptr - file_contents->value + 1;

	int bracket_stack = 1;
	size_t struct_end_bracket_position = 0;
	for (size_t i = search_position; i < file_contents->len; i++) {
		char current_char = MRS_get_char(file_contents, i);
		if (current_char == *tokens_to_str[F_CTOKENS_OPEN_CURLY]) {
			bracket_stack++;
		} else if (current_char ==
			   *tokens_to_str[F_CTOKENS_CLOSE_CURLY]) {
			bracket_stack--;
			if (bracket_stack == 0) {
				struct_end_bracket_position = i;
				break;
			}
		}
	}
	char *struct_semi_colon_ptr = MRS_strstr(file_contents, &semi_colon,
						 struct_end_bracket_position);

	size_t struct_semi_colon_position = 0;
	MRS_get_idx(file_contents, struct_semi_colon_ptr,
		    &struct_semi_colon_position);
	size_t typedef_name_len =
		struct_semi_colon_position - struct_end_bracket_position - 1;

	MRS_String *name = MRS_create(MAX_STRUCT_NAME_LENGTH);

	MRS_setstrn(name,
		    &file_contents->value[struct_end_bracket_position + 1],
		    file_contents->len, typedef_name_len);
	MRS_remove_whitespace(name);

	MRS_free(&semi_colon);
	MRS_free(&open_curly);

	if (name->len == 0) {
		MRS_free(name);
		free(name);
		return NULL;
	}

	return name;
}

void F_seek_to_end_of_struct(MRS_String *file_contents,
			     size_t *current_position)
{
	MRS_String open_curly;
	MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY],
		 strlen(tokens_to_str[F_CTOKENS_OPEN_CURLY]), &open_curly);

	char *struct_open_curly_ptr =
		MRS_strstr(file_contents, &open_curly, *current_position);
	MRS_free(&open_curly);

	size_t open_curly_position = 0;
	MRS_get_idx(file_contents, struct_open_curly_ptr, &open_curly_position);
	size_t bracket_stack = 1;

	for (size_t i = open_curly_position + 1; i < file_contents->len; i++) {
		char current_char = MRS_get_char(file_contents, i);
		if (current_char == *tokens_to_str[F_CTOKENS_OPEN_CURLY]) {
			bracket_stack++;
		} else if (current_char ==
			   *tokens_to_str[F_CTOKENS_CLOSE_CURLY]) {
			bracket_stack--;
			if (bracket_stack == 0) {
				*current_position = i;
				return;
			}
		}
	}
}

void F_get_structs(MRS_String *file_contents,
		   MRS_String *struct_names[MAX_STRUCT_NAME_COUNT],
		   size_t *struct_names_len)
{
	memset(struct_names, 0, sizeof(*struct_names));
	*struct_names_len = 0;

	size_t character_position = 0;
	while (character_position < file_contents->len) {
		int found = F_get_next_keyword_idx(file_contents,
						   character_position,
						   F_CKEYWORDS_STRUCT,
						   &character_position);
		if (found == -1) {
			return;
		}

		MRS_String *name =
			F_get_struct_name(file_contents, character_position);
		if (name != NULL) {
			struct_names[*struct_names_len] = name;
			*struct_names_len += 1;
		}

		MRS_String *typedef_name = F_get_struct_typedef_name(
			file_contents, character_position);
		if (typedef_name != NULL) {
			struct_names[*struct_names_len] = typedef_name;
			*struct_names_len += 1;
		}
		F_seek_to_end_of_struct(file_contents, &character_position);
	}
	return;
}
