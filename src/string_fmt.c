#include <stdlib.h>
#include <strings.h>
#include <stdarg.h>
#include "string.h"
#include "internal.h"

/*
 * Internal function, formats a string using the usual C formatting directive and
 * concatenates the formatted string to the s string. The variadic arguments are
 * passed as a va_list type argument list.
 */
static str *str_sprintf_concat_va(str *s, const char *format, va_list arg_list) {
    size_t buf_len = sizeof(char) * strlen(format) * 2;
    char *buf = _malloc(buf_len);
    if (buf == NULL) {
        return NULL;
    }

    int n_written;
    while(1) {
        va_list arg_list_copy;
        va_copy(arg_list_copy, arg_list);

        n_written = vsnprintf(buf, buf_len, format, arg_list_copy);
        if (n_written < 0 ) {
            printf("ss_lib: vsnprintf() encoding error");
            free(buf);
            return NULL;
        }

        va_end(arg_list_copy);

        // If the number of bytes written is less than the buf_len it
        // means that the vsnprintf() function has not truncated the
        // formatted sting, so we can break. Otherwise, double the space
        // allocated and try again.
        if (n_written < buf_len) break;
        buf_len *= 2;
        char *new_buf = _realloc(buf, buf_len);
        if (new_buf == NULL) {
            free(buf);
            return NULL;
        }
        buf = new_buf;
    }

    // Finally, concat the str string with the formatted
    // one, shrink the free space and return the former.
    s = str_concat_raw_len(s, buf, n_written);
    free(buf);
    if (s == NULL) return NULL;
    s = str_shrink(s);
    if (s == NULL) return NULL;
    return s;
}

/*
 * Format a string using the usual C formatting directive and concatenate the formatted string
 * to the s string.
 *
 * Returns the concatenated s string in case of success or NULL in case of allocations errors.
 */
str *str_sprintf_concat(str *s, const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    s = str_sprintf_concat_va(s, format, arg_list);
    va_end(arg_list);
    return s;
}

/*
 * Format a string using the usual C formatting directive and returns a formatted string.
 *
 * Returns the concatenated s string in case of success or NULL in case of allocations errors.
 */
str *str_sprintf(const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    str *s = str_sprintf_concat_va(str_new_empty(), format, arg_list);
    va_end(arg_list);
    return s;
}