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

    write_check("## Index\n", 1, 9, file_pointer);
    for (int i = 0; i < docs_n; i++) {
        doc doc = docs[i];

        ss index_line = ss_sprintf("[`%s`](#%s)  \n", doc.func_name->buf, doc.func_name->buf);
        write_check(index_line->buf, 1, index_line->len, file_pointer);
        ss_free(index_line);
    }
    write_check("\n", 1, 1, file_pointer);

    for (int i = 0; i < docs_n; i++) {
        doc doc = docs[i];

        if (strcmp(doc.func_name->buf, "ss_new_from_raw_len_cap") == 0) {
            write_check("## String creation and memory management\n\n", 1, 42, file_pointer);
        }
        if (strcmp(doc.func_name->buf, "ss_grow") == 0) {
            write_check("## String manipulation\n\n", 1, 24, file_pointer);
        }
        if (strcmp(doc.func_name->buf, "ss_split_raw_to_iter") == 0) {
            write_check("## String splitting, joining and iteration\n\n", 1, 44, file_pointer);
        }
        if (strcmp(doc.func_name->buf, "ss_sprintf_va") == 0) {
            write_check("## String formatting\n\n", 1, 22, file_pointer);
        }

        ss title = ss_sprintf("### %s \n", doc.func_name->buf);
        ss body = ss_sprintf("%s\n\n", doc.comment->buf);
        ss signature = ss_sprintf("```c\n%s\n```\n\n", doc.func_sign->buf);

        write_check(title->buf, 1, title->len, file_pointer);
        write_check(body->buf, 1, body->len, file_pointer);
        write_check(signature->buf, 1, signature->len, file_pointer);

        ss_free(title); ss_free(body); ss_free(signature);
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