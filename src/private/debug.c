#include <stdio.h>
#include <strings.h>
#include "../string.h"
#include "../string_iter.h"

void ss_debug(ss s, int mem) {
    if (s->buf == NULL) {
        printf("ss {NULL}\n");
        return;
    }
    printf("str {len = %lu, cap = %lu, buf_ptr = %p ('%s')}\n",
        s->len,
        s->cap,
        s->buf,
        s->buf
    );
    if (!mem) return;

    for (int i = 0; i < s->cap+1; i++) {
        if (i == s->cap) {
            printf("%p --> %c   (<-- ends here) \n",  s->buf+i,  s->buf[i]);
        } else {
            printf("%p --> %c\n",  s->buf+i,  s->buf[i]);
        }
    }

}

void ss_iter_debug(ss_iter iter, int mem) {
    if (iter->buf == NULL) {
        printf("ss_iter {NULL}\n");
        return;
    }
    printf("str_iter {buf = %p ('%s'), ptr = %p ('%s'), del = %p ('%s')}\n",
        iter->buf,
        iter->buf,
        iter->ptr,
        iter->ptr,
        iter->del,
        iter->del
    );
    if (!mem) return;

    size_t len = strlen(iter->buf);
    for (int i = 0; i < len + 1; i++) {
        printf("%p --> %c\n",  iter->buf+i,  iter->buf[i]);
    }
}
