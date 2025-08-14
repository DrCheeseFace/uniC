#ifndef S_STRUCT_H
#define S_STRUCT_H

#include "../lib/mr_utils/mrs_strings.h"

#define MAX_STRUCT_NAME_LENGTH 30

#define MAX_STRUCT_NAME_COUNT 50

#define MAX_STRUCT_VARIABLE_COUNT 50

struct S_StructVariable {
	MRS_String *name;
	size_t line_number;
};

struct S_StructInfo {
	MRS_String *name;
	MRS_String *typedef_name;
	MRS_String *filename;

	struct S_StructVariable variable_names[MAX_STRUCT_VARIABLE_COUNT];

	size_t variable_names_len;
	size_t line_number;
};

void S_struct_info_destroy(struct S_StructInfo *a);

void S_get_structs(MRS_String *file_contents, MRS_String *filename,
		   struct S_StructInfo structs_info[MAX_STRUCT_NAME_COUNT],
		   size_t *structs_len);

#endif // !S_STRUCT_H
