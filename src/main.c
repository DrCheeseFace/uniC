#include "util.h"

int main(void)
{
	const char *example_file_name = "12345678_renderer.c";

	char file_prefix[MAX_FILE_PREFIX_LENGTH] = "";
	int len = get_file_prefix(example_file_name, file_prefix);

	(void)len;

	return 0;
}
