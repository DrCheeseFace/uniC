#include "t_utils.h"

void foo(void)
{
	struct T_Context *t_ctx = T_ctx_create("my first context");

	int expected = 3;
	int actual = 2;
	struct T_Case test_case = { .description = "test case 1",
				    .pass = T_ASSERT_EQ(expected, actual) };

	T_ctx_append_test(t_ctx, test_case);

	expected = 3;
	actual = 3;
	test_case = (struct T_Case){ .description = "test case 2",
				     .pass = T_ASSERT_EQ(expected, actual) };
	T_ctx_append_test(t_ctx, test_case);

	expected = true;
	test_case = (struct T_Case){ .description = "test case 3",
				     .pass = T_ASSERT(expected) };
	T_ctx_append_test(t_ctx, test_case);

	T_ctx_log(*t_ctx);
	T_ctx_destroy(t_ctx);
}

void bar(void)
{
	struct T_Context *t_ctx = T_ctx_create("my second context");

	int expected = 3;
	int actual = 3;
	struct T_Case test_case = { .description = "test case 1",
				    .pass = T_ASSERT_EQ(expected, actual) };

	T_ctx_append_test(t_ctx, test_case);

	expected = 3;
	actual = 3;
	test_case = (struct T_Case){ .description = "test case 2",
				     .pass = T_ASSERT_EQ(expected, actual) };
	T_ctx_append_test(t_ctx, test_case);

	expected = true;
	test_case = (struct T_Case){ .description = "test case 3",
				     .pass = T_ASSERT(expected) };
	T_ctx_append_test(t_ctx, test_case);

	T_ctx_log(*t_ctx);
	T_ctx_destroy(t_ctx);
}

int main(void)
{
	foo();
	bar();
}
