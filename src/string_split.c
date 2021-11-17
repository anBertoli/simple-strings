#include <stdlib.h>
#include <strings.h>
#include "string.h"
#include "string_split.h"
#include "alloc.h"

/*
 * Return all the ss substrings generated from splitting the C string `s` with the delimiter string `del`.
 * The function is useful when the caller doesn't want/need to create a string iterator and collect
 * substrings manually. All returned substrings are heap allocated and returned as an array (`*ss`) of
 * length `n`. The array of strings must be freed after use with the dedicated
 * [`ss_list_free`](#ss_list_free) function.
 *
 * Returns an array of strings of length `n` in case of success or NULL in case of allocation failures.
 */
ss *ss_split_raw(const char *s, const char *del, int *n) {
    ss *ss_list = NULL;
    *n = 0;

    // Guard against empty delimiters. Return all
    // the original string and flag the iterator.
    if (strcmp(del, "") == 0) {
        size_t len = strlen(s);
        ss str = ss_new_from_raw_len_free(s, len, len);
        if (str == NULL) return NULL;

        ss_list = ss_malloc(sizeof(ss));
        if (ss_list == NULL) {
            ss_free(str);
            return NULL;
        }
        ss_list[0] = str;
        *n = 1;

        return ss_list;
    }

    // Main loop, split the original string and
    // collect a substring at each iteration.
    const char *curr_ptr = s;

    while (1) {
        char *end = strstr(curr_ptr, del);
        if (end == NULL) {
            size_t len = strlen(curr_ptr);
            if (len == 0) {
                // The ptr points to the end of the string, flag
                // the iterator as exhausted and return END_ITER.
                return ss_list;
            }

            // Return the last substring and flag the iterator
            // as exhausted (next call will return END_ITER).
            ss str = ss_new_from_raw_len_free(curr_ptr, len, len);
            if (str == NULL) {
                ss_list_free(ss_list, *n);
                *n = 0;
                return NULL;
            }

            ss_list = append_to_ss_list(ss_list, n, str);
            return ss_list;
        }

        size_t len = end - curr_ptr;
        if (len == 0) {
            // Consecutive delimiters, avoid returning empty strings.
            // Update the pointer to point to the first char after
            // the delimiter and loop again.
            curr_ptr = end + strlen(del);
            continue;
        }

        ss str = ss_new_from_raw_len_free(curr_ptr, len, len);
        if (str == NULL) {
            ss_list_free(ss_list, *n);
            *n = 0;
            return NULL;
        }

        ss_list = append_to_ss_list(ss_list, n, str);
        if (ss_list == NULL) {
            return NULL;
        }

        curr_ptr = end + strlen(del);
    }
}

static ss *append_to_ss_list(ss *ss_list, int *ss_list_n, ss str) {
    // The allocated string list memory is enlarged with steps of length 10.
    // If th reallocation fails the list is freed and n set to zero.
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
 * The function is useful when the caller doesn't want/need to create a string iterator and collect
 * substrings manually. All returned substrings are heap allocated and returned as an array (`*ss`) of
 * length `n`. The array of strings must be freed after use with the dedicated
 * [`ss_list_free`](#ss_list_free) function.
 *
 * Returns an array of strings of length `n` in case of success or NULL in case of allocation failures.
 */
ss *ss_split_str(ss s, const char *del, int *n) {
    return ss_split_raw(s->buf, del, n);
}

/*
 * Join an array of C strings `s` of length `n` using the provided string separator `sep` between them.
 * The returned string must be freed after use with the [`ss_free`](#ss_free) function.
 *
 * Returns the joined string in case of success or NULL in case of allocation errors.
 */
ss ss_join_raw(char **s, int n, const char *sep) {
    ss join = ss_new_empty();
    ss join_alias = join;
    if (join == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        join = ss_concat_raw(join, s[i]);
        if (join == NULL) {
            ss_free(join_alias);
            return NULL;
        }

        if (i != n-1) {
            join = ss_concat_raw(join, sep);
            if (join == NULL) {
                ss_free(join_alias);
                return NULL;
            }
        }
    }

    return join;
}

/*
 * Join an array of ss strings `s` of length `n` using the provided string separator `sep` between them.
 * The returned string must be freed after use with the [`ss_free`](#ss_free) function.
 *
 * Returns the joined string in case of success or NULL in case of allocation errors.
 */
ss ss_join_str(ss *s, int n, const char *sep) {
    ss join = ss_new_empty();
    ss join_alias = join;
    if (join == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        join = ss_concat_str(join, s[i]);
        if (join == NULL) {
            ss_free(join_alias);
            return NULL;
        }

        if (i != n-1) {
            join = ss_concat_raw(join, sep);
            if (join == NULL) {
                ss_free(join_alias);
                return NULL;
            }
        }
    }

    return join;
}

/*
 * Deallocate the memory used by a ss string array `list`. The string array and all the
 * contained strings can't be used after being freed.
 */
void ss_list_free(ss *list, const int n) {
    for (int i = 0; i < n; i++) ss_free(list[i]);
    free(list);
}

