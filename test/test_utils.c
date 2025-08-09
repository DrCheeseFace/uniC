#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *terminal_color_codes[TEST_SEVERITY_COUNT] = {
	TEST_DEFAULT_COLOR_CODE,
	TEST_BLUE_COLOR_CODE,
	TEST_GREEN_COLOR_CODE,
	TEST_RED_COLOR_CODE,
};

void TEST_reset_severity(void)
{
	printf("%s", terminal_color_codes[TEST_SEVERITY_DEFAULT]);
}

void TEST_set_severity(TEST_Severity severity)
{
	printf("%s", terminal_color_codes[severity]);
}

void TEST_logln(const char *message, TEST_Severity severity)
{
	TEST_set_severity(severity);
	puts(message);
	TEST_reset_severity();
}

void TEST_log(const char *message, TEST_Severity severity)
{
	TEST_set_severity(severity);
	printf("%s", message);
	TEST_reset_severity();
}

bool TEST_assert_eq(void *expected, void *actual, size_t size_of)
{
	return memcmp(expected, actual, size_of) == 0;
}

void TEST_case_log(struct TEST_Case test_case)
{
	TEST_log(test_case.description, TEST_SEVERITY_DEFAULT);
	if (test_case.pass) {
		TEST_logln(" PASS", TEST_SEVERITY_OK);
	} else {
		TEST_logln(" FAILED", TEST_SEVERITY_ERROR);
	}
	TEST_reset_severity();
}

struct TEST_Context *TEST_ctx_create(const char *description)
{
	struct TEST_Context *t_ctx = malloc(sizeof(struct TEST_Context));
	memset(t_ctx, 0, sizeof(*t_ctx));
	strncpy(t_ctx->description, description,
		TEST_MAX_CONTEXT_DESCRIPTION - 1);
	return t_ctx;
}

void TEST_ctx_destroy(struct TEST_Context *t_ctx)
{
	free(t_ctx);
}

void TEST_ctx_append_case(struct TEST_Context *t_ctx,
			  struct TEST_Case test_case)
{
	if (test_case.pass) {
		t_ctx->pass_count++;
	} else {
		t_ctx->fail_count++;
	}

	t_ctx->cases[t_ctx->case_count] = test_case;
	t_ctx->case_count++;
}

void TEST_ctx_log(struct TEST_Context t_ctx)
{
	TEST_logln("", TEST_SEVERITY_DEFAULT);

	TEST_log("description: ", TEST_SEVERITY_DEFAULT);
	TEST_logln(t_ctx.description, TEST_SEVERITY_INFO);

	for (int i = 0; i < t_ctx.case_count; i++) {
		TEST_log(TEST_TAB, TEST_SEVERITY_DEFAULT);
		TEST_case_log(t_ctx.cases[i]);
	}

	if (t_ctx.fail_count != 0) {
		TEST_logln("FAILED", TEST_SEVERITY_ERROR);
	} else {
		TEST_logln("PASSED", TEST_SEVERITY_OK);
	}

	char pass_rate[15];
	sprintf(pass_rate, "%d/%d Passed", t_ctx.pass_count, t_ctx.case_count);
	TEST_logln(pass_rate, TEST_SEVERITY_DEFAULT);
}
