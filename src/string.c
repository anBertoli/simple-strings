#include <stdlib.h>
#include <strings.h>
#include "string.h"
#include "alloc.h"

/*
 * Build a new string copying the provided init string of length len (the len argument
 * doesn't include the null terminator) and making total space for cap + 1 bytes (plus
 * one for the null terminator). If the length of the init string is greater than len,
 * the exceeding bytes are discarded. If cap < len, cap is adjusted to be equal to len.
 * The caller is responsible for providing the correct values of len, cap and init string.
 * The ss string is heap allocated and a pointer to it is returned. The string must be
 * freed after use with the provided ss_free function. Useful shorthands are the
 * ss_new_raw_len and ss_new_raw functions, which are more ergonomic and easier to use.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 */
ss *ss_new_from_raw_len_cap(const char *init, const size_t len, const size_t cap) {
    size_t _cap = cap;
    if (_cap <= len) _cap = len;

    char *buf = _malloc(sizeof(char) * _cap + 1);
    if (buf == NULL) {
        return NULL;
    }

    memcpy(buf, init, len);
    buf[len] = END_STRING;

    ss *s = _malloc(sizeof(ss));
    if (s == NULL) {
        free(buf);
        return NULL;
    }
    *s = (ss){
        .len = len,
        .cap = _cap,
        .buf = buf
    };

    return s;
}

/*
 * Build a new string copying the provided init string of length len (the len argument
 * doesn't include the null terminator). If the length of the init string is greater than
 * len, the exceeding bytes are discarded. The caller is responsible for providing the
 * correct values of len and init string. The ss string is heap allocated and a pointer
 * to it is returned. The ss must be freed after use with the provided ss_free function.
 * The ss_new_raw_len function is a shorthand for ss_new_raw_len_cap(init, len, len * 2).
 *
 * The returned string has length len, but (len * 2 + 1) bytes are allocated (the cap
 * value) and +1 is added for the null terminator. This overallocation is often useful
 * because it's reduces the probability of future reallocations when the string is
 * manipulated.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 */
ss *ss_new_from_raw_len(const char *init, const size_t len) {
    return ss_new_from_raw_len_cap(init, len, len * 2);
}

/*
 * Build a new string copying the provided null terminated init string. The ss_new_raw
 * function is a shorthand for str_new_raw_len(init, len), where len here is calculated
 * with strlen. If the init pointer is NULL a new empty ss string is built. The returned
 * ss string must be freed after use with the provided ss_free function.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 */
ss *ss_new_from_raw(const char *init) {
    return (init == NULL)
        ? ss_new_empty()
        : ss_new_from_raw_len(init, strlen(init));
}

/*
 * Build and returns a new empty ss string with length and the provided cap. In any case
 * the string always has an implicit null term, so 1 byte is allocated anyway. The ss must
 * be freed after use with the provided ss_free function.
 *
 * Returns the new empty string or NULL if the allocation fails.
 */
ss *ss_new_empty_with_cap(const size_t cap) {
    return ss_new_from_raw_len_cap("", 0, cap);
}

/*
 * Build and returns a new empty ss string with length and cap zero. Even in this case the
 * string always has an implicit null term, so 1 byte is allocated anyway. The ss must be
 * freed after use with the provided ss_free function. Note that usually an empty string
 * is built to be filled, so it could be a better idea in several situations to directly
 * preallocate some space via ss_new_empty_with_cap().
 *
 * Returns the new empty string or NULL if the allocation fails.
 */
ss *ss_new_empty(void) {
    return ss_new_from_raw_len("", 0);
}

/*
 * Build and return a clone of the provided ss string. The new string and the old one are
 * independent and both of them must be freed after use. Cloning a string is useful when,
 * for example, we want to mutate a string while also retaining the original content.
 *
 * Returns the new string in case of success or NULL if the allocation fails.
 */
ss *ss_clone(ss *s) {
    return ss_new_from_raw_len_cap(s->buf, s->len, s->cap);
}

/*
 * Create a new ss string slicing the original provided string s. The original ss string
 * is not mutated. Both the original string and the returned one must be freed after use.
 * The slicing boundaries must be provided via the str_index and end_index (0-indexing)
 * and the resulting range is [str_index, end_index) (right boundary not inclusive). If
 * the str_index is >= of the original string length a new empty string is returned. If
 * the end index is < of str_index a new empty string is returned. If end_index > of the
 * original string length, end_index is reduced to original string length.
 *
 * Returns the substring in case of success or NULL if the allocation fails.
 */
ss *ss_slice(ss *s, const int str_index, const int end_index) {
    if (str_index >= s->len) return ss_new_empty();
    if (end_index < str_index) return ss_new_empty();

    // After this, start index will range from 0 to s->len-1,
    // while end index will range from start index to s->len.
    int _end_index = end_index;
    if (end_index > s->len) _end_index = s->len;

    char *start_string = &s->buf[str_index];
    size_t len = _end_index - str_index;

    return ss_new_from_raw_len_cap(start_string, len, len);
}

/*
 * Grow the string to have the specified length. New bytes inserted will be set to
 * zero. If the specified length is smaller than the current length, the function
 * is a no-op.
 *
 * Returns the enlarged substring in case of success or NULL if any allocation fails.
 */
ss *ss_grow(ss *s, size_t len) {
    if (len <= s->len) return s;

    s = ss_reserve_free_space(s, len - s->len);
    if (s == NULL) return NULL;

    memset(s->buf + s->len, 0, len - s->len);
    s->buf[len] = END_STRING;
    s->len = len;
    return s;
}

/*
 * Cut the string at the provided length len. The string is shortened to contain the
 * first len bytes. The allocation space (cap) is left untouched. The bytes after the
 * number len are not cleaned, they are just marked as 'unused' since they are beyond
 * the length of the string. If the length len is greater than the string s length
 * the function is a no-op.
 */
