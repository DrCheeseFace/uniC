#ifndef T_UTILS_H
#define T_UTILS_H

#include <stdbool.h>
#include <sys/types.h>

#define T_DEFAULT_COLOR_CODE "\x1b[0m"
#define T_BLUE_COLOR_CODE "\x1b[34m"
#define T_GREEN_COLOR_CODE "\x1b[32m"
#define T_RED_COLOR_CODE "\x1b[31m"

#define T_MAX_TEST_CASES_PER_CONTEXT 10
#define T_MAX_CONTEXT_DESCRIPTION 100

#define T_TAB "    "

// use me!
#define T_ASSERT_EQ(expected, actual)                                          \
	(T_assert_eq((&expected), (&actual), (sizeof(expected))))

#define T_ASSERT(expected)                                                     \
	(T_assert_eq(&(bool){ expected }, &(bool){ true }, sizeof(bool)))

struct T_Case {
	char description[T_MAX_CONTEXT_DESCRIPTION];
	bool pass;
};

struct T_Context {
	char description[T_MAX_CONTEXT_DESCRIPTION];
	int pass_count;
	int fail_count;
	char *failed_test_descriptions[T_MAX_TEST_CASES_PER_CONTEXT];

	struct T_Case cases[T_MAX_TEST_CASES_PER_CONTEXT];
	int case_count;
};

typedef enum {
	T_SEVERITY_DEFAULT,
	T_SEVERITY_INFO,
	T_SEVERITY_OK,
	T_SEVERITY_ERROR,
	T_SEVERITY_COUNT
} T_Severity;

bool T_assert_eq(void *expected, void *actual, size_t size_of);

struct T_Context *T_ctx_create(const char *description);

void T_ctx_destroy(struct T_Context *t_ctx);

void T_ctx_append_test(struct T_Context *t_ctx, struct T_Case test_case);
void T_ctx_log(struct T_Context t_ctx);

#endif // !T_UTILS_H
