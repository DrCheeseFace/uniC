#include "test_utils.h"

void foo(void)
{
	struct TEST_Context *t_ctx = TEST_ctx_create("my first context");

	int expected = 3;
	int actual = 2;
	struct TEST_Case test_case = { .description = "test case 1",
				       .pass = TEST_ASSERT_EQ(expected,
							      actual) };

	TEST_ctx_append_case(t_ctx, test_case);

	expected = 3;
	actual = 3;
	test_case =
		(struct TEST_Case){ .description = "test case 2",
				    .pass = TEST_ASSERT_EQ(expected, actual) };
	TEST_ctx_append_case(t_ctx, test_case);

	actual = true;
	test_case = (struct TEST_Case){ .description = "test case 3",
					.pass = TEST_ASSERT(actual) };
	TEST_ctx_append_case(t_ctx, test_case);

	TEST_ctx_log(*t_ctx);
	TEST_ctx_destroy(t_ctx);
}

void bar(void)
{
	struct TEST_Context *t_ctx = TEST_ctx_create("my second context");

	int expected = 3;
	int actual = 3;
	struct TEST_Case test_case = { .description = "test case 1",
				       .pass = TEST_ASSERT_EQ(expected,
							      actual) };

	TEST_ctx_append_case(t_ctx, test_case);

	expected = 3;
	actual = 3;
	test_case =
		(struct TEST_Case){ .description = "test case 2",
				    .pass = TEST_ASSERT_EQ(expected, actual) };
	TEST_ctx_append_case(t_ctx, test_case);

	actual = true;
	test_case = (struct TEST_Case){ .description = "test case 3",
					.pass = TEST_ASSERT(actual) };
	TEST_ctx_append_case(t_ctx, test_case);

	TEST_ctx_log(*t_ctx);
	TEST_ctx_destroy(t_ctx);
}

int main(void)
{
	foo();
	bar();
}
