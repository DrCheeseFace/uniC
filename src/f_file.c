#include "f_file.h"
#include <ctype.h>
#include <stdio.h>

const char *keywords_to_str[F_CKEYWORDS_COUNT] = {
	"typedef",
	"struct",
	"enum",
};

const char *tokens_to_str[F_CTOKENS_COUNT] = {
	"{", "}", ";", "\n", "\t", " ",
};

MRS_String *F_get_file_prefix(MRS_String *file_name)
{
	int file_prefix_len = 0;

	char *file_prefix_delimiter =
		MRS_strchr(file_name, FILE_PREFIX_DELIMITER);
	if (file_prefix_delimiter == NULL) {
		return NULL;
	}

	file_prefix_len = file_prefix_delimiter - file_name->value;

	if (file_prefix_len > MAX_FILE_PREFIX_LENGTH) {
		return NULL;
	}

	for (int i = 0; i < file_prefix_len; i++) {
		if (!isalpha(file_name->value[i]) ||
		    isupper(file_name->value[i])) {
			return NULL;
		}
	}

	return MRS_strndup(file_name, file_prefix_len);
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
	MRS_setstrn(contents, buffer, file_size);

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
	MRS_String *keyword_str = MRS_init(0, keywords_to_str[keyword]);

	while (start_position < file_contents->len) {
		char *x =
			MRS_strstr(file_contents, keyword_str, start_position);
		if (x == NULL) {
			MRS_free(keyword_str);
			return -1;
		}

		if (F_keyword_validate_surrounding(file_contents, x, keyword) !=
		    0) {
			start_position += strlen(keywords_to_str[keyword]);
			continue;
		}

		MRS_free(keyword_str);
		int found = MRS_get_idx(file_contents, x, found_position);
		if (found == -1) {
			return -1;
		}
		return 0;
	}

	MRS_free(keyword_str);
	return -1;
}

MRS_String *F_get_struct_name(MRS_String *file_contents,
			      size_t struct_start_position)
{
	MRS_String *open_curly =
		MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY]);

	char *next_open_curly_position =
		MRS_strstr(file_contents, open_curly, struct_start_position);

	MRS_String *name = MRS_create(MAX_STRUCT_NAME_LENGTH);
	const char *start_of_name =
		&file_contents->value[struct_start_position] +
		strlen(keywords_to_str[F_CKEYWORDS_STRUCT]);

	size_t name_length = next_open_curly_position - start_of_name;

	MRS_setstrn(name, start_of_name, name_length);
	MRS_remove_whitespace(name);
	MRS_free(open_curly);

	if (name->len == 0) {
		MRS_free(name);
		return NULL;
	}
	return name;
}

MRS_String *F_get_struct_typedef_name(MRS_String *file_contents,
				      size_t struct_start_position)
{
	(void)file_contents;
	(void)struct_start_position;
	MRS_String *name = MRS_init(MAX_STRUCT_NAME_LENGTH, "TODO");
	return name;
}

size_t *F_seek_to_next_outer_level_position(MRS_String *file_contents,
					    size_t current_position)
{
	(void)file_contents;
	(void)current_position;
	return 0;
}

// TODO THARUN makesure to ONLY get structs in the top level
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
			//TODO do smthin herre
			return;
		}

		MRS_String *name =
			F_get_struct_name(file_contents, character_position);

		character_position++;

		/*F_get_struct_typedef_name(file_contents,*/
		/*			  character_position); // TODO*/

		/*F_forward_to_next_outer_level() // TODO*/

		if (name == NULL) {
			continue;
		}

		struct_names[*struct_names_len] = name;
		*struct_names_len += 1;
	}
	return;
}

/*
* struct MyStruct {};
* typedef struct {
* } MyTypedefStruct;
* 
* typedef struct AnotherStruct {
* } AnotherTypedefStruct;
* 
* enum MyEnum { VALUE0 };
* typedef enum { VALUE1 } MyTypedefEnum;
* typedef enum AnotherEnum { VALUE2 } AnotherTypedefEnum;
*/
