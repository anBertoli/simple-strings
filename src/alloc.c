#include <stdlib.h>
#include <stdio.h>
#include "options.h"

/*
 * If SS_ALLOC_EXIT is defined (through the CMake options at compile time), failures
 * in memory allocation will abort the program. In this case the user doesn't need
 * to check returned string pointers or string errors. Otherwise, the default behaviour
 * is the same as the standard library malloc and realloc functions.
 */

#ifdef SS_ALLOC_EXIT

void *ss_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("ss_lib: cannot allocate memory\n");
        exit(1);
    }
    return ptr;
}

void *ss_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        printf("ss_lib: cannot allocate memory\n");
        exit(1);
    }
    return new_ptr;
}

#else

void *ss_malloc(size_t size) {
    return malloc(size);
}

void *ss_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

#endif
