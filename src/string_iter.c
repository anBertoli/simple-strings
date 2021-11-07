#include <stdlib.h>
#include <strings.h>
#include "string.h"
#include "string_iter.h"
#include "alloc.h"

/*
 * Build a new string iterator from a raw string and a string delimiter. The string
 * iterator is used to split the string into substrings, each of them separated from
 * others by occurrences of the delimiter. The string iterator should be used with
 * the ss_iter_next function in order to yield subsequent substring.
 *
 * The string iterator is heap allocated along with a copy of the passed string. The
 * string iterator must be freed after use, but note that if a call to ss_iter_next
 * returns END_ITER (i.e. iterator exhausted) the string iter has been automatically
 * freed.
 *
 * Returns ss_iter* or NULL in case of allocation failures.
 */
ss_iter *ss_split_raw_to_iter(const char *s, const char *del) {
    char *buf = _malloc(sizeof(char) * strlen(s) + 1);
    if (buf == NULL) return NULL;
    strcpy(buf, s);

    char *buf_del = _malloc(sizeof(char) * strlen(del) + 1);
    if (buf_del == NULL) {
        free(buf);
        return NULL;
    }
    strcpy(buf_del, del);

    ss_iter *s_iter = _malloc(sizeof(ss_iter));
    if (s_iter == NULL) {
        free(buf);
        free(buf_del);
        return NULL;
    };
    *s_iter = (ss_iter){
        .buf = buf,
        .ptr = buf,
        .del = buf_del,
        .end = ITER_ACTIVE,
    };

    return s_iter;
}

/*
 * Build a new string iterator from a string and a string delimiter. It is a shorthand
 * for ss_split_raw(s->buf, del). Look at the documentation of ss_split_raw_to_iter for
 * further details. The string iterator should be used with the ss_iter_next function
 * in order to yield subsequent substring.
 *
 * The string iterator is heap allocated along with a copy of the passed string. The
 * string iterator must be freed after use, but note that if a call to ss_iter_next
 * returns END_ITER (i.e. iterator exhausted) the string iter have been automatically
 * freed.
 *
 * Returns ss_iter* or NULL in case of allocation failures.
 */
ss_iter *ss_split_str_to_iter(ss *s, const char *del) {
    return ss_split_raw_to_iter(s->buf, del);
}

/*
 * Advances the string iterator and returns the next substring. The returned substring
 * starts at the end of the previous delimiter (or the start of the original string in
 * the first iteration) and ends at the start of the next delimiter (or at the end of
 * the original string in the last iteration). Those ss substrings are heap allocated
 * and must be freed after use (with ss_free). If no delimiters are present, all the
 * original string is returned. If the iterator contains an empty delimiter all the
 * original string is returned. Multiple consecutive delimiters are treated as one
 * delimiter, and in general no empty strings are returned from this function.
 *
 * When the iterator is exhausted the function returns END_ITER. The caller can test
 * for this condition to stop the iteration. When END_ITER is returned, ss_iter_next
 * has already taken care of freeing the string iterator. The function returns NULL
 * in case of allocation errors. Also in this case the iterator is automatically freed.
 *
 * Returns a ss* in case of success, NULL in case of allocation failures or END_ITER
 * when the iterator is exhausted.
 */
ss *ss_iter_next(ss_iter *s_iter) {
    // Iterator exhausted, this must be the
    // last call, because the iter is freed.
    if (s_iter->end == ITER_EXHAUS) {
        ss_iter_free(s_iter);
        return END_ITER;
    }

    // Guard against empty delimiters. Return all the original
    // string and flag the iterator as ended (next call will
    // return END_ITER).
    if (strcmp(s_iter->del, "") == 0) {
        size_t len = strlen(s_iter->ptr);
        ss *s = ss_new_from_raw_len_cap(s_iter->ptr, len, len);
        if (s == NULL) {
            ss_iter_free(s_iter);
            return NULL;
        }
        s_iter->end = ITER_EXHAUS;
        return s;
    }

    // Main loop, split the original string and return
    // a substring at each iteration.
    while (1) {
        char *end = strstr(s_iter->ptr, s_iter->del);
        if (end == NULL) {
            size_t len = strlen(s_iter->ptr);
            if (len == 0) {
                // The ptr points to the end of the string, so don't
                // return an empty substring, but free the iterator
                // and return END_ITER.
                ss_iter_free(s_iter);
                return END_ITER;
            }

            // Return the last substring and flag the iterator
            // as exhausted (next call will return END_ITER).
            ss *s = ss_new_from_raw_len_cap(s_iter->ptr, len, len);
            if (s == NULL) {
                ss_iter_free(s_iter);
                return NULL;
            }
            s_iter->end = ITER_EXHAUS;
            return s;
        }

        size_t len = end - s_iter->ptr;
        if (len == 0) {
            // Avoid returning empty strings in case of consecutive
            // delimiters, just update the pointer to point to the
            // first char after the delimiter and loop again.
            s_iter->ptr = end + strlen(s_iter->del);
            continue;
        }

        // Build the substring and update the pointer to
        // point to the first char after the delimiter.
        ss *s = ss_new_from_raw_len_cap(s_iter->ptr, len, len);
        if (s == NULL) {
            ss_iter_free(s_iter);
            return NULL;
        }
        s_iter->ptr = end + strlen(s_iter->del);
        return s;
    }
}

