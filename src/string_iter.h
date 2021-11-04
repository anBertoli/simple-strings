
#ifndef SS_STRING_ITER_H
#define SS_STRING_ITER_H

typedef struct {
    char *buf;
    char *ptr;
    char *del;
} ss_iter;

ss_iter *ss_split_raw_to_iter(const char *s, const char *del);
ss_iter *ss_split_str_to_iter(ss *s, const char *del);
ss **ss_iter_collect(ss_iter *s_iter, int *n_str);
ss *ss_iter_next(ss_iter *s_iter);

ss **ss_split_row(const char *raw_str, const char  *del, int *n_str);
ss **ss_split_str(ss *s, const char  *del, int *n_str);

void ss_iter_free(ss_iter *s_iter);

#endif
