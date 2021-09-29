#include "../src/string.h"
#include "../src/utils.h"
#include "utils.h"


int main(void) {

    test_group("test str_new_raw_len");
    str *s = str_new_raw_len("testing", 7);
    test_equal("should have correct len", 7, s->len);
    test_equal("should have correct cap", 14, s->cap);
    test_strings("should have the correct string", s->buf, "testing");
    str_free(s);

    test_group("test str_new_raw");
    s = str_new_raw(" \\!ehy you__\n");
    test_equal("should have correct len", 13, s->len);
    test_equal("should have correct cap", 26, s->cap);
    test_strings("should have the correct string", s->buf, " \\!ehy you__\n");
    str_free(s);

    test_group("test str_new_raw with NULL");
    s = str_new_raw(NULL);
    test_equal("should be len 0", 0, s->len);
    test_equal("should have cap 0", 0, s->cap);
    test_strings("should have the correct string", s->buf, "");
    str_free(s);

    test_group("test str_new_empty");
    s = str_new_empty();
    test_equal("should be len 0", 0, s->len);
    test_equal("should have cap 0", 0, s->cap);
    test_strings("should have the correct string", s->buf, "");
    str_free(s);

    test_group("test str_clone");
    str *s1 = str_new_raw("ehy");
    str *s2 = str_clone(s1);
    test_equal("should have same len", s1->len, s2->len);
    test_equal("should have same cap", s1->cap, s2->cap);
    test_strings("should have the same string", s1->buf, s2->buf);
    test_cond("should have different buffer pointer", s1->buf != s2->buf);

    str_concat_raw(s1, " how are you?");
    test_cond("should be independent (len)", s1->len != s2->len);
    test_cond("should be independent (cap)", s1->cap != s2->cap);
    test_cond("should be independent (buf)", strcmp(s1->buf, s2->buf) != 0);
    str_free(s1);
    str_free(s2);

    test_group("test str_concat_raw_len");
    s1 = str_new_raw("Ehy");
    str_concat_raw_len(s1, ", how are you?", 14);
    test_equal("should have correct len", 17, s1->len);
    test_equal("should have correct cap", 34, s1->cap);
    test_strings("should have concat the strings", "Ehy, how are you?", s1->buf);
    str_free(s1);

    test_group("test str_concat_raw");
    s1 = str_new_raw("Ehy");
    str_concat_raw(s1, ", I'm fine.");
    test_equal("should have correct len", 14, s1->len);
    test_equal("should have correct cap", 28, s1->cap);
    test_strings("should have concat the strings", "Ehy, I'm fine.", s1->buf);

    test_group("test str_concat");
    s1 = str_new_raw("See you");
    s2 = str_new_raw(" later.");
    str_concat_str(s1, s2);
    test_equal("should have correct len", 14, s1->len);
    test_equal("should have correct cap", 14, s1->cap);
    test_strings("should have concat the strings", "See you later.", s1->buf);
    str_free(s1);
    str_free(s2);




    return test_report();
}