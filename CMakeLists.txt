cmake_minimum_required(VERSION 3.10)
project(simple_strings VERSION 0.1)

# Create options to be used in the compilation process. Who compiles
# the project can choose which option to apply. The choices are reflected
# into the configuration file (src/options/options.h).
option(SS_ALLOC_EXIT "Exit on allocation failures" OFF)
configure_file("src/options/options.h.in" "src/options/options.h")

################################################
# Simple Strings Library
################################################

# Add C source files to target.
add_library(ss STATIC
    "src/alloc.c"
    "src/string.c"
    "src/string_fmt.c"
    "src/string_split.c"
)

# Include the /src/options in the searched include directories
# in order to include also the generated options.h file.
target_include_directories(ss PUBLIC
    "${PROJECT_BINARY_DIR}/src/options"
)

# Merge headers in one file. In this way the callers can refer to
# a unique file header file, which is more ergonomic.
file(WRITE tmp/ss.h "")

file(READ src/string.h CONTENTS)
file(APPEND tmp/ss.h "${CONTENTS}")
file(READ src/string_fmt.h CONTENTS)
file(APPEND tmp/ss.h "${CONTENTS}")
file(READ src/string_split.h CONTENTS)
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
    "src/string_split.c"
    "src/private/debug.c"

    "src/tests/main.c"
    "src/tests/framework/framework.c"
    "src/tests/string_test.c"
    "src/tests/string_fmt_test.c"
    "src/tests/string_split_test.c"
)

# Include the /src/options in the searched include directories
# in order to include also the generated options.h file.
target_include_directories(ss_tests PUBLIC
    "${PROJECT_BINARY_DIR}/src/options"
)



################################################
# Simple Strings Docs Generation
################################################
add_executable(docs
    "src/docs/main.c"
    "src/docs/utils.c"
    "src/docs/parser.c"
    "src/docs/docs.c"

    "src/alloc.c"
    "src/string.c"
    "src/string_fmt.c"
    "src/string_split.c"
    "src/private/debug.c"
)

target_include_directories(docs PUBLIC
    "${PROJECT_BINARY_DIR}/src/options"
)