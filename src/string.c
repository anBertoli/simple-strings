#include <stdlib.h>
#include <strings.h>
#include "string.h"
#include "internal/debug.h"


/*
 * Build a new string copying the provided init string of length len (the len argument
 * doesn't include the null terminator) and making space for cap bytes. If the length
 * of the init string is greater than len, the exceeding bytes are discarded. If cap
 * < len, len is adjusted to be equal to cap. The caller is responsible for providing the
 * correct length, cap and a valid init string. The string is heap allocated and a pointer
 * to it is returned. The string must be freed after use with the provided ss_free function.
 *
 * Useful shorthands are ss_new_raw_len and ss_new_raw functions, which are more
 * ergonomic and easier to use.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 *
 * IMPORTANT
 * The ss struct must not be manipulated by the caller, the fields are updated by the
 * functions of this library and they must be considered read-only.
 */
ss *ss_new_raw_len_cap(const char *init, const size_t len, const size_t cap) {
    size_t _len = len;
    if (cap <= len) _len = cap;

    char *buf = _malloc(sizeof(char) * cap + 1);
    if (buf == NULL) {
        return NULL;
    }

    memcpy(buf, init, _len);
    buf[_len] = END_STRING;

    ss *s = _malloc(sizeof(ss));
    if (s == NULL) {
        free(buf);
        return NULL;
    }
    *s = (ss){
        .len = _len,
        .cap = cap,
        .buf = buf
    };

    return s;
}

/*
 * Build a new string copying the provided init string of length len (the len argument
 * doesn't include the null terminator). If the length of the init string is greater than
 * len, the exceeding bytes are discarded. The caller is responsible for providing the
 * correct len length and a valid init string. The string is heap allocated and a pointer
 * to it is returned. The ss must be freed after use with the provided ss_free function.
 * The ss_new_raw_len function is a shorthand for ss_new_raw_len_cap(init, len, len * 2).
 *
 * The returned string has length len, but (len * 2 + 1) bytes are allocated (defined the
 * cap number, +1 for the null terminator). This is often useful because we reduce the
 * probability of future reallocations when the string is manipulated.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 *
 * IMPORTANT
 * The ss struct must not be manipulated by the caller, the fields are updated by the
 * functions of this library and they must be considered read-only.
 */
ss *ss_new_raw_len(const char *init, const size_t len) {
    return ss_new_raw_len_cap(init, len, len * 2);
}

/*
 * Build a new string copying the provided null terminated 'init' C string. The ss_new_raw
 * function is a shorthand for str_new_raw_len(init, len), where len here is calculated
 * with strlen. If the init pointer is NULL a new empty string is built. The returned
 * string must be freed after use with the provided ss_free function.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 *
 * IMPORTANT
 * The ss struct must not be manipulated by the caller, the fields are updated by the
 * functions of this library and they must be considered read-only.
 */
ss *ss_new_raw(const char *init) {
    return (init == NULL)
        ? ss_new_empty()
        : ss_new_raw_len(init, strlen(init));
}

/*
 * Allocates and returns a new empty string with length zero (it allocates the ss struct
 * itself and one more byte to accommodate the null terminator). The ss must be freed
 * after use with the provided ss_free function.
 *
 * Returns the new empty string or NULL if the allocation fails.
 *
 * IMPORTANT
 * The ss struct must not be manipulated by the caller, the fields are updated by the
 * functions of this library and they must be considered read-only.
 */
ss *ss_new_empty() {
    return ss_new_raw_len("", 0);
}

/*
 * Allocates a new string copied from passed one and returns it. The new string and the
 * old one are independent and both of them must be freed after use. Cloning a string is
 * useful when, for example, we want to manipulate a string, but we want also to retain
 * the original content.
 *
 * Returns the new string in case of success or NULL if the allocation fails.
 */
ss *ss_clone(ss *s) {
    return ss_new_raw_len(s->buf, s->len);
}

/*
 * Concatenate a string (s1, ss type) with a C string (s2, char*) of length len. If the
 * length of the C string is greater than len, the exceeding bytes are not concatenated.
 * The s2 C string is appended to the s1 string, eventually growing s1. The strategy for
 * string growing is the following one: if the resulting string would be n bytes long,
 * the string will be reallocated to occupy (n*2+1) bytes. This is often useful because
 * we reduce the probability of future reallocations when the string is manipulated.
 *
 * Returns s1 if the function succeeded or NULL if the eventual reallocation fails. In
 * case of failure the s1 ss is still valid and must be still freed.
 */
ss *ss_concat_raw_len(ss *s1, const char *s2, const size_t s2_len) {
    size_t new_len = s1->len + s2_len;

    if (new_len > s1->cap) {
        // We need to alias the reallocated buffer to
        // avoid overwriting the original pointer in
        // case of failures.
        size_t new_cap = (sizeof(char) * new_len) * 2;
        char *new_buf = _realloc(s1->buf, new_cap + 1);
        if (new_buf == NULL) return NULL;

        s1->buf = new_buf;
        s1->cap = new_cap;
    }

    // Copy the s2 string starting from the
    // last position of the s1 string (/0).
    memcpy(s1->buf + s1->len, s2, s2_len);
    s1->buf[new_len] = END_STRING;
    s1->len = new_len;

    return s1;
}

