#include "../string_fmt.h"
#include "framework/framework.h"
#include "../private/debug.h"

static ss internal_test_ss_sprintf_va_cat(ss s, const char *format, ...);
static ss internal_test_ss_sprintf_va(const char *format, ...);

void test_ss_sprintf_va_cat(void) {
    test_group("ss_sprintf_va_cat");

    test_subgroup("no variadic arguments");
    ss s = internal_test_ss_sprintf_va_cat(ss_new_empty(), "test=123 test=ehy");
    test_strings("should have formatted string", "test=123 test=ehy", s->buf);
    test_equal("should have correct len", 17, s->len);
    test_equal("should have correct free", 17, s->free);
    ss_free(s);

    test_subgroup("formatting with arguments");
    s = internal_test_ss_sprintf_va_cat(ss_new_empty(), "test=%d test=%s", 123, "success");
    test_strings("should have formatted string", "test=123 test=success", s->buf);
    test_equal("should have correct len", 21, s->len);
    test_equal("should have correct free", 21, s->free);
    ss_free(s);

    test_subgroup("formatting with arguments 2");
    s = internal_test_ss_sprintf_va_cat(ss_new_empty(), "test=%d test=%d", 1231231231, 123123123);
    test_strings("should have formatted string", "test=1231231231 test=123123123", s->buf);
    test_equal("should have correct len", 30, s->len);
    test_equal("should have correct free", 30, s->free);
    ss_free(s);

    test_subgroup("formatting with concat");
    ss s1 = ss_new_from_raw("ehy ");
    s = internal_test_ss_sprintf_va_cat(s1, "test=%d test=%d", 1231231231, 123123123);
    test_strings("should have formatted string", "ehy test=1231231231 test=123123123", s->buf);
    test_equal("should have correct len", 34, s->len);
    test_equal("should have correct free", 34, s->free);
    ss_free(s);
    ss_free(s1);
}

void test_ss_sprintf_va(void) {
    test_group("ss_sprintf_va");

    test_subgroup("no variadic arguments");
    ss s = internal_test_ss_sprintf_va("test=123 test=ehy");
    test_strings("should have formatted string", "test=123 test=ehy", s->buf);
    test_equal("should have correct len", 17, s->len);
    test_equal("should have correct free", 17, s->free);
    ss_free(s);

    test_subgroup("formatting with arguments");
    s = internal_test_ss_sprintf_va("test=%d test=%s", 123, "success");
    test_strings("should have formatted string", "test=123 test=success", s->buf);
    test_equal("should have correct len", 21, s->len);
    test_equal("should have correct free", 21, s->free);
    ss_free(s);

    test_subgroup("formatting with arguments 2");
    s = internal_test_ss_sprintf_va("test=%d test=%d", 1231231231, 123123123);
    test_strings("should have formatted string", "test=1231231231 test=123123123", s->buf);
    test_equal("should have correct len", 30, s->len);
    test_equal("should have correct free", 30, s->free);
    ss_free(s);
}

ss internal_test_ss_sprintf_va_cat(ss s, const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    ss s1 = ss_sprintf_va_cat(s, format, arg_list);
    va_end(arg_list);
    return s1;
}

ss internal_test_ss_sprintf_va(const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    ss s1 = ss_sprintf_va(format, arg_list);
    va_end(arg_list);
    return s1;
}

void test_ss_sprintf_cat(void) {
    test_group("ss_sprintf_cat");

    test_subgroup("no variadic arguments");
    ss s = ss_sprintf_cat(ss_new_empty(), "test=123 test=ehy");
    test_strings("should have formatted string", "test=123 test=ehy", s->buf);
    test_equal("should have correct len", 17, s->len);
    test_equal("should have correct free", 17, s->free);

    test_subgroup("formatting with arguments");
    s = ss_sprintf_cat(ss_new_empty(), "test=%d test=%s", 123, "success");
    test_strings("should have formatted string", "test=123 test=success", s->buf);
    test_equal("should have correct len", 21, s->len);
    test_equal("should have correct free", 21, s->free);

    test_subgroup("formatting with concat");
    ss s1 = ss_new_from_raw("ehy ");
    s = ss_sprintf_cat(s1, "test=%d test=%d", 1231231231, 123123123);
    test_strings("should have formatted string", "ehy test=1231231231 test=123123123", s->buf);
    test_equal("should have correct len", 34, s->len);
    test_equal("should have correct free", 34, s->free);
    ss_free(s);
    ss_free(s1);
}

void test_ss_sprintf(void) {
    test_group("ss_sprintf");

    test_subgroup("no variadic arguments");
    ss s = ss_sprintf("test=123 test=ehy");
    test_strings("should have formatted string", "test=123 test=ehy", s->buf);
    test_equal("should have correct len", 17, s->len);
    test_equal("should have correct free", 17, s->free);

    test_subgroup("formatting with arguments");
    s = ss_sprintf("test=%d test=%s", 123, "success");
    test_strings("should have formatted string", "test=123 test=success", s->buf);
    test_equal("should have correct len", 21, s->len);
    test_equal("should have correct free", 21, s->free);
}


