#include <stdlib.h>
#include <strings.h>
#include <stdarg.h>
#include "string.h"
#include "internal/debug.h"

/*
 * Format a string using the usual C formatting directives and concatenate the
 * formatted string to the ss string s. The function accepts s va_list to
 * accommodate a variable number of arguments.
 *
 * Returns 1 if the function succeeded or 0 if the eventual reallocation fails. In
 * case of failure the s string is still valid and must be still freed.
 */
int ss_sprintf_concat_va(ss *s, const char *format, va_list arg_list) {
    size_t buf_len = sizeof(char) * strlen(format) * 2;
    char *buf = _malloc(buf_len);
    if (buf == NULL) {
        return 0;
    }

    int n_written;
    while(1) {
        va_list arg_list_copy;
        va_copy(arg_list_copy, arg_list);

        n_written = vsnprintf(buf, buf_len, format, arg_list_copy);
        if (n_written < 0 ) {
            printf("ss_lib: vsnprintf() encoding error");
            free(buf);
            return 0;
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
            return 0;
        }
        buf = new_buf;
    }

    // Finally, concat the ss string with the formatted
    // one, shrink the free space and return the former.
    int ok = ss_concat_raw_len(s, buf, n_written);
    free(buf);
    if (!ok) return 0;
    ok = ss_shrink(s);
    if (!ok) return 0;
    return 1;
}

/*
 * Format a string using the usual C formatting directives and concatenate the
 * formatted string to the s string. The function is variadic exactly as sprintf,
 * printf, etc. functions.
 *
 * Returns 1 if the function succeeded or 0 if the eventual reallocation fails. In
 * case of failure the s string is still valid and must be still freed.
 */
int ss_sprintf_concat(ss *s, const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    int ok = ss_sprintf_concat_va(s, format, arg_list);
    va_end(arg_list);
    return ok;
}

/*
 * Format a string using the usual C formatting directive and returns a formatted string.
 * The returned string must be freed with the dedicated ss_free function().
 *
 * Returns a formatted ss string in case of success or NULL in case of allocations errors.
 */
ss *ss_sprintf(const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    ss *s = ss_new_empty();
    if (s == NULL) return 0;
    int ok = ss_sprintf_concat_va(s, format, arg_list);
    va_end(arg_list);
    return ok ? s : 0;
}
