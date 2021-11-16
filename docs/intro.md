# Simple Strings

Simple Strings (SS) is a string library for C designed to augment the libc string functionalities by adding dynamic and heap allocated
strings. Those strings are easier to use and resemble the dynamic string types of other programming languages (like the String type in
Rust).

# How dynamic strings work

At the core of the SS strings library there is a string type represented as a struct
as follows. This struct represents a heap allocated string handled by the SS library.
Functions that instantiate strings returns pointers to those structs (the ss typedef 
below).

```c
struct string {
    size_t len;
    size_t cap;
    char *buf;
};

typedef struct string *ss;
```

The string itself is pointed by the `buf` pointer, has length `len` and there is a null 
terminator at the position `len+1`. The `cap` field indicate how mch space has been allocated
for the string buffer, this space is always >= of the string length. Usually the allocated space
from the library functions is higher than the string length. In this way when the string growns
there's no need of further reallocations.

The `buf` field is safer to be read and write until the position `len-1`, i.e. one can write to the
string buffer starting at the position `ss->buf[0]` to `ss->buf[ss->len-1]`. Library functions
automatically handles the allocated space and the cited above fields to both perform the needed
operations and maintain the string struct consistent. If you need to manually write into the string
make sure the buffer has enough space (or grow it with the dedicated `ss_grow` function).

Both the string struct pointed to by an `ss` pointer and the string buffer are heap allocated. After
use the strings must be freed with the `ss_free` function.
