#include "e_enum.h"
#include "../lib/mr_utils/mrs_misc.h"
#include "f_file.h"
#include <stdlib.h>
#include <string.h>

void E_enum_info_destroy(struct E_EnumInfo *enum_info)
{
	MRS_free(enum_info->filename);
	MRS_free(enum_info->name);
	MRS_free(enum_info->typedef_name);
	free(enum_info->filename);
	free(enum_info->name);
	free(enum_info->typedef_name);

	for (size_t i = 0; i < enum_info->enum_values_len; i++) {
		MRS_free(enum_info->enum_values[i].value);
		free(enum_info->enum_values[i].value);
	}
}

internal void E_seek_to_end_of_enum(MRS_String *file_contents,
				    size_t *enum_position)
{
	for (size_t i = *enum_position; i < file_contents->len; i++) {
		if (MRS_get_char(file_contents, i) ==
		    *tokens_to_str[F_CTOKENS_SEMI_COLON]) {
			return;
		}
		*enum_position += 1;
	}
}

internal void E_enum_values_init(MRS_String *file_contents,
				 char *enum_value_start_position,
				 char *close_curly, struct E_EnumInfo *dest)
{
	size_t enum_variable_len = 0;
	for (char *j = enum_value_start_position; j <= close_curly; j++) {
		if (F_is_valid_variable_char(*j) == 0) {
			enum_variable_len++;
		} else {
			MRS_String *enum_value = malloc(sizeof(*enum_value));
			MRS_init(enum_variable_len, enum_value_start_position,
				 enum_variable_len, enum_value);
			dest->enum_values[dest->enum_values_len].value =
				enum_value;

			size_t idx;
			MRS_get_idx(file_contents, j, &idx);
			dest->enum_values[dest->enum_values_len].line_number =
				F_get_line_number(file_contents, idx);
			dest->enum_values_len++;
			return;
		}
	}
}

internal void E_enum_info_add_enum_values(MRS_String *file_contents,
					  size_t enum_idx,
					  struct E_EnumInfo *dest)
{
	MRS_String comma;
	MRS_init(0, tokens_to_str[F_CTOKENS_COMMA], 1, &comma);

	MRS_String open_curly_mrs;
	MRS_init(0, tokens_to_str[F_CTOKENS_OPEN_CURLY], 1, &open_curly_mrs);
	char *open_curly = MRS_strstr(file_contents, &open_curly_mrs, enum_idx);

	MRS_String close_curly_mrs;
	MRS_init(0, tokens_to_str[F_CTOKENS_CLOSE_CURLY], 1, &close_curly_mrs);
	char *close_curly =
		MRS_strstr(file_contents, &close_curly_mrs, enum_idx);

	MRS_free(&open_curly_mrs);
	MRS_free(&close_curly_mrs);

	for (char *i = open_curly; i < close_curly; i++) {
		if (F_is_valid_variable_char(*i) == 0) {
			E_enum_values_init(file_contents, i, close_curly, dest);

			size_t current_position;
			MRS_get_idx(file_contents, i, &current_position);
			i = MRS_strstr(file_contents, &comma, current_position);
			if (i == NULL) {
				MRS_free(&comma);
				return;
			}
		}
	}
	MRS_free(&comma);
}

internal void E_enum_info_init(MRS_String *file_contents, MRS_String *filename,
			       size_t enum_idx, struct E_EnumInfo *dest)
{
	MRS_String *name =
		F_get_name(file_contents, enum_idx, F_CKEYWORDS_ENUM);
	MRS_String *typedef_name = F_get_typedef_name(file_contents, enum_idx);

	dest->name = name;

	dest->typedef_name = typedef_name;

	dest->filename = malloc(sizeof(*dest->filename));
	MRS_strndup(filename, filename->len, dest->filename);

	dest->line_number = F_get_line_number(file_contents, enum_idx);

	E_enum_info_add_enum_values(file_contents, enum_idx, dest);
}

void E_get_enums(MRS_String *file_contents, MRS_String *filename,
		 struct E_EnumInfo enums_info[MAX_ENUM_VALUES_COUNT],
		 size_t *enums_len)
{
	memset(enums_info, 0,
	       sizeof(struct E_EnumInfo) * MAX_ENUM_VALUES_COUNT);
	*enums_len = 0;

	size_t character_position = 0;
	while (character_position < file_contents->len) {
		int found = F_get_next_keyword_idx(file_contents,
						   character_position,
						   F_CKEYWORDS_ENUM,
						   &character_position);
		if (found == -1) {
			return;
		}

		if (F_check_name_not_used_in_init(file_contents,
						  character_position) == 0) {
			E_enum_info_init(file_contents, filename,
					 character_position,
					 &enums_info[*enums_len]);
			*enums_len += 1;
		}

		E_seek_to_end_of_enum(file_contents, &character_position);
	}
	return;
}
