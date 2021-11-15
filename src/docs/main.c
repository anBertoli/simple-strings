#include <stdlib.h>
#include "main.h"
#include "stdio.h"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("insert files to parse as arguents\n");
        exit(1);
    }

    int docs_n = 0;
    doc *docs = generate_files_documentation(&argv[1], argc-1, &docs_n);
    write_docs_to_file("docs/API.md", docs, docs_n);

    free(docs);
}