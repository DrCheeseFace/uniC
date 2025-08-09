#ifndef MRT_STRINGS_H
#define MRT_STRINGS_H
#include <stddef.h>
#define MAX_STRING_LENGTH 100

typedef struct {
	char *value;
	size_t len;
	size_t capacity;
} MRT_String;

MRT_String *MRT_string_create(size_t capacity);

void MRT_string_free(MRT_String *string);

/*
 *
 * /returns 0 if successful
 */
int MRT_string_strncpy(MRT_String *string, const char *src, size_t len);

void MRT_string_filter(MRT_String *string, const char remove_me);

#endif // !MRT_STRINGS_H
