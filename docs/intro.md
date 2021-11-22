# Simple Strings

## How the library works

Simple Strings (_ss_) is a string library for C designed to augment the libc string functionalities by 
adding dynamic and heap allocated strings. Those strings are easier to use and resemble the dynamic 
string types of other programming languages.

At the core of the _ss_ strings library there is the string type defined as the struct showed below. 
This struct represents a dynamic and heap allocated string. The string buffer itself is pointed to by 
the `buf`pointer, has length `len` (the `len`-th char is the null terminator) and has additional `free` 
bytes allocated and ready to be used in further string manipulation. Functions that instantiate the 
strings return pointers to them (the `ss` type defined below). Both the struct and the string buffer 
are heap allocated.

```c
struct string {
    size_t len;
    size_t free;
    char *buf;
};

typedef struct string *ss;
```

The _ss_ strings are generated and returned via dedicated constructor functions and they are usually 
manipulated with the library functions. As a unique and obvious exception, the `buf` field is safer 
to be read and write until the position `len-1`, i.e. one can write to the string buffer starting at 
the position `ss->buf[0]` until `ss->buf[ss->len-1]`. If you need to write directly into the string 
buffer make sure the buffer itself has enough space (otherwise grow it with the dedicated `ss_grow`
function). The functions of the library automatically handle the allocated space and the fields mentioned
above to both perform the needed operations and maintain the string state consistent. In any case, 
**the `len` and `free` fields must be considered read-only**. 

Both the string struct pointed to by an `ss` pointer and the string buffer itself are heap allocated. 
After use, they must be freed passing the `ss` string to the `ss_free` function. If a library function 
creates a new string, also that new string must be freed after use. Similarly, string lists must be 
freed after use with the dedicated `ss_list_free` function.

This is a simple example of using _ss_ strings: 
```c
ss name = ss_new_from_raw("John");
if (!name) {
    // ... handle error
}

printf("len: %d, %s\n", name->len, name->buf);
// Output: 4, John

ss_err err = ss_concat_raw(name, " Dover");
if (err) {
    // ... handle error
}

printf("len: %d, %s\n", name->len, name->buf);
// Output: 10, John Dover

ss_free(name);
```

## Error handling

Some operations on strings can fail due to allocations errors. Those functions could return an error in
the form of a `ss_err` type, defined similarly the enum below. If the function is successful it returns 
`err_none`, which has value zero in the enum and could be conveniently tested with a `if (err)` statement. 

```c
typedef enum ss_err {
    err_none = 0,
    err_alloc = 1,
    err_format = 2
} ss_err;
```

## Installation