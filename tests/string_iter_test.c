#include <strings.h>
#include "../src/string.h"
#include "../src/string_fmt.h"
#include "../src/internal/debug.h"
#include "../src/string_iter.h"
#include "framework/framework.h"

void test_tokens_from_iter(ss_iter *s_iter, char **tokens, int num_tokens);
void test_tokens_from_list(ss_list *str_list, char **tokens, int num_tokens);

void test_ss_split_raw_to_iter(void) {
    test_group("ss_split_raw_to_iter");

    ss_iter *s_iter = ss_split_raw_to_iter("Ehy how are you?", " ");
    test_cond("should point to the start", s_iter->buf == s_iter->ptr);
    test_cond("should have correct delimiter", strcmp(s_iter->del, " ") == 0);
    test_cond("should have not ended", s_iter->end == ITER_ACTIVE);
    test_strings("should have copied the del", " ", s_iter->del);
    ss_iter_free(s_iter);
}

void test_ss_split_str_to_iter(void) {
    test_group("ss_split_str_to_iter");

    ss *s = ss_new_from_raw("Ehy how are you?");
    ss_iter *s_iter = ss_split_str_to_iter(s, " ");
    test_cond("should point to the start", s_iter->buf == s_iter->ptr);
    test_cond("should have correct delimiter", strcmp(s_iter->del, " ") == 0);
    test_cond("should have not ended", s_iter->end == ITER_ACTIVE);
    test_strings("should have copied the del", " ", s_iter->del);
    ss_iter_free(s_iter);
}