/*
 * Concatenate a string (s1, ss type) with a null terminated C string (s2, char*). The
 * s2 string is appended to s1, eventually growing the s1 string. Basically, it is a
 * shorthand for str_concat_raw(s1, s1, strlen(s2)).
 *
 * Returns s1 if the function succeeded or NULL if the eventual reallocation fails. In
 * case of failure the s1 str is still valid and must be still freed.
 */
ss *ss_concat_raw(ss *s1, const char *s2) {
    size_t init_len = (s2 == NULL) ? 0 : strlen(s2);
    return ss_concat_raw_len(s1, s2, init_len);
}

/*
 * Concatenate a string (s1, ss type) with another string (s2, ss type). The s2 string is
 * appended to s1, eventually growing the s1 string. Both strings are still valid after the
 * function call and must be freed separately.
 *
 * Returns s1 if the function succeeded or NULL if the eventual reallocation fails.
 */
ss *ss_concat_str(ss *s1, ss *s2) {
    return ss_concat_raw_len(s1, s2->buf, s2->len);
}

/*
 * Removes the characters contained in the cutset string from the start and the end of the
 * string s. After the trimming operation, the string length is reduced while the allocation
 * size (the cap) is left untouched. If all characters are trimmed the result is a valid but
 * empty string.
 *
 * Returns the modified (trimmed) string s.
 */
ss *ss_trim(ss *s, const char *cutset) {
    size_t start = 0;
    size_t end = s->len - 1;

    // Calculate the new boundaries of the string, start and end
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
 * Removes the characters contained in the cutset string from the start of the string s.
 * After the trimming operation, the string length is reduced while the allocation size
 * is left untouched. If all characters are trimmed the result is a valid but empty string.
 *
 * Returns the modified (start-trimmed) s string.
 */
ss *ss_trim_left(ss *s, const char *cutset) {
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
 * Removes the characters contained in the cutset string from the end of the string s.
 * After the trimming operation, the string length is reduced while the allocation size
 * is left untouched. If all characters are trimmed the result is a valid but empty string.
 *
 * Returns the modified (end-trimmed) s string.
 */
ss *ss_trim_right(ss *s, const char *cutset) {
    size_t last = s->len - 1;

    while ((last >= 0) && strchr(cutset, s->buf[last])) {
        if (last == 0) break; // avoid overflow
        last--;
    }
    size_t len = last == 0 ? 0 : last + 1;

    s->buf[len] = END_STRING;
    s->len = len;
    return s;
}

/*
 * Cut the string at the provided length. The string is shortened to contain the
 * first len bytes. The allocation space (cap) is left untouched. The bytes after
 * the len-th one are not cleaned, they are just marked as 'unused'. If
 * length > string length the function is a no-op.
 */
void ss_cut(ss *s, size_t len) {
    if (len > s->len) return;
    s->len = len;
    s->buf[len] = END_STRING;
}

/*
 * Erase the string. The length of the string is set to 0 and a null terminator
 * is written in the first byte of the string buffer. The allocation space (cap)
 * is left untouched, so future string manipulations can be performed with fewer
 * reallocations. The bytes after the len-th one are not cleaned, they are just
 * marked as 'unused'.
 */
void ss_clear(ss *s) {
    s->len = 0;
    s->buf[0] = END_STRING;
}

/*
 * Changes the allocated space for the string. The operation doesn't change the
 * stored string itself, both in the content and the length. It only changes the
 * available (allocated) space beyond the string len. The function is useful
 * to reserve more space earlier in order to avoid frequent reallocations
 * later. If the passed cap argument is smaller than the actual already
 * allocated space, the function returns s without any modification.
 */
ss *ss_set_free_space(ss *s, size_t free_space) {
    size_t new_cap = s->len + free_space;

    char *new_buf = _realloc(s->buf, sizeof(char) * new_cap + 1);
    if (new_buf == NULL) return NULL;

    s->buf = new_buf;
    s->cap = new_cap;
    return s;
}

/*
 * Grow the allocated space (cap) by 'space' bytes. The operation doesn't change the
 * stored string itself, both in the content and the length. It only changes the
 * available (allocated) space beyond the string len. The function is useful
 * to reserve more space earlier in order to avoid frequent reallocations
 */
ss *ss_grow(ss *s, size_t space) {
    return ss_set_free_space(s, (s->cap - s->len) + space);
}

/*
 * Reduce the allocated space (cap) to contain exactly the current string. The
 * allocated string itself is left untouched. It is possible to combine a call
 * to ss_clear with a call to ss_shrink to drastically reduce the amount
 * of allocated space.
 */
ss *ss_shrink(ss *s) {
    return ss_set_free_space(s, 0);
}

/*
 * Deallocate the memory used by a string. The string can't be used after being freed.
 */
void ss_free(ss *s) {
    if (s == NULL || s->buf == NULL) return;
    free(s->buf);
    s->buf = NULL,
    s->len = 0;
    s->cap = 0;
    free(s);
}
