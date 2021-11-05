#include <stdlib.h>
#include <strings.h>
#include "../src/string.h"
#include "../src/string_fmt.h"
#include "../src/internal/debug.h"
#include "../src/string_iter.h"
#include "framework/framework.h"


void test_tokens_iter(const char *desc, ss_iter *s_iter, int n_tokens, char **tokens) {
    for (int i = 0; ; i++) {

        ss *s_next = ss_iter_next(s_iter);
        if (s_next == END_ITER) break;
        if (s_next == NULL) exit(1);

        if (i+1 > n_tokens) {
            test_failure("should have the correct number of words");
            return;
        }

        if (strcmp(tokens[i], s_next->buf) != 0) {
            test_failure(desc);
            return;
        }
        if (strlen(tokens[i]) != s_next->len || s_next->cap != s_next->len) {
            test_failure("should have len = cap = length of token");
            return;
        }
    }

    test_success("should have the correct number of words");
    test_success("should have len = cap = length of word");
    test_success(desc);
}

void test_tokens_list(const char *desc, ss_list *str_list, char **wanted_list, int n_wanted) {
    if (str_list->len != n_wanted) {
        test_failure("should have the correct number of words");
        return;
    }

    for (int i = 0; i < str_list->len; i++) {
        if (strcmp(wanted_list[i], str_list->list[i]->buf) != 0) {
            test_failure(desc);
            return;
        }
        if (
            strlen(wanted_list[i]) != str_list->list[i]->len ||
            str_list->list[i]->cap != str_list->list[i]->len
        ) {
            test_failure("should have len = cap = length of token");
            return;
        }
    }

    test_success("should have the correct number of words");
    test_success("should have len = cap = length of word");
    test_success(desc);
}
