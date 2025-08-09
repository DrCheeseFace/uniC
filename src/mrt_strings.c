#include "mrt_strings.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

MRT_String *MRT_string_create(size_t capacity)
{
	MRT_String *out = malloc(sizeof(*out));
	out->value = malloc(sizeof(char) * capacity);
	out->capacity = capacity;
	out->len = 0;
	return out;
}

void MRT_string_free(MRT_String *string)
{
	free(string->value);
	free(string);
}

void MRT_string_filter(MRT_String *string, const char remove_me)
{
	char filtered[string->len];
	size_t filtered_len = 0;
	for (size_t i = 0; i < string->len; i++) {
		if (string->value[i] != remove_me) {
			filtered[filtered_len] = string->value[i];
			filtered_len++;
		}
	}
	string->len = filtered_len;
	MRT_string_strncpy(string, filtered, filtered_len);
}

int MRT_string_strncpy(MRT_String *string, const char *src, size_t len)
{
	if (len > string->capacity) {
		return 1;
	}

	for (size_t i = 0; i < len; i++) {
		string->value[i] = src[i];
	}
	string->len = len;

	return 0;
}
