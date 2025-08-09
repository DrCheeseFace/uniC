#include "../src/util.h"
#include "test_utils.h"
#include <string.h>

void test_get_file_prefix(void)
{
	struct TEST_Context *t_ctx = TEST_ctx_create("test_get_file_prefix");

	const char *example_file_name = "r_renderer.c";
	char file_prefix[MAX_FILE_PREFIX_LENGTH] = "";
	int len = get_file_prefix(example_file_name, file_prefix);
	int expected_length = 1;
	const char *expected_prefix = "r";

	struct TEST_Case test_case = {
		.description = "r_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};
	TEST_ctx_append_case(t_ctx, test_case);

	test_case = (struct TEST_Case){ .description = "r_renderer.c length",
					.pass = TEST_ASSERT_EQ(expected_length,
							       len) };
	TEST_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);
	example_file_name = "renderer.c";
	len = get_file_prefix(example_file_name, file_prefix);
	expected_length = 0;
	expected_prefix = "";

	test_case = (struct TEST_Case){
		.description = "renderer.c string",
		.pass = (TEST_assert_eq((&expected_prefix), (&file_prefix),
					(strlen(expected_prefix))))
	};
	TEST_ctx_append_case(t_ctx, test_case);

	test_case = (struct TEST_Case){ .description = "renderer.c length",
					.pass = TEST_ASSERT_EQ(expected_length,
							       len) };
	TEST_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);
	example_file_name = "abcde_renderer.c";
	len = get_file_prefix(example_file_name, file_prefix);
	expected_length = 5;
	expected_prefix = "abcde";

	test_case = (struct TEST_Case){
		.description = "abcde_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};

	TEST_ctx_append_case(t_ctx, test_case);
	test_case =
		(struct TEST_Case){ .description = "abcde_renderer.c length",
				    .pass = TEST_ASSERT_EQ(expected_length,
							   len) };
	TEST_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);
	example_file_name = "abcdef_renderer.c";
	len = get_file_prefix(example_file_name, file_prefix);
	expected_length = 0;
	expected_prefix = "";

	test_case = (struct TEST_Case){
		.description = "abcdef_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};

	TEST_ctx_append_case(t_ctx, test_case);
	test_case =
		(struct TEST_Case){ .description = "abcdef_renderer.c length",
				    .pass = TEST_ASSERT_EQ(expected_length,
							   len) };
	TEST_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);

	example_file_name = "ac1d_renderer.c";
	len = get_file_prefix(example_file_name, file_prefix);
	expected_length = 0;
	expected_prefix = "";

	test_case = (struct TEST_Case){
		.description = "ac1d_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};

	TEST_ctx_append_case(t_ctx, test_case);
	test_case = (struct TEST_Case){ .description = "ac1d_renderer.c length",
					.pass = TEST_ASSERT_EQ(expected_length,
							       len) };
	TEST_ctx_append_case(t_ctx, test_case);

	strncpy(file_prefix, " ", MAX_FILE_PREFIX_LENGTH);
	example_file_name = "1_renderer.c";
	len = get_file_prefix(example_file_name, file_prefix);
	expected_length = 0;
	expected_prefix = "";

	test_case = (struct TEST_Case){
		.description = "1_renderer.c string",
		.pass = !strncmp(file_prefix, expected_prefix, expected_length)
	};

	TEST_ctx_append_case(t_ctx, test_case);
	test_case = (struct TEST_Case){ .description = "1_renderer.c length",
					.pass = TEST_ASSERT_EQ(expected_length,
							       len) };
	TEST_ctx_append_case(t_ctx, test_case);

	TEST_ctx_log(*t_ctx);
	TEST_ctx_destroy(t_ctx);
}

int main(void)
{
	test_get_file_prefix();
}
