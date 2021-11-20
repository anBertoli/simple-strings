
#ifndef SS_STRING_H
#define SS_STRING_H

#include <stdio.h>
#include "string_err.h"

#define END_STRING (char)'\0'

typedef struct ss {
    size_t len;
    size_t free;
    char *buf;
} *ss;

ss ss_new_from_raw_len_free(const char *init, size_t len, size_t avail);
ss ss_new_from_raw_len(const char *init, size_t len);
ss ss_new_from_raw(const char *init);
ss ss_new_empty_with_free(size_t avail);
ss ss_new_empty(void);
ss ss_clone(ss s);
ss_err ss_set_free_space(ss s, size_t avail);
ss_err ss_reserve_free_space(ss s, size_t avail);
void ss_free(ss s);

ss_err ss_grow(ss s, size_t len);
void ss_shrink(ss s, size_t len);
void ss_clear(ss s);
size_t ss_index(ss haystack, const char *needle);
size_t ss_index_last(ss haystack, const char *needle);
ss_err ss_concat_raw_len(ss s1, const char *s2, size_t s2_len);
ss_err ss_concat_raw(ss s1, const char *s2);
ss_err ss_concat_str(ss s1, ss s2);
ss_err ss_prepend_raw_len(const char *s1, ss s2, size_t s1_len);
ss_err ss_prepend_raw(const char *s1, ss s2);
ss_err ss_prepend_str(ss s1, ss s2);
void ss_slice(ss s, size_t str_index, size_t end_index);
void ss_trim(ss s, const char *cutset);
void ss_trim_left(ss s, const char *cutset);
void ss_trim_right(ss s, const char *cutset);
void ss_to_lower(ss s);
void ss_to_upper(ss s);

#endif
