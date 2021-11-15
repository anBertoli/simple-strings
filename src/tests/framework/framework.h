#include <stdbool.h>

#ifndef SS_TEST_FRAMEWORK_H
#define SS_TEST_FRAMEWORK_H

#define test_cond(desc, cond) t_test_cond(desc, cond, __FILE__, __LINE__)
#define test_equal(desc, want, got) t_test_equal(desc, want, got, __FILE__, __LINE__)
#define test_strings(desc, want, got) t_test_strings(desc, want, got, __FILE__, __LINE__)
#define test_success(desc) t_test_success(desc, __FILE__, __LINE__)
#define test_failure(desc) t_test_failure(desc, __FILE__, __LINE__)

#define test_report() t_test_report()
#define test_group(desc) t_test_group(desc)
#define test_subgroup(desc) t_test_subgroup(desc)

void t_test_cond(const char *desc, bool cond, const char *filename, int line);
void t_test_equal(const char *desc, int want, int got, const char *filename, int line);
void t_test_strings(const char *desc, const char *want, const char *got, const char *filename, int line);
void t_test_success(const char *desc, const char *filename, int line);
void t_test_failure(const char *desc, const char *filename, int line);

void t_test_group(const char *desc);
void t_test_subgroup(const char *desc);
int t_test_report(void);

#endif