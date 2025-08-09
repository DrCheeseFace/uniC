#ifndef MRS_STRINGS_H
#define MRS_STRINGS_H
#include <stddef.h>

#define MRS_MAX_STRING_LENGTH 100

typedef struct {
	char *value;
	size_t len;
	size_t capacity;
} MRS_String;

MRS_String *MRS_string_create(size_t capacity);

void MRS_string_free(MRS_String *string);

/*
 * basically strncpy
 *
 * /returns 0 if successful
 */
int MRS_string_strncpy(MRS_String *string, const char *src, size_t len);

void MRS_string_filter(MRS_String *string, const char remove_me);

#endif // !MRS_STRINGS_H
