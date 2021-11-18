#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string.h"
#include "alloc.h"

/*
 * Build a new string copying the provided `init` string of length `len` (the length argument doesn't
 * include the null terminator) and allocating additional `avail` bytes. If the length of the initial
 * string is greater than `len`, the exceeding bytes are discarded. Even with `len` and `avail` both
 * equal to zero, one byte is allocated anyway (for the null terminator). The caller is responsible for
 * providing the correct values of the three arguments. If the `init` string is NULL a new empty ss
 * string is built. The newly created string is heap allocated and a pointer to it is returned. The
 * string must be freed after use with the provided `ss_free` function. Useful shorthands are the
 * `ss_new_from_raw_len` and `ss_new_from_raw` functions, which are more ergonomic and easier to use.
 *
 * The returned string has length `len`, but additional `avail` bytes are allocated. This overallocation
 * is useful because it's reduces the probability of future reallocations when the string is
 * manipulated.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 */
ss ss_new_from_raw_len_free(const char *init, size_t len, size_t avail) {
    if (init == NULL) return ss_new_empty();

    char *buf = ss_malloc(sizeof(char) * (len + avail + 1));
    if (buf == NULL) {
        return NULL;
    }

    memcpy(buf, init, len);
    buf[len] = END_STRING;

    ss s = ss_malloc(sizeof(struct s));
    if (s == NULL) {
        free(buf);
        return NULL;
    }
    *s = (struct s){
        .len = len,
        .free = avail,
        .buf = buf
    };

    return s;
}

/*
 * Build a new string copying the provided `init` string of length `len` (the length argument
 * doesn't include the null terminator). If the length of the initial string is greater than the
 * provided `len`, the exceeding bytes are discarded. The caller is responsible for providing
 * valid values for the arguments. If the `init` string is NULL a new empty ss string is built.
 * The new ss string is heap allocated and a pointer to it is returned. The function is basically
 * a shorthand for `ss_new_raw_len_cap(init, len, len)`. The string must be freed after use with
 * the provided `ss_free` function.
 *
 * The returned string has length `len`, but additional `len` bytes are allocated. This overallocation
 * is useful because it's reduces the probability of future reallocations when the string is
 * manipulated.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 */
ss ss_new_from_raw_len(const char *init, size_t len) {
    if (init == NULL) return ss_new_empty();
    return ss_new_from_raw_len_free(init, len, len);
}

/*
 * Build a new string copying the provided null terminated `init` C string. This function is a
 * shorthand for `str_new_raw_len(init, strlen(init), strlen(init))`. If the `init` string is
 * NULL a new empty ss string is built. The returned string must be freed after use with the
 * provided `ss_free` function.
 *
 * The returned string has length `strlen(init)`, but additional `strlen(init)` bytes are allocated.
 * This overallocation is useful because it's reduces the probability of future reallocations when
 * the string is manipulated.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 */
ss ss_new_from_raw(const char *init) {
    if (init == NULL) return ss_new_empty();
    size_t len = strlen(init);
    return ss_new_from_raw_len_free(init, len, len);
}

/*
 * Build and returns a new empty ss string with length zero and `avail` bytes allocated (+ 1
 * for the null terminator). In any case the string has an implicit null term, so 1 byte is
 * allocated anyway. The ss must be freed after use with the provided `ss_free` function.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 */
ss ss_new_empty_with_free(size_t avail) {
    return ss_new_from_raw_len_free("", 0, avail);
}

/*
 * Build and returns a new empty ss string with length and no free space. Even in this case the
 * string always has an implicit null term, so 1 byte is allocated anyway. The ss must be freed
 * after use with the provided `ss_free` function. Note that usually an empty string is built to
 * be filled, so it could be more convenient to directly preallocate some space via the
 * `ss_new_empty_with_cap` function.
 *
 * Returns the newly generated string or NULL if the allocation fails.
 */
ss ss_new_empty(void) {
    return ss_new_from_raw_len("", 0);
}

/*
 * Build and return a clone of the provided ss string `s`. The new string and the old one are
 * independent and both of them must be freed after use with the `ss_free` function. Cloning a
 * string is useful when, for example, we want to mutate a string while also retaining the
 * original content. The string `s` is not modified.
 *
 * Returns the cloned string or NULL if the allocation fails.
 */
ss ss_clone(ss s) {
    return ss_new_from_raw_len_free(s->buf, s->len, s->free);
}

/*
 * Sets the allocated space not already used for the string `s` to be equal to `avail` bytes. The
 * operation doesn't change the stored string itself, both in the content and the length. It only
 * changes the allocated available space to be used for future operations on the string. The function
 * is useful for example to reserve more space earlier in order to avoid frequent reallocations later.
 * The string `s` is modified in place.
 *
 * Returns the modified string `s` if case of success or NULL if any reallocation fails. In case of
 * failure the ss string `s` is still valid and must be freed after use.
 */
