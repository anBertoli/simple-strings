
#ifndef SS_STRING_H
#define SS_STRING_H

#include <stdio.h>

#define END_STRING '\0'

/*
 * IMPORTANT: the ss struct fields must not be manipulated by the caller, they are
 * updated by the functions of this library and they must be considered read-only.
 */
typedef struct {
    unsigned int len;
    unsigned int cap;
    char *buf;
} ss;

/*
 * Creation and memory manipulation functions.
 */

ss *ss_new_from_raw_len_cap(const char *init, const size_t len, const size_t cap);
ss *ss_new_from_raw_len(const char *init, const size_t len);
ss *ss_new_from_raw(const char *init);
ss *ss_new_empty_with_cap(const size_t cap);
ss *ss_new_empty(void);

ss *ss_clone(ss *s);
ss *ss_slice(ss *s, const int str_index, const int end_index);

ss *ss_set_free_space(ss *s, size_t free_space);
ss *ss_grow(ss *s, size_t space);
ss *ss_shrink(ss *s);

void ss_free(ss *s);

/*
 * Searching functions.
 */

int ss_index(ss *s, const char *needle);

/*
 * Concatenation functions.
 */

ss *ss_concat_raw_len(ss *s1, const char *s2, const size_t s2_len);
ss *ss_concat_raw(ss *s1, const char *s2);
ss *ss_concat_str(ss *s1, ss *s2);

/*
 * Trimming and cutting functions.
 */

void ss_trim(ss *s, const char *cutset);
void ss_trim_left(ss *s, const char *cutset);
void ss_trim_right(ss *s, const char *cutset);

void ss_cut(ss *s, size_t len);
void ss_clear(ss *s);

#endif
