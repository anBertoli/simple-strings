#include <stdlib.h>
#include <strings.h>
#include "../src/string.h"
#include "../src/string_fmt.h"
#include "../src/internal/debug.h"
#include "../src/string_iter.h"
#include "framework/framework.h"

void test_tokens_iter(const char *desc, ss_iter *s_iter, int n_tokens, char **tokens);
void test_tokens_list(const char *desc, ss_list *str_list, char **wanted_list, int n_wanted);

void test_ss_split_raw_to_iter(void) {
    test_group("test ss_split_raw");

//    ss_iter *s_iter = ss_split_raw_to_iter("Ehy how are you?", " ");
//    test_cond("should point to the start", s_iter->buf == s_iter->ptr);
//    test_strings("should have copied the del", " ", s_iter->del);
//    ss_iter_free(s_iter);
}

void test_ss_split_str_to_iter(void) {
    test_group("test ss_split");

//    ss *s = ss_new_from_raw("Ehy how are you?");
//    ss_iter *s_iter = ss_split_str_to_iter(s, "?|!");
//    test_cond("should point to the start", s_iter->buf == s_iter->ptr);
//    test_strings("should have copied the del", "?|!", s_iter->del);
//    ss_iter_free(s_iter);
}

void test_ss_iter_next(void) {
    test_group("test ss_iter_next");

//    test_subgroup("split in words");
//    ss_iter *s_iter = ss_split_raw_to_iter("Ehy how are you?", " ");
//    test_tokens_iter(
//            "should split in words", s_iter, 4,
//            (char *[]){"Ehy", "how", "are", "you?"}
//            );
//
//    test_subgroup("missing delimiter");
//    s_iter = ss_split_raw_to_iter("Ehy how are you?", "_");
//    test_tokens_iter(
//            "should return the entire string", s_iter, 1,
//            (char *[]){"Ehy how are you?"}
//            );
//
//    test_subgroup("empty delimiter");
//    s_iter = ss_split_raw_to_iter("Ehy how are you?", "");
//    test_tokens_iter(
//            "should return the entire string", s_iter, 1,
//            (char *[]){"Ehy how are you?"}
//            );
//
//    test_subgroup("multiple consecutive delimiters");
//    s_iter = ss_split_raw_to_iter("   Ehy    how   are   you?   ", " ");
//    test_tokens_iter(
//            "shouldn't return empty tokens", s_iter, 4,
//            (char *[]){"Ehy", "how", "are", "you?"}
//            );
//
//    test_subgroup("only consecutive delimiters");
//    s_iter = ss_split_raw_to_iter("       ", " ");
//    test_tokens_iter(
//            "should return no tokens", s_iter, 0,
//            (char *[]){}
//            );
}

void test_ss_iter_collect(void) {
    test_group("test ss_collect_iter");

//    test_subgroup("split in words");
//    ss_iter *s_iter = ss_split_raw_to_iter("Ehy how are you?", " ");
//    ss_list *list = ss_iter_collect(s_iter);
//    test_tokens_list(
//            "should split in words", list,(char *[]){"Ehy", "how", "are", "you?"}, 4
//            );
//
//    test_subgroup("missing delimiter");
//    s_iter = ss_split_raw_to_iter("Ehy how are you?", "_");
//    list = ss_iter_collect(s_iter);
//    test_tokens_list(
//            "should return the entire string", list,
//            (char *[]){"Ehy how are you?"}, 1
//            );
//
//    test_subgroup("empty delimiter");
//    s_iter = ss_split_raw_to_iter("Ehy how are you?", "");
//    list = ss_iter_collect(s_iter);
//    test_tokens_list(
//            "should return the entire string", list,
//            (char *[]){"Ehy how are you?"}, 1
//            );
//
//    test_subgroup("multiple consecutive delimiters");
//    s_iter = ss_split_raw_to_iter("   Ehy    how   are   you?   ", " ");
//    list = ss_iter_collect(s_iter);
//    test_tokens_list(
//            "shouldn't return empty tokens", list,
//            (char *[]){"Ehy", "how", "are", "you?"}, 4
//            );
//
//    test_subgroup("only consecutive delimiters");
//    s_iter = ss_split_raw_to_iter("      ", " ");
//    list = ss_iter_collect(s_iter);
//    test_tokens_list(
//            "shouldn't return empty tokens", list,
//            (char *[]){}, 0
//            );
}

