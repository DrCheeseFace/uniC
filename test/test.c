#include "../lib/mr_utils/mrs_strings.h"
#include "../lib/mr_utils/mrt_test.h"
#include "../src/f_file.h"

int test_get_file_prefix(void)
{
	struct MRT_Context *t_ctx = MRT_ctx_create("test_get_file_prefix");

	const char *example_file_name = "r_renderer.c";
	char file_prefix[MAX_FILE_PREFIX_LENGTH] = "";
	int len = F_get_file_prefix(example_file_name, file_prefix);
	int expected_length = 1;
	const char *expected_prefix = "r";

	struct MRT_Case test_case = {
		.description = "r_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};
	MRT_ctx_append_case(t_ctx, test_case);

	test_case = (struct MRT_Case){ .description = "r_renderer.c length",
				       .pass = MRT_ASSERT_EQ(expected_length,
							     len) };
	MRT_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);
	example_file_name = "renderer.c";
	len = F_get_file_prefix(example_file_name, file_prefix);
	expected_length = 0;
	expected_prefix = "";

	test_case = (struct MRT_Case){
		.description = "renderer.c string",
		.pass = (MRT_assert_eq((&expected_prefix), (&file_prefix),
				       (strlen(expected_prefix))))
	};
	MRT_ctx_append_case(t_ctx, test_case);

	test_case = (struct MRT_Case){ .description = "renderer.c length",
				       .pass = MRT_ASSERT_EQ(expected_length,
							     len) };
	MRT_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);
	example_file_name = "abcde_renderer.c";
	len = F_get_file_prefix(example_file_name, file_prefix);
	expected_length = 5;
	expected_prefix = "abcde";

	test_case = (struct MRT_Case){
		.description = "abcde_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};

	MRT_ctx_append_case(t_ctx, test_case);
	test_case = (struct MRT_Case){ .description = "abcde_renderer.c length",
				       .pass = MRT_ASSERT_EQ(expected_length,
							     len) };
	MRT_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);
	example_file_name = "abcdef_renderer.c";
	len = F_get_file_prefix(example_file_name, file_prefix);
	expected_length = 0;
	expected_prefix = "";

	test_case = (struct MRT_Case){
		.description = "abcdef_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};

	MRT_ctx_append_case(t_ctx, test_case);
	test_case =
		(struct MRT_Case){ .description = "abcdef_renderer.c length",
				   .pass = MRT_ASSERT_EQ(expected_length,
							 len) };
	MRT_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);

	example_file_name = "ac1d_renderer.c";
	len = F_get_file_prefix(example_file_name, file_prefix);
	expected_length = 0;
	expected_prefix = "";

	test_case = (struct MRT_Case){
		.description = "ac1d_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};

	MRT_ctx_append_case(t_ctx, test_case);
	test_case = (struct MRT_Case){ .description = "ac1d_renderer.c length",
				       .pass = MRT_ASSERT_EQ(expected_length,
							     len) };
	MRT_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);
	example_file_name = "1_renderer.c";
	len = F_get_file_prefix(example_file_name, file_prefix);
	expected_length = 0;
	expected_prefix = "";

	test_case = (struct MRT_Case){
		.description = "1_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};

	MRT_ctx_append_case(t_ctx, test_case);
	test_case = (struct MRT_Case){ .description = "1_renderer.c length",
				       .pass = MRT_ASSERT_EQ(expected_length,
							     len) };
	MRT_ctx_append_case(t_ctx, test_case);

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
