
#ifndef SS_TEST_UTILS_H
#define SS_TEST_UTILS_H

void test_tokens_iter(const char *desc, ss_iter *s_iter, int n_tokens, char **tokens);
void test_tokens_list(const char *desc, ss_list *str_list, char **wanted_list, int n_wanted);

#endif