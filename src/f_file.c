#include "f_file.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *keywords_to_str[F_CKEYWORDS_COUNT] = {
	"typedef",
	"struct",
	"enum",
};

const char *tokens_to_str[F_CTOKENS_COUNT] = {
	"{", "}", ";", "=", "\n", "\t", " ",
};

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
