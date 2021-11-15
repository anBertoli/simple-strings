#include <stdbool.h>
#include <stdlib.h>
#include "main.h"

doc *generate_files_documentation(char **file_paths, int n_files, int *n_docs) {
    doc *docs = NULL;
    int docs_n = 0;

    for (int i = 0; i < n_files; i++) {
        printf("reading file '%s'...\n", file_paths[i]);
        ss file_contents = read_file_to_string(file_paths[i]);
        if (file_contents == NULL) {
            printf("error reading file, exiting..\n");
            exit(1);
        }

        int docs_file_n = 0;
        doc *docs_file = parse_file_comments(file_contents, &docs_file_n);
        docs = realloc_docs(docs, docs_n + docs_file_n);
        if (docs == NULL) {
            printf("realloc documentations failed\n");
            exit(1);
        }
        for (int j = 0; j < docs_file_n; j++) docs[docs_n + j] = docs_file[j];
        docs_n += docs_file_n;

        ss_free(file_contents);
        free(docs_file);
    }

    *n_docs = docs_n;
    return docs;
}

doc *parse_file_comments(ss file_content, int *nd) {
    doc *docs = NULL;
    int n_docs = 0;

    int n_lines = 0;
    ss *lines = ss_split_str(file_content, "\n", &n_lines);
    bool in_comment = false;

    for (int i = 0; i < n_lines; i++) {
        ss line = lines[i];
        ss_trim(line, "\n\t\r ");

        // end comment
        int pos_end = ss_index(line, "*/");
        if (pos_end == 0 && in_comment) {
            parse_end_comment_line(&docs[n_docs], line);
            parse_func_line(&docs[n_docs], lines[i+1]);
            ss_trim(docs[n_docs].comment, "\t\n\r ");
            in_comment = false;
            n_docs++;
            continue;
        }

        // in comment
        if (in_comment) {
            parse_comment_line(&docs[n_docs], line);
            continue;
        }

        // start comment
        int pos_start = ss_index(line, "/*");
        if (pos_start == 0) {
            if (n_docs % 20 == 0) docs = realloc_docs(docs, n_docs + 20);
            docs[n_docs] = new_doc();
            parse_start_comment_line(&docs[n_docs], line);
            in_comment = true;
            continue;
        }
    }

    ss_list_free(lines, n_lines);

    *nd = n_docs;
    return docs;
}

void parse_comment_line(doc *doc, ss line) {
    if (line->buf[0] == '*') line->buf[0] = ' ';
    ss_trim(line, " ");
    ss_concat_str(doc->comment, line);
    ss_concat_raw(doc->comment, "\n");
}

void parse_start_comment_line(doc *doc, ss line) {
    ss_trim(line, "/* ");
    if (line->len == 0) return;
    ss_concat_str(doc->comment, line);
    ss_concat_raw(doc->comment, "\n");
}

void parse_end_comment_line(doc *doc, ss line) {
    int pos_end = ss_index(line, "*/");
    if (pos_end == -1) {
        printf("end comment line: closing '*/' not found in line '%s'\n", line->buf);
        exit(1);
    }
    ss_slice(line, 0, pos_end);
    if (line->len == 0) return;
    if (line->buf[0] == '*') line->buf[0] = ' ';
    ss_trim(line, " ");
    if (line->len == 0) return;
    ss_concat_raw(doc->comment, line->buf);
}

void parse_func_line(doc *doc, ss line) {
    ss_trim(line, "\n\t\r ");
    int after_parameters = ss_index(line, ")");
    if (after_parameters == -1) return;

    ss_slice(line, 0, after_parameters+1);
    ss_concat_raw(line, ";");
    ss_concat_str(doc->func_sign, line);

    int after_first_word = ss_index(line, " ");
    ss_slice(line, after_first_word, line->len);
    ss_trim_left(line, " *");
    int start_parameters = ss_index(line, "(");
    ss_slice(line, 0, start_parameters);
    ss_concat_str(doc->func_name, line);
}

