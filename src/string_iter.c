#include <stdlib.h>
#include <strings.h>
#include "string.h"
#include "string_iter.h"
#include "alloc.h"

/*
 * Build a new string iterator from a raw string `s` and a string delimiter `del`. The string
 * iterator is used to split the string `s` into substrings, each of them separated from the
 * others by the occurrences of the delimiter `del`. The string iterator should be used with
 * the [`ss_iter_next`](#ss_iter_next) function to yield subsequent substring. The string
 * iterator is heap allocated along with a copy of the passed string. The string iterator
 * must be freed after use with the [`ss_iter_free`](#ss_iter_free) function.
 *
 * Returns a string iterator or NULL in case of allocation failures.
 */
ss_iter ss_split_raw_to_iter(const char *s, const char *del) {
    char *buf = ss_malloc(sizeof(char) * strlen(s) + 1);
    if (buf == NULL) return NULL;
    strcpy(buf, s);

    char *buf_del = ss_malloc(sizeof(char) * strlen(del) + 1);
    if (buf_del == NULL) {
        free(buf);
        return NULL;
    }
    strcpy(buf_del, del);

    ss_iter iter = ss_malloc(sizeof(struct si));
    if (iter == NULL) {
        free(buf);
        free(buf_del);
        return NULL;
    }
    *iter = (struct si){
        .buf = buf,
        .ptr = buf,
        .del = buf_del,
        .end = ITER_ACTIVE,
    };

    return iter;
}

/*
 * Build a new string iterator from a raw string `s` and a string delimiter `del`. The string
 * iterator is used to split the string `s` into substrings, each of them separated from the
 * others by the occurrences of the delimiter `del`. The string iterator should be used with
 * the [`ss_iter_next`](#ss_iter_next) function to yield subsequent substring. The string
 * iterator is heap allocated along with a copy of the passed string. The string iterator
 * must be freed after use with the [`ss_iter_free`](#ss_iter_free) function.
 *
 * Returns a string iterator or NULL in case of allocation failures.
 */
ss_iter ss_split_str_to_iter(ss s, const char *del) {
    return ss_split_raw_to_iter(s->buf, del);
}

/*
 * Advances the string iterator `iter` and returns the next substring. The returned substring
 * starts at the end of the previous delimiter (or the start of the original string in the first
 * iteration) and ends at the start of the next delimiter (or at the end of the original string
 * in the last iteration). The returned ss substrings are heap allocated and must be freed after
 * use as usual (with the [`ss_free`](#ss_free) function). If no delimiters are present in the
 * string or the iterator contains an empty delimiter all the original string is returned. Multiple
 * consecutive delimiters are treated as one delimiter, and in general no empty strings are returned
 * from this function. When the iterator is exhausted the function returns END_ITER. The caller can
 * test for this condition to stop the iteration and free it.
 *
 * Returns a ss string in case of success, NULL in case of allocation failures or END_ITER when
 * the iterator is exhausted. In case of errors the iterator `iter` is partially (or totally) consumed
 * but still valid. It could be used again from that point, but must be freed after use in any case.
 */