void ss_cut(ss *s, size_t len) {
    if (len >= s->len) return;
    s->len = len;
    s->buf[len] = END_STRING;
}

/*
 * Erase the string. The length of the string is set to 0 and a null terminator
 * is written in the first byte of the string buffer. The allocation space (cap)
 * is left untouched, so future string manipulations can be performed with fewer
 * reallocations. The bytes after the number len are not cleaned, they are just
 * marked as 'unused' since they are beyond the length of the string.
 */
void ss_clear(ss *s) {
    ss_cut(s, 0);
}



/*
 * Changes the allocated space for the string. The operation doesn't change the
 * stored string itself, both in the content and the length. It only changes the
 * available (allocated) space beyond the string len. The function is useful to
 * reserve more space earlier in order to avoid frequent reallocations later.
 *
 * Returns the ss string s if the function succeeded or NULL if the eventual
 * reallocation fails. In case of failure the ss string s is still valid and
 * must be freed after use.
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
 * Enlarge the free allocated space (the space after the string itself) to at least
 * 'free_space' bytes. The operation doesn't change the stored string buffer itself,
 * both in the content and the length. It only changes the available space beyond the
 * string length. The function is useful to reserve more space earlier in order to
 * avoid frequent reallocations. If enough space is already present the function is
 * a no-op.
 *
 * Returns the ss string s if the function succeeded or NULL if the eventual
 * reallocation fails. In case of failure the ss string s is still valid and
 * must be freed after use.
 */
ss *ss_reserve_free_space(ss *s, size_t free_space) {
    if (s->cap - s->len >= free_space) return s;
    return ss_set_free_space(s, free_space);
}


/*
 * Deallocate the memory used by a ss string s. The string can't be used after
 * being freed.
 */
void ss_free(ss *s) {
    if (s == NULL || s->buf == NULL) return;
    free(s->buf);
    s->buf = NULL,
    s->len = 0;
    s->cap = 0;
    free(s);
}









/*
 * Returns the position (0-indexed) of the first occurrence of the substring needle in the
 * ss string provided as first argument. Returns -1 if no occurrence is found.
 */
int ss_index(ss *s, const char *needle) {
    char *sub_ptr = strstr(s->buf, needle);
    return sub_ptr != NULL
        ? sub_ptr - s->buf
        : -1;
}








/*
 * Concatenate a ss string s1 with a C string s2 of length s2_len. If the length of the C
 * string is greater than s2_len, the exceeding bytes are discarded. The s2 C string is
 * appended to the ss string s1, eventually growing s1. The strategy used in concat
 * functions is: if the string s1 has enough allocated space to contain also the string s2
 * the content is simply appended, otherwise the s1 string will be grown in order to have
 * a capacity (cap) of (2*n + 1) bytes, where n is the resulting (concatenated) string length.
 *
 * Returns the concatenated string s1 if the function succeeded or NULL if the eventual
 * reallocation fails. In case of failure the string s1 is still valid and must be
 * freed after use.
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
 * Concatenate a ss string s1 with a null terminated C string s2. The s2 string is
 * appended to s1, eventually growing the s1 string. Basically, it is a shorthand for
 * str_concat_raw(s1, s1, strlen(s2)).
 *
 * Returns the concatenated string s1 if the function succeeded or NULL if the eventual
 * reallocation fails. In case of failure the ss string s1 is still valid and must be
 * freed after use.
 */
ss *ss_concat_raw(ss *s1, const char *s2) {
    return s2 != NULL
        ? ss_concat_raw_len(s1, s2, strlen(s2))
        : s1;
}

/*
 * Concatenate a ss string s1 with another ss string s2. The s2 string is appended to
 * s1, eventually growing the s1 string. Both strings are still valid after the
 * function call and must be freed separately.
 *
 * Returns the concatenated string s1 if the function succeeded or NULL if the eventual
 * reallocation fails. In case of failure the ss string s1 is still valid and must be
 * freed after use.
 */
ss *ss_concat_str(ss *s1, ss *s2) {
    return ss_concat_raw_len(s1, s2->buf, s2->len);
}








/*
 * Removes characters contained in the cutset string from both the start and the end of the
 * ss string s. After the trimming operation, the string length is reduced while the allocation
 * size (the capacity cap) is left untouched. If all characters are trimmed the result is a
 * valid but empty ss string.
 */
void ss_trim(ss *s, const char *cutset) {
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
}

/*
 * Removes characters contained in the cutset string from the start of the ss string s.
 * After the trimming operation, the string length is reduced while the allocation size
 * (the capacity cap) is left untouched. If all characters are trimmed the result is a
 * valid but empty string.
 */
void ss_trim_left(ss *s, const char *cutset) {
    size_t start = 0;
    size_t last = s->len - 1;

    while ((start <= last) && strchr(cutset, s->buf[start])) start++;
    size_t len = start > last ? 0 : (last - start + 1);
    if (start != 0) {
        memmove(s->buf, s->buf + start, len);
    }

    s->buf[len] = END_STRING;
    s->len = len;
}

/*
 * Removes characters contained in the cutset string from the end of the ss string s.
 * After the trimming operation, the string length is reduced while the allocation
 * size is left untouched. If all characters are trimmed the result is a valid but
 * empty ss string.
 */
void ss_trim_right(ss *s, const char *cutset) {
    size_t last = s->len - 1;

    while ((last >= 0) && strchr(cutset, s->buf[last])) {
        if (last == 0) break; // avoid overflow
        last--;
    }
    size_t len = last == 0 ? 0 : last + 1;

    s->buf[len] = END_STRING;
    s->len = len;
}





