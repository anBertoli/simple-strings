#include <strings.h>
#include "../src/string_fmt.h"
#include "framework/framework.h"


void test_ss_sprintf(void) {
    test_group("test ss_sprintf");

    test_subgroup("no variadic arguments");
    ss *s = ss_sprintf("test=123 test=ehy");
    test_strings("should have formatted the string", "test=123 test=ehy", s->buf);
    test_equal("should have correct len", 17, s->len);
    test_equal("should have correct cap", 17, s->cap);

    test_subgroup("formatting with arguments");
    s = ss_sprintf("test=%d test=%s", 123, "success");
    test_strings("should have formatted the string", "test=123 test=success", s->buf);
    test_equal("should have correct len", 21, s->len);
    test_equal("should have correct cap", 21, s->cap);
}

ss *_test_ss_sprintf_va(const char *format, ...);

void test_ss_sprintf_va(void) {
    test_group("test ss_sprintf_va");

    test_subgroup("no variadic arguments");
    ss *s = _test_ss_sprintf_va("test=123 test=ehy");
    test_strings("should have formatted the string", "test=123 test=ehy", s->buf);
    test_equal("should have correct len", 17, s->len);
    test_equal("should have correct cap", 17, s->cap);
    ss_free(s);

    test_subgroup("formatting with arguments");
    s = _test_ss_sprintf_va("test=%d test=%s", 123, "success");
    test_strings("should have formatted the string", "test=123 test=success", s->buf);
    test_equal("should have correct len", 21, s->len);
    test_equal("should have correct cap", 21, s->cap);
    ss_free(s);
}


ss *_test_ss_sprintf_va(const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    ss *s1 = ss_sprintf_va(format, arg_list);
    va_end(arg_list);
    return s1;
}