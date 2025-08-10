#include "../src/f_file.h"
#include "../src/mrs_strings.h"
#include "mrt_test.h"
#include <stdio.h>
#include <string.h>

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
	MRS_String *expected = MRS_init(strlen(expected_str), expected_str);

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

int test_mrs_strings_strstr(void)
{
	struct MRT_Context *t_ctx = MRT_ctx_create("test_mrs_strings_strstr");

	const char *haystack = "11151111111111111231";
	const char *needle = "31";

	MRS_String *a = MRS_init(strlen(haystack), haystack);

	MRS_String *b = MRS_init(strlen(needle), needle);

	struct MRT_Case test_case = (struct MRT_Case){
		.description = "11151111111111111231 | 31",
		.pass = MRT_ASSERT_EQ(a->value[18], *MRS_strstr(a, b, 0))
	};
	MRT_ctx_append_case(t_ctx, test_case);

	size_t start_idx = 3;
	test_case = (struct MRT_Case){
		.description = "11151111111111111231 | 31 startidx=3",
		.pass = MRT_ASSERT_EQ(a->value[18],
				      *MRS_strstr(a, b, &start_idx))
	};
	MRT_ctx_append_case(t_ctx, test_case);

	test_case = (struct MRT_Case){
		.description = "11151111111111111231 | 31 startidx=NULL",
		.pass = MRT_ASSERT_EQ(a->value[18], *MRS_strstr(a, b, NULL))
	};
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(b, "52");
	test_case = (struct MRT_Case){
		.description = "11151111111111111231 | 52",
		.pass = MRT_ASSERT_NULL(MRS_strstr(a, b, 0))
	};
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_free(a);
	MRS_free(b);

	int failed = MRT_ctx_log(t_ctx);
	MRT_ctx_free(t_ctx);
	return failed;
}

int test_mrs_strings_filter(void)
{
	struct MRT_Context *t_ctx = MRT_ctx_create("test_mrs_strings_filter");

	MRS_String *actual = MRS_init(20, "12121");
	MRS_String *expected = MRS_init(20, "111");
	MRS_filter(actual, '2');

	struct MRT_Case test_case =
		(struct MRT_Case){ .description = "12121 | 2",
				   .pass = !MRS_strcmp(expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(actual, "2222");
	MRS_strcpy(expected, "");
	MRS_filter(actual, '2');
	test_case = (struct MRT_Case){ .description = "2222 | 2",
				       .pass = !MRS_strcmp(expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(actual, "2222");
	MRS_strcpy(expected, "2222");
	MRS_filter(actual, '3');
	test_case = (struct MRT_Case){ .description = "2222 | 3",
				       .pass = !MRS_strcmp(expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_free(actual);
	MRS_free(expected);

	int failed = MRT_ctx_log(t_ctx);
	MRT_ctx_free(t_ctx);
	return failed;
}

int test_mrs_strings_strcat(void)
{
	struct MRT_Context *t_ctx = MRT_ctx_create("test_mrs_strings_strcat");

	MRS_String *actual = MRS_init(10, "aaabbb");
	MRS_String *append = MRS_init(10, "cc");
	MRS_String *expected = MRS_init(10, "aaabbbcc");
	MRS_strcat(actual, append);

	struct MRT_Case test_case =
		(struct MRT_Case){ .description = "aaabbb | cc",
				   .pass = !MRS_strcmp(expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(actual, "123456789");
	MRS_strcpy(append, "10");
	test_case =
		(struct MRT_Case){ .description =
					   "123456789 | 10 over capacity",
				   .pass = 0 == !MRS_strcat(actual, append) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(actual, "123456789");
	MRS_strcpy(append, "0");
	MRS_strcpy(expected, "1234567890");
	MRS_strcat(actual, append);
	test_case = (struct MRT_Case){ .description =
					       "123456789 | 1 exactly capacity",
				       .pass = !MRS_strcmp(expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(actual, "123456789");
	MRS_strcpy(append, "");
	MRS_strcpy(expected, "123456789");
	MRS_strcat(actual, append);
	test_case = (struct MRT_Case){ .description = "123456789 | empty src",
				       .pass = !MRS_strcmp(expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(actual, "");
	MRS_strcpy(append, "123");
	MRS_strcpy(expected, "123");
	MRS_strcat(actual, append);
	test_case = (struct MRT_Case){ .description = "123456789 | empty dest",
				       .pass = !MRS_strcmp(expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_strcpy(actual, "");
	MRS_strcpy(append, "");
	MRS_strcpy(expected, "");
	MRS_strcat(actual, append);
	test_case = (struct MRT_Case){ .description = "empty dest and src",
				       .pass = !MRS_strcmp(expected, actual) };
	MRT_ctx_append_case(t_ctx, test_case);

	MRS_free(actual);
	MRS_free(expected);
	MRS_free(append);

	int failed = MRT_ctx_log(t_ctx);
	MRT_ctx_free(t_ctx);
	return failed;
}

int test_get_(void)
{
	struct MRT_Context *t_ctx = MRT_ctx_create("test_get_file_contents");

	MRS_String *file_contents =
		F_get_file_contents("./test/testdata/test1.c");
	F_get_structs(file_contents);

	int failed = MRT_ctx_log(t_ctx);
	MRT_ctx_free(t_ctx);
	return failed;
}

int main(void)
{
	int err = 0;
	err = err || test_get_file_prefix();
	err = err || test_get_file_contents();
	err = err || test_mrs_strings_strstr();
	err = err || test_mrs_strings_filter();
	err = err || test_mrs_strings_strcat();
	return err;
}
