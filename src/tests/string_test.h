
#ifndef SS_STRING_TEST_H
#define SS_STRING_TEST_H

void test_ss_new_from_raw_len_cap(void);
void test_ss_new_from_raw_len(void);
void test_ss_new_from_raw(void);
void test_ss_new_empty_with_cap(void);
void test_ss_new_empty(void);
void test_ss_clone(void);

void test_ss_grow(void);
void test_ss_cut(void);
void test_ss_clear(void);

void test_ss_set_free_space(void);
void test_ss_reserve_free_space(void);

void test_ss_index(void);

void test_ss_concat_raw_len(void);
void test_ss_concat_raw(void);
void test_ss_concat_str(void);

void test_ss_slice(void);
void test_ss_trim(void);
void test_ss_trim_right(void);
void test_ss_trim_left(void);

void test_ss_to_lower(void);
void test_ss_to_upper(void);

#endif