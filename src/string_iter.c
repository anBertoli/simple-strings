#include <stdlib.h>
#include <strings.h>
#include "string.h"
#include "internal/debug.h"


/*
 * Build a new string iterator from a raw string and a string delimiter. The string
 * iterator is used to split the string into substrings, each of them separated from
 * others by occurrences of the delimiter. The string iterator should be used with
 * the ss_iter_next function in order to yield subsequent substring.
 *
 * The string iterator is heap allocated along with a copy of the passed string. The
 * string iterator must be freed after use, but note that if a call to ss_iter_next
 * returns END_ITER (i.e. iterator exhausted) the string iter have been automatically
 * freed.
 *
 * Returns ss_iter* or NULL in case of allocation failures.
 */
ss_iter *ss_split_raw(const char *s, const char *del) {
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
        .ptr  = buf,
        .del = buf_del
    };

    return s_iter;
}

/*
 * Build a new string iterator from a string and a string delimiter. It is a shorthand
 * for ss_split_raw(s->buf, del). Look at the documentation of ss_split_raw for further
 * details. The string iterator should be used with the ss_iter_next function in order
 * to yield subsequent substring.
 *
 * The string iterator is heap allocated along with a copy of the passed string. The
 * string iterator must be freed after use, but note that if a call to ss_iter_next
 * returns END_ITER (i.e. iterator exhausted) the string iter have been automatically
 * freed.
 *
 * Returns ss_iter* or NULL in case of allocation failures.
 */
ss_iter *ss_split(ss *s, const char *del) {
    return ss_split_raw(s->buf, del);
}

/*
 * Advance the string iterator and return the next substring. The returned substring
 * starts at the end of the previous delimiter (or the start of the original string)
 * and ends at the start of the next delimiter. Those ss substrings are heap allocated
 * and must be freed after use (wth ss_free). If no more delimiters are present, the
 * remaining part of the string is returned. If the iterator contains an empty delimiter
 * the entire string is returned. Multiple consecutive delimiters are treated as one
 * delimiter, and in general no empty strings are returned from this function. If the
 * delimiter is not found in the string, the entire string is returned.
 *
 * When the iterator is exhausted ss_iter_next returns END_ITER. The caller can test
 * for this condition to stop the iteration. When END_ITER is returned, ss_iter_next
 * has already taken care of freeing the string iterator and the ss string must be
 * considered invalid to be used.
 *
 * Returns a ss* in case of success, NULL in case of allocation failures or END_ITER
 * when the iterator is exhausted.
 */
ss *ss_iter_next(ss_iter *s_iter) {
    // Iterator exhausted, this must be the
    // last call, because the iter is freed.
    if (s_iter->ptr == NULL) {
        ss_iter_free(s_iter);
        return END_ITER;
    }

    // Guard against empty delimiter. Return all the
    // string and flag the iterator as ended (ptr to NULL).
    if (strcmp(s_iter->del, "") == 0) {
        ss *s = ss_new_raw_len(s_iter->ptr, strlen(s_iter->ptr));
        ss_shrink(s);
        if (s == NULL) return NULL;
        s_iter->ptr = NULL;
        return s;
    }

    while (1) {
        char *end = strstr(s_iter->ptr, s_iter->del);
        if (end == NULL) {
            size_t len = strlen(s_iter->ptr);
            if (len == 0) {
                // Delimiter at the end, don't
                // return an empty string.
                ss_iter_free(s_iter);
                return END_ITER;
            }

            // Return the last substring and flag
            // the iterator as ended (ptr to NULL).
            ss *s = ss_new_raw_len(s_iter->ptr, len);
            ss_shrink(s);
            if (s == NULL) return NULL;
            s_iter->ptr = NULL;
            return s;
        }

        size_t len = end - s_iter->ptr;
        if (len == 0) {
            // Avoid returning empty strings in
            // case of consecutive delimiters.
            s_iter->ptr = end + strlen(s_iter->del);
            continue;
        }

        ss *s = ss_new_raw_len_cap(s_iter->ptr, len, len);
        if (s == NULL) return NULL;
        s_iter->ptr = end + strlen(s_iter->del);
        return s;
    }
}

