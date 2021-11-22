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

This is a basic example using _ss_ strings: 
```c
ss name = ss_new_from_raw("John");
if (!name) {
    // ... handle error
}

printf("len: %d buf: %s\n", name->len, name->buf);
ss_free(name);

// Output: len 4, buf: John
```

## Error handling

Some operations on strings can fail due to allocations errors. Those functions could return an error in
the form of a `ss_err` type, defined similarly the enum below. If the function is successful it returns 
`err_none`, which has value zero in the enum and could be conveniently tested with a `if (err)` statement. 
Note that if the library is compiled with the `--with-exit` flag, memory allocations errors will abort 
the program and there's no need to check errors.

```c
typedef enum ss_err {
    err_none = 0,
    ...
} ss_err;
```

This is a simple example of string manipulations and errors checking:
```c
ss first_name = ss_new_from_raw("John");
if (!name) {
    // ... handle error
}
ss_err err = ss_concat_raw(first_name, " Dover");
if (err) {
    printf("error: %s\n", ss_err_str(err))
    return;
}

printf("len: %d buf: %s\n", name->len, name->buf);
ss_free(name);

// Output: len 10, buf: John Dover
```

If you don't need/want to check the returned errors or the library is compiled with the `--with-exit` 
flag, the example will simplify to:
```c
ss first_name = ss_new_from_raw("John");
ss_concat_raw(first_name, " Dover");

printf("len: %d buf: %s\n", name->len, name->buf);
ss_free(name);

// Output: len 10, buf: John Dover
```

## Installation

Library installation requires CMake version 3.10 or higher. A convenient setup script is provided to
install the library. The script is the `setup.sh` file at the root of the project. The script could
also be used to test the library before installing it (recommended). To install the library run the
following command in the root of the directory.

```shell
./setup.sh install
```

The options `--with-exit` could be used to customize the installation. If used, the library functions
will abort the program in case of allocation errors (if malloc/realloc fail). In this case there's no
need to check for `ss_err` errors returned from several functions of the library.

```shell
./setup.sh install --with-exit
```

Finally, it is recommended to run the unit tests before installing the library. This can be done with
the following command.

```shell
./setup.sh test --with-exit
```


