
#ifndef SS_STRING_SPLIT_H
#define SS_STRING_SPLIT_H

ss *ss_split_raw(const char *s, const char  *del, int *n);
ss *ss_split_str(ss s, const char  *del, int *n);
ss ss_join_raw(char **s, int n, const char *sep);
ss ss_join_str(ss *s, int n, const char *sep);
void ss_list_free(ss *s_list, int n);

static ss *append_to_ss_list(ss *ss_list, int *ss_list_n, ss str);

#endif