void test_ss_split_row(void) {
    test_group("test ss_collect_from_row");
//    int n_tokens;
//
//    test_subgroup("split in words");
//    ss **tokens = ss_split_row("Ehy how are you?", " ", &n_tokens);
//    test_tokens_list(
//        "should split in words", tokens, n_tokens,
//        (char *[]){"Ehy", "how", "are", "you?"}, 4
//    );
//
//    test_subgroup("missing delimiter");
//    tokens = ss_split_row("Ehy how are you?", "_", &n_tokens);
//    test_tokens_list(
//        "should return the entire string", tokens, n_tokens,
//        (char *[]){"Ehy how are you?"}, 1
//    );
//
//    test_subgroup("empty delimiter");
//    tokens = ss_split_row("Ehy how are you?", "", &n_tokens);
//    test_tokens_list(
//        "should return the entire string", tokens, n_tokens,
//        (char *[]){"Ehy how are you?"}, 1
//    );
//
//    test_subgroup("multiple consecutive delimiters");
//    tokens = ss_split_row("   Ehy    how   are   you?   ", " ", &n_tokens);
//    test_tokens_list(
//        "shouldn't return empty tokens", tokens, n_tokens,
//        (char *[]){"Ehy", "how", "are", "you?"}, 4
//    );
//
//    test_subgroup("only consecutive delimiters");
//    tokens = ss_split_row("      ", " ", &n_tokens);
//    test_tokens_list(
//        "shouldn't return empty tokens", tokens, n_tokens,
//        (char *[]){}, 0
//    );
}

void test_ss_split_str(void) {
    test_group("test ss_collect_from_str");
//    int n_tokens;
//
//    test_subgroup("split in words");
//    ss *s = ss_new_from_raw("Ehy how are you?");
//    ss **tokens = ss_split_str(s, " ", &n_tokens);
//    test_tokens_list(
//        "should split in words", tokens, n_tokens,
//        (char *[]){"Ehy", "how", "are", "you?"}, 4
//    );
//    free(tokens);
//    free(s);
//
//    test_subgroup("missing delimiter");
//    s = ss_new_from_raw("Ehy how are you?");
//    tokens = ss_split_str(s, "_", &n_tokens);
//    test_tokens_list(
//        "should return the entire string", tokens, n_tokens,
//        (char *[]){"Ehy how are you?"}, 1
//    );
//    free(tokens);
//    free(s);
//
//    test_subgroup("empty delimiter");
//    s = ss_new_from_raw("Ehy how are you?");
//    tokens = ss_split_str(s, "", &n_tokens);
//    test_tokens_list(
//        "should return the entire string", tokens, n_tokens,
//        (char *[]){"Ehy how are you?"}, 1
//    );
//    free(tokens);
//    free(s);
//
//    test_subgroup("multiple consecutive delimiters");
//    s = ss_new_from_raw("   Ehy    how   are   you?   ");
//    tokens = ss_split_str(s, " ", &n_tokens);
//    test_tokens_list(
//        "shouldn't return empty tokens", tokens, n_tokens,
//        (char *[]){"Ehy", "how", "are", "you?"}, 4
//    );
//    free(tokens);
//    free(s);
//
//    test_subgroup("only consecutive delimiters");
//    s = ss_new_from_raw("      ");
//    tokens = ss_split_str(s, " ", &n_tokens);
//    test_tokens_list(
//        "shouldn't return empty tokens", tokens, n_tokens,
//        (char *[]){}, 0
//    );
//    free(tokens);
//    free(s);
}




/*
 * Utils.
 */
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

