#include "string_test.h"
#include "string_fmt_test.h"
#include "string_iter_test.h"
#include "framework/framework.h"

int main(void) {
    // string.c tests
    test_ss_new_from_raw_len_cap();
    test_ss_new_from_raw_len();
    test_ss_new_from_raw();
    test_ss_new_empty();
    test_ss_new_empty_with_cap();
    test_ss_clone();
    test_ss_slice();

    test_set_free_space();
    test_grow();
    test_shrink();

    test_ss_concat_raw_len();
    test_ss_concat_raw();
    test_ss_concat_str();

    test_trim();
    test_trim_right();
    test_trim_left();
    test_cut();
    test_clear();

    // string_fmt.c
    test_ss_sprintf();
    test_ss_sprintf_va();

    // string_fmt.c
    test_ss_split_raw_to_iter();
    test_ss_split_str_to_iter();
    test_ss_iter_next();
    test_ss_iter_collect();
    test_ss_split_row();
    test_ss_split_str();

    return test_report();
}
