
#ifndef SS_STRING_ITER_H
#define SS_STRING_ITER_H

#define END_ITER (ss *)1

#define ITER_ACTIVE 0
#define ITER_EXHAUS 1

typedef struct {
    char *buf;
    char *ptr;
    char *del;
    int end;
} ss_iter;

ss_iter *ss_split_raw_to_iter(const char *s, const char *del);
ss_iter *ss_split_str_to_iter(ss *s, const char *del);
ss *ss_iter_next(ss_iter *s_iter);

ss **ss_iter_collect(ss_iter *s_iter, int *n);
ss **ss_split_raw(const char *raw_str, const char  *del, int *n);
ss **ss_split_str(ss *s, const char  *del, int *n);

ss *ss_join_raw(char **s, const int n, const char *sep);
ss *ss_join_str(ss **s, const int n, const char *sep);

void ss_iter_free(ss_iter *s_iter);
void ss_list_free(ss **s_list, int n);

#endif
