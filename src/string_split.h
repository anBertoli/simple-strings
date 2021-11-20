
#ifndef SS_STRING_SPLIT_H
#define SS_STRING_SPLIT_H

ss *ss_split_raw(const char *s, const char  *del, int *n);
ss *ss_split_str(ss s, const char  *del, int *n);
ss_err ss_join_raw_cat(ss s, const char **str, int n, const char *sep);
ss ss_join_raw(const char **str, int n, const char *sep);
ss_err ss_join_str_cat(ss s, ss *str, int n, const char *sep);
ss ss_join_str(ss *str, int n, const char *sep);
void ss_list_free(ss *list, int n);

#endif
