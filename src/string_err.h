
#ifndef SS_STRING_ERR_H
#define SS_STRING_ERR_H

typedef enum ss_err {
    err_none = 0,
    err_alloc = 1,
    err_format = 2
} ss_err;

const char *ss_err_str(ss_err err);

#endif
