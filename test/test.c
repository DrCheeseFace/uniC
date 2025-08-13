#include "../lib/mr_utils/mrs_strings.h"
#include "../lib/mr_utils/mrt_test.h"
#include "../src/f_file.h"
#include "../src/s_struct.h"
#include <stdio.h>
#include <stdlib.h>

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

	MRS_String *expected_struct_name = MRS_create(64);

	struct MRT_Context *t_ctx =
		MRT_ctx_create("test_get_structs -> testdata/getstructs.h");

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

		S_destroy_struct_info(&structs_info[i]);
	}

	MRS_free(expected_struct_name);
	free(expected_struct_name);
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
	return err;
}
