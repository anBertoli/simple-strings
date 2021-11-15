#include <strings.h>
#include "../string.h"
#include "framework/framework.h"
#include "../private/debug.h"

void test_ss_new_from_raw_len_cap(void) {
    test_group("ss_new_from_raw_len_cap");

    test_subgroup("base case");
    ss s = ss_new_from_raw_len_cap("testing", 7, 20);
    test_equal("should have correct len", 7, s->len);
    test_equal("should have correct cap", 20, s->cap);
    test_strings("should have the correct string", s->buf, "testing");
    ss_free(s);

    test_subgroup("empty string");
    s = ss_new_from_raw_len_cap("", 0, 0);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct cap", 0, s->cap);
    test_strings("should have the correct string", s->buf, "");
    ss_free(s);

    test_subgroup("len < string length");
    s = ss_new_from_raw_len_cap("testing", 3, 20);
    test_equal("should have correct len", 3, s->len);
    test_equal("should have correct cap", 20, s->cap);
    test_strings("should have the correct string", s->buf, "tes");
    ss_free(s);

    test_subgroup("len > cap");
    s = ss_new_from_raw_len_cap("testing", 3, 1);
    test_equal("should have correct len", 3, s->len);
    test_equal("should have correct cap", 3, s->cap);
    test_strings("should have the correct string", s->buf, "tes");
    ss_free(s);
}

void test_ss_new_from_raw_len(void) {
    test_group("ss_new_from_raw_len");

    test_subgroup("base case");
    ss s = ss_new_from_raw_len("testing", 7);
    test_equal("should have correct len", 7, s->len);
    test_equal("should have correct cap", 14, s->cap);
    test_strings("should have the correct string", s->buf, "testing");
    ss_free(s);

    test_subgroup("empty string");
    s = ss_new_from_raw_len("", 0);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct cap", 0, s->cap);
    test_strings("should have the correct string", s->buf, "");
    ss_free(s);

    test_subgroup("len < string length");
    s = ss_new_from_raw_len("testing", 3);
    test_equal("should cut the string", 3, s->len);
    test_equal("should have correct cap", 6, s->cap);
    test_strings("should have the correct string", s->buf, "tes");
    ss_free(s);
}

void test_ss_new_from_raw(void) {
    test_group("ss_new_from_raw");

    test_subgroup("non empty string");
    ss s = ss_new_from_raw(" \\!ehy you__\n");
    test_equal("should have correct len", 13, s->len);
    test_equal("should have correct cap", 26, s->cap);
    test_strings("should have the correct string", s->buf, " \\!ehy you__\n");
    ss_free(s);

    test_subgroup("empty string");
    s = ss_new_from_raw("");
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct cap", 0, s->cap);
    test_strings("should have the correct string", s->buf, "");
    ss_free(s);

    test_subgroup("null string");
    s = ss_new_from_raw(NULL);
    test_equal("should have len 0 with NULL", 0, s->len);
    test_equal("should have cap 0 with NULL", 0, s->cap);
    test_strings("should have the correct string with NULL", s->buf, "");
    ss_free(s);
}

void test_ss_new_empty_with_cap(void) {
    test_group("ss_new_empty_with_cap");

    test_subgroup("base case");
    ss s = ss_new_empty_with_cap(10);
    test_equal("should have len 0", 0, s->len);
    test_equal("should have cap 10", 10, s->cap);
    test_strings("should have the correct string", s->buf, "");
    ss_free(s);

    test_subgroup("zero cap");
    s = ss_new_empty_with_cap(0);
    test_equal("should have len 0", 0, s->len);
    test_equal("should have cap 0", 0, s->cap);
    test_strings("should have the correct string", s->buf, "");
    ss_free(s);
}

void test_ss_new_empty(void) {
    test_group("ss_new_empty");

    test_subgroup("base case");
    ss s = ss_new_empty();
    test_equal("should have len 0", 0, s->len);
    test_equal("should have cap 0", 0, s->cap);
    test_strings("should have the correct string", s->buf, "");
    ss_free(s);
}