ss ss_set_free_space(ss s, size_t avail) {
    size_t new_space = s->len + 1 + avail;

    char *new_buf = ss_realloc(s->buf, sizeof(char) * new_space);
    if (new_buf == NULL) {
        return NULL;
    }

    s->buf = new_buf;
    s->free = avail;
    return s;
}

/*
 * Enlarge the allocated space not already used by the string `s` to be at least `avail` bytes long. The
 * operation doesn't change the stored string, it only changes the available space beyond the string end.
 * The function is useful to reserve more space earlier in order to avoid frequent reallocations in the
 * future. If enough space is already present the function is a no-op. The string `s` is modified in place.
 *
 * Returns the modified string `s` if case of success or NULL if any reallocation fails. In case of failure
 * the ss string `s` is still valid and must be freed after use.
 */
ss ss_reserve_free_space(ss s, size_t avail) {
    if (s->free >= avail) return s;
    return ss_set_free_space(s, avail);
}

/*
 * Deallocate the memory used by the ss string `s`. The string can't be used after being freed
 * since it will point to deallocated memory.
 */
void ss_free(ss s) {
    if (s == NULL || s->buf == NULL) return;
    free(s->buf);
    s->buf = NULL,
    s->len = 0;
    s->free = 0;
    free(s);
}

/*
 * Grow the `s` string to have the specified length `len`. Note that here the function enlarges
 * the string buffer itself, and eventually the allocates space. New bytes inserted will be set
 * to zero and they will be safe to be written. If the specified length `len` is smaller than the
 * current length, the function is a no-op. The string `s` is modified in place.
 *
 * Returns the modified ss string `s` if case of success or NULL if any reallocation fails. In
 * case of failure the ss string `s` is still valid and must be freed after use.
 */
ss ss_grow(ss s, size_t len) {
    if (len <= s->len) return s;

    s = ss_reserve_free_space(s, len - s->len);
    if (s == NULL) {
        return NULL;
    }

    memset(s->buf + s->len, 0, len - s->len);
    s->free -= len - s->len;
    s->buf[len] = END_STRING;
    s->len = len;
    return s;
}

/*
 * Shrink the string `s` to have the provided length `len`. The string is shortened to contain only the
 * first `len` bytes and a null termination character is written at the position `len` of the string buffer.
 * The total allocated space is left untouched, while the free space grows. The bytes in the string buffer
 * after the `len`-th one are not cleaned, they are just considered unused. If the length `len` is greater
 * than the current string length the function is a no-op. The string `s` is modified in place.
 */
void ss_shrink(ss s, size_t len) {
    if (len >= s->len) return;
    s->free += s->len - len;
    s->buf[len] = END_STRING;
    s->len = len;
}

/*
 * Erase the content of the string `s`. The length of the string is set to 0 and a null termination
 * character is written in the first position of the string buffer. The total allocation space is
 * left untouched, while the free space grows. The old bytes are not cleaned, they are just considered
 * unused. It is a shorthand for `ss_shrink(s, 0)`. The string `s` is modified in place.
 */
void ss_clear(ss s) {
    ss_shrink(s, 0);
}

/*
 * Returns the position (0-indexed) of the starting position of the first occurrence of the substring
 * `needle` in the ss string `haystack` provided as first argument. Returns -1 if no occurrence is
 * found or if `needle` is NULL or an empty string. The string `haystack` is not modified.
 */
size_t ss_index(ss s, const char *needle) {
    if (needle == NULL || *needle == '\0') return -1;
    char *sub_ptr = strstr(s->buf, needle);
    return sub_ptr != NULL
        ? sub_ptr - s->buf
        : -1;
}

/*
 * Returns the position (0-indexed) of the starting position of the last occurrence of the substring
 * `needle` in the ss string `haystack` provided as first argument. Returns -1 if no occurrence is
 * found or if `needle` is NULL or an empty string. The string `haystack` is not modified.
 */
size_t ss_index_last(ss haystack, const char *needle) {
    if (needle == NULL || *needle == '\0') return -1;

    char *str_ptr = haystack->buf;
    char *result = NULL;
    while (1) {
        char *pos = strstr(str_ptr, needle);
        if (pos == NULL) break;
        result = pos;
        str_ptr = pos + 1;
    }
    if (result == NULL) {
        return -1;
    }

    return result - haystack->buf;
}




