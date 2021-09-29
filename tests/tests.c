#include <stdlib.h>
#include "../src/string.h"
#include "../src/utils.h"
#include "tests.h"
#include "utils.h"

void test_str_new_raw_len(void) {
    test_group("test str_new_raw_len");

    str *s = str_new_raw_len("testing", 7);
    test_equal("should have correct len", 7, s->len);
    test_equal("should have correct cap", 14, s->cap);
    test_strings("should have the correct string", s->buf, "testing");
    str_free(s);
}

void test_str_new_raw(void) {
    test_group("test str_new_raw");

    test_subgroup("non empty string");
    str *s = str_new_raw(" \\!ehy you__\n");
    test_equal("should have correct len", 13, s->len);
    test_equal("should have correct cap", 26, s->cap);
    test_strings("should have the correct string", s->buf, " \\!ehy you__\n");
    str_free(s);

    test_subgroup("with NULL char*");
    s = str_new_raw(NULL);
    test_equal("should have len 0 with NULL", 0, s->len);
    test_equal("should have cap 0 with NULL", 0, s->cap);
    test_strings("should have the correct string with NULL", s->buf, "");
    str_free(s);
}

void test_str_new_empty(void) {
    test_group("test str_new_empty");

    str *s = str_new_empty();
    test_equal("should have len 0", 0, s->len);
    test_equal("should have cap 0", 0, s->cap);
    test_strings("should have the correct string", s->buf, "");
    str_free(s);
}

void test_str_clone(void) {
    test_group("test str_clone");

    test_subgroup("normal clone");
    str *s1 = str_new_raw("ehy");
    str *s2 = str_clone(s1);
    test_equal("should have same len", s1->len, s2->len);
    test_equal("should have same cap", s1->cap, s2->cap);
    test_strings("should have the same string", s1->buf, s2->buf);
    test_cond("should have different buffer pointers", s1->buf != s2->buf);

    test_subgroup("test independence");
    str_concat_raw(s1, " how are you?");
    test_cond("should have different len", s1->len != s2->len);
    test_cond("should have different cap", s1->cap != s2->cap);
    test_cond("should have different strings", strcmp(s1->buf, s2->buf) != 0);
    str_free(s1);
    str_free(s2);
}

void test_str_concat_raw_len(void) {
    test_group("test str_concat_raw_len");

    test_subgroup("non empty strings");
    str *s1 = str_new_raw("Ehy ");
    str_concat_raw_len(s1, "you!", 4);
    test_equal("should have correct len", 8, s1->len);
    test_equal("should have correct cap", 8, s1->cap);
    test_strings("should have concat the strings", "Ehy you!", s1->buf);
    str_free(s1);

    test_subgroup("non empty strings with reallocations");
    s1 = str_new_raw("See you");
    str_concat_raw_len(s1, " later!!!", 9);
    test_equal("should have extended the len", 16, s1->len);
    test_equal("should have extended the cap", 32, s1->cap);
    test_strings("should have concat the strings", "See you later!!!", s1->buf);
    str_free(s1);

    test_subgroup("one empty string");
    s1 = str_new_raw("See you");
    str_concat_raw_len(s1, "", 0);
    test_equal("shouldn't have extended the len", 7, s1->len);
    test_equal("shouldn't have extended the cap", 14, s1->cap);
    test_strings("should have concat the strings", "See you", s1->buf);
    str_free(s1);

    test_subgroup("both empty strings");
    s1 = str_new_raw("");
    str_concat_raw_len(s1, "", 0);
    test_equal("shouldn't have extended the len", 0, s1->len);
    test_equal("shouldn't have extended the cap", 0, s1->cap);
    test_strings("should have concat the strings", "", s1->buf);
    str_free(s1);
}

void test_str_concat(void) {
    test_group("test str_concat");

    test_subgroup("non empty strings");
    str *s1 = str_new_raw("See you");
    str *s2 = str_new_raw(" later.");
    str_concat_str(s1, s2);
    test_equal("should have correct len", 14, s1->len);
    test_equal("should have correct cap", 14, s1->cap);
    test_strings("should have concat the strings", "See you later.", s1->buf);
    str_free(s1);
    str_free(s2);

    test_subgroup("non empty strings with reallocations");
    s1 = str_new_raw("See you");
    s2 = str_new_raw(" later!!!");
    str_concat_str(s1, s2);
    test_equal("should have extended the len", 16, s1->len);
    test_equal("should have extended the cap", 32, s1->cap);
    test_strings("should have concat the strings", "See you later!!!", s1->buf);
    str_free(s1);
    str_free(s2);

    test_subgroup("one empty string");
    s1 = str_new_raw("See you");
    s2 = str_new_raw("");
    str_concat_str(s1, s2);
    test_equal("shouldn't have extended the len", 7, s1->len);
    test_equal("shouldn't have extended the cap", 14, s1->cap);
    test_strings("should have concat the strings", "See you", s1->buf);
    str_free(s1);
    str_free(s2);

    test_subgroup("both empty strings");
    s1 = str_new_raw("");
    s2 = str_new_raw("");
    str_concat_str(s1, s2);
    test_equal("shouldn't have extended the len", 0, s1->len);
    test_equal("shouldn't have extended the cap", 0, s1->cap);
    test_strings("should have concat the strings", "", s1->buf);
    str_free(s1);
    str_free(s2);
}

