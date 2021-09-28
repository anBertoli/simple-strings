#include <stdlib.h>
#include <strings.h>
#include "ss_string.h"


/*
 * Build a new string iterator from a raw C string and a string delimiter. The string
 * iterator is used to split the string into substrings, each of them separated from
 * others by the delimiter. The string iterator should be used with the str_iter_next
 * function in order to yield subsequent substring. The returned substrings are null
 * terminated and doesn't contain the delimiter.
 *
 * Those substrings are heap allocated and must be freed after use. When the iterator
 * is exhausted str_iter_next calls returns END_ITER, the user can test for this
 * condition to stop iterating.
 *
 * The string iterator is heap allocated along with a copy of the passed C string. The
 * string iterator must be freed after use, but note that if a call to str_iter_next
 * returns END_ITER (i.e. iterator exhausted) the string iter is automatically freed.
 *
 * Returns str_iter* or NULL in case of allocation failures.
 */
str_iter *str_split_raw(const char *s, const char *del) {
    char *buf = _malloc(sizeof(char) * strlen(s) + 1);
    if (buf == NULL) return NULL;
    strcpy(buf, s);

    char *buf_del = _malloc(sizeof(char) * strlen(del) + 1);
    if (buf_del == NULL) {
        free(buf);
        return NULL;
    }
    strcpy(buf_del, del);

    str_iter *s_iter = _malloc(sizeof(str_iter));
    if (s_iter == NULL) {
        free(buf);
        free(buf_del);
        return NULL;
    };
    *s_iter = (str_iter){
        .buf = buf,
        .ptr  = buf,
        .del = buf_del
    };

    return s_iter;
}

/*
 * Build a new string iterator from a library string and a string delimiter. It
 * is a shorthand for str_split_raw(s->buf, del). Look at the documentation of
 * str_split_raw for further details.
 *
 * Returns str_iter* or NULL in case of allocation failures.
 */
str_iter *str_split(str *s, const char *del) {
    return str_split_raw(s->buf, del);
}

/*
 * Advance the string iterator returning the next substring. The returned substring
 * ranges from the end of the previous delimiter (or the string start) to the start
 * of the next delimiter. Those substrings are heap allocated and must be freed after
 * use (wth str_free).
 *
 * When the iterator is exhausted str_iter_next calls return END_ITER. The caller can
 * test for this condition to stop the iteration. When END_ITER is returned, the
 * str_iter_next function takes care of freeing the string iterator.
 *
 * Returns a str* in case of success, NULL in case of allocation failures or END_ITER
 * when the iterator is exhausted.
 */
str *str_iter_next(str_iter *s_iter) {
    // Iterator already exhausted,
    // nothing to do here.
    if (s_iter->ptr == NULL) return END_ITER;

    while (1) {
        char *end = strstr(s_iter->ptr, s_iter->del);
        if (end == NULL) {
            // No delimiter found in the remaining part
            // of the string. Return that string part
            // and 'close' the iterator.
            size_t len = strlen(s_iter->ptr);
            if (len == 0) {
                // Delimiter at the end, don't
                // return an empty string.
                str_iter_free(s_iter);
                return NULL;
            }

            str *s = str_new_raw_len(s_iter->ptr, strlen(s_iter->ptr));
            if (s == NULL) return NULL;
            str_iter_free(s_iter);
            return s;
        }

        // Create a new allocated str using as string template
        // the substring starting from the current position
        // till the start of the delimiter. Then update the
        // current position. Avoid returning empty strings
        // in case of subsequent delimiters.
        size_t len = end - s_iter->ptr;
        if (len == 0) {
            s_iter->ptr = end + strlen(s_iter->del);
            continue;
        }

        str *s = str_new_raw_len(s_iter->ptr, len);
        if (s == NULL) return NULL;

        s_iter->ptr = end + strlen(s_iter->del);
        return s;
    }
}

/*
 * Useful function to collect all the substrings that a str_iter could produce.
 * The function is useful when the caller doesn't want/need to iterate and
 * collect substrings manually. All substrings are heap allocated and returned
 * as an array (str **) that must be freed after use. The n_str pointer is
 * set to the number of resulting substrings.
 *
 * Returns a list of heap allocated strings (str**) of length n_str in case of
 * success or NULL in case of allocation failures.
 *
 * Example:
 *
 *      str_iter *s_words_iter = str_split_raw("Hi Paul, how are you?", " ");
 *      if (s_words == NULL) {
 *          // error handling
 *      }
 *
 *      int nw;
 *      str **s_words = str_iter_collect(s_words_iter, &nw);
 *      if (s_words == NULL) {
 *          // error handling
 *      }
 *      for (int i = 0; i < nw; i++) printf("%d: %s\n", i, s_words[i]->buf);
 *      // 0: Hi
 *      // 1: Paul,
 *      // 2: how
 *      // 3: are
 *      // 4: you?
 *
 *      free(s_words);
 */
str **str_iter_collect(str_iter *s_iter, int *n_str) {
    str **str_array = NULL;
    *n_str = 0;
    int i = 0;

    while (1) {
        str *str_next = str_iter_next(s_iter);
        if (str_next == END_ITER) break;
        if (str_next == NULL) return NULL;

        // The allocated list space is enlarged with steps of
        // length 10. If the reallocation fails, the old
        // piece of memory is freed.
        if (i % 10 == 0) {
            str **new_str_array = _realloc(str_array, sizeof(str*) * (i + 10));
            if (new_str_array == NULL) {
                free(str_array);
                return NULL;
            }
            str_array = new_str_array;
        }


        // Append to the str list the new str substring.
        // Shrink the new string before the append op.
        str_shrink(str_next);
        str_array[i] = str_next;
        i++;
    }

    // The iterator is automatically freed
    // when the iterator is exhausted.
    *n_str = i;

    return str_array;
}

str **str_split_raw_collect(const char *raw_str, const char  *del, int *n_str) {
    str_iter *s_iter = str_split_raw(raw_str, del);
    if (s_iter == NULL) return NULL;
    return str_iter_collect(s_iter, n_str);
}

/*
 *
 */
void str_iter_free(str_iter *s_iter) {
    if (s_iter == NULL || s_iter->buf == NULL) return;
    free(s_iter->buf);
    s_iter->buf = NULL;
    s_iter->ptr = NULL;
    free(s_iter->del);
    s_iter->del = NULL;
    free(s_iter);
}










