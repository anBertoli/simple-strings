#include <stdio.h>
#include <strings.h>
#include "../string.h"
#include "../string_iter.h"

void ss_debug(ss *s, int mem) {
    if (s->buf == NULL) {
        printf("ss {NULL}\n");
        return;
    }
    printf(
        "str {len = %lu, cap = %lu, buf_ptr = %p ('%s')}\n",
        s->len, s->cap, s->buf, s->buf
    );

    if (!mem) return;
    for (int i = 0; i < s->cap+1; i++) i == s->cap
        ? printf("%p --> %c   (<-- ends here) \n",  s->buf+i,  s->buf[i])
        : printf("%p --> %c\n",  s->buf+i,  s->buf[i]);
}

void ss_iter_debug(ss_iter *s, int mem) {
    if (s->buf == NULL) {
        printf("ss_iter {NULL}\n");
        return;
    }
    printf(
        "str_iter {buf = %p ('%s'), ptr = %p ('%s'), del = %p ('%s')}\n",
        s->buf, s->buf, s->ptr, s->ptr, s->del, s->del
    );

    if (!mem) return;
    int len = strlen(s->buf);
    for (int i = 0; i < len + 1; i++) printf("%p --> %c\n",  s->buf+i,  s->buf[i]);
}