/*
 * Collect and return all the ss substrings that a ss_iter could produce. The function is
 * useful when the caller doesn't want/need to iterate and collect substrings manually;
 * ss_split_row and ss_split_str have similar functions but ss_iter_collect is more flexible
 * since it could be used also when some substrings were already obtained from an iterator.
 * All substrings are heap allocated and returned as an array of *ss that must be freed after
 * use with the dedicated ss_list_free function.
 *
 * Returns an array of strings (ss*) of length n in case of success or NULL in case of
 * allocation failures. In case of errors the strings list is freed along with the iterator.
 */
ss **ss_iter_collect(ss_iter *s_iter, int *n) {
    *n = 0;
    ss **str_list = _malloc(sizeof(ss));
    if (str_list == NULL) {
        ss_iter_free(s_iter);
        return NULL;
    }

    // Loop until the iterator is exhausted. If END_ITER is returned, the
    // iterator was already freed, so we can just break and return the
    // strings list.
    while (1) {
        ss *str_next = ss_iter_next(s_iter);
        if (str_next == END_ITER) break;
        if (str_next == NULL) {
            ss_list_free(str_list, *n);
            ss_iter_free(s_iter);
            *n = 0;
            return NULL;
        }

        // The allocated string list memory is
        // enlarged with steps of length 20.
        if (*n % 20 == 0) {
            ss **new_str_array = _realloc(str_list, sizeof(ss*) * (*n + 20));
            if (new_str_array == NULL) {
                ss_list_free(str_list, *n);
                ss_iter_free(s_iter);
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
 * Return all the ss substrings generated from splitting the raw_str argument with the
 * delimiter string. The function is useful when the caller doesn't want/need to
 * create a string iterator and collect substrings manually. All substrings are heap
 * allocated and returned as an array of ss (ss**) that must be freed after use with
 * the dedicated ss_list_free function.
 *
 * Returns an array of strings (ss**) of length n in case of success or NULL in case of
 * allocation failures. In case of errors the strings list is freed along with the iterator.
 */
ss **ss_split_row(const char *raw_str, const char *del, int *n) {
    ss_iter *s_iter = ss_split_raw_to_iter(raw_str, del);
    if (s_iter == NULL) return NULL;
    return ss_iter_collect(s_iter, n);
}

/*
 * Return all the ss substrings generated from splitting the ss string argument with
 * the delimiter string. The function is useful when the caller doesn't want/need to
 * create a string iterator and collect substrings manually. All substrings are heap
 * allocated and returned as an array of *ss that must be freed after use with the
 * dedicated ss_list_free function.
 *
 * Returns an array of strings (ss**) of length n in case of success or NULL in case of
 * allocation failures. In case of errors the strings list is freed along with the iterator.
 */
ss **ss_split_str(ss *s, const char *del, int *n) {
    ss_iter *s_iter = ss_split_str_to_iter(s, del);
    if (s_iter == NULL) return NULL;
    return ss_iter_collect(s_iter, n);
}

/*
 * Deallocate the memory used by a string iterator. The string iterator can't be
 * used after being freed. Note that some functions automatically consumes and
 * frees the iterator, so it shouldn't be freed again.
 */
void ss_iter_free(ss_iter *s_iter) {
    free(s_iter->buf);
    free(s_iter->del);
    s_iter->buf = NULL;
    s_iter->ptr = NULL;
    s_iter->del = NULL;
    free(s_iter);
}

/*
 * Deallocate the memory used by a ss string array. The string array and all the
 * strings can't be used after being freed.
 */
void ss_list_free(ss **s_list, const int n) {
    for (int i = 0; i < n; i++) ss_free(s_list[i]);
    free(s_list);
}

