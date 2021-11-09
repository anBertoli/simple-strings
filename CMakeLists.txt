cmake_minimum_required(VERSION 3.10)
project(simple_strings VERSION 0.1)

################################################
# Simple Strings Library

# add C files to target
add_library(ss
    "src/alloc.c"
    "src/string.c"
    "src/string_fmt.c"
    "src/string_iter.c"
)

# merge headers in one file
file(WRITE tmp/ss.h "")

file(READ src/string.h CONTENTS)
file(APPEND tmp/ss.h "${CONTENTS}")
file(READ src/string_fmt.h CONTENTS)
file(APPEND tmp/ss.h "${CONTENTS}")
file(READ src/string_iter.h CONTENTS)
file(APPEND tmp/ss.h "${CONTENTS}")

# install the library (usually /usr/local/lib/libss.a)
# and the header file (usually /usr/local/include/ss.h)
install(TARGETS ss DESTINATION lib)
install(FILES tmp/ss.h DESTINATION include)