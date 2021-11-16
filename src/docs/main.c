#include <stdlib.h>
#include "main.h"
#include "stdio.h"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("insert files to parse as arguments\n");
        exit(1);
    }

    // Generate functions docs.
    int funcs_docs_n = 0;
    doc *funcs_docs = parse_files_comments(&argv[1], argc - 1, &funcs_docs_n);
    ss api_docs = gen_funcs_documentation(funcs_docs, funcs_docs_n);
    free(funcs_docs);

    // Concatenate intro do function docs.
    ss readme = read_file_to_string("docs/intro.md");
    ss_concat_raw(readme, "\n");
    ss_concat_str(readme, api_docs);

    write_string_to_file(api_docs, "docs/api.md");
    write_string_to_file(readme, "README.md");

    ss_free(api_docs);
    ss_free(readme);

}