#include "f_file.h"
#include "../lib/mr_utils/mrs_misc.h"
#include "s_struct.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *keywords_to_str[F_CKEYWORDS_COUNT] = {
	"typedef",
	"struct",
	"enum",
};

const char *tokens_to_str[F_CTOKENS_COUNT] = { "{",  "}", ";", "=", "\n",
					       "\t", " ", ",", "'" };

const char valid_variable_name_chars[VALID_VARIABLE_NAME_CHARS_COUNT] = {
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c',
	'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
	'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C',
	'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_',
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
internal int F_keyword_validate_surrounding(MRS_String *file_contents,
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

int F_is_valid_variable_char(char c)
{
	for (size_t i = 0; i < VALID_VARIABLE_NAME_CHARS_COUNT; i++) {
		if (c == valid_variable_name_chars[i]) {
			return 0;
		}
	}
	return 1;
}

size_t F_get_line_number(MRS_String *file_contents, size_t idx)
{
	size_t line_count = 1;
	for (size_t i = 0; i < idx; i++) {
		if (MRS_get_char(file_contents, i) ==
		    *tokens_to_str[F_CTOKENS_NEWLINE]) {
			line_count++;
		}
	}

	return line_count;
}

// TODO THARUN handle '}' being an enum value
MRS_String *F_get_typedef_name(MRS_String *file_contents,
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
		char next_char = MRS_get_char(file_contents, i + 1);
		char prev_char = MRS_get_char(file_contents, i - 1);
		if (current_char == *tokens_to_str[F_CTOKENS_OPEN_CURLY]) {
			bracket_stack++;
		} else if (current_char ==
				   *tokens_to_str[F_CTOKENS_CLOSE_CURLY] &&
			   !(next_char ==
				     *tokens_to_str[F_CTOKENS_SINGLE_QUOTE] &&
			     prev_char ==
				     *tokens_to_str[F_CTOKENS_SINGLE_QUOTE])) {
			bracket_stack--;
			if (bracket_stack == 0) {
				struct_end_bracket_position = i;
				break;
			}
		}
	}
	char *struct_semi_colon_ptr = MRS_strstr(file_contents, &semi_colon,
						 struct_end_bracket_position);

	MRS_free(&semi_colon);
	MRS_free(&open_curly);

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

	if (name->len == 0) {
		MRS_free(name);
		free(name);
		return NULL;
	}

	MRS_shrink_to_fit(name);
	return name;
}

MRS_String *F_get_name(MRS_String *file_contents, size_t struct_start_position,
		       F_CKeywords keyword)
{
	MRS_String open_curly;
	MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY],
		 strlen(tokens_to_str[F_CTOKENS_OPEN_CURLY]), &open_curly);

	char *next_open_curly_position =
		MRS_strstr(file_contents, &open_curly, struct_start_position);

	MRS_free(&open_curly);

	MRS_String *name = MRS_create(MAX_STRUCT_NAME_LENGTH);
	char *start_of_name = &file_contents->value[struct_start_position] +
			      strlen(keywords_to_str[keyword]);

	size_t name_length = next_open_curly_position - start_of_name;

	MRS_setstrn(name, start_of_name, name_length, name_length);
	MRS_remove_whitespace(name);

	if (name->len == 0) {
		MRS_free(name);
		free(name);
		return NULL;
	}

	MRS_shrink_to_fit(name);
	return name;
}

int F_check_name_not_used_in_init(MRS_String *file_contents,
				  size_t struct_keyword_idx)
{
	int has_bracket = 0;
	for (size_t i = struct_keyword_idx; i < file_contents->len; i++) {
		if (file_contents->value[i] ==
		    *tokens_to_str[F_CTOKENS_SEMI_COLON]) {
			break;
		}
		if (file_contents->value[i] ==
		    *tokens_to_str[F_CTOKENS_OPEN_CURLY]) {
			has_bracket = 1;
		}
	}
	if (!has_bracket) {
		return 1;
	}

	return 0;
}