/*
 * Concatenate the string `s1` with a C string `s2` of length `s2_len`. If the length of the C string
 * is greater than `s2_len`, exceeding bytes are discarded. The `s2` C string is appended to the string
 * `s1`, eventually growing the allocated space for `s1`. The strategy used in concat functions is the
 * following: if the string `s1` has enough allocated space to contain also the string `s2` the content
 * of the latter is simply appended, otherwise the `s1` string will be grown in order to have total
 * allocated space equal to (2*n + 1) bytes, where n is the resulting (concatenated) string length. In
 * this case, both the final `len` and the free space `free` will be equal to n. The string `s1` is
 * modified in place.
 *
 * Returns the string `s1` concatenated with the C string `s2` if case of success or NULL if eventual
 * reallocations fail. In case of failure the string `s1` is still valid and must be freed after use.
 */
ss ss_concat_raw_len(ss s1, const char *s2, size_t s2_len) {
    size_t new_len = s1->len + s2_len;

    if (s2_len > s1->free) {
        // We need to alias the reallocated buffer to
        // avoid overwriting the original pointer in
        // case of failures.
        size_t new_alloc = new_len * 2 + 1;
        char *new_buf = ss_realloc(s1->buf, sizeof(char) * new_alloc);
        if (new_buf == NULL) {
            return NULL;
        }

        s1->free = new_alloc - 1 - s1->len;
        s1->buf = new_buf;
    }

    // Copy the s2 string starting the copy at
    // the last position of the s1 string (/0).
    memcpy(s1->buf + s1->len, s2, s2_len);
    s1->buf[new_len] = END_STRING;
    s1->len = new_len;
    s1->free -= s2_len;

    return s1;
}

/*
 * Concatenate the string `s1` with a null terminated C string `s2`. The `s2` string is appended to
 * s1, eventually growing the allocated space for `s1`. If the `s2` string is NULL the function
 * simply returns `s1`. The string `s1` is modified in-place. Basically, it is a shorthand for
 * `str_concat_raw_len(s1, s1, strlen(s2))` (see that function to have more info about the concat
 * mechanism).
 *
 * Returns the string `s1` concatenated with the C string `s2` if case of success or NULL if eventual
 * reallocations fail. In case of failure the string `s1` is still valid and must be freed after use.
 */
ss ss_concat_raw(ss s1, const char *s2) {
    if (s2 == NULL) return s1;
    return ss_concat_raw_len(s1, s2, strlen(s2));
}

/*
 * Concatenate the string `s1` with a second ss string `s2`. The `s2` string is appended to s1,
 * eventually growing the allocated space for `s1`. The string `s1` is modified in-place. Basically
 * it is a shorthand for `ss_concat_raw_len(s1, s2->buf, s2->len)` (see that function to have more
 * info about the concat mechanism).Both strings are still valid after the function call and must
 * be freed separately.
 *
 * Returns the string `s1` concatenated with the C string `s2` if case of success or NULL if eventual
 * reallocations fail. In case of failure both strings are still valid and must be freed after use.
 */
ss ss_concat_str(ss s1, ss s2) {
    return ss_concat_raw_len(s1, s2->buf, s2->len);
}

/*
 * Prepend a C string `s1` of length `s1_len` to a ss string `s2`. If the length of the C string is
 * greater than `s1_len`, exceeding bytes are discarded. The `s1` C string is prepended to the ss
 * string `s2`, eventually growing the allocated space for the latter. The strategy used in prepend
 * functions is the following: if the string `s2` has enough allocated space to contain also the string
 * `s1` the content is simply prepended, otherwise the `s2` string will be grown in order to have
 * allocated space of (2*n + 1) bytes, where n is the resulting string length. In this case, both the
 * final `len` and the free space `free` will be equal to n. The string `s2` is modified in place.
 *
 * Returns the string `s2` with the C string `s1` prepended in case of success or NULL if eventual
 * reallocations fail. In case of failure the string `s2` is still valid and must be freed after use.
 */
ss ss_prepend_raw_len(const char *s1, ss s2, size_t s1_len) {
    size_t new_len = s2->len + s1_len;

    if (s1_len > s2->free) {
        // We need to alias the reallocated buffer to
        // avoid overwriting the original pointer in
        // case of failures.
        size_t new_alloc = new_len * 2 + 1;
        char *new_buf = ss_realloc(s2->buf, sizeof(char) * new_alloc);
        if (new_buf == NULL) {
            return NULL;
        }

        s2->free = new_alloc - 1 - s2->len;
        s2->buf = new_buf;
    }

    // Make space for the string to prepend, then
    // copy that string at the beginning.
    memmove(s2->buf + s1_len, s2->buf, s2->len);
    memcpy(s2->buf, s1, s1_len);
    s2->buf[new_len] = END_STRING;
    s2->len = new_len;
    s2->free -= s1_len;

    return s2;
}

