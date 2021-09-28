#include "../src/string.h"
#include "utils.h"

#undef END_STRING
#define END_STRING '@'


int main(void) {

    test_group("Should retain the correct length");
    test_cond("should have length 4", 4 == 4);
    test_cond("should have length 4", 4 == 3);
    test_equal("should have equal length", 4, 5);
    test_equal("should have equal length", 5, 5);

    return test_report();
}