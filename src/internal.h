
#ifndef SS_UTILS_H
#define SS_UTILS_H

#include <stdio.h>
#include <strings.h>
#include "string.h"

static void str_debug(str *s, int mem) {
    if (s->buf == NULL) {
        printf("str {NULL}\n");
        return;
    }
    printf(
        "str {len = %d, cap = %d, buf_ptr = %p ('%s')}\n",
        s->len, s->cap, s->buf, s->buf
    );
    if (!mem) return;

    for (int i = 0; i < s->cap+1+10; i++) {
        if (i == s->cap) {
            printf("%p --> %c   (<-- ends here) \n",  s->buf+i,  s->buf[i]);
        } else {
            printf("%p --> %c\n",  s->buf+i,  s->buf[i]);
        }
    }
}

static void str_iter_debug(str_iter *s, int mem) {
    if (s->buf == NULL) {
        printf("str_iter {NULL}\n");
        return;
    }
    printf(
        "str_iter {buf = %p ('%s'), ptr = %p ('%s'), del = %p ('%s')}\n",
        s->buf, s->buf, s->ptr, s->ptr, s->del, s->del
    );
    if (!mem) return;

    int len = strlen(s->buf);
    for (int i = 0; i < len + 1; i++) {
        printf("%p --> %c\n",  s->buf+i,  s->buf[i]);
    }
}

#endif
