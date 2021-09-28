#include <stdlib.h>
#include <strings.h>
#include "string.h"
#include "utils.h"

/*
 * Build a new str copying the provided init string of length len (excluding the null
 * terminator). If the length of the init string is greater than len, the exceeding
 * bytes are discarded. The string is heap allocated and a pointer to it is returned.
 * The str must be freed after use with the provided str_free function.
 *
 * The returned string has length len, but (len * 2 + 1) bytes are allocated (cap).
 * This is often useful because we reduce the probability of future reallocations
 * when the string is manipulated.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 *
 * Example:
 *
 *      str *sentence = str_new_raw_len("Hello!", 6);
 *      if (str == NULL) {
 *          // .. error handling
 *      }
 *
 *      printf("%s\n", sentence->buf);  // --> Hello!
 */
str *str_new_raw_len(const char *init, const size_t len) {
    size_t cap = len * 2;
    char *buf = _malloc(sizeof(char) * cap + 1);
    if (buf == NULL) {
        return NULL;
    }

    memcpy(buf, init, len);
    buf[len] = END_STRING;

    str *s = _malloc(sizeof(str));
    if (s == NULL) {
        free(buf);
        return NULL;
    }
    *s = (str){
        .len = len,
        .cap = cap,
        .buf = buf
    };

    return s;
}

/*
 * Build a new str copying the provided null terminated 'init' string. The
 * str_new_raw function is a shorthand for str_new_raw_len(init, len), where
 * len here is calculated with strlen. If the init pointer is NULL a new
 * empty string is built. The returned string must be freed after use with
 * the provided str_free function.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 */
str *str_new_raw(const char *init) {
    size_t init_len = (init == NULL) ? 0 : strlen(init);
    return str_new_raw_len(init, init_len);
}

/*
 * Allocates and returns a new empty str with length zero (it allocates only one
 * byte, the null terminator). The str must be freed after use with the provided
 * str_free function.
 *
 * Returns the new empty string or NULL if the allocation fails.
 */
str *str_new_empty() {
    return str_new_raw_len("", 0);
}

/*
 * Allocates a new string and return a string pointer pointing to it. The new
 * string is an exact copy of the original string passed as argument. The new
 * string and the old one are independent and both of them must be freed after
 * use.
 *
 * Returns the new string in case of success or NULL if the allocation fails.
 */
str *str_clone(str *s) {
    return str_new_raw_len(s->buf, s->len);
}

/*
 * Concatenate a lib string (s1, str type) with a C string (s2, char*) of length
 * len. If the length of the C string is greater than len, the exceeding bytes
 * are not concatenated. The s2 string is appended to s1, eventually growing
 * the s1 str.
 *
 * Returns s1 if the function succeeded or NULL if the eventual reallocation
 * failed. In case of failure the s1 str is still valid.
 */
str *str_concat_raw_len(str *s1, const char *s2, const size_t len) {
    size_t new_len = s1->len + len;

    if (new_len > s1->cap) {
        size_t new_cap = (sizeof(char) * new_len) * 2;

        // We need to alias the re-allocated buffer to
        // avoid overwriting the original one in case
        // of allocation failure.
        char *new_buf = _realloc(s1->buf, new_cap + 1);
        if (new_buf == NULL) return NULL;

        s1->buf = new_buf;
        s1->cap = new_cap;
    }

    memcpy(s1->buf + s1->len, s2, len);
    s1->buf[new_len] = END_STRING;
    s1->len = new_len;

    return s1;
}

/*
 * Concatenate a lib string (s1, str type) with a null terminated C string
 * (s2, char*). The s2 string is appended to s1, eventually growing the s1
 * string. It is a shorthand for str_concat_raw(s1, s1, strlen(s2)).
 *
 * Returns s1 if the function succeeded or NULL if the eventual reallocation
 * failed. In case of failure the s1 str is still valid.
 */
str *str_concat_raw(str *s1, const char *s2) {
    size_t init_len = (s2 == NULL) ? 0 : strlen(s2);
    return str_concat_raw_len(s1, s2, init_len);
}

/*
 * Concatenate a lib string s1 with another lib string s2. The s2 string is
 * appended to s1, eventually growing the s1 string. Both strings are still
 * valid after the function call.
 *
 * Returns s1 if the function succeeded or NULL if the eventual reallocation
 * failed.
 */
str *str_concat_str(str *s1, str *s2) {
    return str_concat_raw_len(s1, s2->buf, s2->len);
}

/*
 * Removes the characters contained in the cutset string from the start and
 * the end of the string s. After the trimming operation, the string length
 * is reduced while the allocation size is left untouched.
 *
 * Returns the modified (trimmed) s string.
 */
str *str_trim(str *s, const char *cutset) {
    size_t start = 0;
    size_t end = s->len - 1;

    // Calculate boundaries of trimmed string, start and end
    // refer to the first and last char not in the cutset
    // respectively. It is possible that start > end when
    // the string contains only chars form the cutset.
    while ((start <= end) && strchr(cutset, s->buf[start])) start++;
    while ((end > start) && strchr(cutset, s->buf[end])) end--;

    size_t len = start > end ? 0 : (end - start + 1);
    if (start != 0) {
        memmove(s->buf, s->buf + start, len);
    }

    s->buf[len] = END_STRING;
    s->len = len;
    return s;
}

/*
 * Removes the characters contained in the cutset string from the start of the
 * string s. After the trimming operation, the string length is reduced while
 * the allocation size is left untouched.
 *
 * Returns the modified (start-trimmed) s string.
 */
str *str_trim_left(str *s, const char *cutset) {
    size_t start = 0;
    size_t last = s->len - 1;

    while ((start <= last) && strchr(cutset, s->buf[start])) start++;

    size_t len = start > last ? 0 : (last - start + 1);
    if (start != 0) {
        memmove(s->buf, s->buf + start, len);
    }

    s->buf[len] = END_STRING;
    s->len = len;
    return s;
}

/*
 * Removes the characters contained in the cutset string from the end of the
 * string s. After the trimming operation, the string length is reduced while
 * the allocation size is left untouched.
 *
 * Returns the modified (end-trimmed) s string.
 */
str *str_trim_right(str *s, const char *cutset) {
    size_t last = s->len - 1;

    while ((last >= 0) && strchr(cutset, s->buf[last])) last--;
    size_t len = last < 0 ? 0 : last + 1;

    s->buf[len] = END_STRING;
    s->len = len;
    return s;
}

/*
 * Clear the string, the string in discarded and transformed into an empty one.
 * The allocation space is left untouched, so future string manipulations
 * can be performed with fewer reallocations.
 */
void str_clear(str *s) {
    s->len = 0;
    s->buf[0] = END_STRING;
}

/*
 * Reduce the allocated space (defined 'cap') to contain exactly the current
 * string. The allocated string itself is left untouched.
 */
str *str_shrink(str *s) {
    char *new_buf = _realloc(s->buf, sizeof(char) * s->len + 1);
    if (new_buf == NULL) return NULL;

    s->buf = new_buf;
    s->cap = s->len;
    return s;
}

/*
 * Free the library string. The string cannot be used after being freed.
 */
void str_free(str *s) {
    if (s == NULL || s->buf == NULL) return;
    free(s->buf);
    s->buf = NULL,
    s->len = 0;
    s->cap = 0;
    free(s);
}
