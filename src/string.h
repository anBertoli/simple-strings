
#ifndef SS_STRING_H
#define SS_STRING_H

#include <stdio.h>

#define END_STRING '\0'
#define END_ITER (ss *)1

/*
 * Memory allocation functions.
 */

void *_malloc(size_t size);
void *_realloc(void *ptr, size_t size);

/*
 * String type and methods.
 */

typedef struct {
    unsigned int len;
    unsigned int cap;
    char *buf;
} ss;

ss *ss_new_raw_len_cap(const char *init, const size_t len, const size_t cap);
ss *ss_new_raw_len(const char *init, const size_t len);
ss *ss_new_raw(const char *init);
ss *ss_new_empty(void);
ss *ss_clone(ss *s);

ss *ss_concat_raw_len(ss *s1, const char *s2, const size_t s2_len);
ss *ss_concat_raw(ss *s1, const char *s2);
ss *ss_concat_str(ss *s1, ss *s2);

void ss_trim(ss *s, const char *cutset);
void ss_trim_left(ss *s, const char *cutset);
void ss_trim_right(ss *s, const char *cutset);

void ss_cut(ss *s, size_t len);
void ss_clear(ss *s);

ss *ss_set_free_space(ss *s, size_t free_space);
ss *ss_grow(ss *s, size_t space);
ss *ss_shrink(ss *s);

void ss_free(ss *s);

/*
 * String formatting functions.
 */

ss *ss_sprintf_concat_va(ss *s, const char *format, va_list arg_list);
ss *ss_sprintf_concat(ss *s, const char *format, ...);
ss *ss_sprintf(const char *format, ...);

/*
 * String iterator type and methods.
 */

typedef struct {
    char *buf;
    char *ptr;
    char *del;
} ss_iter;

ss_iter *ss_split_raw(const char *s, const char *del);
ss_iter *ss_split(ss *s, const char *del);

ss *ss_iter_next(ss_iter *s_iter);

ss **ss_collect_iter(ss_iter *s_iter, int *n_str);
ss **ss_collect_from_row(const char *raw_str, const char  *del, int *n_str);
ss **ss_collect_from_str(ss *s, const char  *del, int *n_str);

void ss_iter_free(ss_iter *s_iter);

#endif
