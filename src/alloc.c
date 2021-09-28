#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "utils.h"

/*
 * If STR_ALLOC_EXIT is defined (usually through the compiler flags), failures in
 * memory allocation will abort the program. In this case the user doesn't need to
 * check the returned string pointer for NULL values. Otherwise, the default
 * behaviour is the same as the standard library malloc and realloc functions.
 */

#ifdef STR_ALLOC_EXIT

void *_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("str: cannot allocate memory\n");
        exit(1);
    }
    return ptr;
}

void *_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        printf("str: cannot allocate memory\n");
        exit(1);
    }
    return new_ptr;
}

#else

void *_malloc(size_t size) {
    return malloc(size);
}

void *_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

#endif
