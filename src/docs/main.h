
#ifndef DOCS_MAIN_H
#define DOCS_MAIN_H

#include "../string.h"
#include "../string_fmt.h"
#include "../string_iter.h"
#include "../private/debug.h"

ss read_file_to_string(char *path);
void write_string_to_file(ss string, char *path);

typedef struct func_doc {
    ss comment;
    ss func_sign;
    ss func_name;
} func_doc;

func_doc new_func_doc(void);
func_doc *realloc_func_docs(func_doc *docs, int n_docs);
void free_doc(func_doc doc);

ss gen_funcs_documentation(func_doc *funcs, int funcs_n);

func_doc *parse_files_comments(char **file_paths, int n_files, int *n_docs);
func_doc *parse_file_comments(ss file_content, int *n);
void parse_start_comment_line(func_doc *doc, ss line);
void parse_comment_line(func_doc *doc, ss line);
void parse_end_comment_line(func_doc *doc, ss line);
void parse_func_line(func_doc *doc, ss line);

#endif
