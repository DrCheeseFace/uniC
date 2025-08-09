#include "../../src/f_file.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int G_get_file_prefix(const char *file_name,
		      char file_prefix[MAX_FILE_PREFIX_LENGTH])
{
	int file_prefix_len = 0;

	char *file_prefix_delimiter = strchr(file_name, FILE_PREFIX_DELIMITER);
	if (file_prefix_delimiter != NULL) {
		file_prefix_len = file_prefix_delimiter - file_name;

		if (file_prefix_len > MAX_FILE_PREFIX_LENGTH) {
			return 0;
		}

		for (int i = 0; i < file_prefix_len; i++) {
			if (!isalpha(file_name[i]) || isupper(file_name[i])) {
				return 0;
			}
		}

		strncpy(file_prefix, file_name, file_prefix_len);
	}

	return file_prefix_len;
}

void G_get_file_contents(const char *file_name)
{
	FILE *file = fopen(file_name, "rb");
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = malloc(file_size);
	fread(buffer, 1, file_size, file);
	free(buffer);
}