void test_ss_clone(void) {
    test_group("ss_clone");

    test_subgroup("base case");
    ss s1 = ss_new_from_raw("ehy");
    ss s2 = ss_clone(s1);
    test_equal("should have same len", s1->len, s2->len);
    test_equal("should have same cap", s1->cap, s2->cap);
    test_strings("should have the same string", s1->buf, s2->buf);
    test_cond("should have different buffer pointers", s1->buf != s2->buf);

    test_subgroup("must be independent");
    ss_concat_raw(s1, " how are you?");
    test_cond("should have different len", s1->len != s2->len);
    test_cond("should have different cap", s1->cap != s2->cap);
    test_cond("should have different strings", strcmp(s1->buf, s2->buf) != 0);
    ss_free(s1);
    ss_free(s2);
}

void test_ss_set_free_space(void) {
    test_group("ss_set_free_space");

    test_subgroup("new free space > 0");
    ss s1 = ss_new_from_raw("ehy you");
    ss_set_free_space(s1, 10);
    test_equal("shouldn't change the len", 7, s1->len);
    test_equal("should augment the cap", 17, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);

    test_subgroup("new free space = 0");
    s1 = ss_new_from_raw("ehy you");
    ss_set_free_space(s1, 0);
    test_equal("shouldn't change the len", 7, s1->len);
    test_equal("should set cap = len", 7, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);
}

void test_ss_reserve_free_space(void) {
    test_group("ss_set_free_space");

    test_subgroup("enlarge free space");
    ss s1 = ss_new_from_raw_len_cap("ehy you", 7, 7);
    ss_reserve_free_space(s1, 10);
    test_equal("shouldn't change the len", 7, s1->len);
    test_equal("should augment the cap", 17, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);

    test_subgroup("new free space = 0");
    s1 = ss_new_from_raw_len_cap("ehy you", 7, 7);
    ss_reserve_free_space(s1, 0);
    test_equal("shouldn't change the len", 7, s1->len);
    test_equal("shouldn't change the cap", 7, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);

    test_subgroup("already free space");
    s1 = ss_new_from_raw_len_cap("ehy you",7, 20);
    ss_reserve_free_space(s1, 10);
    test_equal("shouldn't change the len", 7, s1->len);
    test_equal("should set cap = len", 20, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);
}