/*
 * Prepend the null terminated C string `s1` to a ss string `s2`. The `s1` string is prepended to `s2`,
 * eventually growing the allocated space of `s2`. The string `s2` is modified in-place. Basically, it
 * is a shorthand for `ss_prepend_raw_len(s1, s2, strlen(s1))` (see that function to have more info
 * about the concat mechanism). If the `s1` string is NULL the function simply returns `s2`.
 *
 * Returns the string `s2` prepended with the C string `s1` if case of success or NULL if eventual
 * reallocations fail. In case of failure the string `s2` is still valid and must be freed after use.
 */
ss ss_prepend_raw(const char *s1, ss s2) {
    if (s1 == NULL) return s2;
    return ss_prepend_raw_len(s1, s2, strlen(s1));
}

/*
 * Prepend the ss string `s1` to another ss string `s2`. The `s1` string is appended to `s2`, eventually
 * growing the allocated space of `s2`. Both strings are still valid after the function call and must be
 * freed separately. The string `s2` is modified in-place. Basically, it is a shorthand for
 * `ss_prepend_raw_len(s1->buf, s2, s1->len)` (see that function to have more info about the concat
 * mechanism).
 *
 * Returns the string `s2` prepended with the C string `s1` if case of success or NULL if eventual
 * reallocations fail. In case of failure both strings are still valid and must be freed after use.
 */
ss ss_prepend_str(ss s1, ss s2) {
    return ss_prepend_raw_len(s1->buf, s2, s1->len);
}

/*
 * Obtain a substring of the ss string `s` slicing it with the provided indexes. The slicing boundaries
 * are provided via the `str_index` and `end_index` arguments (0-indexed) and the resulting substring
 * starts from the position `str_index` (inclusive) and ends at `end_index` (not inclusive). If the
 * `str_index` is >= of the original string length no changes are made. If the end index is < of `str_index`
 * no changes are made. If `end_index` > of the original string length, `end_index` is reduced to be equal
 * to the original string length before slicing the string. The string `s` is modified in place.
 */
void ss_slice(ss s, size_t str_index, size_t end_index) {
    if (str_index >= s->len) return;
    if (end_index < str_index) return;

    // After this, start index will range from 0 to s->len-1,
    // while end index will range from start_index to s->len.
    size_t _end_index = end_index;
    if (end_index > s->len) _end_index = s->len;

    size_t len = _end_index - str_index;
    memmove(s->buf, &s->buf[str_index], len);
    s->free += s->len - len;
    s->buf[len] = END_STRING;
    s->len = len;
}

/*
 * Removes characters contained in the `cutset` string from both the start and the end of the
 * ss string `s`. After the trimming operation, the string length is reduced while the allocation
 * size is left untouched, but more free space will be available. If all characters are trimmed
 * the result is a valid but empty string. The string `s` is modified in place.
 */
void ss_trim(ss s, const char *cutset) {
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

    s->free += s->len - len;
    s->buf[len] = END_STRING;
    s->len = len;
}

/*
 * Removes characters contained in the `cutset` string from the start of the ss string `s`.
 * After the trimming operation, the string length is reduced while the allocation size
 * is left untouched, but more free space will be available. If all characters are trimmed
 * the result is a valid but empty string. The string `s` is modified in place.
 */
void ss_trim_left(ss s, const char *cutset) {
    size_t start = 0;
    size_t last = s->len - 1;

    while ((start <= last) && strchr(cutset, s->buf[start])) start++;
    size_t len = start > last ? 0 : (last - start + 1);
    if (start != 0) {
        memmove(s->buf, s->buf + start, len);
    }

    s->free += s->len - len;
    s->buf[len] = END_STRING;
    s->len = len;
}

/*
 * Removes characters contained in the `cutset` string from the end of the ss string `s`.
 * After the trimming operation, the string length is reduced while the allocation size
 * is left untouched, but more free space will be available. If all characters are trimmed
 * the result is a valid but empty string. The string `s` is modified in place.
 */
void ss_trim_right(ss s, const char *cutset) {
    size_t last = s->len - 1;

    while ((last >= 0) && strchr(cutset, s->buf[last])) {
        if (last == 0) break; // avoid overflow
        last--;
    }
    size_t len = last == 0 ? 0 : last + 1;

    s->free += s->len - len;
    s->buf[len] = END_STRING;
    s->len = len;
}

/*
 * Modify the ss string `s` by turning each character into its lowercase version.
 * Modifies the string in place.
 */
void ss_to_lower(ss s) {
    for (int i = 0; i < s->len; i++) {
        s->buf[i] = tolower(s->buf[i]);
    }
}

/*
 * Modify the ss string `s` by turning each character into its uppercase version.
 * Modifies the string in place.
 */
void ss_to_upper(ss s) {
    for (int i = 0; i < s->len; i++) {
        s->buf[i] = toupper(s->buf[i]);
    }
}

