
#ifndef SS_STRING_H
#define SS_STRING_H
#define END_STRING '\0'
#define END_ITER (str *)1

#include <stdio.h>

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
} str;

str *str_new_raw_len(const char *init, const size_t len);
str *str_new_raw(const char *init);
str *str_new_empty();

str *str_clone(str *s);

str *str_concat_raw_len(str *s1, const char *s2, const size_t s2_len);
str *str_concat_raw(str *s1, const char *s2);
str *str_concat_str(str *s1, str *s2);

str *str_trim(str *s, const char *cutset);
str *str_trim_left(str *s, const char *cutset);
str *str_trim_right(str *s, const char *cutset);

void str_clear(str *s);
str *str_shrink(str *s);
void str_free(str *s);

/*
 * String iterator type and methods.
 */

typedef struct {
    char *buf;
    char *ptr;
    char *del;
} str_iter;

str_iter *str_split_raw(const char *s, const char *del);
str_iter *str_split(str *s, const char *del);
str *str_iter_next(str_iter *s_iter);

str **str_iter_collect(str_iter *s_iter, int *n_str);
str **str_split_raw_collect(const char *raw_str, const char  *del, int *n_str);

void str_iter_free(str_iter *s_iter);

#endif
