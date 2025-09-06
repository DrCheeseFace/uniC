#ifndef E_ENUM_H
#define E_ENUM_H

#include "../lib/mr_utils/mrs_strings.h"
#include "f_file.h"
#include <stddef.h>
#define MAX_ENUM_VALUES_COUNT 64

struct E_EnumInfo {
	MRS_String *name;
	MRS_String *typedef_name;
	MRS_String *filename;

	struct F_Variable enum_values[MAX_ENUM_VALUES_COUNT];

	size_t enum_values_len;
	size_t line_number;
};

void E_enum_info_destroy(struct E_EnumInfo *enum_info);

void E_get_enums(MRS_String *file_contents, MRS_String *filename,
		 struct E_EnumInfo enums_info[MAX_ENUM_VALUES_COUNT],
		 size_t *enums_len);

#endif // !E_ENUM_H
