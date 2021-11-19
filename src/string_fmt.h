
#ifndef SS_STRING_FMT_H
#define SS_STRING_FMT_H

#include <stdarg.h>
#include "string.h"

ss_err ss_sprintf_va_cat(ss s, const char *format, va_list arg_list);
ss ss_sprintf_va(const char *format, va_list arg_list);

ss_err ss_sprintf_cat(ss s, const char *format, ...);
ss ss_sprintf(const char *format, ...);

#endif
