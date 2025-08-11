#include "../lib/mr_utils/mrs_strings.h"
#include "../lib/mr_utils/mrt_test.h"
#include "../src/f_file.h"

int test_get_file_prefix(void)
{
	struct MRT_Context *t_ctx = MRT_ctx_create("test_get_file_prefix");

	MRS_String *example_file_name = MRS_init(64, "r_renderer.c");
	MRS_String *file_prefix = F_get_file_prefix(example_file_name);
	MRS_String *expected_prefix = MRS_init(64, "r");

	struct MRT_Case test_case = { .description = "r_renderer.c string",
				      .pass = !MRS_strcmp(expected_prefix,
							  file_prefix) };
	MRT_ctx_append_case(t_ctx, test_case);
	MRS_free(file_prefix);

	MRS_strcpy(example_file_name, "renderer.c");
	file_prefix = F_get_file_prefix(example_file_name);
	test_case = (struct MRT_Case){
		.description = "renderer.c string",
		.pass = file_prefix == NULL,

	};
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(example_file_name, "abcde_renderer.c");
	file_prefix = F_get_file_prefix(example_file_name);
	MRS_strcpy(expected_prefix, "abcde");
	test_case = (struct MRT_Case){ .description = "abcde_renderer.c string",
				       .pass = !MRS_strcmp(expected_prefix,
							   file_prefix) };
	MRT_ctx_append_case(t_ctx, test_case);
	MRS_free(file_prefix);

	MRS_strcpy(example_file_name, "abcdef_renderer.c");
	file_prefix = F_get_file_prefix(example_file_name);
	test_case =
		(struct MRT_Case){ .description = "abcdef_renderer.c string",
				   .pass = file_prefix == NULL };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(example_file_name, "ac1d_renderer.c");
	file_prefix = F_get_file_prefix(example_file_name);
	test_case = (struct MRT_Case){ .description = "ac1d_renderer.c string",
				       .pass = file_prefix == NULL };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(example_file_name, "1_renderer.c");
	file_prefix = F_get_file_prefix(example_file_name);
	test_case = (struct MRT_Case){ .description = "1_renderer.c string",
				       .pass = file_prefix == NULL };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_free(example_file_name);
	MRS_free(expected_prefix);

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
	MRS_String *expected = MRS_init(0, expected_str);

	struct MRT_Case test_case =
		(struct MRT_Case){ .description = "getfilecontents basic",
				   .pass = !MRS_strcmp(expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_free(actual);
	MRS_free(expected);

	int failed = MRT_ctx_log(t_ctx);
	MRT_ctx_free(t_ctx);
	return failed;
}

int test_get_structs(void)
{
	struct MRT_Context *t_ctx = MRT_ctx_create("test_get_structs");

	MRS_String *file_contents =
		F_get_file_contents("test/testdata/getstructs.h");

	MRS_String *struct_names[MAX_STRUCT_NAME_COUNT];
	size_t struct_names_len;

	F_get_structs(file_contents, struct_names, &struct_names_len);

	MRS_free(file_contents);

	for (size_t i = 0; i < struct_names_len; i++) {
		MRS_free(struct_names[i]);
	}

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
