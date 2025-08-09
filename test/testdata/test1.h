#ifndef TES_UTILS_H
#define TES_UTILS_H

#include <stdbool.h>
#include <sys/types.h>

#define TES_DEFAULT_COLOR_CODE "\x1b[0m"
#define TES_BLUE_COLOR_CODE "\x1b[34m"
#define TES_GREEN_COLOR_CODE "\x1b[32m"
#define TES_RED_COLOR_CODE "\x1b[31m"

#define TES_MAX_TES_CASES_PER_CONTEXT 100
#define TES_MAX_CONTEXT_DESCRIPTION 100

#define TES_TAB "    "

// use me!
#define TES_ASSERT_EQ(expected, actual)                                        \
	(TES_assert_eq((&expected), (&actual), (sizeof(expected))))

#define TES_ASSERT(actual)                                                     \
	(TES_assert_eq(&(bool){ actual }, &(bool){ true }, sizeof(bool)))

struct MyStruct { /* ... */
};
typedef struct { /* ... */
} MyTypedefStruct;
typedef struct AnotherStruct { /* ... */
} AnotherTypedefStruct;

struct TES_Case {
	char description[TES_MAX_CONTEXT_DESCRIPTION];
	bool pass;
};

struct TES_Context {
	char description[TES_MAX_CONTEXT_DESCRIPTION];
	int pass_count;
	int fail_count;
	char *failed_test_descriptions[TES_MAX_TES_CASES_PER_CONTEXT];

	struct TES_Case cases[TES_MAX_TES_CASES_PER_CONTEXT];
	int case_count;
};

typedef enum TES_Severity {
	TES_SEVERITY_DEFAULT,
	TES_SEVERITY_INFO,
	TES_SEVERITY_OK,
	TES_SEVERITY_ERROR,
	TES_SEVERITY_COUNT
} TES_SeverityTypedef;

bool TES_assert_eq(void *expected, void *actual, size_t size_of);

struct TES_Context *TES_ctx_create(const char *description);

void TES_ctx_destroy(struct TES_Context *t_ctx);

void TES_ctx_append_case(struct TES_Context *t_ctx, struct TES_Case test_case);

/*
 * returns 0 if passed 
 */
int TES_ctx_log(struct TES_Context t_ctx);

#endif // !TES_UTILS_H
