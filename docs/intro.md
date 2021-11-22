# Simple Strings

Simple Strings (_ss_) is a string library for C designed to augment the libc string functionalities by
adding dynamic and heap allocated strings. Those strings are easier to use and resemble the dynamic
string types of other programming languages.

## How the library works

At the core of the _ss_ strings library there is the struct showed below. This struct represents a dynamic
and heap allocated string. The string buffer itself is pointed to by the `buf` pointer, has length `len` 
(the `len`-th char is the null terminator) and has additional `free` bytes allocated and ready to be used 
in further string manipulation. Functions that instantiate strings return pointers to them (the `ss` type). 
Both the struct and the string buffer are heap allocated. From now, we will use the term string to define
the `ss` pointer type.  

```c
typedef struct ss {
    size_t len;
    size_t free;
    char *buf;
} *ss;
```

The _ss_ strings are generated and returned via dedicated constructor functions and they are usually 
manipulated using the functions of the library. As a unique and obvious exception, the `buf` field is 
safer to be read and write until the position `len-1`, i.e. one can write in the string buffer starting
at the position `ss->buf[0]` until the position `ss->buf[ss->len-1]`. If you need to write directly 
into the string buffer make sure the buffer itself has enough space (otherwise grow it with the dedicated 
`ss_grow` function). The functions of the library automatically handle the allocated space and the mentioned 
fields to both perform the needed operations and maintain the string state consistent. In any case, **the
`len` and `free` fields must be considered read-only**. 

Both the string struct pointed to by the `ss` pointer and the string buffer itself are heap allocated. After
use, they must be freed passing the `ss` string to the `ss_free` function. If a function of the library 
creates a new string, also the new string must be freed after use. Similarly, string lists must be freed
after use with the dedicated `ss_list_free` function.

This is the most basic example using _ss_ strings: 
```c
ss name = ss_new_from_raw("John");
if (!name) {
    // ... handle error
}

ss_concat_raw(name, " Dover");
ss_prepend_raw("My name is: ", name);

printf("len: %d buf: %s\n", name->len, name->buf);
ss_free(name);

// Output: len: 22 buf: 'My name is: John Dover'
```

The library offers a wide range of functions, both higher level functions for easy string manipulation
but also a set of low level utilities that make possible to optimize code for high performance and
reduce drastically the penalty for using a higher level string library. In general, the functions of
the library preallocate additional memory to reduce the probability of future reallocations when the 
strings are manipulated. These low level details can be tuned using specific functions. The 
`ss_new_from_raw_len_free` constructor, as an example, allows to specify the amount of available but 
still unused memory, while `ss_reserve_free_space` controls the same parameter but for strings already 
created. The general line followed is to reduce the number of allocations not the quantity of memory 
allocated for performance reasons. Still, programs that need a minimal memory footprint can use those 
low-level functions to control the memory usage in very detail.

## Error handling

Some operations on strings can fail due to allocations errors. These functions could return an error in
the form of a `ss_err` type, defined as the enum below. If the function is successful it returns `err_none`,
which has value zero and could be conveniently tested with a `if (err)` statement. Note that if the library
is compiled with the `--with-exit` flag, memory allocation errors abort the program and there's no need 
to check these errors and to check for NULL values returned from string constructors. The `ss_err_str` function 
could be used to get a static read-only string containing a textual representation of an error code.

```c
typedef enum ss_err {
    err_none = 0,
    err_alloc = 1,
    err_format = 2
} ss_err;
```

## Installation

Library installation requires CMake version 3.10 or higher. A convenient setup script could be used to 
easily install the library. The script is the `setup.sh` file in project root. The script could also
be used to test the library before installing it (recommended). To install the library run the following
command in the root of the project.

```shell
./setup.sh install
```

The options `--with-exit` could be used to customize the installation. If used, the functions of the 
library abort the program in case of allocation errors (mainly malloc/realloc failures). In this case 
there's no need to check for `ss_err` errors or NULL values returned from the library functions.

```shell
./setup.sh install --with-exit
```

Finally, it is recommended to run the unit tests before installing the library. This can be done with
the following command.

```shell
./setup.sh test --with-exit
```


