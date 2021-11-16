#include <stdlib.h>
#include "main.h"

func_doc new_func_doc(void) {
    return (func_doc){
        .comment = ss_new_empty_with_cap(200),
        .func_name = ss_new_empty_with_cap(20),
        .func_sign = ss_new_empty_with_cap(20)
    };
}

func_doc *realloc_func_docs(func_doc *docs, int n_docs) {
    docs = realloc(docs, sizeof(func_doc) * n_docs);
    if (!docs) {
        printf("docs realloc fails\n");
        exit(1);
    }
    return docs;
}

void free_doc(func_doc doc) {
    ss_free(doc.func_name);
    ss_free(doc.comment);
    ss_free(doc.func_sign);
}