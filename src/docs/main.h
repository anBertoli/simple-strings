
#ifndef DOCS_MAIN_H
#define DOCS_MAIN_H

#include "../string.h"
#include "../string_fmt.h"
#include "../string_iter.h"
#include "../private/debug.h"

ss read_file_to_string(char *path);
void write_string_to_file(ss string, char *path);

typedef struct doc {
    ss comment;
    ss func_sign;
    ss func_name;
} doc;

doc new_doc(void);
doc *realloc_docs(doc *docs, int n_docs);

ss gen_funcs_documentation(doc *funcs, int funcs_n);

doc *parse_files_comments(char **file_paths, int n_files, int *n_docs);
doc *parse_file_comments(ss file_content, int *n);
void parse_start_comment_line(doc *doc, ss line);
void parse_comment_line(doc *doc, ss line);
void parse_end_comment_line(doc *doc, ss line);
void parse_func_line(doc *doc, ss line);

#endif
