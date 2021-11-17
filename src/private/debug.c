#include <stdio.h>
#include <strings.h>
#include "../string.h"

void ss_debug(ss s, int mem) {
    if (s->buf == NULL) {
        printf("ss {NULL}\n");
        return;
    }
    printf("str {len = %lu, free = %lu, buf_ptr = %p ('%s')}\n",
        s->len,
        s->free,
        s->buf,
        s->buf
    );
    if (!mem) return;

    for (int i = 0; i < s->len + 1 + s->free; i++) {
        if (i == s->len + 1 + s->free) {
            printf("%p --> %c   (<-- ends here) \n",  s->buf+i,  s->buf[i]);
        } else {
            printf("%p --> %c\n",  s->buf+i,  s->buf[i]);
        }
    }

}
