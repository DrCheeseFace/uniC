#include "f_file.h"
#include "mrs_strings.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *keywords_to_str[F_CKEYWORDS_COUNT] = {
	"typedef",
	"struct",
	"enum",
};

const char *tokens_to_str[F_CTOKENS_COUNT] = {
	"{",
	"}",
};

int F_get_file_prefix(const char *file_name,
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

MRS_String *F_get_file_contents(const char *file_name)
{
	FILE *file = fopen(file_name, "rb");
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char buffer[file_size];
	fread(buffer, 1, file_size, file);
	fclose(file);

	MRS_String *contents = MRS_create(file_size);
	MRS_strncpy(contents, buffer, file_size);

	return contents;
}

size_t F_get_next_keyword_idx(MRS_String *file_contents, size_t start_position,
			      F_CKeywords keyword)
{
	MRS_String *keyword_str = MRS_init(strlen(keywords_to_str[keyword]),
					   keywords_to_str[keyword]);

	char *x = MRS_strstr(file_contents, keyword_str, &start_position);

	MRS_free(keyword_str);
	return x - file_contents->value;
}

MRS_String *F_get_struct_name(MRS_String *file_contents,
			      size_t struct_start_position)
{
	char *x = strstr(&file_contents->value[struct_start_position],
			 tokens_to_str[F_CTOKENS_OPEN_CURLY]);

	MRS_String *name = MRS_create(MAX_STRUCT_NAME_LENGTH);
	const char *start_of_name =
		&file_contents->value[struct_start_position] +
		strlen(keywords_to_str[F_CKEYWORDS_STRUCT]);

	size_t name_length = x - start_of_name;

	MRS_strncpy(name, start_of_name, name_length);
	MRS_filter(name, ' ');
	return name;
}

// TODO THARUN makesure to ONLY get structs in the top level
void F_get_struct_names(MRS_String *file_contents)
{
	size_t character_position = 0;
	character_position = F_get_next_keyword_idx(
		file_contents, character_position, F_CKEYWORDS_STRUCT);

	MRS_String *name = F_get_struct_name(file_contents, character_position);
	MRS_free(name);
	(void)name;

	if (character_position == 0) { // not found?
		return;
	}
}

/*
* struct MyStruct {};
* typedef struct {
* } MyTypedefStruct;
* 
* typedef struct AnotherStruct {
* } AnotherTypedefStruct;
* 
* enum MyEnum { VALUE0 };
* typedef enum { VALUE1 } MyTypedefEnum;
* typedef enum AnotherEnum { VALUE2 } AnotherTypedefEnum;
*/
