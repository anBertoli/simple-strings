cmake_minimum_required(VERSION 3.10)
project(simple_strings VERSION 0.1)

################################################
# Simple Strings Library
################################################

# Add C source files to target.
add_library(ss STATIC
    "src/alloc.c"
    "src/string.c"
    "src/string_fmt.c"
    "src/string_iter.c"
)

# Merge headers in one file. In this way the callers can refer to
# a unique file header file, which is more ergonomic.
file(WRITE tmp/ss.h "")

file(READ src/string.h CONTENTS)
file(APPEND tmp/ss.h "${CONTENTS}")
file(READ src/string_fmt.h CONTENTS)
file(APPEND tmp/ss.h "${CONTENTS}")
file(READ src/string_iter.h CONTENTS)
file(APPEND tmp/ss.h "${CONTENTS}")

# Install the library (usually /usr/local/lib/libss.a)
# and the header file (usually /usr/local/include/ss.h).
install(TARGETS ss DESTINATION lib)
install(FILES tmp/ss.h DESTINATION include)

################################################
# Simple Strings Tests
################################################

add_executable(ss_tests
    "src/alloc.c"
    "src/string.c"
    "src/string_fmt.c"
    "src/string_iter.c"
    "src/private/debug.c"

    "src/tests/main.c"
    "src/tests/framework/framework.c"
    "src/tests/string_test.c"
    "src/tests/string_fmt_test.c"
    "src/tests/string_iter_test.c"
)
