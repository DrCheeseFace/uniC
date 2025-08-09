#include "t_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *terminal_color_codes[T_SEVERITY_COUNT] = {
	T_DEFAULT_COLOR_CODE,
	T_BLUE_COLOR_CODE,
	T_GREEN_COLOR_CODE,
	T_RED_COLOR_CODE,
};

void T_reset_severity(void)
{
	printf("%s", terminal_color_codes[T_SEVERITY_DEFAULT]);
}

void T_set_severity(T_Severity severity)
{
	printf("%s", terminal_color_codes[severity]);
}

void T_logln(const char *message, T_Severity severity)
{
	T_set_severity(severity);
	puts(message);
	T_reset_severity();
}

void T_log(const char *message, T_Severity severity)
{
	T_set_severity(severity);
	printf("%s", message);
	T_reset_severity();
}

bool T_assert_eq(void *expected, void *actual, size_t size_of)
{
	return memcmp(expected, actual, size_of) == 0;
}

void T_test_case_log(struct T_Case test_case)
{
	T_log(test_case.description, T_SEVERITY_DEFAULT);
	if (test_case.pass) {
		T_logln(" PASS", T_SEVERITY_OK);
	} else {
		T_logln(" FAILED", T_SEVERITY_ERROR);
	}
	T_reset_severity();
}

struct T_Context *T_ctx_create(const char *description)
{
	struct T_Context *t_ctx = malloc(sizeof(struct T_Context));
	memset(t_ctx, 0, sizeof(*t_ctx));
	strncpy(t_ctx->description, description, T_MAX_CONTEXT_DESCRIPTION - 1);
	return t_ctx;
}

void T_ctx_destroy(struct T_Context *t_ctx)
{
	free(t_ctx);
}

void T_ctx_append_test(struct T_Context *t_ctx, struct T_Case test_case)
{
	if (test_case.pass) {
		t_ctx->pass_count++;
	} else {
		t_ctx->fail_count++;
	}

	t_ctx->cases[t_ctx->case_count] = test_case;
	t_ctx->case_count++;
}

void T_ctx_log(struct T_Context t_ctx)
{
	T_logln("", T_SEVERITY_DEFAULT);

	T_log("description: ", T_SEVERITY_DEFAULT);
	T_logln(t_ctx.description, T_SEVERITY_INFO);

	for (int i = 0; i < t_ctx.case_count; i++) {
		T_log(T_TAB, T_SEVERITY_DEFAULT);
		T_test_case_log(t_ctx.cases[i]);
	}

	if (t_ctx.fail_count != 0) {
		T_logln("FAILED", T_SEVERITY_ERROR);
	} else {
		T_logln("PASSED", T_SEVERITY_OK);
	}

	char pass_rate[15];
	sprintf(pass_rate, "%d/%d Passed", t_ctx.pass_count, t_ctx.case_count);
	T_logln(pass_rate, T_SEVERITY_DEFAULT);
}
