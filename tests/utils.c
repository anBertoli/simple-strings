#include <stdbool.h>
#include <stdio.h>

static int num_tests = 0;
static int num_failed = 0;

void _test_cond(const char *desc, const bool cond, const char *filename, const int line) {
    num_tests++;
    printf("  %s\t%s\n", cond ? "✅" : "❌", desc);
    if (!cond) {
        num_failed++;
        printf("\t\t%s, line %d\n", filename, line);
    }
}

void _test_equal(const char *desc, const int want, const int got, const char *filename, const int line) {
    num_tests++;

    bool equal = want == got;
    printf("  %s\t%s\n", equal ? "✅" : "❌", desc);
    if (!equal) {
        num_failed++;
        printf("\t\twant: %d, got: %d,", want, got);
        printf("%s, line %d\n", filename, line);
    }
}

void _test_group(const char *desc) {
    printf("\n  ======= %s =======\n", desc);
}

int _test_report(void) {
    printf("\n  ======= Tests Report =======\n");
    printf("  Tests: %d, passed: %d, failed: %d\n", num_tests, num_tests - num_failed, num_failed);
    printf("  There are some failed tests.\n\n");
    return num_failed ? 1 : 0;
}