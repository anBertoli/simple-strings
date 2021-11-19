#include "string_test.h"
#include "string_err_test.h"
#include "string_fmt_test.h"
#include "string_split_test.h"
#include "framework/framework.h"

int main(void) {
    // string.c tests
    test_ss_new_from_raw_len_free();
    test_ss_new_from_raw_len();
    test_ss_new_from_raw();
    test_ss_new_empty_with_free();
    test_ss_new_empty();
    test_ss_clone();
    test_ss_set_free_space();
    test_ss_reserve_free_space();
    test_ss_grow();
    test_ss_shrink();
    test_ss_clear();
    test_ss_index();
    test_ss_index_last();
    test_ss_concat_raw_len();
    test_ss_concat_raw();
    test_ss_concat_str();
    test_ss_prepend_raw_len();
    test_ss_prepend_raw();
    test_ss_prepend_str();
    test_ss_slice();
    test_ss_trim();
    test_ss_trim_right();
    test_ss_trim_left();
    test_ss_to_lower();
    test_ss_to_upper();

    // string_err.c
    test_ss_error();

    // string_fmt.c
    test_ss_sprintf_va_cat();
    test_ss_sprintf_va();
    test_ss_sprintf_cat();
    test_ss_sprintf();

    // string_split.c
    test_ss_split_raw();
    test_ss_split_str();
    test_ss_join_raw_cat();
    test_ss_join_raw();
    test_ss_join_str_cat();
    test_ss_join_str();

    return test_report();
}
