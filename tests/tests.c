#include <stdlib.h>
#include <strings.h>
#include "../src/string.h"
#include "../src/string_fmt.h"
#include "../src/string_iter.h"
#include "framework/framework.h"
#include "utils.h"

/*
 * String manipulation tests.
 */

void test_ss_new_raw_len_cap(void) {
    test_group("test ss_new_raw_len_cap");

    ss *s = ss_new_from_raw_len_cap("testing", 7, 14);
    test_equal("should have correct len", 7, s->len);
    test_equal("should have correct cap", 14, s->cap);
    test_strings("should have the correct string", s->buf, "testing");
    ss_free(s);

    s = ss_new_from_raw_len_cap("testing", 3, 1);
    test_equal("should have correct len", 3, s->len);
    test_equal("should have correct cap", 3, s->cap);
    test_strings("should have the correct string", s->buf, "tes");
    ss_free(s);
}

void test_ss_new_raw_len(void) {
    test_group("test ss_new_raw_len");

    ss *s = ss_new_from_raw_len("testing", 7);
    test_equal("should have correct len", 7, s->len);
    test_equal("should have correct cap", 14, s->cap);
    test_strings("should have the correct string", s->buf, "testing");
    ss_free(s);

    s = ss_new_from_raw_len("testing", 3);
    test_equal("should cut the string", 3, s->len);
    test_equal("should have correct cap", 6, s->cap);
    test_strings("should have the correct string", s->buf, "tes");
    ss_free(s);
}

void test_ss_new_raw(void) {
    test_group("test ss_new_raw");

    test_subgroup("non empty string");
    ss *s = ss_new_from_raw(" \\!ehy you__\n");
    test_equal("should have correct len", 13, s->len);
    test_equal("should have correct cap", 26, s->cap);
    test_strings("should have the correct string", s->buf, " \\!ehy you__\n");
    ss_free(s);

    test_subgroup("with NULL char*");
    s = ss_new_from_raw(NULL);
    test_equal("should have len 0 with NULL", 0, s->len);
    test_equal("should have cap 0 with NULL", 0, s->cap);
    test_strings("should have the correct string with NULL", s->buf, "");
    ss_free(s);
}

void test_ss_new_empty(void) {
    test_group("test ss_new_empty");

    ss *s = ss_new_empty();
    test_equal("should have len 0", 0, s->len);
    test_equal("should have cap 0", 0, s->cap);
    test_strings("should have the correct string", s->buf, "");
    ss_free(s);
}

void test_ss_clone(void) {
    test_group("test ss_clone");

    test_subgroup("normal clone");
    ss *s1 = ss_new_from_raw("ehy");
    ss *s2 = ss_clone(s1);
    test_equal("should have same len", s1->len, s2->len);
    test_equal("should have same cap", s1->cap, s2->cap);
    test_strings("should have the same string", s1->buf, s2->buf);
    test_cond("should have different buffer pointers", s1->buf != s2->buf);

    test_subgroup("test independence");
    ss_concat_raw(s1, " how are you?");
    test_cond("should have different len", s1->len != s2->len);
    test_cond("should have different cap", s1->cap != s2->cap);
    test_cond("should have different strings", strcmp(s1->buf, s2->buf) != 0);
    ss_free(s1);
    ss_free(s2);
}

void test_ss_concat_raw_len(void) {
    test_group("test ss_concat_raw_len");

    test_subgroup("non empty strings");
    ss *s1 = ss_new_from_raw("Ehy ");
    ss_concat_raw_len(s1, "you!", 4);
    test_equal("should have correct len", 8, s1->len);
    test_equal("should have correct cap", 8, s1->cap);
    test_strings("should have concat the strings", "Ehy you!", s1->buf);
    ss_free(s1);

    test_subgroup("non empty strings with reallocations");
    s1 = ss_new_from_raw("See you");
    ss_concat_raw_len(s1, " later!!!", 9);
    test_equal("should have extended the len", 16, s1->len);
    test_equal("should have extended the cap", 32, s1->cap);
    test_strings("should have concat the strings", "See you later!!!", s1->buf);
    ss_free(s1);

    test_subgroup("one empty string");
    s1 = ss_new_from_raw("See you");
    ss_concat_raw_len(s1, "", 0);
    test_equal("shouldn't have extended the len", 7, s1->len);
    test_equal("shouldn't have extended the cap", 14, s1->cap);
    test_strings("should have concat the strings", "See you", s1->buf);
    ss_free(s1);

    test_subgroup("both empty strings");
    s1 = ss_new_from_raw("");
    ss_concat_raw_len(s1, "", 0);
    test_equal("shouldn't have extended the len", 0, s1->len);
    test_equal("shouldn't have extended the cap", 0, s1->cap);
    test_strings("should have concat the strings", "", s1->buf);
    ss_free(s1);
}

