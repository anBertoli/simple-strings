#include <stdbool.h>

#define test_cond(desc, cond) _test_cond(desc, cond, __FILE__, __LINE__)
#define test_equal(desc, want, got) _test_equal(desc, want, got, __FILE__, __LINE__)
#define test_strings(desc, want, got) _test_strings(desc, want, got, __FILE__, __LINE__)
#define test_report() _test_report()
#define test_group(desc) _test_group(desc)

void _test_cond(const char *desc, const bool cond, const char *filename, const int line);
void _test_equal(const char *desc, const int want, const int got, const char *filename, const int line);
void _test_strings(const char *desc, const char *want, const char *got, const char *filename, const int line);

void _test_group(const char *desc);
int _test_report(void);
