#include <stdlib.h>
#include "main.h"

doc new_doc(void) {
    return (doc){
        .comment = ss_new_empty_with_cap(200),
        .func_name = ss_new_empty_with_cap(20),
        .func_sign = ss_new_empty_with_cap(20)
    };
}

doc *realloc_docs(doc *docs, int n_docs) {
    docs = realloc(docs, sizeof(doc) * n_docs);
    if (!docs) {
        printf("docs realloc fails\n");
        exit(1);
    }
    return docs;
}