void test_ss_concat_raw(void) {
    test_group("test ss_concat_raw");

    test_subgroup("non empty strings");
    ss *s1 = ss_new_from_raw("Ehy ");
    ss_concat_raw(s1, "you!");
    test_equal("should have correct len", 8, s1->len);
    test_equal("should have correct cap", 8, s1->cap);
    test_strings("should have concat the strings", "Ehy you!", s1->buf);
    ss_free(s1);

    test_subgroup("non empty strings with reallocations");
    s1 = ss_new_from_raw("See you");
    ss_concat_raw(s1, " later!!!");
    test_equal("should have extended the len", 16, s1->len);
    test_equal("should have extended the cap", 32, s1->cap);
    test_strings("should have concat the strings", "See you later!!!", s1->buf);
    ss_free(s1);

    test_subgroup("one empty string");
    s1 = ss_new_from_raw("See you");
    ss_concat_raw(s1, "");
    test_equal("shouldn't have extended the len", 7, s1->len);
    test_equal("shouldn't have extended the cap", 14, s1->cap);
    test_strings("should have concat the strings", "See you", s1->buf);
    ss_free(s1);

    test_subgroup("both empty strings");
    s1 = ss_new_from_raw("");
    ss_concat_raw(s1, "");
    test_equal("shouldn't have extended the len", 0, s1->len);
    test_equal("shouldn't have extended the cap", 0, s1->cap);
    test_strings("should have concat the strings", "", s1->buf);
    ss_free(s1);
}

