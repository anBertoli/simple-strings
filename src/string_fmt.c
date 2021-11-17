#include <stdlib.h>
#include <strings.h>
#include <stdarg.h>
#include "string.h"
#include "alloc.h"

/*
 * Formats and returns a string using the usual C formatting directives. The function
 * accepts a `va_list` to accommodate a variable number of arguments. In case of success
 * the returned string must be freed after use with the dedicated [`ss_free`](#ss_free)
 * function. The argument list should be started (`va_start`) before providing it to this
 * function and must be ended (`va_end`) after the function call.
 *
 * Returns the formatted string in case of success or NULL in case of allocations errors.
 */
ss ss_sprintf_va(const char *format, va_list arg_list) {
    size_t buf_len = sizeof(char) * strlen(format) * 2;
    char *buf = ss_malloc(buf_len);
    if (buf == NULL) {
        return NULL;
    }

    // The buffer to write the formatted string initially is two times
    // long the format string. We try to format the string with vsnprintf
    // providing the buffer and its length along with the format string
    // and the variadic arguments. If the resulting formatted string is
    // not truncated we have finished. Otherwise, we retry doubling the
    // buffer.
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
        // means that the vsnprintf has not truncated the formatted
        // string, so we can break. Otherwise, retry.
        if (n_written < buf_len) break;

        buf_len *= 2;
        char *new_buf = ss_realloc(buf, buf_len);
        if (new_buf == NULL) {
            free(buf);
            return NULL;
        }
        buf = new_buf;
    }

    // Finally, build the ss string with the
    // formatted C string and return it.
    ss s = ss_new_from_raw_len_free(buf, n_written, n_written);
    free(buf);
    return s;
}

/*
 * Formats and returns a string using the usual C formatting directive. The returned string must
 * be freed after use as usual with the dedicated [`ss_free`](#ss_free) function.
 *
 * Returns the formatted string in case of success or NULL in case of allocations errors.
 */
ss ss_sprintf(const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    ss s1 = ss_sprintf_va(format, arg_list);
    va_end(arg_list);
    return s1;
}