ss ss_iter_next(ss_iter iter) {
    if (iter->end == ITER_EXHAUS) {
        return END_ITER;
    }

    // Guard against empty delimiters. Return all the original
    // string and flag the iterator as ended (next call will
    // return END_ITER).
    if (strcmp(iter->del, "") == 0) {
        size_t len = strlen(iter->ptr);
        ss s = ss_new_from_raw_len_cap(iter->ptr, len, len);
        if (s == NULL) {
            return NULL;
        }

        iter->end = ITER_EXHAUS;
        return s;
    }

    // Main loop, split the original string and return a
    // substring at each iteration.
    while (1) {
        char *end = strstr(iter->ptr, iter->del);
        if (end == NULL) {
            size_t len = strlen(iter->ptr);
            if (len == 0) {
                // The ptr points to the end of the string, flag
                // the iterator as exhausted and return END_ITER.
                iter->end = ITER_EXHAUS;
                return END_ITER;
            }

            // Return the last substring and flag the iterator
            // as exhausted (next call will return END_ITER).
            ss s = ss_new_from_raw_len_cap(iter->ptr, len, len);
            if (s == NULL) {
                return NULL;
            }

            iter->end = ITER_EXHAUS;
            return s;
        }

        size_t len = end - iter->ptr;
        if (len == 0) {
            // Consecutive delimiters, avoid returning empty strings.
            // Update the pointer to point to the first char after
            // the delimiter and loop again.
            iter->ptr = end + strlen(iter->del);
            continue;
        }

        // Build the substring and update the pointer to
        // point to the first char after the delimiter.
        ss s = ss_new_from_raw_len_cap(iter->ptr, len, len);
        if (s == NULL) {
            return NULL;
        }

        iter->ptr = end + strlen(iter->del);
        return s;
    }
}

/*
 * Collect and return all the ss substrings that the ss_iter `iter` could produce. The function is
 * useful when the caller doesn't want/need to iterate and collect substrings manually;
 * [`ss_split_raw`](#ss_split_raw) and [`ss_split_str`](#ss_split_str) have similar goals but this
 * function is more flexible since it could also accept iterators partially consumed. All returned
 * substrings are heap allocated and returned as an array (`*ss`) of length `n`. The array of strings
 * must be freed after use with the dedicated [`ss_list_free`](#ss_list_free) function.
 *
 * Returns an array of strings of length `n` in case of success or NULL in case of allocation failures.
 * In case of errors the iterator `iter` is partially (or totally) consumed but still valid. It could be
 * used again from that point, but must be freed after use in any case.
 */
ss *ss_iter_collect(ss_iter iter, int *n) {
    *n = 0;
    ss *str_list = ss_malloc(sizeof(ss));
    if (str_list == NULL) {
        return NULL;
    }

    // Loop until the iterator is exhausted. If END_ITER is returned, the
    // iterator was already freed, so we can just break and return the
    // strings list.
    while (1) {
        ss str_next = ss_iter_next(iter);
        if (str_next == END_ITER) break;
        if (str_next == NULL) {
            ss_list_free(str_list, *n);
            *n = 0;
            return NULL;
        }

        // The allocated string list memory is
        // enlarged with steps of length 20.
        if (*n % 20 == 0) {
            ss *new_str_array = ss_realloc(str_list, sizeof(ss) * (*n + 20));
            if (new_str_array == NULL) {
                ss_list_free(str_list, *n);
                *n = 0;
                return NULL;
            } else {
                str_list = new_str_array;
            }
        }

        // Append the new substring to the list.
        str_list[*n] = str_next;
        (*n)++;
    }

    return str_list;
}

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
    ss_iter iter = ss_split_raw_to_iter(s, del);
    if (iter == NULL) return NULL;

    ss *str = ss_iter_collect(iter, n);
    if (str == NULL) {
        ss_iter_free(iter);
        return NULL;
    }

    return str;
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
    ss_iter iter = ss_split_str_to_iter(s, del);
    if (iter == NULL) return NULL;

    ss *str = ss_iter_collect(iter, n);
    if (str == NULL) {
        ss_iter_free(iter);
        return NULL;
    }

    return str;
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
    if (join == NULL) return NULL;

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
 * Deallocate the memory used by a string iterator `iter`. The string iterator can't be
 * used after being freed.
 */
void ss_iter_free(ss_iter iter) {
    free(iter->buf);
    free(iter->del);
    iter->buf = NULL;
    iter->ptr = NULL;
    iter->del = NULL;
    iter->end = ITER_EXHAUS;
    free(iter);
}

/*
 * Deallocate the memory used by a ss string array `list`. The string array and all the
 * contained strings can't be used after being freed.
 */
void ss_list_free(ss *list, const int n) {
    for (int i = 0; i < n; i++) ss_free(list[i]);
    free(list);
}