void test_ss_concat_str(void) {
    test_group("test ss_concat_str");

    test_subgroup("non empty strings");
    ss *s1 = ss_new_from_raw("See you");
    ss *s2 = ss_new_from_raw(" later.");
    ss_concat_str(s1, s2);
    test_equal("should have correct len", 14, s1->len);
    test_equal("should have correct cap", 14, s1->cap);
    test_strings("should have concat the strings", "See you later.", s1->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("non empty strings with reallocations");
    s1 = ss_new_from_raw("See you");
    s2 = ss_new_from_raw(" later!!!");
    ss_concat_str(s1, s2);
    test_equal("should have extended the len", 16, s1->len);
    test_equal("should have extended the cap", 32, s1->cap);
    test_strings("should have concat the strings", "See you later!!!", s1->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("one empty string");
    s1 = ss_new_from_raw("See you");
    s2 = ss_new_from_raw("");
    ss_concat_str(s1, s2);
    test_equal("shouldn't have extended the len", 7, s1->len);
    test_equal("shouldn't have extended the cap", 14, s1->cap);
    test_strings("should have concat the strings", "See you", s1->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("both empty strings");
    s1 = ss_new_from_raw("");
    s2 = ss_new_from_raw("");
    ss_concat_str(s1, s2);
    test_equal("shouldn't have extended the len", 0, s1->len);
    test_equal("shouldn't have extended the cap", 0, s1->cap);
    test_strings("should have concat the strings", "", s1->buf);
    ss_free(s1);
    ss_free(s2);
}

void test_trim(void) {
    test_group("test ss_trim");

    test_subgroup("one char in cutset");
    ss *s1 = ss_new_from_raw("   ehy   ");
    ss_trim(s1, " ");
    test_equal("should reduce the len", 3, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("should have trimmed the string", "ehy", s1->buf);
    ss_free(s1);

    test_subgroup("one char in cutset");
    s1 = ss_new_from_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    ss_trim(s1, "?");
    test_equal("should reduce the len", 26, s1->len);
    test_equal("shouldn't change the cap", 54, s1->cap);
    test_strings("should have trimmed the string", "__!!?!?_!_!?ehy!_!_!_?!_!!", s1->buf);
    ss_free(s1);

    test_subgroup("multiple chars in cutset");
    s1 = ss_new_from_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    ss_trim(s1, "_!?");
    test_equal("should reduce the len", 3, s1->len);
    test_equal("shouldn't change the cap", 54, s1->cap);
    test_strings("should have trimmed the string", "ehy", s1->buf);
    ss_free(s1);

    test_subgroup("cut only right");
    s1 = ss_new_from_raw("-!?ehy!_?");
    ss_trim(s1, "_!?");
    test_equal("should reduce the len", 6, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("should have trimmed the string", "-!?ehy", s1->buf);
    ss_free(s1);

    test_subgroup("all chars removed");
    s1 = ss_new_from_raw("-ey!eyeh?ehy!_hhhh?");
    ss_trim(s1, "_h!y?e-?");
    test_equal("should reduce the len", 0, s1->len);
    test_equal("shouldn't change the cap", 38, s1->cap);
    test_strings("should have trimmed all the string", "", s1->buf);
    ss_free(s1);
}

void test_trim_right(void) {
    test_group("test ss_trim_right");

    test_subgroup("one char in cutset");
    ss *s1 = ss_new_from_raw("   ehy   ");
    ss_trim_right(s1, " ");
    test_equal("should reduce the len", 6, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("should have trimmed the string", "   ehy", s1->buf);
    ss_free(s1);

    test_subgroup("one char in cutset");
    s1 = ss_new_from_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    ss_trim(s1, "?");
    test_equal("should reduce the len", 26, s1->len);
    test_equal("shouldn't change the cap", 54, s1->cap);
    test_strings("should have trimmed the string", "__!!?!?_!_!?ehy!_!_!_?!_!!", s1->buf);
    ss_free(s1);

    test_subgroup("multiple chars in cutset");
    s1 = ss_new_from_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    ss_trim_right(s1, "_!?");
    test_equal("should reduce the len", 15, s1->len);
    test_equal("shouldn't change the cap", 54, s1->cap);
    test_strings("should have trimmed the string", "__!!?!?_!_!?ehy", s1->buf);
    ss_free(s1);

    test_subgroup("no trim left");
    s1 = ss_new_from_raw("_!?ehy!?-");
    ss_trim_right(s1, "_!?");
    test_equal("shouldn't change the len", 9, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("shouldn' have trimmed the string", "_!?ehy!?-", s1->buf);
    ss_free(s1);

    test_subgroup("all chars removed");
    s1 = ss_new_from_raw("-ey!eyeh?ehy!_hhhh?");
    ss_trim_right(s1, "_h!y?e-");
    test_equal("should reduce the len", 0, s1->len);
    test_equal("shouldn't change the cap", 38, s1->cap);
    test_strings("should have trimmed all the string", "", s1->buf);
    ss_free(s1);
}

void test_trim_left(void) {
    test_group("test ss_trim_left");

    test_subgroup("one char in cutset");
    ss *s1 = ss_new_from_raw("   ehy   ");
    ss_trim_left(s1, " ");
    test_equal("should reduce the len", 6, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("should have trimmed the string", "ehy   ", s1->buf);
    ss_free(s1);

    test_subgroup("one char in cutset");
    s1 = ss_new_from_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    ss_trim_left(s1, "?");
    test_equal("shouldn't reduce the len", 27, s1->len);
    test_equal("shouldn't change the cap", 54, s1->cap);
    test_strings("should have trimmed the string", "__!!?!?_!_!?ehy!_!_!_?!_!!?", s1->buf);
    ss_free(s1);

    test_subgroup("multiple chars in cutset");
    s1 = ss_new_from_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    ss_trim_left(s1, "_!?");
    test_equal("should reduce the len", 15, s1->len);
    test_equal("shouldn't change the cap", 54, s1->cap);
    test_strings("should have trimmed the string", "ehy!_!_!_?!_!!?", s1->buf);
    ss_free(s1);

    test_subgroup("no trim right");
    s1 = ss_new_from_raw("-!?ehy!?_");
    ss_trim_left(s1, "_!?");
    test_equal("shouldn't change the len", 9, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("shouldn' have trimmed the string", "-!?ehy!?_", s1->buf);
    ss_free(s1);

    test_subgroup("all chars removed");
    s1 = ss_new_from_raw("-ey!eyeh?ehy!_hhhh?");
    ss_trim_left(s1, "_h!y?e-");
    test_equal("should reduce the len", 0, s1->len);
    test_equal("shouldn't change the cap", 38, s1->cap);
    test_strings("should have trimmed all the string", "", s1->buf);
    ss_free(s1);
}

void test_cut(void) {
    test_group("test ss_cut");

    test_subgroup("non empty string");
    ss *s1 = ss_new_from_raw("ehy you");
    ss_cut(s1, 4);
    test_equal("should reduce the len", 4, s1->len);
    test_equal("shouldn't change the cap", 14, s1->cap);
    test_strings("should have reduced the string", "ehy ", s1->buf);
    ss_free(s1);

    test_subgroup("len cut > string len");
    s1 = ss_new_from_raw("ehy you");
    ss_cut(s1, 10);
    test_equal("shouldn't reduce the len", 7, s1->len);
    test_equal("shouldn't change the cap", 14, s1->cap);
    test_strings("shouldn't have reduced the string", "ehy you", s1->buf);
    ss_free(s1);

    test_subgroup("empty string");
    s1 = ss_new_from_raw("");
    ss_cut(s1, 10);
    test_equal("shouldn't change the len", 0, s1->len);
    test_equal("shouldn't change the cap", 0, s1->cap);
    test_strings("should maintain the empty string", "", s1->buf);
    ss_free(s1);
}

void test_clear(void) {
    test_group("test ss_clear");

    test_subgroup("non empty string");
    ss *s1 = ss_new_from_raw("ehy you");
    ss_clear(s1);
    test_equal("should reduce the len to zero", 0, s1->len);
    test_equal("shouldn't change the cap", 14, s1->cap);
    test_strings("should have cleaned the string", "", s1->buf);
    ss_free(s1);

    test_subgroup("already empty string");
    s1 = ss_new_from_raw("");
    ss_clear(s1);
    test_equal("shouldn't change the len", 0, s1->len);
    test_equal("shouldn't change the cap", 0, s1->cap);
    test_strings("should maintain the empty string", "", s1->buf);
    ss_free(s1);
}

void test_set_free_space(void) {
    test_group("test ss_grow");

    test_subgroup("new free space > 0");
    ss *s1 = ss_new_from_raw("ehy you");
    ss_set_free_space(s1, 10);
    test_equal("shouldn't change the len to zero", 7, s1->len);
    test_equal("should augment the cap", 17, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);

    test_subgroup("new free space = 0");
    s1 = ss_new_from_raw("ehy you");
    ss_set_free_space(s1, 0);
    test_equal("shouldn't change the len to zero", 7, s1->len);
    test_equal("should set cap = len", 7, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);
}

void test_grow(void) {
    test_group("test ss_grow");

    test_subgroup("new free space > 0");
    ss *s1 = ss_new_from_raw("ehy you");
    ss_grow(s1, 20);
    test_equal("shouldn't change the len to zero", 7, s1->len);
    test_equal("should change the cap", 34, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);

    test_subgroup("new free space = 0");
    s1 = ss_new_from_raw("ehy you");
    ss_grow(s1, 0);
    test_equal("shouldn't change the len to zero", 7, s1->len);
    test_equal("shouldn't change the cap", 14, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);
}

void test_shrink(void) {
    test_group("test ss_shrink");

    ss *s1 = ss_new_from_raw("ehy you");
    ss_shrink(s1);
    test_equal("shouldn't change the len to zero", 7, s1->len);
    test_equal("should change the cap", 7, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);
}

/*
 * String formatting tests.
 */

void test_ss_sprintf(void) {
    test_group("test ss_sprintf");

    test_subgroup("no variadic arguments");
    ss *s = ss_sprintf("test=123 test=ehy");
    test_strings("should have formatted the string", "test=123 test=ehy", s->buf);
    test_equal("should have correct len", 17, s->len);
    test_equal("should have correct cap", 34, s->cap);

    test_subgroup("formatting with arguments");
    s = ss_sprintf("test=%d test=%s", 123, "success");
    test_strings("should have formatted the string", "test=123 test=success", s->buf);
    test_equal("should have correct len", 21, s->len);
    test_equal("should have correct cap", 42, s->cap);
}

void test_ss_sprintf_concat(void) {
    test_group("test ss_sprintf_concat");

    test_subgroup("with empty string");
    ss *s = ss_new_empty();
    ss_sprintf_concat(s, "test=123 test=ehy");
    test_strings("should have formatted the string", "test=123 test=ehy", s->buf);
    test_equal("should have correct len", 17, s->len);
    test_equal("should have correct cap", 34, s->cap);
    free(s);

    test_subgroup("formatting with arguments");
    s = ss_new_from_raw("ehy, ");
    ss_sprintf_concat(s, "test=%d test=%s", 123, "success");
    test_strings("should have formatted the string", "ehy, test=123 test=success", s->buf);
    test_equal("should have correct len", 26, s->len);
    test_equal("should have correct cap", 52, s->cap);
    free(s);

    test_subgroup("formatting with arguments, enough free space");
    s = ss_new_from_raw_len_cap("ehy, ", 5, 30);
    ss_sprintf_concat(s, "test=%d test=%s", 123, "success");
    test_strings("should have formatted the string", "ehy, test=123 test=success", s->buf);
    test_equal("should have correct len", 26, s->len);
    test_equal("should have correct cap", 30, s->cap);
    free(s);
}

/*
 * Iterator tests.
 */

void test_ss_split_raw(void) {
    test_group("test ss_split_raw");

    ss_iter *s_iter = ss_split_raw_to_iter("Ehy how are you?", " ");
    test_cond("should point to the start", s_iter->buf == s_iter->ptr);
    test_strings("should have copied the del", " ", s_iter->del);
    ss_iter_free(s_iter);
}

void test_ss_split(void) {
    test_group("test ss_split");

    ss *s = ss_new_from_raw("Ehy how are you?");
    ss_iter *s_iter = ss_split_str_to_iter(s, "?|!");
    test_cond("should point to the start", s_iter->buf == s_iter->ptr);
    test_strings("should have copied the del", "?|!", s_iter->del);
    ss_iter_free(s_iter);
}

void test_ss_iter_next(void) {
    test_group("test ss_iter_next");

    test_subgroup("split in words");
    ss_iter *s_iter = ss_split_raw_to_iter("Ehy how are you?", " ");
    test_tokens_iter(
        "should split in words", s_iter, 4,
        (char *[]){"Ehy", "how", "are", "you?"}
    );

    test_subgroup("missing delimiter");
    s_iter = ss_split_raw_to_iter("Ehy how are you?", "_");
    test_tokens_iter(
        "should return the entire string", s_iter, 1,
        (char *[]){"Ehy how are you?"}
    );

    test_subgroup("empty delimiter");
    s_iter = ss_split_raw_to_iter("Ehy how are you?", "");
    test_tokens_iter(
        "should return the entire string", s_iter, 1,
        (char *[]){"Ehy how are you?"}
    );

    test_subgroup("multiple consecutive delimiters");
    s_iter = ss_split_raw_to_iter("   Ehy    how   are   you?   ", " ");
    test_tokens_iter(
        "shouldn't return empty tokens", s_iter, 4,
        (char *[]){"Ehy", "how", "are", "you?"}
    );

    test_subgroup("only consecutive delimiters");
    s_iter = ss_split_raw_to_iter("       ", " ");
    test_tokens_iter(
        "should return no tokens", s_iter, 0,
        (char *[]){}
    );
}

void test_ss_collect_iter(void) {
    test_group("test ss_collect_iter");
    int n_tokens;

    test_subgroup("split in words");
    ss_iter *s_iter = ss_split_raw_to_iter("Ehy how are you?", " ");
    ss **tokens = ss_iter_collect(s_iter, &n_tokens);
    test_tokens_list(
        "should split in words", tokens, n_tokens,
        (char *[]){"Ehy", "how", "are", "you?"}, 4
    );

    test_subgroup("missing delimiter");
    s_iter = ss_split_raw_to_iter("Ehy how are you?", "_");
    tokens = ss_iter_collect(s_iter, &n_tokens);
    test_tokens_list(
        "should return the entire string", tokens, n_tokens,
        (char *[]){"Ehy how are you?"}, 1
    );

    test_subgroup("empty delimiter");
    s_iter = ss_split_raw_to_iter("Ehy how are you?", "");
    tokens = ss_iter_collect(s_iter, &n_tokens);
    test_tokens_list(
        "should return the entire string", tokens, n_tokens,
        (char *[]){"Ehy how are you?"}, 1
    );

    test_subgroup("multiple consecutive delimiters");
    s_iter = ss_split_raw_to_iter("   Ehy    how   are   you?   ", " ");
    tokens = ss_iter_collect(s_iter, &n_tokens);
    test_tokens_list(
        "shouldn't return empty tokens", tokens, n_tokens,
        (char *[]){"Ehy", "how", "are", "you?"}, 4
    );

    test_subgroup("only consecutive delimiters");
    s_iter = ss_split_raw_to_iter("      ", " ");
    tokens = ss_iter_collect(s_iter, &n_tokens);
    test_tokens_list(
        "shouldn't return empty tokens", tokens, n_tokens,
        (char *[]){}, 0
    );
}

void test_ss_collect_from_row(void) {
    test_group("test ss_collect_from_row");
    int n_tokens;

    test_subgroup("split in words");
    ss **tokens = ss_split_row("Ehy how are you?", " ", &n_tokens);
    test_tokens_list(
        "should split in words", tokens, n_tokens,
        (char *[]){"Ehy", "how", "are", "you?"}, 4
    );

    test_subgroup("missing delimiter");
    tokens = ss_split_row("Ehy how are you?", "_", &n_tokens);
    test_tokens_list(
        "should return the entire string", tokens, n_tokens,
        (char *[]){"Ehy how are you?"}, 1
    );

    test_subgroup("empty delimiter");
    tokens = ss_split_row("Ehy how are you?", "", &n_tokens);
    test_tokens_list(
        "should return the entire string", tokens, n_tokens,
        (char *[]){"Ehy how are you?"}, 1
    );

    test_subgroup("multiple consecutive delimiters");
    tokens = ss_split_row("   Ehy    how   are   you?   ", " ", &n_tokens);
    test_tokens_list(
        "shouldn't return empty tokens", tokens, n_tokens,
        (char *[]){"Ehy", "how", "are", "you?"}, 4
    );

    test_subgroup("only consecutive delimiters");
    tokens = ss_split_row("      ", " ", &n_tokens);
    test_tokens_list(
        "shouldn't return empty tokens", tokens, n_tokens,
        (char *[]){}, 0
    );
}

void test_ss_collect_from_str(void) {
    test_group("test ss_collect_from_str");
    int n_tokens;

    test_subgroup("split in words");
    ss *s = ss_new_from_raw("Ehy how are you?");
    ss **tokens = ss_split_str(s, " ", &n_tokens);
    test_tokens_list(
        "should split in words", tokens, n_tokens,
        (char *[]){"Ehy", "how", "are", "you?"}, 4
    );
    free(tokens);
    free(s);

    test_subgroup("missing delimiter");
    s = ss_new_from_raw("Ehy how are you?");
    tokens = ss_split_str(s, "_", &n_tokens);
    test_tokens_list(
        "should return the entire string", tokens, n_tokens,
        (char *[]){"Ehy how are you?"}, 1
    );
    free(tokens);
    free(s);

    test_subgroup("empty delimiter");
    s = ss_new_from_raw("Ehy how are you?");
    tokens = ss_split_str(s, "", &n_tokens);
    test_tokens_list(
        "should return the entire string", tokens, n_tokens,
        (char *[]){"Ehy how are you?"}, 1
    );
    free(tokens);
    free(s);

    test_subgroup("multiple consecutive delimiters");
    s = ss_new_from_raw("   Ehy    how   are   you?   ");
    tokens = ss_split_str(s, " ", &n_tokens);
    test_tokens_list(
        "shouldn't return empty tokens", tokens, n_tokens,
        (char *[]){"Ehy", "how", "are", "you?"}, 4
    );
    free(tokens);
    free(s);

    test_subgroup("only consecutive delimiters");
    s = ss_new_from_raw("      ");
    tokens = ss_split_str(s, " ", &n_tokens);
    test_tokens_list(
        "shouldn't return empty tokens", tokens, n_tokens,
        (char *[]){}, 0
    );
    free(tokens);
    free(s);
}

int main(void) {
    test_ss_new_raw_len_cap();
    test_ss_new_raw_len();
    test_ss_new_raw();
    test_ss_new_empty();
    test_ss_clone();

    test_ss_concat_raw_len();
    test_ss_concat_raw();
    test_ss_concat_str();

    test_trim();
    test_trim_right();
    test_trim_left();

    test_cut();
    test_clear();
    test_set_free_space();
    test_grow();
    test_shrink();

    test_ss_sprintf();
    test_ss_sprintf_concat();

    test_ss_split_raw();
    test_ss_split();
    test_ss_iter_next();
    test_ss_collect_iter();
    test_ss_collect_from_row();
    test_ss_collect_from_str();

    return test_report();
}
