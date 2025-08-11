#ifndef F_FILE_H
#define F_FILE_H

#include "../lib/mr_utils/mrs_strings.h"

#define MAX_FILE_PREFIX_LENGTH 5

#define FILE_PREFIX_DELIMITER '_'

#define MAX_STRUCT_NAME_LENGTH 30

#define MAX_STRUCT_NAME_COUNT 50

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
	F_CTOKENS_NEWLINE,
	F_CTOKENS_TAB,
	F_CTOKENS_SPACE,
	F_CTOKENS_COUNT,
} F_CTokens;

/*
 * `returns` NULL if prefix length found is larger than `MAX_FILE_PREFIX_LENGTH`
 * `returns` NULL if prefix contains `non-alpha`
 * `returns` NULL if prefix contains `uppercase`
 *
 * `returns` NULL if not found 
 */
MRS_String *F_get_file_prefix(MRS_String *file_name);

/*
 * dont forget to call free on the returned value
 *
 * `returns` file contents as `MRS_String`
 */
MRS_String *F_get_file_contents(const char *file_name);

void F_get_structs(MRS_String *file_contents,
		   MRS_String *struct_names[MAX_STRUCT_NAME_COUNT],
		   size_t *struct_names_len);

#endif // !F_FILE_H
