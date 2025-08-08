#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_PREFIX_LENGTH 5

int main(void)
{
	const char *file_name = "r_renderer.c";
	char delimiter = '_';
	char file_prefix[MAX_FILE_PREFIX_LENGTH] = "";

	char *file_prefix_delimiter = strchr(file_name, delimiter);
	if (file_prefix_delimiter != NULL) {
		size_t prefix_len = file_prefix_delimiter - file_name;
		strncpy(file_prefix, file_name, prefix_len);
		file_prefix[prefix_len] = '\0';
	}

	return 0;
}
