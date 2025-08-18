#include "../lib/mr_utils/mrs_strings.h"
#include "../lib/mr_utils/mrt_test.h"
#include "../src/e_enum.h"
#include "../src/f_file.h"
#include "../src/s_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_get_file_prefix(void)
{
	struct MRT_Context *t_ctx = MRT_ctx_create("test_get_file_prefix");

	MRS_String example_file_name;
	MRS_init(64, "r_renderer.c", strlen("r_renderer.c"),
		 &example_file_name);
	MRS_String file_prefix;
	int actual_result = F_get_file_prefix(&example_file_name, &file_prefix);
	MRS_String expected_prefix;
	MRS_init(64, "r", strlen("r"), &expected_prefix);

	struct MRT_Case test_case = { .description = "r_renderer.c string",
				      .pass = !MRS_strcmp(&expected_prefix,
							  &file_prefix) };
	MRT_ctx_append_case(t_ctx, test_case);
	MRS_free(&file_prefix);

	MRS_setstr(&example_file_name, "renderer.c", strlen("renderer.c"));
	actual_result = F_get_file_prefix(&example_file_name, &file_prefix);
	test_case = (struct MRT_Case){
		.description = "renderer.c string",
		.pass = actual_result == -1,

	};
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_setstr(&example_file_name, "abcde_renderer.c",
		   strlen("abcde_renderer.c"));
	actual_result = F_get_file_prefix(&example_file_name, &file_prefix);
	MRS_setstr(&expected_prefix, "abcde", strlen("abcde"));
	test_case = (struct MRT_Case){ .description = "abcde_renderer.c string",
				       .pass = !MRS_strcmp(&expected_prefix,
							   &file_prefix) };
	MRT_ctx_append_case(t_ctx, test_case);
	MRS_free(&file_prefix);

	MRS_setstr(&example_file_name, "abcdef_renderer.c",
		   strlen("abcdef_renderer.c"));
	actual_result = F_get_file_prefix(&example_file_name, &file_prefix);
	test_case =
		(struct MRT_Case){ .description = "abcdef_renderer.c string",
				   .pass = actual_result == -1 };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_setstr(&example_file_name, "ac1d_renderer.c",
		   strlen("ac1d_renderer.c"));
	actual_result = F_get_file_prefix(&example_file_name, &file_prefix);
	test_case = (struct MRT_Case){ .description = "ac1d_renderer.c string",
				       .pass = actual_result == -1 };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_setstr(&example_file_name, "1_renderer.c", strlen("1_renderer.c"));
	actual_result = F_get_file_prefix(&example_file_name, &file_prefix);
	test_case = (struct MRT_Case){ .description = "1_renderer.c string",
				       .pass = actual_result == -1 };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_free(&example_file_name);
	MRS_free(&expected_prefix);

	int failed = MRT_ctx_log(t_ctx);
	MRT_ctx_free(t_ctx);
	return failed;
}