void test_ss_grow(void) {
    test_group("ss_grow");

    test_subgroup("grow > 0");
    ss s1 = ss_new_from_raw_len_cap("ehy you", 7, 7);
    ss_grow(s1, 10);
    test_equal("shouldn't change the len", 10, s1->len);
    test_equal("should change the cap", 10, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    test_cond("should have inserted the 0s",s1->buf[7] == 0 && s1->buf[8] == 0 && s1->buf[9] == 0);
    ss_free(s1);

    test_subgroup("grow = 0");
    s1 = ss_new_from_raw_len_cap("ehy you", 7, 7);
    ss_grow(s1, 5);
    test_equal("shouldn't change the len", 7, s1->len);
    test_equal("shouldn't change the cap", 7, s1->cap);
    test_strings("shouldn't change the string", "ehy you", s1->buf);
    ss_free(s1);
}

void test_ss_cut(void) {
    test_group("ss_cut");

    test_subgroup("non empty string");
    ss s1 = ss_new_from_raw("ehy you");
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

void test_ss_clear(void) {
    test_group("ss_clear");

    test_subgroup("non empty string");
    ss s1 = ss_new_from_raw("ehy you");
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


void test_ss_index(void) {
    test_group("ss_index");

    test_subgroup("one char, match");
    ss s1 = ss_new_from_raw("Ehy ehy ehY");
    int i = ss_index(s1, "e");
    test_equal("should have found first occurrence", 4, i);

    test_subgroup("one char, no match");
    i = ss_index(s1, "z");
    test_equal("should have found first occurrence", -1, i);

    test_subgroup("multiple char, match");
    i = ss_index(s1, "ehy");
    test_equal("should have found first occurrence", 4, i);

    test_subgroup("multiple char, no match");
    i = ss_index(s1, "ehz");
    test_equal("should have found first occurrence", -1, i);

    test_subgroup("multiple char, match at start");
    i = ss_index(s1, "Ehy");
    test_equal("should have found first occurrence", 0, i);

    test_subgroup("multiple char, match at end");
    i = ss_index(s1, "ehY");
    test_equal("should have found first occurrence", 8, i);

    ss_free(s1);
}

void test_ss_concat_raw_len(void) {
    test_group("ss_concat_raw_len");

    test_subgroup("non empty strings");
    ss s1 = ss_new_from_raw("Ehy ");
    ss_concat_raw_len(s1, "you!", 4);
    test_equal("should have correct len", 8, s1->len);
    test_equal("should have correct cap", 8, s1->cap);
    test_strings("should have concat the strings", "Ehy you!", s1->buf);
    ss_free(s1);

    test_subgroup("non empty strings with len < raw string length");
    s1 = ss_new_from_raw("See you");
    ss_concat_raw_len(s1, " later!!!", 3);
    test_equal("should have extended the len", 10, s1->len);
    test_equal("should have extended the cap", 14, s1->cap);
    test_strings("should have concat the strings", "See you la", s1->buf);
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
    test_group("ss_concat_raw");

    test_subgroup("non empty strings");
    ss s1 = ss_new_from_raw("Ehy ");
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
    test_group("ss_concat_str");

    test_subgroup("non empty strings");
    ss s1 = ss_new_from_raw("See you");
    ss s2 = ss_new_from_raw(" later.");
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

void test_ss_prepend_raw_len(void) {
    test_group("ss_prepend_raw_len");

    test_subgroup("non empty strings");
    ss s1 = ss_new_from_raw("you!");
    ss_prepend_raw_len("Ehy ", s1, 4);
    test_equal("should have correct len", 8, s1->len);
    test_equal("should have correct cap", 8, s1->cap);
    test_strings("should have prepended the strings", "Ehy you!", s1->buf);
    ss_free(s1);

    test_subgroup("non empty strings with len < raw string length");
    s1 = ss_new_from_raw("later!!!");
    ss_prepend_raw_len("See you ", s1, 3);
    test_equal("should have extended the len", 11, s1->len);
    test_equal("should have extended the cap", 16, s1->cap);
    test_strings("should have prepended the strings", "Seelater!!!", s1->buf);
    ss_free(s1);

    test_subgroup("non empty strings with reallocations");
    s1 = ss_new_from_raw("later!!!");
    ss_prepend_raw_len("Hey, see you ", s1, 13);
    test_equal("should have extended the len", 21, s1->len);
    test_equal("should have extended the cap", 42, s1->cap);
    test_strings("should have prepended the strings", "Hey, see you later!!!", s1->buf);
    ss_free(s1);

    test_subgroup("one empty string");
    s1 = ss_new_from_raw("later!!!");
    ss_prepend_raw_len("", s1, 0);
    test_equal("shouldn't have extended the len", 8, s1->len);
    test_equal("shouldn't have extended the cap", 16, s1->cap);
    test_strings("should have prepended the strings", "later!!!", s1->buf);
    ss_free(s1);

    test_subgroup("both empty strings");
    s1 = ss_new_from_raw("");
    ss_prepend_raw_len("", s1, 0);
    test_equal("shouldn't have extended the len", 0, s1->len);
    test_equal("shouldn't have extended the cap", 0, s1->cap);
    test_strings("should have prepended the strings", "", s1->buf);
    ss_free(s1);
}

void test_ss_prepend_raw(void) {
    test_group("ss_prepend_raw");

    test_subgroup("non empty strings");
    ss s = ss_new_from_raw("you!");
    ss_prepend_raw("Ehy ", s);
    test_equal("should have correct len", 8, s->len);
    test_equal("should have correct cap", 8, s->cap);
    test_strings("should have prepended the strings", "Ehy you!", s->buf);
    ss_free(s);

    test_subgroup("non empty strings with reallocations");
    s = ss_new_from_raw("later!!!");
    ss_prepend_raw("Hey, see you ", s);
    test_equal("should have extended the len", 21, s->len);
    test_equal("should have extended the cap", 42, s->cap);
    test_strings("should have concat the strings", "Hey, see you later!!!", s->buf);
    ss_free(s);

    test_subgroup("one empty string");
    s = ss_new_from_raw("See you");
    ss_prepend_raw("", s);
    test_equal("shouldn't have extended the len", 7, s->len);
    test_equal("shouldn't have extended the cap", 14, s->cap);
    test_strings("should have concat the strings", "See you", s->buf);
    ss_free(s);

    test_subgroup("both empty strings");
    s = ss_new_from_raw("");
    ss_prepend_raw("", s);
    test_equal("shouldn't have extended the len", 0, s->len);
    test_equal("shouldn't have extended the cap", 0, s->cap);
    test_strings("should have concat the strings", "", s->buf);
    ss_free(s);
}

void test_ss_prepend_str(void) {
    test_group("ss_prepend_str");

    test_subgroup("non empty strings");
    ss s1 = ss_new_from_raw("See you");
    ss s2 = ss_new_from_raw(" later.");
    ss_prepend_str(s1, s2);
    test_equal("should have correct len", 14, s2->len);
    test_equal("should have correct cap", 14, s2->cap);
    test_strings("should have prepended the strings", "See you later.", s2->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("non empty strings with reallocations");
    s1 = ss_new_from_raw("Hey, see you ");
    s2 = ss_new_from_raw("later!!!");
    ss_prepend_str(s1, s2);
    test_equal("should have extended the len", 21, s2->len);
    test_equal("should have extended the cap", 42, s2->cap);
    test_strings("should have prepended the strings", "Hey, see you later!!!", s2->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("one empty string");
    s1 = ss_new_from_raw("See you");
    s2 = ss_new_empty();
    ss_prepend_str(s1, s2);
    test_equal("shouldn't have extended the len", 7, s1->len);
    test_equal("shouldn't have extended the cap", 14, s1->cap);
    test_strings("should have prepended the strings", "See you", s1->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("both empty strings");
    s1 = ss_new_empty();
    s2 = ss_new_empty();
    ss_prepend_str(s1, s2);
    test_equal("shouldn't have extended the len", 0, s1->len);
    test_equal("shouldn't have extended the cap", 0, s1->cap);
    test_strings("should have prepended the strings", "", s1->buf);
    ss_free(s1);
    ss_free(s2);
}

void test_ss_slice(void) {
    test_group("ss_slice");

    test_subgroup("slice all string");
    ss s1 = ss_new_from_raw_len_cap("ehy how are you?", 16, 20);
    ss_slice(s1, 0, 16);
    test_equal("should have same len", 16, s1->len);
    test_equal("should have same cap", 20, s1->cap);
    test_strings("should have the same string", "ehy how are you?", s1->buf);
    ss_free(s1);

    test_subgroup("slice substring");
    s1 = ss_new_from_raw_len_cap("ehy how are you?", 16, 20);
    ss_slice(s1, 3, 14);
    test_equal("slice should have correct len", 11, s1->len);
    test_equal("slice should have correct cap", 20, s1->cap);
    test_strings("slice should have correct string", s1->buf, " how are yo");
    ss_free(s1);

    test_subgroup("end index > len");
    s1 = ss_new_from_raw_len_cap("ehy how are you?", 16, 20);
    ss_slice(s1, 3, 100);
    test_equal("slice should have correct len", s1->len, 13);
    test_equal("slice should have correct cap", s1->cap, 20);
    test_strings("slice should have correct string", s1->buf, " how are you?");
    ss_free(s1);

    test_subgroup("start index > len = no change");
    s1 = ss_new_from_raw_len_cap("ehy how are you?", 16, 20);
    ss_slice(s1, 100, 102);
    test_equal("slice should have correct len", s1->len, 16);
    test_equal("slice should have correct cap (cap = len)", s1->cap, 20);
    test_strings("slice should have correct string", s1->buf, "ehy how are you?");
    ss_free(s1);

    test_subgroup("start index > end index = no change");
    s1 = ss_new_from_raw_len_cap("ehy how are you?", 16, 20);
    ss_slice(s1, 5, 2);
    test_equal("slice should have correct len", s1->len, 16);
    test_equal("slice should have correct cap (cap = len)", s1->cap, 20);
    test_strings("slice should have correct string", s1->buf, "ehy how are you?");
    ss_free(s1);

    test_subgroup("start index > end index and start index > len = no change");
    s1 = ss_new_from_raw_len_cap("ehy how are you?", 16, 20);
    ss_slice(s1, 50, 2);
    test_equal("slice should have correct len", s1->len, 16);
    test_equal("slice should have correct cap (cap = len)", s1->cap, 20);
    test_strings("slice should have correct string", s1->buf, "ehy how are you?");
    ss_free(s1);
}

void test_ss_trim(void) {
    test_group("ss_trim");

    test_subgroup("one char in cutset");
    ss s1 = ss_new_from_raw("   ehy   ");
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

void test_ss_trim_right(void) {
    test_group("ss_trim_right");

    test_subgroup("one char in cutset");
    ss s1 = ss_new_from_raw("   ehy   ");
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
    s1 = ss_new_from_raw("_!?ehy!?_");
    ss_trim_right(s1, "_!?");
    test_equal("shouldn't change the len", 6, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("shouldn' have trimmed the string", "_!?ehy", s1->buf);
    ss_free(s1);

    test_subgroup("all chars removed");
    s1 = ss_new_from_raw("-ey!eyeh?ehy!_hhhh?");
    ss_trim_right(s1, "_h!y?e-");
    test_equal("should reduce the len", 0, s1->len);
    test_equal("shouldn't change the cap", 38, s1->cap);
    test_strings("should have trimmed all the string", "", s1->buf);
    ss_free(s1);
}

void test_ss_trim_left(void) {
    test_group("ss_trim_left");

    test_subgroup("one char in cutset");
    ss s1 = ss_new_from_raw("   ehy   ");
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
    s1 = ss_new_from_raw("_!?ehy!?_");
    ss_trim_left(s1, "_!?");
    test_equal("shouldn't change the len", 6, s1->len);
    test_equal("shouldn't change the cap", 18, s1->cap);
    test_strings("shouldn' have trimmed the string", "ehy!?_", s1->buf);
    ss_free(s1);

    test_subgroup("all chars removed");
    s1 = ss_new_from_raw("-ey!eyeh?ehy!_hhhh?");
    ss_trim_left(s1, "_h!y?e-");
    test_equal("should reduce the len", 0, s1->len);
    test_equal("shouldn't change the cap", 38, s1->cap);
    test_strings("should have trimmed all the string", "", s1->buf);
    ss_free(s1);
}

void test_ss_to_lower(void) {
    test_group("ss_to_lower");

    test_subgroup("base case");
    ss s1 = ss_new_from_raw("1234567890EHY    eHy  EhY!?_");
    ss_to_lower(s1);
    test_equal("shouldn't reduce the len", 28, s1->len);
    test_equal("shouldn't change the cap", 56, s1->cap);
    test_strings("should have turned the string to lowercase", "1234567890ehy    ehy  ehy!?_", s1->buf);
}

void test_ss_to_upper(void) {
    test_group("ss_to_upper");

    test_subgroup("base case");
    ss s1 = ss_new_from_raw("1234567890EHY    eHy  EhY!?_");
    ss_to_upper(s1);
    test_equal("shouldn't reduce the len", 28, s1->len);
    test_equal("shouldn't change the cap", 56, s1->cap);
    test_strings("should have turned the string to uppercase", "1234567890EHY    EHY  EHY!?_", s1->buf);
}