void test_ss_iter_next(void) {
    test_group("ss_iter_next");

    test_subgroup("split in words");
    ss_iter *s_iter = ss_split_raw_to_iter("Ehy how are you?", " ");
    test_tokens_from_iter(s_iter, (char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("missing delimiter");
    s_iter = ss_split_raw_to_iter("Ehy how are you?", "_");
    test_tokens_from_iter(s_iter, (char *[]){"Ehy how are you?"}, 1);

    test_subgroup("empty delimiter");
    s_iter = ss_split_raw_to_iter("Ehy how are you?", "");
    test_tokens_from_iter(s_iter, (char *[]){"Ehy how are you?"}, 1);

    test_subgroup("multiple consecutive delimiters");
    s_iter = ss_split_raw_to_iter("   Ehy    how   are   you?   ", " ");
    test_tokens_from_iter(s_iter, (char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("only consecutive delimiters");
    s_iter = ss_split_raw_to_iter("       ", " ");
    test_tokens_from_iter(s_iter,(char *[]){}, 0);
}

void test_ss_iter_collect(void) {
    test_group("ss_iter_collect");

    test_subgroup("split in words");
    ss_iter *s_iter = ss_split_raw_to_iter("Ehy how are you?", " ");
    ss_list *list = ss_iter_collect(s_iter);
    test_tokens_from_list(list,(char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("missing delimiter");
    s_iter = ss_split_raw_to_iter("Ehy how are you?", "_");
    list = ss_iter_collect(s_iter);
    test_tokens_from_list(list,(char *[]){"Ehy how are you?"}, 1);

    test_subgroup("empty delimiter");
    s_iter = ss_split_raw_to_iter("Ehy how are you?", "");
    list = ss_iter_collect(s_iter);
    test_tokens_from_list(list,(char *[]){"Ehy how are you?"}, 1);

    test_subgroup("multiple consecutive delimiters");
    s_iter = ss_split_raw_to_iter("   Ehy    how   are   you?   ", " ");
    list = ss_iter_collect(s_iter);
    test_tokens_from_list(list,(char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("only consecutive delimiters");
    s_iter = ss_split_raw_to_iter("      ", " ");
    list = ss_iter_collect(s_iter);
    test_tokens_from_list(list,(char *[]){}, 0);
}

void test_ss_split_raw(void) {
    test_group("ss_split_row");

    test_subgroup("split in words");
    ss_list *tokens = ss_split_row("Ehy how are you?", " ");
    test_tokens_from_list(tokens, (char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("missing delimiter");
    tokens = ss_split_row("Ehy how are you?", "_");
    test_tokens_from_list(tokens,(char *[]){"Ehy how are you?"}, 1);

    test_subgroup("empty delimiter");
    tokens = ss_split_row("Ehy how are you?", "");
    test_tokens_from_list(tokens,(char *[]){"Ehy how are you?"}, 1);

    test_subgroup("multiple consecutive delimiters");
    tokens = ss_split_row("   Ehy    how   are   you?   ", " ");
    test_tokens_from_list(tokens,(char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("only consecutive delimiters");
    tokens = ss_split_row("      ", " ");
    test_tokens_from_list(tokens,(char *[]){}, 0);
}

void test_ss_split_str(void) {
    test_group("test ss_collect_from_str");

    test_subgroup("split in words");
    ss *s = ss_new_from_raw("Ehy how are you?");
    ss_list *list = ss_split_str(s, " ");
    test_tokens_from_list(list,(char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("missing delimiter");
    s = ss_new_from_raw("Ehy how are you?");
    list = ss_split_str(s, "_");
    test_tokens_from_list(list, (char *[]){"Ehy how are you?"}, 1);

    test_subgroup("empty delimiter");
    s = ss_new_from_raw("Ehy how are you?");
    list = ss_split_str(s, "");
    test_tokens_from_list(list,(char *[]){"Ehy how are you?"}, 1);

    test_subgroup("multiple consecutive delimiters");
    s = ss_new_from_raw("   Ehy    how   are   you?   ");
    list = ss_split_str(s, " ");
    test_tokens_from_list(list,(char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("only consecutive delimiters");
    s = ss_new_from_raw("      ");
    list = ss_split_str(s, " ");
    test_tokens_from_list(list, (char *[]){}, 0);
}


// Utilities used in iterator and lists tests.

void test_tokens_from_iter(ss_iter *s_iter, char **tokens, int num_tokens) {
    int i = 0;
    while (1) {
        ss *s_next = ss_iter_next(s_iter);
        if (s_next == END_ITER) break;
        if (s_next == NULL) {
            test_failure("s_next == NULL");
            return;
        }

        if (i+1 > num_tokens) {
            test_failure("should have the correct number of substrings");
            return;
        }
        if (strcmp(tokens[i], s_next->buf) != 0) {
            ss *s = ss_sprintf("want '%s', got '%s'", tokens[i], s_next->buf);
            test_failure(s->buf);
            return;
        }
        if (strlen(tokens[i]) != s_next->len || s_next->cap != s_next->len) {
            ss *s = ss_sprintf("len wanted token: %d, got len %d, got cap %d ", strlen(tokens[i]), s_next->len, s_next->cap);
            test_failure(s->buf);
            return;
        }
        i++;
    }

    if (i != num_tokens) {
        ss *s = ss_sprintf("want '%s' tokens, got '%s'", num_tokens, i);
        test_failure(s->buf);
        return;
    }

    test_success("should have the correct number of words");
    test_success("should have len = cap = length of word");
}

void test_tokens_from_list(ss_list *str_list, char **tokens, int num_tokens) {
    if (str_list->len != num_tokens) {
        test_failure("should have the correct number of substrings");
        return;
    }

    for (int i = 0; i < str_list->len; i++) {
        if (strcmp(tokens[i], str_list->list[i]->buf) != 0) {
            ss *s = ss_sprintf("want '%s', got '%s'", tokens[i], str_list->list[i]->buf);
            test_failure(s->buf);
            return;
        }

        if (strlen(tokens[i]) != str_list->list[i]->len || str_list->list[i]->cap != str_list->list[i]->len) {
            ss *s = ss_sprintf(
                "len wanted token: %d, got len %d, got cap %d",
               strlen(tokens[i]), str_list->list[i]->len, str_list->list[i]->cap
           );
            test_failure(s->buf);
            return;
        }
    }

    test_success("should have the correct number of words");
    test_success("should have len = cap = length of word");
}

