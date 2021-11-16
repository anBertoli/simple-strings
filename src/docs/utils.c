#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "main.h"

void write_string_to_file(ss string, char *path) {
    FILE *file_pointer = fopen(path, "w");
    if (file_pointer == NULL) {
        perror("error opening the file");
        exit(1);
    }

    size_t n = fwrite(string->buf, 1, string->len, file_pointer);
    if (n != string->len) {
        if (ferror(file_pointer)) {
            perror("error writing file");
        } else {
            printf("error writing file");
        }
        exit(1);
    }
}

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
    ss_cut(s, i);

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

ss gen_funcs_documentation(doc *funcs, int funcs_n) {
    ss api_docs = ss_new_empty();
    ss_concat_raw(api_docs, "# Library API\n");

    // Index
    ss_concat_raw(api_docs, "## Index\n");
    for (int i = 0; i < funcs_n; i++) {
        ss index_line = ss_sprintf("[`%s`](#%s)  \n", funcs[i].func_name->buf, funcs[i].func_name->buf);
        ss_concat_str(api_docs, index_line);
        ss_free(index_line);
    }
    ss_concat_raw(api_docs, "\n");

    // Functions
    for (int i = 0; i < funcs_n; i++) {
        doc doc = funcs[i];

        if (strcmp(doc.func_name->buf, "ss_new_from_raw_len_cap") == 0) {
            ss_concat_raw(api_docs, "## String creation and memory management\n\n");
        }
        if (strcmp(doc.func_name->buf, "ss_grow") == 0) {
            ss_concat_raw(api_docs, "## String manipulation\n\n");
        }
        if (strcmp(doc.func_name->buf, "ss_split_raw_to_iter") == 0) {
            ss_concat_raw(api_docs, "## String splitting, joining and iteration\n\n");
        }
        if (strcmp(doc.func_name->buf, "ss_sprintf_va") == 0) {
            ss_concat_raw(api_docs, "## String formatting\n\n");
        }

        ss title = ss_sprintf("### %s \n", doc.func_name->buf);
        ss body = ss_sprintf("%s\n\n", doc.comment->buf);
        ss signature = ss_sprintf("```c\n%s\n```\n\n", doc.func_sign->buf);

        ss_concat_str(api_docs, title);
        ss_concat_str(api_docs, body);
        ss_concat_str(api_docs, signature);

        ss_free(title); ss_free(body); ss_free(signature);
        ss_free(doc.func_name);
        ss_free(doc.func_sign);
        ss_free(doc.comment);
    }

    return api_docs;
}
