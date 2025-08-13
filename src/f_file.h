#ifndef F_FILE_H
#define F_FILE_H

#include "../lib/mr_utils/mrs_strings.h"

#define MAX_FILE_PREFIX_LENGTH 5

#define FILE_PREFIX_DELIMITER '_'
#define VALID_VARIABLE_NAME_CHARS_COUNT 37
#define MAX_VARIABLE_NAME_LENGTH 64

typedef enum {
	F_CKEYWORDS_TYPEDEF,
	F_CKEYWORDS_STRUCT,
	F_CKEYWORDS_ENUM,
	F_CKEYWORDS_COUNT,
} F_CKeywords;

typedef enum {
	F_CTOKENS_OPEN_CURLY,
	F_CTOKENS_CLOSE_CURLY,
	F_CTOKENS_SEMI_COLON,
	F_CTOKENS_EQUALS,
	F_CTOKENS_NEWLINE,
	F_CTOKENS_TAB,
	F_CTOKENS_SPACE,
	F_CTOKENS_COUNT,
} F_CTokens;

extern const char *keywords_to_str[F_CKEYWORDS_COUNT];

extern const char *tokens_to_str[F_CTOKENS_COUNT];

extern const char valid_variable_name_chars[VALID_VARIABLE_NAME_CHARS_COUNT];

/*
 * `returns` 0 if successful 
 */
int F_get_file_prefix(MRS_String *file_name, MRS_String *dest);

/*
 * dont forget to call free on the returned value
 *
 * `returns` file contents as `MRS_String`
 */
MRS_String *F_get_file_contents(const char *file_name);

// returns 0 if found
int F_get_next_keyword_idx(MRS_String *file_contents, size_t start_position,
			   F_CKeywords keyword, size_t *found_position);

// returns 0 if valid
int F_is_valid_variable_char(char c);

size_t F_get_line_number(MRS_String *file_contents, size_t idx);

#endif // !F_FILE_H
