#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

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

void _test_strings(const char *desc, const char *want, const char *got, const char *filename, const int line) {
    num_tests++;
    bool equal = strcmp(got, want) == 0;
    printf("  %s\t%s\n", equal ? "✅" : "❌", desc);
    if (!equal) {
        num_failed++;
        printf("\t\twant: '%s', got: '%s',", want, got);
        printf("%s, line %d\n", filename, line);
    }
}

void _test_success(const char *desc, const char *filename, const int line) {
    num_tests++;
    printf("  %s\t%s\n", "✅", desc);
}

void _test_failure(const char *desc, const char *filename, const int line) {
    num_tests++;
    num_failed++;
    printf("  %s\t%s\n", "❌", desc);
}

void _test_group(const char *desc) {
    printf("\n  ======= %s =======\n", desc);
}

void _test_subgroup(const char *desc) {
    printf("\n\t--- %s --- \n", desc);
}

int _test_report(void) {
    printf("\n  ======= Tests Report =======\n\n");
    printf("  Tests: %d, passed: %d, failed: %d\n", num_tests, num_tests - num_failed, num_failed);
    if (num_failed)
        printf("  ❌ There are some failed tests.\n\n");
    else
        printf("  ✅ All tests passed.\n\n");
    return num_failed ? 1 : 0;
}


/*
 *
 */
