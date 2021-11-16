# Simple Strings

Simple Strings (SS) is a string library for C designed to augment the libc string functionalities by adding dynamic 
and heap allocated strings. Those strings are easier to use and resemble the dynamic string types of other programming 
languages.

At the core of the ss strings library there is a string type represented as the struct `string`
showed below. This struct represents a heap allocated string handled by the SS library. Functions
that instantiate strings returns pointers to those structs (the `ss` typedef).

```c
struct string {
    size_t len;
    size_t cap;
    char *buf;
};

typedef struct string *ss;
```

The string itself to is pointed to by the `buf` pointer, has length `len` and there is a null 
terminator after the string itself (at the position `len`). The `cap` field indicates how much
space has been allocated for the string buffer, this space is always >= of the string length. Usually 
the space allocated from the library functions is higher than the string length. In this way when the 
string grows there's no need of further reallocations.

The `buf` field is safer to be read and write until the position `len-1`, i.e. one can write to the
string buffer starting at the position `ss->buf[0]` until `ss->buf[ss->len-1]`. The functions of the
library automatically handles the allocated space and the fields mentioned above to both perform the 
needed operations and maintain the string state consistent. If you need to write into the string make
sure the buffer it has enough space (or grow it with the dedicated `ss_grow` function).

Both the string struct pointed to by an `ss` pointer and the string buffer are heap allocated. After
use the strings must be freed with the `ss_free` function.