int test_get_file_contents(void)
{
	struct MRT_Context *t_ctx = MRT_ctx_create("test_get_file_contents");

	MRS_String *actual =
		F_get_file_contents("test/testdata/getfilecontent.c");

	const char *expected_str = "int main(void)\n{\n\treturn 0;\n}\n";
	MRS_String expected;
	MRS_init(0, expected_str, strlen(expected_str), &expected);

	struct MRT_Case test_case =
		(struct MRT_Case){ .description = "getfilecontents basic",
				   .pass = !MRS_strcmp(&expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_free(actual);
	free(actual);
	MRS_free(&expected);

	int failed = MRT_ctx_log(t_ctx);
	MRT_ctx_free(t_ctx);
	return failed;
}

int test_get_structs(void)
{
	struct MRT_Context *t_ctx =
		MRT_ctx_create("test_get_structs -> testdata/getstructs.h");

	size_t expected_struct_count = 8;
	const char *expected_struct_names[8] = {
		"MyStruct",	 NULL,	     "mystruct",    NULL,
		"AnotherStruct", "TES_Case", "TES_Context", "G_GameState",
	};

	const char *expected_struct_typedef_names[8] = {
		NULL,
		"MyTypedefStruct",
		"mystructtypedefname",
		"nogapstruct",
		"AnotherTypedefStruct",
		NULL,
		NULL,
		NULL,
	};

	size_t expected_struct_line_number[8] = { 3, 6, 10, 14, 18, 22, 24, 29 };

	size_t expected_struct_variables_count[8] = { 1, 1, 1, 1, 1, 1, 2, 15 };

	const char *expected_struct_variables[8][15] = {
		{ "window_w" },
		{ "window_w" },
		{ "window_w" },
		{ "window_w" },
		{ "window_w" },
		{ "window_w" },
		{ "pass_count", "fail_count" },
		{ "window_w", "window_h", "scale", "target_fps",
		  "target_frametime_ms", "tsumo", "riichi", "double_riichi",
		  "ippatsu", "haitei", "chankan", "rinshan", "tenhou",
		  "conditions", "selector_idx" }
	};

	size_t expected_struct_variables_line_number[8][15] = {
		{ 4 },
		{ 7 },
		{ 11 },
		{ 15 },
		{ 19 },
		{ 22 },
		{ 25, 26 },
		{ 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45 }
	};

	MRS_String *expected_struct_name = MRS_create(64);

	MRS_String filename;
	MRS_init(16, "getstructs.h", strlen("getstructs.h"), &filename);
	MRS_String *file_contents =
		F_get_file_contents("test/testdata/getstructs.h");

	struct S_StructInfo structs_info[MAX_STRUCT_NAME_COUNT];
	size_t struct_names_len;

	S_get_structs(file_contents, &filename, structs_info,
		      &struct_names_len);

	MRS_free(file_contents);
	free(file_contents);

	struct MRT_Case test_case =
		(struct MRT_Case){ .description = "struct names count",
				   .pass = MRT_ASSERT_EQ(expected_struct_count,
							 struct_names_len) };
	MRT_ctx_append_case(t_ctx, test_case);

	for (size_t i = 0; i < struct_names_len; i++) {
		snprintf(test_case.description, sizeof(test_case.description),
			 "checking for struct '%s' correct line number %zu",
			 expected_struct_names[i],
			 expected_struct_line_number[i]);
		test_case.pass = structs_info[i].line_number ==
				 expected_struct_line_number[i];
		MRT_ctx_append_case(t_ctx, test_case);

		if (expected_struct_names[i] != NULL) {
			MRS_setstr(expected_struct_name,
				   expected_struct_names[i],
				   strlen(expected_struct_names[i]));
			snprintf(test_case.description,
				 sizeof(test_case.description),
				 "checking for expected struct name '%s'",
				 expected_struct_names[i]);
			test_case.pass = !MRS_strcmp(structs_info[i].name,
						     expected_struct_name);
		} else {
			snprintf(test_case.description,
				 sizeof(test_case.description),
				 "checking for NULL struct name");
			test_case.pass = structs_info[i].name == NULL;
		}
		MRT_ctx_append_case(t_ctx, test_case);

		if (expected_struct_typedef_names[i] != NULL) {
			MRS_setstr(expected_struct_name,
				   expected_struct_typedef_names[i],
				   strlen(expected_struct_typedef_names[i]));
			snprintf(
				test_case.description,
				sizeof(test_case.description),
				"checking for expected struct typedef name '%s'",
				expected_struct_typedef_names[i]);
			test_case.pass =
				!MRS_strcmp(structs_info[i].typedef_name,
					    expected_struct_name);
		} else {
			snprintf(test_case.description,
				 sizeof(test_case.description),
				 "checking for NULL struct typedef name");
			test_case.pass = structs_info[i].typedef_name == NULL;
		}
		MRT_ctx_append_case(t_ctx, test_case);

		snprintf(test_case.description, sizeof(test_case.description),
			 "checking for expected struct variable count %zu",
			 expected_struct_variables_count[i]);
		test_case.pass = structs_info[i].variable_names_len ==
				 expected_struct_variables_count[i];
		MRT_ctx_append_case(t_ctx, test_case);

		for (size_t j = 0; j < expected_struct_variables_count[i];
		     j++) {
			snprintf(
				test_case.description,
				sizeof(test_case.description),
				"checking for expected struct variable name %s",
				expected_struct_variables[i][j]);
			test_case.pass = !strcmp(
				structs_info[i].variable_names[j].name->value,
				expected_struct_variables[i][j]);
			MRT_ctx_append_case(t_ctx, test_case);

			snprintf(
				test_case.description,
				sizeof(test_case.description),
				"checking for expected struct variable name %s line number %zu",
				expected_struct_variables[i][j],
				expected_struct_variables_line_number[i][j]);
			test_case.pass =
				structs_info[i].variable_names[j].line_number ==
				expected_struct_variables_line_number[i][j];
			MRT_ctx_append_case(t_ctx, test_case);
		}

		S_struct_info_destroy(&structs_info[i]);
	}

	MRS_free(expected_struct_name);
	free(expected_struct_name);
	MRS_free(&filename);

	int failed = MRT_ctx_log(t_ctx);
	MRT_ctx_free(t_ctx);
	return failed;
}

int test_get_enums(void)
{
	struct MRT_Context *t_ctx =
		MRT_ctx_create("test_get_enums -> getenums.h");

	size_t expected_enums_count = 7;
	const char *expected_enum_names[7] = {
		"longenum",    "multilineenum", "MyEnum", NULL,
		"AnotherEnum", "letter",	"bruh",
	};

	const char *expected_enum_typedef_names[7] = {
		NULL, NULL, NULL, "MyTypedefEnum", "AnotherTypedefEnum",
		NULL, NULL,
	};

	size_t expected_enum_line_number[7] = { 2, 3, 8, 10, 11, 26, 32 };

	const char *expected_enum_values[7][4] = {
		{ "enum1", "enum2", "enum3", "enum4" },
		{ "ENUM1", "ENUM2", "ENUM3", "ENUM4" },
		{ "VALUE0", NULL, NULL, NULL },
		{ "VALUE1", NULL, NULL, NULL },
		{ "VALUE2", NULL, NULL, NULL },
		{ "E1", "E2", "E3", "E4" },
		{ "one", "two", "three", "four" },
	};

	size_t expected_enum_values_len[7] = { 4, 4, 1, 1, 1, 4, 4 };

	size_t expected_enum_values_line_number[7][4] = {
		{ 2, 2, 2, 2 },	    { 3, 4, 5, 6 },  { 8, 0, 0, 0 },
		{ 10, 0, 0, 0 },    { 13, 0, 0, 0 }, { 26, 27, 28, 29 },
		{ 33, 33, 33, 33 },
	};
	MRS_String *expected_name = MRS_create(64);
	MRS_String *expected_enum_name = MRS_create(64);

	MRS_String *file_contents =
		F_get_file_contents("test/testdata/getenums.h");

	MRS_String filename;
	MRS_init(strlen("getenums.h"), "getenums.h", strlen("getenums.h"),
		 &filename);

	struct E_EnumInfo enums_info[MAX_ENUM_VALUES_COUNT];
	size_t enum_names_len;

	E_get_enums(file_contents, &filename, enums_info, &enum_names_len);

	struct MRT_Case test_case = (struct MRT_Case){
		.description = "checking number of enums found",
		.pass = expected_enums_count == enum_names_len
	};

	for (size_t i = 0; i < enum_names_len; i++) {
		if (expected_enum_names[i] != NULL) {
			MRS_setstr(expected_enum_name, expected_enum_names[i],
				   strlen(expected_enum_names[i]));
			snprintf(test_case.description,
				 sizeof(test_case.description),
				 "checking for expected enum name '%s'",
				 expected_enum_names[i]);
			test_case.pass = !MRS_strcmp(enums_info[i].name,
						     expected_enum_name);
		} else {
			snprintf(test_case.description,
				 sizeof(test_case.description),
				 "checking for NULL enum name");
			test_case.pass = enums_info[i].name == NULL;
		}
		MRT_ctx_append_case(t_ctx, test_case);
		if (expected_enum_typedef_names[i] != NULL) {
			MRS_setstr(expected_enum_name,
				   expected_enum_typedef_names[i],
				   strlen(expected_enum_typedef_names[i]));
			snprintf(test_case.description,
				 sizeof(test_case.description),
				 "checking for expected enum typedef name '%s'",
				 expected_enum_typedef_names[i]);
			test_case.pass = !MRS_strcmp(enums_info[i].typedef_name,
						     expected_enum_name);
		} else {
			snprintf(test_case.description,
				 sizeof(test_case.description),
				 "checking for NULL enum typedef name");
			test_case.pass = enums_info[i].typedef_name == NULL;
		}
		MRT_ctx_append_case(t_ctx, test_case);

		snprintf(test_case.description, sizeof(test_case.description),
			 "checking for enum line number");
		test_case.pass = enums_info[i].line_number ==
				 expected_enum_line_number[i];
		MRT_ctx_append_case(t_ctx, test_case);

		snprintf(test_case.description, sizeof(test_case.description),
			 "checking number of enum values for %s",
			 expected_enum_names[i] ? expected_enum_names[i] :
						  "typedef enum");
		test_case.pass = enums_info[i].enum_values_len ==
				 expected_enum_values_len[i];
		MRT_ctx_append_case(t_ctx, test_case);

		for (size_t j = 0; j < enums_info[i].enum_values_len; j++) {
			MRS_setstr(expected_name, expected_enum_values[i][j],
				   strlen(expected_enum_values[i][j]));
			snprintf(test_case.description,
				 sizeof(test_case.description),
				 "checking enum value %zu for %s: '%s'", j + 1,
				 expected_enum_names[i] ?
					 expected_enum_names[i] :
					 "typedef enum",
				 expected_enum_values[i][j]);
			test_case.pass =
				!MRS_strcmp(enums_info[i].enum_values[j].value,
					    expected_name);
			MRT_ctx_append_case(t_ctx, test_case);

			snprintf(
				test_case.description,
				sizeof(test_case.description),
				"checking line number for enum value '%s' in %s",
				expected_enum_values[i][j],
				expected_enum_names[i] ?
					expected_enum_names[i] :
					"typedef enum");
			test_case.pass =
				enums_info[i].enum_values[j].line_number ==
				expected_enum_values_line_number[i][j];
			MRT_ctx_append_case(t_ctx, test_case);
		}

		E_enum_info_destroy(&enums_info[i]);
	}

	MRS_free(expected_enum_name);
	free(expected_enum_name);

	MRS_free(expected_name);
	free(expected_name);

	MRS_free(file_contents);
	free(file_contents);

	MRS_free(&filename);

	int failed = MRT_ctx_log(t_ctx);
	MRT_ctx_free(t_ctx);
	return failed;
}

int main(void)
{
	int err = 0;
	err = err || test_get_file_prefix();
	err = err || test_get_file_contents();
	err = err || test_get_structs();
	err = err || test_get_enums();
	return err;
}