/*
 * Collect and return all the ss substrings that a ss_iter could produce. The function is
 * useful when the caller doesn't want/need to iterate and collect substrings manually.
 * All substrings are heap allocated and returned as an array (ss **) that must be freed
 * after use. The n_str pointer is set to the number of resulting substrings in the
 * returned ss list.
 *
 * Returns a list of heap allocated strings (ss**) of length n_str in case of success or
 * NULL in case of allocation failures. In case of errors the strings already collected
 * are discarded while the string iterator is consumed and freed nonetheless.
 */
ss **ss_collect_iter(ss_iter *s_iter, int *n_str) {
    ss **str_array = NULL;
    *n_str = 0;
    int i = 0;

    while (1) {
        // If END_ITER is returned, the
        // str_iter was already freed.
        ss *str_next = ss_iter_next(s_iter);
        if (str_next == END_ITER) break;
        if (str_next == NULL) {
            free(str_array);
            return NULL;
        }

        // The allocated list space is enlarged
        // with steps of length 20.
        if (i % 20 == 0) {
            ss **new_str_array = _realloc(str_array, sizeof(ss*) * (i + 20));
            if (new_str_array != NULL) str_array = new_str_array;
            else {
                free(str_array);
                return NULL;
            }
        }

        ss_shrink(str_next);
        str_array[i] = str_next;
        i++;
    }

    *n_str = i;
    return str_array;
}

/*
 * Return all the ss substrings generated from splitting the raw_str argument with the
 * delimiter string. The function is useful when the caller doesn't want/need to
 * create a string iterator and collect substrings manually.
 *
 * Returns a list of heap allocated strings (ss**) of length n_str in case of success or
 * NULL in case of allocation failures. In case of errors the strings already collected
 * are discarded while the string iterator is consumed and freed nonetheless.
 */
ss **ss_collect_from_row(const char *raw_str, const char  *del, int *n_str) {
    ss_iter *s_iter = ss_split_raw(raw_str, del);
    if (s_iter == NULL) return NULL;

    ss **substrings = ss_collect_iter(s_iter, n_str);
    if (substrings == NULL) {
        // If an error occurred the iterator wasn't
        // freed automatically during the iteration.
        ss_iter_free(s_iter);
        return NULL;
    }

    return substrings;
}

/*
 * Return all the ss substrings generated from splitting the ss string argument with
 * the delimiter string. The function is useful when the caller doesn't want/need
 * to create a string iterator and collect substrings manually.
 *
 * Returns a list of heap allocated strings (ss**) of length n_str in case of success or
 * NULL in case of allocation failures. In case of errors the strings already collected
 * are discarded while the string iterator is consumed and freed nonetheless.
 */
ss **ss_collect_from_str(ss *s, const char  *del, int *n_str) {
    ss_iter *s_iter = ss_split(s, del);
    if (s_iter == NULL) return NULL;

    ss ** substrings = ss_collect_iter(s_iter, n_str);
    if (substrings == NULL) {
        // If an error occurred the iterator wasn't
        // freed automatically during the iteration.
        ss_iter_free(s_iter);
        return NULL;
    }

    return substrings;
}

/*
 * Deallocate the memory used by a string iterator. The string iterator can't be
 * used after being freed. It's a no-op if the pointed struct was already freed.
 */
void ss_iter_free(ss_iter *s_iter) {
    if (s_iter == NULL || s_iter->buf == NULL) return;
    free(s_iter->buf);
    free(s_iter->del);
    s_iter->buf = NULL;
    s_iter->ptr = NULL;
    s_iter->del = NULL;
    free(s_iter);
}
