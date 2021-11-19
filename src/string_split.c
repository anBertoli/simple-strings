#include <stdlib.h>
#include <strings.h>
#include "string.h"
#include "string_split.h"
#include "alloc.h"

static ss *concat_to_ss_list(ss *ss_list, int *ss_list_n, ss str);

/*
 * Return all the ss substrings generated from splitting the C string `s` with the delimiter string `del`.
 * All the returned substrings are heap allocated and returned as an array (`*ss`) of length `n`. The array
 * of strings must be freed after use with the dedicated `ss_list_free` function. If the delimiter `del` is
 * empty or it didn't match anywhere in the string `s`, the function returns all the original string (in the
 * form of a ss string). Consecutive matches of the delimiter are treated as a single delimiter so no empty
 * strings are returned in this case. Also, if the delimiter matches at the end of the string `s` no empty
 * string is returned. The value pointed to by the `n` pointer will be the number of strings present in the
 * returned array.
 *
 * Returns an array of strings of length `n` in case of success or NULL in case of allocation failures.
 */
ss *ss_split_raw(const char *s, const char *del, int *n) {
    const int empty_del = strcmp(del, "") == 0;
    const char *curr_ptr = s;
    ss *ss_list = NULL;
    *n = 0;

    while (1) {
        char *match_start = strstr(curr_ptr, del);
        if (match_start == NULL || empty_del) {
            // No more matches or empty delimiter.

            // If the curr_ptr points to the end of the string
            // len == 0. No strings returned and we are done.
            size_t len = strlen(curr_ptr);
            if (len == 0) {
                return ss_list;
            }

            ss str = ss_new_from_raw_len_free(curr_ptr, len, len);
            if (str == NULL) {
                ss_list_free(ss_list, *n);
                *n = 0;
                return NULL;
            }
            ss_list = concat_to_ss_list(ss_list, n, str);
            return ss_list;
        }

        size_t len = match_start - curr_ptr;
        if (len == 0) {
            // Consecutive delimiters, avoid returning empty
            // strings, update the pointer and loop again.
            curr_ptr = match_start + strlen(del);
            continue;
        }

        ss str = ss_new_from_raw_len_free(curr_ptr, len, len);
        if (str == NULL) {
            ss_list_free(ss_list, *n);
            *n = 0;
            return NULL;
        }
        ss_list = concat_to_ss_list(ss_list, n, str);
        if (ss_list == NULL) {
            return NULL;
        }

        // Update the pointer to point to the first
        // char after the matched delimiter.
        curr_ptr = match_start + strlen(del);
    }
}

// Handle the string list memory. The list allocated memory is enlarged with steps
// of length 10 and the count is updated, then the provided string is appended to
// the list. If the reallocation fails the list is freed and n is set to zero.
static ss *concat_to_ss_list(ss *ss_list, int *ss_list_n, ss str) {
    if (*ss_list_n % 10 == 0) {
        ss *new_str_list = ss_realloc(ss_list, sizeof(ss) * (*ss_list_n + 10));
        if (new_str_list == NULL) {
            ss_list_free(ss_list, *ss_list_n);
            *ss_list_n = 0;
            return NULL;
        }
        ss_list = new_str_list;
    }

    // Append the new substring to the list.
    ss_list[*ss_list_n] = str;
    (*ss_list_n)++;
    return ss_list;
}

/*
 * Return all the ss substrings generated from splitting the ss string `s` with the delimiter string `del`.
 * All the returned substrings are heap allocated and returned as an array (`*ss`) of length `n`. The array
 * of strings must be freed after use with the dedicated `ss_list_free` function. If the delimiter `del` is
 * empty or it didn't match anywhere in the string `s`, the function returns all the original string (in the
 * form of a ss string). Consecutive matches of the delimiter are treated as a single delimiter so no empty
 * strings are returned in this case. Also, if the delimiter matches at the end of the string `s` no empty
 * string is returned. The value pointed to by the `n` pointer will be the number of strings present in the
 * returned array. The `s` string is not modified.
 *
 * Returns an array of strings of length `n` in case of success or NULL in case of allocation failures.
 */
ss *ss_split_str(ss s, const char *del, int *n) {
    return ss_split_raw(s->buf, del, n);
}

/*
 * Join an array of C strings `str` of length `n` using the provided string separator `sep` between them.
 * The resulting string is concatenated at the end of the provided `s` string. The `s` string is modified
 * in place and returned.
 *
 * Returns the modified string `s` if case of success or NULL if any reallocation fails. In case of
 * failure the ss string `s` is still valid and must be freed after use.
 */
ss_err ss_join_raw_cat(ss s, const char **str, int n, const char *sep) {
    for (int i = 0; i < n; i++) {
        ss_err err = ss_concat_raw(s, str[i]);
        if (err) return err;

        if (i != n-1) {
            err = ss_concat_raw(s, sep);
            if (err) return err;
        }
    }
    return err_none;
}

/*
 * Join an array of C strings `str` of length `n` using the provided string separator `sep` between them.
 * The resulting (joined) string is then returned as a new ss string. The returned string must be freed
 * after use with the provided `ss_free` function.
 *
 * Returns the joined string in case of success or NULL in case of allocation errors.
 */
ss ss_join_raw(const char **str, int n, const char *sep) {
    ss s1 = ss_new_empty();
    if (s1 == NULL) return NULL;

    ss_err err = ss_join_raw_cat(s1, str, n, sep);
    if (err) {
        ss_free(s1);
        return NULL;
    }
    return s1;
}

/*
 * Join an array of ss strings `str` of length `n` using the provided string separator `sep` between them.
 * The resulting string is concatenated at the end of the provided `s` string. The `s` string is modified
 * in place and returned.
 *
 * Returns the modified string `s` if case of success or NULL if any reallocation fails. In case of
 * failure the ss string `s` is still valid and must be freed after use.
 */
ss_err ss_join_str_cat(ss s, ss *str, int n, const char *sep) {
    for (int i = 0; i < n; i++) {
        ss_err err = ss_concat_str(s, str[i]);
        if (err) return err;

        if (i != n-1) {
            err = ss_concat_raw(s, sep);
            if (err) return err;
        }
    }
    return err_none;
}

/*
 * Join an array of ss strings `str` of length `n` using the provided string separator `sep` between them.
 * The resulting (joined) string is then returned as a new ss string. The returned string must be freed
 * after use with the provided `ss_free` function.
 *
 * Returns the joined string in case of success or NULL in case of allocation errors.
 */
ss ss_join_str(ss *str, int n, const char *sep) {
    ss s1 = ss_new_empty();
    if (s1 == NULL) return NULL;

    ss_err err = ss_join_str_cat(s1, str, n, sep);
    if (err) {
        ss_free(s1);
        return NULL;
    }
    return s1;
}

/*
 * Deallocate the memory used by a ss string array `list`. The string array and all the
 * contained strings can't be used after being freed.
 */
void ss_list_free(ss *list, const int n) {
    for (int i = 0; i < n; i++) ss_free(list[i]);
    free(list);
}

