
#ifndef SS_STRING_FMT_H
#define SS_STRING_FMT_H

ss *ss_sprintf_concat_va(ss *s, const char *format, va_list arg_list);
ss *ss_sprintf_concat(ss *s, const char *format, ...);
ss *ss_sprintf(const char *format, ...);

#endif
