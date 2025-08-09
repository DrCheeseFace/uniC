#include "util.h"
#include <ctype.h>
#include <string.h>

int get_file_prefix(const char *file_name,
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
