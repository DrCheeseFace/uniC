#ifndef F_FILE_H

#define F_FILE_H

#include "mrs_strings.h"
#include <stddef.h>
#define MAX_FILE_PREFIX_LENGTH 5
#define FILE_PREFIX_DELIMITER '_'
#define MAX_STRUCT_NAME_LENGTH 30

typedef enum {
	F_CKEYWORDS_TYPEDEF,
	F_CKEYWORDS_STRUCT,
	F_CKEYWORDS_ENUM,
	F_CKEYWORDS_COUNT,
} F_CKeywords;

typedef enum {
	F_CTOKENS_OPEN_CURLY,
	F_CTOKENS_CLOSE_CURLY,
	F_CTOKENS_COUNT,
} F_CTokens;

/*
 * \ if prefix length found is larger than MAX_FILE_PREFIX_LENGTH, returns not found (0)
 * \ if prefix contains non-alpha returns not found
 * \ if prefix contains uppercase returns not found
 *
 * \ returns length of file prefix
 */
int F_get_file_prefix(const char *src, char dest[MAX_FILE_PREFIX_LENGTH]);

/*
 * \ dont forget to call free on the returned value
 *
 * \ returns file contents in char array
 */
MRS_String *F_get_file_contents(const char *file_name);

void F_get_struct_names(MRS_String *file_contents);

#endif // !F_FILE_H
