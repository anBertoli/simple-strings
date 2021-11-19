#include "../../src/string_err.h"
#include "framework/framework.h"
#include "string_err_test.h"

void test_ss_error(void) {

    test_group("ss_err_str");

    test_subgroup("test all strings");
    ss_err err = err_none;
    const char *str_err = ss_err_str(err);
    test_strings("should have correct 'err none' string", "", str_err);

    err = err_alloc;
    str_err = ss_err_str(err);
    test_strings("should have correct 'err alloc' string", "allocation error", str_err);

    err = err_format;
    str_err = ss_err_str(err);
    test_strings("should have correct 'err alloc' string", "formatting error", str_err);

    err = 10;
    str_err = ss_err_str(err);
    test_strings("should have correct 'err unknown' string", "unknown error", str_err);
}