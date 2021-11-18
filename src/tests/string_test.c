#include <strings.h>
#include "../string.h"
#include "framework/framework.h"
#include "../private/debug.h"

void test_ss_new_from_raw_len_free(void) {
    test_group("ss_new_from_raw_len_free");

    test_subgroup("empty string");
    ss s = ss_new_from_raw_len_free("", 0, 0);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have correct string", s->buf, "");
    ss_free(s);

    test_subgroup("NULL string");
    s = ss_new_from_raw_len_free(NULL, 6, 6);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have correct string", s->buf, "");
    ss_free(s);

    test_subgroup("base case");
    s = ss_new_from_raw_len_free("testing", 7, 20);
    test_equal("should have correct len", 7, s->len);
    test_equal("should have correct free", 20, s->free);
    test_strings("should have correct string", s->buf, "testing");
    ss_free(s);

    test_subgroup("len < string length");
    s = ss_new_from_raw_len_free("testing", 3, 20);
    test_equal("should have correct len", 3, s->len);
    test_equal("should have correct free", 20, s->free);
    test_strings("should have correct string", s->buf, "tes");
    ss_free(s);

    test_subgroup("len > free");
    s = ss_new_from_raw_len_free("testing", 3, 0);
    test_equal("should have correct len", 3, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have correct string", s->buf, "tes");
    ss_free(s);
}

void test_ss_new_from_raw_len(void) {
    test_group("ss_new_from_raw_len");

    test_subgroup("empty string");
    ss s = ss_new_from_raw_len("", 0);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have correct string", s->buf, "");
    ss_free(s);

    test_subgroup("NULL string");
    s = ss_new_from_raw_len(NULL, 10);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have correct string", s->buf, "");
    ss_free(s);

    test_subgroup("base case");
    s = ss_new_from_raw_len("testing", 7);
    test_equal("should have correct len", 7, s->len);
    test_equal("should have correct free", 7, s->free);
    test_strings("should have correct string", s->buf, "testing");
    ss_free(s);

    test_subgroup("len < string length");
    s = ss_new_from_raw_len("testing", 3);
    test_equal("should cut string", 3, s->len);
    test_equal("should have correct free", 3, s->free);
    test_strings("should have correct string", s->buf, "tes");
    ss_free(s);
}

void test_ss_new_from_raw(void) {
    test_group("ss_new_from_raw");

    test_subgroup("empty string");
    ss s = ss_new_from_raw("");
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have correct string", s->buf, "");
    ss_free(s);

    test_subgroup("NULL string");
    s = ss_new_from_raw(NULL);
    test_equal("should have correct len", 0, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have correct string", s->buf, "");
    ss_free(s);

    test_subgroup("non empty string");
    s = ss_new_from_raw(" \\!ehy you__\n");
    test_equal("should have correct len", 13, s->len);
    test_equal("should have correct free", 13, s->free);
    test_strings("should have correct string", s->buf, " \\!ehy you__\n");
    ss_free(s);
}

void test_ss_new_empty_with_free(void) {
    test_group("ss_new_empty_with_free");

    test_subgroup("base case");
    ss s = ss_new_empty_with_free(10);
    test_equal("should have len 0", 0, s->len);
    test_equal("should have free 10", 10, s->free);
    test_strings("should have correct string", s->buf, "");
    ss_free(s);

    test_subgroup("zero free");
    s = ss_new_empty_with_free(0);
    test_equal("should have len 0", 0, s->len);
    test_equal("should have free 0", 0, s->free);
    test_strings("should have correct string", s->buf, "");
    ss_free(s);
}

void test_ss_new_empty(void) {
    test_group("ss_new_empty");

    test_subgroup("base case");
    ss s = ss_new_empty();
    test_equal("should have len 0", 0, s->len);
    test_equal("should have free 0", 0, s->free);
    test_strings("should have correct string", s->buf, "");
    ss_free(s);
}

void test_ss_clone(void) {
    test_group("ss_clone");

    test_subgroup("base case");
    ss s1 = ss_new_from_raw("ehy");
    ss s2 = ss_clone(s1);
    test_equal("should have same len", s1->len, s2->len);
    test_equal("should have same free", s1->free, s2->free);
    test_strings("should have same string", s1->buf, s2->buf);
    test_cond("should have different buffer pointers", s1->buf != s2->buf);

    test_subgroup("must be independent");
    ss_concat_raw(s1, " how are you?");
    test_cond("should have different len", s1->len != s2->len);
    test_cond("should have different free", s1->free != s2->free);
    test_cond("should have different strings", strcmp(s1->buf, s2->buf) != 0);
    ss_free(s1);
    ss_free(s2);
}

void test_ss_set_free_space(void) {
    test_group("ss_set_free_space");

    test_subgroup("new free space > 0");
    ss s = ss_new_from_raw("ehy you");
    ss_set_free_space(s, 10);
    test_equal("shouldn't change len", 7, s->len);
    test_equal("should augment free", 10, s->free);
    test_strings("shouldn't change string", "ehy you", s->buf);
    ss_free(s);

    test_subgroup("new free space = 0");
    s = ss_new_from_raw("ehy you");
    ss_set_free_space(s, 0);
    test_equal("shouldn't change len", 7, s->len);
    test_equal("should reduce free", 0, s->free);
    test_strings("shouldn't change string", "ehy you", s->buf);
    ss_free(s);
}

void test_ss_reserve_free_space(void) {
    test_group("ss_set_free_space");

    test_subgroup("enlarge free space");
    ss s = ss_new_from_raw_len_free("ehy you", 7, 7);
    ss_reserve_free_space(s, 10);
    test_equal("shouldn't change len", 7, s->len);
    test_equal("should augment free", 10, s->free);
    test_strings("shouldn't change string", "ehy you", s->buf);
    ss_free(s);

    test_subgroup("new free space = 0");
    s = ss_new_from_raw_len_free("ehy you", 7, 7);
    ss_reserve_free_space(s, 0);
    test_equal("shouldn't change len", 7, s->len);
    test_equal("shouldn't change free", 7, s->free);
    test_strings("shouldn't change string", "ehy you", s->buf);
    ss_free(s);

    test_subgroup("already free space");
    s = ss_new_from_raw_len_free("ehy you", 7, 20);
    ss_reserve_free_space(s, 10);
    test_equal("shouldn't change len", 7, s->len);
    test_equal("shouldn't change free", 20, s->free);
    test_strings("shouldn't change string", "ehy you", s->buf);
    ss_free(s);
}

void test_ss_grow(void) {
    test_group("ss_grow");

    test_subgroup("grow > 0");
    ss s = ss_new_from_raw_len_free("ehy you", 7, 7);
    ss_grow(s, 10);
    test_equal("shouldn't change len", 10, s->len);
    test_equal("should change free", 4, s->free);
    test_strings("shouldn't change string", "ehy you", s->buf);
    test_cond("should have inserted 0s", s->buf[7] == 0 && s->buf[8] == 0 && s->buf[9] == 0);
    ss_free(s);

    test_subgroup("grow < len");
    s = ss_new_from_raw_len_free("ehy you", 7, 7);
    ss_grow(s, 5);
    test_equal("shouldn't change len", 7, s->len);
    test_equal("shouldn't change free", 7, s->free);
    test_strings("shouldn't change string", "ehy you", s->buf);
    ss_free(s);
}

void test_ss_shrink(void) {
    test_group("ss_shrink");

    test_subgroup("base case");
    ss s = ss_new_from_raw("ehy you");
    ss_shrink(s, 4);
    test_equal("should reduce len", 4, s->len);
    test_equal("should augment free", 10, s->free);
    test_strings("should have reduced string", "ehy ", s->buf);
    ss_free(s);

    test_subgroup("len shrink > string len");
    s = ss_new_from_raw("ehy you");
    ss_shrink(s, 10);
    test_equal("shouldn't reduce len", 7, s->len);
    test_equal("shouldn't change free", 7, s->free);
    test_strings("shouldn't have reduced string", "ehy you", s->buf);
    ss_free(s);

    test_subgroup("empty string");
    s = ss_new_from_raw("");
    ss_shrink(s, 0);
    test_equal("shouldn't change len", 0, s->len);
    test_equal("shouldn't change free", 0, s->free);
    test_strings("should maintain  empty string", "", s->buf);
    ss_free(s);
}

void test_ss_clear(void) {
    test_group("ss_clear");

    test_subgroup("base case");
    ss s = ss_new_from_raw("ehy you");
    ss_clear(s);
    test_equal("should erase len", 0, s->len);
    test_equal("should augment free", 14, s->free);
    test_strings("should have cleaned string", "", s->buf);
    ss_free(s);

    test_subgroup("empty string");
    s = ss_new_from_raw("");
    ss_clear(s);
    test_equal("shouldn't change len", 0, s->len);
    test_equal("shouldn't change free", 0, s->free);
    test_strings("should maintain empty string", "", s->buf);
    ss_free(s);
}

void test_ss_index(void) {
    test_group("ss_index");

    ss s = ss_new_from_raw("Ehy ehy Ehy eHy");

    test_subgroup("one char, no match");
    size_t i = ss_index(s, "d");
    test_equal("should have found no occurrence", -1, i);

    test_subgroup("multiple char, no match");
    i = ss_index(s, "ehz");
    test_equal("should have found no occurrence", -1, i);

    test_subgroup("one char, match");
    i = ss_index(s, "e");
    test_equal("should have found first occurrence", 4, i);

    test_subgroup("multiple char, match at start");
    i = ss_index(s, "Ehy");
    test_equal("should have found first occurrence", 0, i);

    test_subgroup("multiple char, match at middle");
    i = ss_index(s, "ehy");
    test_equal("should have found first occurrence in th middle", 4, i);

    test_subgroup("multiple char, match at end");
    i = ss_index(s, "eHy");
    test_equal("should have found first occurrence at  end", 12, i);

    ss_free(s);
}

void test_ss_index_last(void) {
    test_group("ss_index_last");

    ss s = ss_new_from_raw("Ehy ehy Ehy eHy");

    test_subgroup("one char, no match");
    size_t i = ss_index_last(s, "z");
    test_equal("should have found no match", -1, i);

    test_subgroup("multiple char, no match");
    i = ss_index_last(s, "ehz");
    test_equal("should have found no match", -1, i);

    test_subgroup("one char, match");
    i = ss_index_last(s, "e");
    test_equal("should have found first occurrence", 12, i);

    test_subgroup("multiple char, match at start");
    i = ss_index_last(s, "Ehy ehy");
    test_equal("should have found last occurrence", 0, i);

    test_subgroup("multiple char, match in middle");
    i = ss_index_last(s, "ehy");
    test_equal("should have found last occurrence in  middle", 4, i);

    test_subgroup("multiple char, match at end");
    i = ss_index_last(s, "Ehy");
    test_equal("should have found last occurrence", 8, i);

    ss_free(s);
}

void test_ss_concat_raw_len(void) {
    test_group("ss_concat_raw_len");

    test_subgroup("base case");
    ss s = ss_new_from_raw("Ehy ");
    ss_concat_raw_len(s, "you!", 4);
    test_equal("should have extended len", 8, s->len);
    test_equal("should have changed free", 0, s->free);
    test_strings("should have concat strings", "Ehy you!", s->buf);
    ss_free(s);

    test_subgroup("len < raw string length");
    s = ss_new_from_raw("See you");
    ss_concat_raw_len(s, " later!!!", 3);
    test_equal("should have extended len", 10, s->len);
    test_equal("should have changed free", 4, s->free);
    test_strings("should have concat strings", "See you la", s->buf);
    ss_free(s);

    test_subgroup("with reallocations");
    s = ss_new_from_raw("See you");
    ss_concat_raw_len(s, " later!!!", 9);
    test_equal("should have extended len", 16, s->len);
    test_equal("should have changed free", 16, s->free);
    test_strings("should have concat strings", "See you later!!!", s->buf);
    ss_free(s);

    test_subgroup("one empty string");
    s = ss_new_from_raw("See you");
    ss_concat_raw_len(s, "", 0);
    test_equal("shouldn't have changed len", 7, s->len);
    test_equal("shouldn't have changed free", 7, s->free);
    test_strings("should have concat strings", "See you", s->buf);
    ss_free(s);

    test_subgroup("both empty strings");
    s = ss_new_from_raw("");
    ss_concat_raw_len(s, "", 0);
    test_equal("shouldn't have extended len", 0, s->len);
    test_equal("shouldn't have extended free", 0, s->free);
    test_strings("should have concat strings", "", s->buf);
    ss_free(s);
}

void test_ss_concat_raw(void) {
    test_group("ss_concat_raw");

    test_subgroup("base case");
    ss s = ss_new_from_raw("Ehy ");
    ss_concat_raw(s, "you!");
    test_equal("should have correct len", 8, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have concat strings", "Ehy you!", s->buf);
    ss_free(s);

    test_subgroup("with reallocations");
    s = ss_new_from_raw("See you");
    ss_concat_raw(s, " later!!!");
    test_equal("should have extended len", 16, s->len);
    test_equal("should have extended free", 16, s->free);
    test_strings("should have concat strings", "See you later!!!", s->buf);
    ss_free(s);

    test_subgroup("with empty string");
    s = ss_new_from_raw("See you");
    ss_concat_raw(s, "");
    test_equal("shouldn't have extended len", 7, s->len);
    test_equal("shouldn't have extended free", 7, s->free);
    test_strings("should have concat strings", "See you", s->buf);
    ss_free(s);

    test_subgroup("both empty strings");
    s = ss_new_from_raw("");
    ss_concat_raw(s, "");
    test_equal("shouldn't have extended len", 0, s->len);
    test_equal("shouldn't have extended free", 0, s->free);
    test_strings("should have concat strings", "", s->buf);
    ss_free(s);
}

void test_ss_concat_str(void) {
    test_group("ss_concat_str");

    test_subgroup("non empty strings");
    ss s1 = ss_new_from_raw("See you");
    ss s2 = ss_new_from_raw(" later.");
    ss_concat_str(s1, s2);
    test_equal("should have correct len", 14, s1->len);
    test_equal("should have correct free", 0, s1->free);
    test_strings("should have concat strings", "See you later.", s1->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("non empty strings with reallocations");
    s1 = ss_new_from_raw("See you");
    s2 = ss_new_from_raw(" later!!!");
    ss_concat_str(s1, s2);
    test_equal("should have extended len", 16, s1->len);
    test_equal("should have extended free", 16, s1->free);
    test_strings("should have concat strings", "See you later!!!", s1->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("one empty string");
    s1 = ss_new_from_raw("See you");
    s2 = ss_new_from_raw("");
    ss_concat_str(s1, s2);
    test_equal("shouldn't have extended len", 7, s1->len);
    test_equal("shouldn't have extended free", 7, s1->free);
    test_strings("should have concat strings", "See you", s1->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("both empty strings");
    s1 = ss_new_from_raw("");
    s2 = ss_new_from_raw("");
    ss_concat_str(s1, s2);
    test_equal("shouldn't have extended len", 0, s1->len);
    test_equal("shouldn't have extended free", 0, s1->free);
    test_strings("should have concat strings", "", s1->buf);
    ss_free(s1);
    ss_free(s2);
}

void test_ss_prepend_raw_len(void) {
    test_group("ss_prepend_raw_len");

    test_subgroup("non empty strings");
    ss s = ss_new_from_raw("you!");
    ss_prepend_raw_len("Ehy ", s, 4);
    test_equal("should have correct len", 8, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have prepended strings", "Ehy you!", s->buf);
    ss_free(s);

    test_subgroup("non empty strings with len < raw string length");
    s = ss_new_from_raw("later!!!");
    ss_prepend_raw_len("See you ", s, 3);
    test_equal("should have extended len", 11, s->len);
    test_equal("should have extended free", 5, s->free);
    test_strings("should have prepended strings", "Seelater!!!", s->buf);
    ss_free(s);

    test_subgroup("non empty strings with reallocations");
    s = ss_new_from_raw("later!!!");
    ss_prepend_raw_len("Hey, see you ", s, 13);
    test_equal("should have extended len", 21, s->len);
    test_equal("should have extended free", 21, s->free);
    test_strings("should have prepended strings", "Hey, see you later!!!", s->buf);
    ss_free(s);

    test_subgroup("one empty string");
    s = ss_new_from_raw("later!!!");
    ss_prepend_raw_len("", s, 0);
    test_equal("shouldn't have extended len", 8, s->len);
    test_equal("shouldn't have extended free", 8, s->free);
    test_strings("should have prepended strings", "later!!!", s->buf);
    ss_free(s);

    test_subgroup("both empty strings");
    s = ss_new_from_raw("");
    ss_prepend_raw_len("", s, 0);
    test_equal("shouldn't have extended len", 0, s->len);
    test_equal("shouldn't have extended free", 0, s->free);
    test_strings("should have prepended strings", "", s->buf);
    ss_free(s);
}

void test_ss_prepend_raw(void) {
    test_group("ss_prepend_raw");

    test_subgroup("non empty strings");
    ss s = ss_new_from_raw("you!");
    ss_prepend_raw("Ehy ", s);
    test_equal("should have correct len", 8, s->len);
    test_equal("should have correct free", 0, s->free);
    test_strings("should have prepended strings", "Ehy you!", s->buf);
    ss_free(s);

    test_subgroup("non empty strings with reallocations");
    s = ss_new_from_raw("later!!!");
    ss_prepend_raw("Hey, see you ", s);
    test_equal("should have extended len", 21, s->len);
    test_equal("should have extended free", 21, s->free);
    test_strings("should have concat strings", "Hey, see you later!!!", s->buf);
    ss_free(s);

    test_subgroup("one empty string");
    s = ss_new_from_raw("See you");
    ss_prepend_raw("", s);
    test_equal("shouldn't have extended len", 7, s->len);
    test_equal("shouldn't have extended free", 7, s->free);
    test_strings("should have concat strings", "See you", s->buf);
    ss_free(s);

    test_subgroup("both empty strings");
    s = ss_new_from_raw("");
    ss_prepend_raw("", s);
    test_equal("shouldn't have extended len", 0, s->len);
    test_equal("shouldn't have extended free", 0, s->free);
    test_strings("should have concat strings", "", s->buf);
    ss_free(s);
}

void test_ss_prepend_str(void) {
    test_group("ss_prepend_str");

    test_subgroup("non empty strings");
    ss s1 = ss_new_from_raw("See you");
    ss s2 = ss_new_from_raw(" later.");
    ss_prepend_str(s1, s2);
    test_equal("should have correct len", 14, s2->len);
    test_equal("should have correct free", 0, s2->free);
    test_strings("should have prepended strings", "See you later.", s2->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("non empty strings with reallocations");
    s1 = ss_new_from_raw("Hey, see you ");
    s2 = ss_new_from_raw("later!!!");
    ss_prepend_str(s1, s2);
    test_equal("should have extended len", 21, s2->len);
    test_equal("should have extended free", 21, s2->free);
    test_strings("should have prepended strings", "Hey, see you later!!!", s2->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("one empty string");
    s1 = ss_new_from_raw("See you");
    s2 = ss_new_empty();
    ss_prepend_str(s1, s2);
    test_equal("shouldn't have extended len", 7, s1->len);
    test_equal("shouldn't have extended free", 7, s1->free);
    test_strings("should have prepended strings", "See you", s1->buf);
    ss_free(s1);
    ss_free(s2);

    test_subgroup("both empty strings");
    s1 = ss_new_empty();
    s2 = ss_new_empty();
    ss_prepend_str(s1, s2);
    test_equal("shouldn't have extended len", 0, s1->len);
    test_equal("shouldn't have extended free", 0, s1->free);
    test_strings("should have prepended strings", "", s1->buf);
    ss_free(s1);
    ss_free(s2);
}

void test_ss_slice(void) {
    test_group("ss_slice");

    test_subgroup("slice all string");
    ss s = ss_new_from_raw_len_free("ehy how are you?", 16, 16);
    ss_slice(s, 0, 16);
    test_equal("should have same len", 16, s->len);
    test_equal("should have same free", 16, s->free);
    test_strings("should have same string", "ehy how are you?", s->buf);
    ss_free(s);

    test_subgroup("slice substring");
    s = ss_new_from_raw_len_free("ehy how are you?", 16, 16);
    ss_slice(s, 3, 14);
    test_equal("slice should have correct len", 11, s->len);
    test_equal("slice should have correct free", 21, s->free);
    test_strings("slice should have correct string", s->buf, " how are yo");
    ss_free(s);

    test_subgroup("end index > len");
    s = ss_new_from_raw_len_free("ehy how are you?", 16, 16);
    ss_slice(s, 3, 100);
    test_equal("slice should have correct len", s->len, 13);
    test_equal("slice should have correct free", s->free, 19);
    test_strings("slice should have correct string", s->buf, " how are you?");
    ss_free(s);

    test_subgroup("start index > len = no change");
    s = ss_new_from_raw_len_free("ehy how are you?", 16, 16);
    ss_slice(s, 100, 102);
    test_equal("slice should have correct len", s->len, 16);
    test_equal("slice should have correct free", s->free, 16);
    test_strings("slice should have correct string", s->buf, "ehy how are you?");
    ss_free(s);

    test_subgroup("start index > end index = no change");
    s = ss_new_from_raw_len_free("ehy how are you?", 16, 16);
    ss_slice(s, 5, 2);
    test_equal("slice should have correct len", s->len, 16);
    test_equal("slice should have correct free (free = len)", s->free, 16);
    test_strings("slice should have correct string", s->buf, "ehy how are you?");
    ss_free(s);

    test_subgroup("start index > end index and start index > len = no change");
    s = ss_new_from_raw_len_free("ehy how are you?", 16, 16);
    ss_slice(s, 50, 2);
    test_equal("slice should have correct len", s->len, 16);
    test_equal("slice should have correct free (free = len)", s->free, 16);
    test_strings("slice should have correct string", s->buf, "ehy how are you?");
    ss_free(s);
}

void test_ss_trim(void) {
    test_group("ss_trim");

    test_subgroup("no cut");
    ss s = ss_new_from_raw("!?_ehy?!_");
    ss_trim(s, " ");
    test_equal("should reduce len", 9, s->len);
    test_equal("should change free", 9, s->free);
    test_strings("should have trimmed string", "!?_ehy?!_", s->buf);
    ss_free(s);

    test_subgroup("one char in cutset");
    s = ss_new_from_raw("   ehy   ");
    ss_trim(s, " ");
    test_equal("should reduce len", 3, s->len);
    test_equal("should change free", 15, s->free);
    test_strings("should have trimmed string", "ehy", s->buf);
    ss_free(s);

    test_subgroup("multiple chars in cutset");
    s = ss_new_from_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    ss_trim(s, "_!?");
    test_equal("should reduce len", 3, s->len);
    test_equal("should change free", 51, s->free);
    test_strings("should have trimmed string", "ehy", s->buf);
    ss_free(s);

    test_subgroup("cut only right");
    s = ss_new_from_raw("-!?ehy!_?");
    ss_trim(s, "_!?");
    test_equal("should reduce len", 6, s->len);
    test_equal("should change free", 12, s->free);
    test_strings("should have trimmed string", "-!?ehy", s->buf);
    ss_free(s);

    test_subgroup("cut only left");
    s = ss_new_from_raw("_!?ehy!_-");
    ss_trim(s, "_!?");
    test_equal("should reduce len", 6, s->len);
    test_equal("should change free", 12, s->free);
    test_strings("should have trimmed string", "ehy!_-", s->buf);
    ss_free(s);

    test_subgroup("all chars removed");
    s = ss_new_from_raw("-ey!eyeh?ehy!_hhhh?");
    ss_trim(s, "_h!y?e-?");
    test_equal("should reduce len", 0, s->len);
    test_equal("should change free", 38, s->free);
    test_strings("should have trimmed all string", "", s->buf);
    ss_free(s);
}

void test_ss_trim_right(void) {
    test_group("ss_trim_right");

    test_subgroup("no cut");
    ss s = ss_new_from_raw("   ehy   ");
    ss_trim_right(s, ")");
    test_equal("should reduce len", 9, s->len);
    test_equal("should change free", 9, s->free);
    test_strings("should have trimmed string", "   ehy   ", s->buf);
    ss_free(s);

    test_subgroup("one char in cutset");
    s = ss_new_from_raw("   ehy   ");
    ss_trim_right(s, " ");
    test_equal("should reduce len", 6, s->len);
    test_equal("should change free", 12, s->free);
    test_strings("should have trimmed string", "   ehy", s->buf);
    ss_free(s);

    test_subgroup("multiple chars in cutset");
    s = ss_new_from_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    ss_trim_right(s, "_!?");
    test_equal("should reduce len", 15, s->len);
    test_equal("should change free", 39, s->free);
    test_strings("should have trimmed string", "__!!?!?_!_!?ehy", s->buf);
    ss_free(s);

    test_subgroup("no trim left");
    s = ss_new_from_raw("_!?ehy!?-");
    ss_trim_right(s, "_!?");
    test_equal("shouldn't change len", 9, s->len);
    test_equal("shouldn't change free", 9, s->free);
    test_strings("shouldn' have trimmed string", "_!?ehy!?-", s->buf);
    ss_free(s);

    test_subgroup("all chars removed");
    s = ss_new_from_raw("-ey!eyeh?ehy!_hhhh?");
    ss_trim_right(s, "_h!y?e-");
    test_equal("should reduce len", 0, s->len);
    test_equal("shouldn't change free", 38, s->free);
    test_strings("should have trimmed all string", "", s->buf);
    ss_free(s);
}

void test_ss_trim_left(void) {
    test_group("ss_trim_left");

    test_subgroup("no cut");
    ss s = ss_new_from_raw("   ehy   ");
    ss_trim_left(s, ")");
    test_equal("should reduce len", 9, s->len);
    test_equal("should change free", 9, s->free);
    test_strings("should have trimmed string", "   ehy   ", s->buf);
    ss_free(s);

    test_subgroup("one char in cutset");
    s = ss_new_from_raw("   ehy   ");
    ss_trim_left(s, " ");
    test_equal("should reduce len", 6, s->len);
    test_equal("should change free", 12, s->free);
    test_strings("should have trimmed string", "ehy   ", s->buf);
    ss_free(s);

    test_subgroup("multiple chars in cutset");
    s = ss_new_from_raw("__!!?!?_!_!?ehy!_!_!_?!_!!?");
    ss_trim_left(s, "_!?");
    test_equal("should reduce len", 15, s->len);
    test_equal("should change free", 39, s->free);
    test_strings("should have trimmed string", "ehy!_!_!_?!_!!?", s->buf);
    ss_free(s);

    test_subgroup("no trim right");
    s = ss_new_from_raw("-!?ehy!?_");
    ss_trim_left(s, "_!?");
    test_equal("should change len", 9, s->len);
    test_equal("should change free", 9, s->free);
    test_strings("should have trimmed string", "-!?ehy!?_", s->buf);
    ss_free(s);

    test_subgroup("all chars removed");
    s = ss_new_from_raw("-ey!eyeh?ehy!_hhhh?");
    ss_trim_left(s, "_h!y?e-");
    test_equal("should reduce len", 0, s->len);
    test_equal("should change free", 38, s->free);
    test_strings("should have trimmed all string", "", s->buf);
    ss_free(s);
}

void test_ss_to_lower(void) {
    test_group("ss_to_lower");

    test_subgroup("base case");
    ss s = ss_new_from_raw("1234567890EHY    eHy  EhY!?_");
    ss_to_lower(s);
    test_equal("shouldn't reduce len", 28, s->len);
    test_equal("shouldn't change free", 28, s->free);
    test_strings("should have turned string to lowercase", "1234567890ehy    ehy  ehy!?_", s->buf);
}

void test_ss_to_upper(void) {
    test_group("ss_to_upper");

    test_subgroup("base case");
    ss s = ss_new_from_raw("1234567890EHY    eHy  EhY!?_");
    ss_to_upper(s);
    test_equal("shouldn't reduce len", 28, s->len);
    test_equal("shouldn't change free", 28, s->free);
    test_strings("should have turned string to uppercase", "1234567890EHY    EHY  EHY!?_", s->buf);
}