void test_trim(void) {
    test_group("test str_trim");

    test_subgroup("one char in cutset");
    str *s1 = str_new_raw("   ehy   ");
    str_trim(s1, " ");
    test_equal("should reduce the len", 3, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("should have trimmed the string", "ehy", s1->buf);
    str_free(s1);

    test_subgroup("multiple chars in cutset");
    s1 = str_new_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    str_trim(s1, "_!?");
    test_equal("should reduce the len", 3, s1->len);
    test_equal("shouldn't change the cap", 54, s1->cap);
    test_strings("should have trimmed the string", "ehy", s1->buf);
    str_free(s1);

    test_subgroup("cut only right");
    s1 = str_new_raw("-!?ehy!_?");
    str_trim(s1, "_!?");
    test_equal("should reduce the len", 6, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("should have trimmed the string", "-!?ehy", s1->buf);
    str_free(s1);

    test_subgroup("all chars removed");
    s1 = str_new_raw("-ey!eyeh?ehy!_hhhh?");
    str_trim(s1, "_h!y?e-?");
    test_equal("should reduce the len", 0, s1->len);
    test_equal("shouldn't change the cap", 38, s1->cap);
    test_strings("should have trimmed all the string", "", s1->buf);
    str_free(s1);
}

void test_trim_right(void) {
    test_group("test str_trim_right");

    test_subgroup("one char in cutset");
    str *s1 = str_new_raw("   ehy   ");
    str_trim_right(s1, " ");
    test_equal("should reduce the len", 6, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("should have trimmed the string", "   ehy", s1->buf);
    str_free(s1);

    test_subgroup("multiple chars in cutset");
    s1 = str_new_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    str_trim_right(s1, "_!?");
    test_equal("should reduce the len", 15, s1->len);
    test_equal("shouldn't change the cap", 54, s1->cap);
    test_strings("should have trimmed the string", "__!!?!?_!_!?ehy", s1->buf);
    str_free(s1);

    test_subgroup("no trim left");
    s1 = str_new_raw("_!?ehy!?-");
    str_trim_right(s1, "_!?");
    test_equal("shouldn't change the len", 9, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("shouldn' have trimmed the string", "_!?ehy!?-", s1->buf);
    str_free(s1);

    test_subgroup("all chars removed");
    s1 = str_new_raw("-ey!eyeh?ehy!_hhhh?");
    str_trim_right(s1, "_h!y?e-");
    test_equal("should reduce the len", 0, s1->len);
    test_equal("shouldn't change the cap", 38, s1->cap);
    test_strings("should have trimmed all the string", "", s1->buf);
    str_free(s1);
}

void test_trim_left(void) {
    test_group("test str_trim_left");

    test_subgroup("one char in cutset");
    str *s1 = str_new_raw("   ehy   ");
    str_trim_right(s1, " ");
    test_equal("should reduce the len", 6, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("should have trimmed the string", "ehy   ", s1->buf);
    str_free(s1);

    test_subgroup("multiple chars in cutset");
    s1 = str_new_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    str_trim_right(s1, "_!?");
    test_equal("should reduce the len", 15, s1->len);
    test_equal("shouldn't change the cap", 54, s1->cap);
    test_strings("should have trimmed the string", "ehy!_!_!_?!_!!?", s1->buf);
    str_free(s1);

    test_subgroup("no trim right");
    s1 = str_new_raw("_!?ehy!?_");
    str_trim_right(s1, "_!?");
    test_equal("shouldn't change the len", 9, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("shouldn' have trimmed the string", "_!?ehy!?-", s1->buf);
    str_free(s1);

    test_subgroup("all chars removed");
    s1 = str_new_raw("-ey!eyeh?ehy!_hhhh?");
    str_trim_right(s1, "_h!y?e-");
    test_equal("should reduce the len", 0, s1->len);
    test_equal("shouldn't change the cap", 38, s1->cap);
    test_strings("should have trimmed all the string", "", s1->buf);
    str_free(s1);
}

void test_clear(void) {
    test_group("test str_clear");

    test_subgroup("non empty string");
    str *s1 = str_new_raw("ehy you");
    str_clear(s1);
    test_equal("should reduce the len to zero", 0, s1->len);
    test_equal("shouldn't change the cap", 14, s1->cap);
    test_strings("should have cleaned the string", "", s1->buf);
    str_free(s1);

    test_subgroup("already empty string");
    s1 = str_new_raw("");
    str_clear(s1);
    test_equal("shouldn't change the len", 0, s1->len);
    test_equal("shouldn't change the cap", 0, s1->cap);
    test_strings("should maintain the empty string", "", s1->buf);
    str_free(s1);
}

void test_grow(void) {
    test_group("test str_grow");

    test_subgroup("new free space > 0");
    str *s1 = str_new_raw("ehy you");
    str_grow(s1, 20);
    test_equal("shouldn't change the len to zero", 7, s1->len);
    test_equal("should change the cap", 27, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    str_free(s1);

    test_subgroup("new free space = 0");
    s1 = str_new_raw("ehy you");
    str_grow(s1, 0);
    test_equal("shouldn't change the len to zero", 7, s1->len);
    test_equal("should change the cap", 7, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    str_free(s1);
}

void test_shrink(void) {
    test_group("test str_shrink");

    str *s1 = str_new_raw("ehy you");
    str_shrink(s1);
    test_equal("shouldn't change the len to zero", 7, s1->len);
    test_equal("should change the cap", 7, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    str_free(s1);
}

void test_str_split_raw(void) {
    test_group("test str_split_raw");

    str_iter *s_iter = str_split_raw("Ehy how are you?", " ");
    test_cond("should point to the start", s_iter->buf == s_iter->ptr);
    test_strings("should have copied the del", " ", s_iter->del);
    str_iter_free(s_iter);
}

void test_str_split(void) {
    test_group("test str_split");

    str *s = str_new_raw("Ehy how are you?");
    str_iter *s_iter = str_split(s, "?|!");
    test_cond("should point to the start", s_iter->buf == s_iter->ptr);
    test_strings("should have copied the del", "?|!", s_iter->del);
    str_iter_free(s_iter);
}


void test_str_iter_next(void) {
    test_group("test str_iter_next");

    test_subgroup("split in words");
    str_iter *s_iter = str_split_raw("Ehy how are you?", " ");
    test_tokens_iter(
        "should split in words", s_iter, 4,
        (char *[]){"Ehy", "how", "are", "you?"}
    );

    test_subgroup("missing delimiter");
    s_iter = str_split_raw("Ehy how are you?", "_");
    test_tokens_iter(
        "should return the entire string", s_iter, 1,
        (char *[]){"Ehy how are you?"}
    );

    test_subgroup("empty delimiter");
    s_iter = str_split_raw("Ehy how are you?", "");
    test_tokens_iter(
        "should return the entire string", s_iter, 1,
        (char *[]){"Ehy how are you?"}
    );

    test_subgroup("multiple consecutive delimiters");
    s_iter = str_split_raw("   Ehy    how   are   you?   ", " ");
    test_tokens_iter(
        "shouldn't return empty tokens", s_iter, 4,
        (char *[]){"Ehy", "how", "are", "you?"}
    );

    test_subgroup("only consecutive delimiters");
    s_iter = str_split_raw("       ", " ");
    test_tokens_iter(
        "should return no tokens", s_iter, 0,
        (char *[]){}
    );
}

void test_str_collect_iter(void) {
    test_group("test str_collect_iter");
    int n_tokens;

    test_subgroup("split in words");
    str_iter *s_iter = str_split_raw("Ehy how are you?", " ");
    str **tokens = str_collect_iter(s_iter, &n_tokens);
    test_tokens_list(
        "should split in words", tokens, n_tokens,
        (char *[]){"Ehy", "how", "are", "you?"}, 4
    );

    test_subgroup("missing delimiter");
    s_iter = str_split_raw("Ehy how are you?", "_");
    tokens = str_collect_iter(s_iter, &n_tokens);
    test_tokens_list(
        "should return the entire string", tokens, n_tokens,
        (char *[]){"Ehy how are you?"}, 1
    );

    test_subgroup("empty delimiter");
    s_iter = str_split_raw("Ehy how are you?", "");
    tokens = str_collect_iter(s_iter, &n_tokens);
    test_tokens_list(
        "should return the entire string", tokens, n_tokens,
        (char *[]){"Ehy how are you?"}, 1
    );

    test_subgroup("multiple consecutive delimiters");
    s_iter = str_split_raw("   Ehy    how   are   you?   ", " ");
    tokens = str_collect_iter(s_iter, &n_tokens);
    test_tokens_list(
        "shouldn't return empty tokens", tokens, n_tokens,
        (char *[]){"Ehy", "how", "are", "you?"}, 4
    );

    test_subgroup("only consecutive delimiters");
    s_iter = str_split_raw("      ", " ");
    tokens = str_collect_iter(s_iter, &n_tokens);
    test_tokens_list(
    "shouldn't return empty tokens", tokens, n_tokens,
    (char *[]){}, 0
    );
}


int main(void) {
    test_str_new_raw_len();
    test_str_new_raw();
    test_str_new_empty();
    test_str_clone();
    test_str_concat_raw_len();
    test_str_concat();
    test_trim();
    test_trim_right();
    test_clear();
    test_grow();
    test_shrink();

    test_str_split_raw();
    test_str_split();
    test_str_iter_next();
    test_str_collect_iter();

    return test_report();
}
