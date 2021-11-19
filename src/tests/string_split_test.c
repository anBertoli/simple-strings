#include <strings.h>
#include "../string.h"
#include "../string_fmt.h"
#include "../string_split.h"
#include "framework/framework.h"
#include "../private/debug.h"

void test_strings_from_list(ss *got_str_list, int got_str_num, char **want_strings, int want_strings_num);

void test_ss_split_raw(void) {
    test_group("ss_split_raw");
    int n = 0;

    test_subgroup("missing delimiter");
    ss *strings = ss_split_raw("Ehy how are you?", "_", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy how are you?"}, 1);
    ss_list_free(strings, n);

    test_subgroup("empty delimiter");
    strings = ss_split_raw("Ehy how are you?", "", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy how are you?"}, 1);
    ss_list_free(strings, n);

    test_subgroup("empty string to split");
    strings = ss_split_raw("", "", &n);
    test_strings_from_list(strings, n, (char *[]) {""}, 0);
    ss_list_free(strings, n);

    test_subgroup("split in words");
    strings = ss_split_raw("Ehy how are you?", " ", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy", "how", "are", "you?"}, 4);
    ss_list_free(strings, n);

    test_subgroup("multi-char delimiter");
    strings = ss_split_raw("Ehy  how  are  you?", "  ", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy", "how", "are", "you?"}, 4);
    ss_list_free(strings, n);

    test_subgroup("multiple consecutive delimiters");
    strings = ss_split_raw("   Ehy    how   are   you?   ", " ", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy", "how", "are", "you?"}, 4);
    ss_list_free(strings, n);

    test_subgroup("only consecutive delimiters");
    strings = ss_split_raw("      ", " ", &n);
    test_strings_from_list(strings, n, (char *[]) {}, 0);
    ss_list_free(strings, n);
}

void test_ss_split_str(void) {
    test_group("ss_split_str");
    int n = 0;

    test_subgroup("missing delimiter");
    ss s = ss_new_from_raw("Ehy how are you?");
    ss *strings = ss_split_str(s, "_", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy how are you?"}, 1);
    ss_list_free(strings, n);
    ss_free(s);

    test_subgroup("empty delimiter");
    s = ss_new_from_raw("Ehy how are you?");
    strings = ss_split_str(s, "", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy how are you?"}, 1);
    ss_list_free(strings, n);
    ss_free(s);

    test_subgroup("empty string to split");
    s = ss_new_empty();
    strings = ss_split_str(s, "", &n);
    test_strings_from_list(strings, n, (char *[]) {""}, 0);
    ss_list_free(strings, n);
    ss_free(s);

    test_subgroup("split in words");
    s = ss_new_from_raw("Ehy how are you?");
    strings = ss_split_str(s, " ", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy", "how", "are", "you?"}, 4);
    ss_list_free(strings, n);
    ss_free(s);

    test_subgroup("multi-char delimiter");
    s = ss_new_from_raw("Ehy  how  are  you?");
    strings = ss_split_str(s, "  ", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy", "how", "are", "you?"}, 4);
    ss_list_free(strings, n);
    ss_free(s);

    test_subgroup("multiple consecutive delimiters");
    s = ss_new_from_raw("   Ehy    how   are   you?   ");
    strings = ss_split_str(s, " ", &n);
    test_strings_from_list(strings, n, (char *[]) {"Ehy", "how", "are", "you?"}, 4);
    ss_list_free(strings, n);
    ss_free(s);

    test_subgroup("only consecutive delimiters");
    s = ss_new_from_raw("     ");
    strings = ss_split_str(s, " ", &n);
    test_strings_from_list(strings, n, (char *[]) {}, 0);
    ss_list_free(strings, n);
    ss_free(s);
}

void test_ss_join_raw_cat(void) {
    test_group("ss_join_raw_cat");

    test_subgroup("empty separator");
    ss s = ss_new_empty();
    ss_join_raw_cat(s, (const char **)(char *[]){"how", "are", "you?" }, 3, "");
    test_strings("should have joined strings", "howareyou?", s->buf);
    test_equal("should have correct len", 10, s->len);
    test_equal("should have correct free", 10, s->free);
    ss_free(s);

    test_subgroup("empty strings");
    s = ss_new_empty();
    ss_join_raw_cat(s, (const char **)(char *[]){"", "", ""}, 3, "___");
    test_strings("should have joined strings", "______", s->buf);
    test_equal("should have correct len", 6, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("no strings");
    s = ss_new_empty();
    ss_join_raw_cat(s, (const char **)(char *[]){}, 0, "_");
    test_strings("should return empty string", "", s->buf);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("empty string and separator");
    s = ss_new_empty();
    ss_join_raw_cat(s, (const char **)(char *[]){"", "", ""}, 3, "");
    test_strings("should have joined strings", "", s->buf);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("simple join");
    s = ss_new_empty();
    ss_join_raw_cat(s, (const char **)(char *[]){"how", "are", "you?" }, 3, "___");
    test_strings("should have joined strings", "how___are___you?", s->buf);
    test_equal("should have correct len", 16, s->len);
    test_equal("should have correct free", 2, s->free);
    ss_free(s);

    test_subgroup("simple join with concat");
    s = ss_new_from_raw("prefix_");
    ss_join_raw_cat(s, (const char **)(char *[]){"how", "are", "you?" }, 3, "___");
    test_strings("should have joined strings", "prefix_how___are___you?", s->buf);
    test_equal("should have correct len", 23, s->len);
    test_equal("should have correct free", 9, s->free);
    ss_free(s);
}

void test_ss_join_raw(void) {
    test_group("ss_join_raw");

    test_subgroup("empty separator");
    ss s = ss_join_raw((const char **)(char *[]){"how", "are", "you?" }, 3, "");
    test_strings("should have joined strings", "howareyou?", s->buf);
    test_equal("should have correct len", 10, s->len);
    test_equal("should have correct free", 10, s->free);
    ss_free(s);

    test_subgroup("empty strings");
    s = ss_join_raw((const char **)(char *[]){"", "", ""}, 3, "___");
    test_strings("should have joined strings", "______", s->buf);
    test_equal("should have correct len", 6, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("no strings");
    s = ss_join_raw((const char **)(char *[]){}, 0, "_");
    test_strings("should return empty string", "", s->buf);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("empty string and separator");
    s = ss_join_raw((const char **)(char *[]){"", "", ""}, 3, "");
    test_strings("should have joined strings", "", s->buf);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("simple join");
    s = ss_join_raw((const char **)(char *[]){"how", "are", "you?" }, 3, "___");
    test_strings("should have joined strings", "how___are___you?", s->buf);
    test_equal("should have correct len", 16, s->len);
    test_equal("should have correct free", 2, s->free);
    ss_free(s);
}

void test_ss_join_str_cat(void) {
    test_group("ss_join_str_cat");

    test_subgroup("empty separator");
    ss s1 = ss_new_from_raw("how");
    ss s2 = ss_new_from_raw("are");
    ss s3 = ss_new_from_raw("you?");
    ss s = ss_new_empty();
    ss_join_str_cat(s, (ss []){s1, s2, s3}, 3, "");
    test_strings("should have joined strings", "howareyou?", s->buf);
    test_equal("should have correct len", 10, s->len);
    test_equal("should have correct free", 10, s->free);
    ss_free(s);
    ss_free(s1);
    ss_free(s2);
    ss_free(s3);

    test_subgroup("empty strings");
    s1 = ss_new_from_raw("");
    s2 = ss_new_from_raw("");
    s3 = ss_new_from_raw("");
    s = ss_new_empty();
    ss_join_str_cat(s, (ss []){s1, s2, s3}, 3, "___");
    test_strings("should have joined strings", "______", s->buf);
    test_equal("should have correct len", 6, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("no strings");
    s = ss_new_empty();
    ss_join_str_cat(s, (ss []){}, 0, "_");
    test_strings("should return empty string", "", s->buf);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("empty string and separator");
    s = ss_new_empty();
    ss_join_str_cat(s, (ss []){s1, s2, s3 }, 3, "");
    test_strings("should have joined strings", "", s->buf);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);
    ss_free(s1);
    ss_free(s2);
    ss_free(s3);

    test_subgroup("simple join");
    s1 = ss_new_from_raw("how");
    s2 = ss_new_from_raw("are");
    s3 = ss_new_from_raw("you?");
    s = ss_new_empty();
    ss_join_str_cat(s, (ss []){s1, s2, s3 }, 3, "___");
    test_strings("should have joined the strings", "how___are___you?", s->buf);
    test_equal("should have correct len", 16, s->len);
    test_equal("should have correct free", 2, s->free);
    ss_free(s);

    test_subgroup("simple join with concat");
    s = ss_new_from_raw("prefix_");
    ss_join_str_cat(s, (ss []){s1, s2, s3 }, 3, "___");
    test_strings("should have joined the strings", "prefix_how___are___you?", s->buf);
    test_equal("should have correct len", 23, s->len);
    test_equal("should have correct free", 9, s->free);
    ss_free(s);
    ss_free(s1);
    ss_free(s2);
    ss_free(s3);
}

void test_ss_join_str(void) {
    test_group("ss_join_str");

    test_subgroup("empty separator");
    ss s1 = ss_new_from_raw("how");
    ss s2 = ss_new_from_raw("are");
    ss s3 = ss_new_from_raw("you?");
    ss s = ss_join_str((ss []){s1, s2, s3}, 3, "");
    test_strings("should have joined strings", "howareyou?", s->buf);
    test_equal("should have correct len", 10, s->len);
    test_equal("should have correct free", 10, s->free);
    ss_free(s);
    ss_free(s1);
    ss_free(s2);
    ss_free(s3);

    test_subgroup("empty strings");
    s1 = ss_new_from_raw("");
    s2 = ss_new_from_raw("");
    s3 = ss_new_from_raw("");
    s = ss_join_str((ss []){s1, s2, s3}, 3, "___");
    test_strings("should have joined strings", "______", s->buf);
    test_equal("should have correct len", 6, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("no strings");
    s = ss_join_str((ss []){}, 0, "_");
    test_strings("should return empty string", "", s->buf);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);

    test_subgroup("empty string and separator");
    s = ss_join_str((ss []){s1, s2, s3 }, 3, "");
    test_strings("should have joined strings", "", s->buf);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    ss_free(s);
    ss_free(s1);
    ss_free(s2);
    ss_free(s3);

    test_subgroup("simple join");
    s1 = ss_new_from_raw("how");
    s2 = ss_new_from_raw("are");
    s3 = ss_new_from_raw("you?");
    s = ss_join_str((ss []){s1, s2, s3 }, 3, "___");
    test_strings("should have joined the strings", "how___are___you?", s->buf);
    test_equal("should have correct len", 16, s->len);
    test_equal("should have correct free", 2, s->free);
    ss_free(s);
    ss_free(s1);
    ss_free(s2);
    ss_free(s3);
}


void test_strings_from_list(ss *got_str_list, int got_str_num, char **want_strings, int want_strings_num) {
    if (got_str_num != want_strings_num) {
        ss s = ss_sprintf(
                "should have correct number of substrings: want '%d', got '%d'",
                want_strings_num, got_str_num
        );
        test_failure(s->buf);
        ss_free(s);
        return;
    }

    for (int i = 0; i < got_str_num; i++) {
        if (strcmp(want_strings[i], got_str_list[i]->buf) != 0) {
            ss s = ss_sprintf("want '%s', got '%s'", want_strings[i], got_str_list[i]->buf);
            test_failure(s->buf);
            ss_free(s);
            return;
        }

        if (strlen(want_strings[i]) != got_str_list[i]->len || got_str_list[i]->free != got_str_list[i]->len) {
            ss s = ss_sprintf(
                "want len: %d, got len: %d, want free: %d, got free %d",
                strlen(want_strings[i]), got_str_list[i]->len, got_str_list[i]->len, got_str_list[i]->free
           );
            test_failure(s->buf);
            ss_free(s);
            return;
        }
    }

    test_success("should have correct number of strings");
    test_success("should have correct and equal strings");
    test_success("should have len = free");
}

