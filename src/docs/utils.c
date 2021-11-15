#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "main.h"

ss read_file_to_string(char *path) {
    FILE *file_pointer = fopen(path, "r");
    if (file_pointer == NULL) {
        perror("error opening the file");
        return NULL;
    }

    ss s = ss_new_empty();
    if (s == NULL) return NULL;

    size_t i = 0;
    while (1) {
        s = ss_grow(s, i + 1000);
        if (s == NULL) return NULL;
        size_t n_read = fread(&s->buf[i], 1, 1000 * sizeof(char), file_pointer);
        if (n_read == 0) {
            break;
        }

        i += n_read;
        ss_cut(s, i);
    }
    if (ferror(file_pointer)) {
        perror("error reading file");
        return NULL;
    }

    if (fclose(file_pointer)) {
        perror("error closing file");
        return NULL;
    }

    return s;
}

void write_check(const void *ptr, size_t size, size_t n_items, FILE *file);

void write_docs_to_file(char * filepath, doc *docs, int docs_n) {
    FILE *file_pointer = fopen(filepath, "w");
    if (file_pointer == NULL) {
        perror("error opening the file");
        exit(1);
    }

    for (int i = 0; i < docs_n; i++) {
        doc doc = docs[i];

        write_check("### ", 1, 4, file_pointer);
        write_check(doc.func_name->buf, 1, doc.func_name->len, file_pointer);
        write_check("\n", 1, 1, file_pointer);
        write_check(doc.comment->buf, 1, doc.comment->len, file_pointer);
        write_check("\n\n", 1, 2, file_pointer);

        write_check("```c\n", 1, 5, file_pointer);
        write_check(doc.func_sign->buf, 1, doc.func_sign->len, file_pointer);
        write_check("\n", 1, 1, file_pointer);
        write_check("```", 1, 3, file_pointer);

        write_check("\n\n", 1, 2, file_pointer);

        ss_free(doc.func_name);
        ss_free(doc.func_sign);
        ss_free(doc.comment);
    }
}

void write_check(const void *ptr, size_t size, size_t n_items, FILE *file) {
    size_t n = fwrite(ptr, size, n_items, file);
    if (n != n_items) {
        if (ferror(file)) {
            perror("error writing file");
        } else {
            printf("error writing file");
        }
        exit(1);
    }
}