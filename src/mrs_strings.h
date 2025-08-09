#ifndef MRS_STRINGS_H
#define MRS_STRINGS_H
#include <stddef.h>

#define MRS_MAX_STRING_LENGTH 100

typedef struct {
	char *value;
	size_t len;
	size_t capacity;
} MRS_String;

MRS_String *MRS_create(size_t capacity);

/*
 * /returns NULL if failed (double check capacity is enough)
 */
MRS_String *MRS_init(size_t capacity, const char *value);

void MRS_free(MRS_String *string);

/*
 * /returns 0 if successful
 */
int MRS_strcpy(MRS_String *string, const char *src);

/*
 * /returns 0 if successful
 */
int MRS_strncpy(MRS_String *string, const char *src, size_t len);

/*
 * /returns 0 if successful
 */
int MRS_strcmp(MRS_String *a, MRS_String *b);

/*
 * /If haystack_start_idx is NULL, defaults to 0
 *
 * /returns NULL if not found
 */
char *MRS_strstr(MRS_String *haystack, MRS_String *needle,
		 size_t *haystack_start_idx);

void MRS_filter(MRS_String *string, const char remove_me);

#endif // !MRS_STRINGS_H
