#include <stdbool.h>

#define test_cond(desc, cond) _test_cond(desc, cond, __FILE__, __LINE__)
#define test_equal(desc, want, got) _test_equal(desc, want, got, __FILE__, __LINE__)
#define test_strings(desc, want, got) _test_strings(desc, want, got, __FILE__, __LINE__)
#define test_success(desc) _test_success(desc, __FILE__, __LINE__)
#define test_failure(desc) _test_failure(desc, __FILE__, __LINE__)


#define test_report() _test_report()
#define test_group(desc) _test_group(desc)
#define test_subgroup(desc) _test_subgroup(desc)

void _test_cond(const char *desc, const bool cond, const char *filename, const int line);
void _test_equal(const char *desc, const int want, const int got, const char *filename, const int line);
void _test_strings(const char *desc, const char *want, const char *got, const char *filename, const int line);
void _test_success(const char *desc, const char *filename, const int line);
void _test_failure(const char *desc, const char *filename, const int line);


void _test_group(const char *desc);
void _test_subgroup(const char *desc);
int _test_report(void);
