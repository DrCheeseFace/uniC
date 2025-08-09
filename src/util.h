
#ifndef UTIL_H

#define UTIL_H

#define MAX_FILE_PREFIX_LENGTH 5
#define FILE_PREFIX_DELIMITER '_'

/*
 * if prefix length found is larger than MAX_FILE_PREFIX_LENGTH, returns not found (0)
 *
 * /returns length of file prefix
 */
int get_file_prefix(const char *src, char dest[MAX_FILE_PREFIX_LENGTH]);

#endif // !UTIL_H
