#include <strings.h>
#include "../string.h"
#include "../string_fmt.h"
#include "../string_split.h"
#include "framework/framework.h"
#include "../private/debug.h"

void test_tokens_from_list(ss *got_str_list, int got_num_str, char **want_strings, int want_num_strings);

void test_ss_split_raw(void) {
    test_group("ss_split_raw");
    int n = 0;

    test_subgroup("split in words");
    ss *tokens = ss_split_raw("Ehy how are you?", " ", &n);
    test_tokens_from_list(tokens, n, (char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("missing delimiter");
    tokens = ss_split_raw("Ehy how are you?", "_", &n);
    test_tokens_from_list(tokens, n, (char *[]){"Ehy how are you?"}, 1);

    test_subgroup("empty delimiter");
    tokens = ss_split_raw("Ehy how are you?", "", &n);
    test_tokens_from_list(tokens, n, (char *[]){"Ehy how are you?"}, 1);

    test_subgroup("multiple consecutive delimiters");
    tokens = ss_split_raw("   Ehy    how   are   you?   ", " ", &n);
    test_tokens_from_list(tokens, n, (char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("only consecutive delimiters");
    tokens = ss_split_raw("      ", " ", &n);
    test_tokens_from_list(tokens, n, (char *[]){}, 0);
}

void test_ss_split_str(void) {
    test_group("ss_collect_from_str");
    int n = 0;

    test_subgroup("split in words");
    ss s = ss_new_from_raw("Ehy how are you?");
    ss *list = ss_split_str(s, " ", &n);
    test_tokens_from_list(list, n, (char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("missing delimiter");
    s = ss_new_from_raw("Ehy how are you?");
    list = ss_split_str(s, "_", &n);
    test_tokens_from_list(list, n, (char *[]){"Ehy how are you?"}, 1);

    test_subgroup("empty delimiter");
    s = ss_new_from_raw("Ehy how are you?");
    list = ss_split_str(s, "", &n);
    test_tokens_from_list(list, n, (char *[]){"Ehy how are you?"}, 1);

    test_subgroup("multiple consecutive delimiters");
    s = ss_new_from_raw("   Ehy    how   are   you?   ");
    list = ss_split_str(s, " ", &n);
    test_tokens_from_list(list, n, (char *[]){"Ehy", "how", "are", "you?"}, 4);

    test_subgroup("only consecutive delimiters");
    s = ss_new_from_raw("      ");
    list = ss_split_str(s, " ", &n);
    test_tokens_from_list(list, n, (char *[]){}, 0);
}

void test_ss_join_raw(void) {
    test_group("ss_join_raw");

    test_subgroup("simple join");

    ss s1 = ss_join_raw((char *[]){ "how", "are", "you?" }, 3, "___");
    test_strings("should have joined the strings", "how___are___you?", s1->buf);
    test_equal("should have the correct len", 16, s1->len);
    test_equal("should have the correct free", 2, s1->free);
    ss_free(s1);

    test_subgroup("empty separator");
    s1 = ss_join_raw((char *[]){ "how", "are", "you?" }, 3, "");
    test_strings("should have joined the strings", "howareyou?", s1->buf);
    test_equal("should have the correct len", 10, s1->len);
    test_equal("should have the correct free", 10, s1->free);
    ss_free(s1);

    test_subgroup("empty strings");
    s1 = ss_join_raw((char *[]){ "", "", "" }, 3, "___");
    test_strings("should have joined the strings", "______", s1->buf);
    test_equal("should have the correct len", 6, s1->len);
    test_equal("should have the correct free", 0, s1->free);
    ss_free(s1);

    test_subgroup("empty string and separator");
    s1 = ss_join_raw((char *[]){ "", "", "" }, 3, "");
    test_strings("should have joined the strings", "", s1->buf);
    test_equal("should have the correct len", 0, s1->len);
    test_equal("should have the correct free", 0, s1->free);
    ss_free(s1);
}

void test_ss_join_str(void) {
    test_group("ss_join_str");

    test_subgroup("simple join");
    ss s1 = ss_new_from_raw("how");
    ss s2 = ss_new_from_raw("are");
    ss s3 = ss_new_from_raw("you?");

    ss s4 = ss_join_str((ss []){ s1,s2,s3 }, 3, "___");
    test_strings("should have joined the strings", "how___are___you?", s4->buf);
    test_equal("should have the correct len", 16, s4->len);
    test_equal("should have the correct free", 2, s4->free);
    ss_free(s4);

    test_subgroup("empty separator");
    s4 = ss_join_str((ss []){ s1,s2,s3 }, 3, "");
    test_strings("should have joined the strings", "howareyou?", s4->buf);
    test_equal("should have the correct len", 10, s4->len);
    test_equal("should have the correct free", 10, s4->free);
    ss_free(s4);

    ss_free(s1);
    ss_free(s2);
    ss_free(s3);

    test_subgroup("empty strings");
    s1 = ss_new_from_raw("");
    s2 = ss_new_from_raw("");
    s3 = ss_new_from_raw("");

    s4 = ss_join_str((ss []){ s1,s2,s3 }, 3, "___");
    test_strings("should have joined the strings", "______", s4->buf);
    test_equal("should have the correct len", 6, s4->len);
    test_equal("should have the correct free", 0, s4->free);
    ss_free(s4);

    test_subgroup("empty string and separator");
    s4 = ss_join_str((ss []){ s1,s2,s3 }, 3, "");
    test_strings("should have joined the strings", "", s4->buf);
    test_equal("should have the correct len", 0, s4->len);
    test_equal("should have the correct free", 0, s4->free);
    ss_free(s4);

    ss_free(s1);
    ss_free(s2);
    ss_free(s3);
}


void test_tokens_from_list(ss *got_str_list, int got_num_str, char **want_strings, int want_num_strings) {
    if (got_num_str != want_num_strings) {
        ss s = ss_sprintf(
                "should have the correct number of substrings: want '%d', got '%d'",
                want_num_strings, got_num_str
        );
        test_failure(s->buf);
        return;
    }

    for (int i = 0; i < got_num_str; i++) {
        if (strcmp(want_strings[i], got_str_list[i]->buf) != 0) {
            ss s = ss_sprintf("want '%s', got '%s'", want_strings[i], got_str_list[i]->buf);
            test_failure(s->buf);
            return;
        }

        if (strlen(want_strings[i]) != got_str_list[i]->len || got_str_list[i]->free != got_str_list[i]->len) {
            ss s = ss_sprintf(
                "len token, wanted: %d, got %d, want free: %d, got free %d",
                strlen(want_strings[i]), got_str_list[i]->len, got_str_list[i]->len, got_str_list[i]->free
           );
            test_failure(s->buf);
            return;
        }
    }

    test_success("should have the correct number of words");
    test_success("should have len = free = length of word");
}

