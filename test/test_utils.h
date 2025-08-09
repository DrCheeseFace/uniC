#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>
#include <sys/types.h>

#define TEST_DEFAULT_COLOR_CODE "\x1b[0m"
#define TEST_BLUE_COLOR_CODE "\x1b[34m"
#define TEST_GREEN_COLOR_CODE "\x1b[32m"
#define TEST_RED_COLOR_CODE "\x1b[31m"

#define TEST_MAX_TEST_CASES_PER_CONTEXT 100
#define TEST_MAX_CONTEXT_DESCRIPTION 100

#define TEST_TAB "    "

// use me!
#define TEST_ASSERT_EQ(expected, actual)                                       \
	(TEST_assert_eq((&expected), (&actual), (sizeof(expected))))

#define TEST_ASSERT(actual)                                                    \
	(TEST_assert_eq(&(bool){ actual }, &(bool){ true }, sizeof(bool)))

struct TEST_Case {
	char description[TEST_MAX_CONTEXT_DESCRIPTION];
	bool pass;
};

struct TEST_Context {
	char description[TEST_MAX_CONTEXT_DESCRIPTION];
	int pass_count;
	int fail_count;
	char *failed_test_descriptions[TEST_MAX_TEST_CASES_PER_CONTEXT];

	struct TEST_Case cases[TEST_MAX_TEST_CASES_PER_CONTEXT];
	int case_count;
};

typedef enum {
	TEST_SEVERITY_DEFAULT,
	TEST_SEVERITY_INFO,
	TEST_SEVERITY_OK,
	TEST_SEVERITY_ERROR,
	TEST_SEVERITY_COUNT
} TEST_Severity;

bool TEST_assert_eq(void *expected, void *actual, size_t size_of);

struct TEST_Context *TEST_ctx_create(const char *description);

void TEST_ctx_destroy(struct TEST_Context *t_ctx);

void TEST_ctx_append_case(struct TEST_Context *t_ctx,
			  struct TEST_Case test_case);

void TEST_ctx_log(struct TEST_Context t_ctx);

#endif // !TEST_UTILS_H
