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
use the strings must be freed with the `ss_free` function. All the docs of the functions of the library 
specify if they modify the passed string in place or a new one is created, and also in this case the 
strings must be freed after use. Similarly, also string iterators and list of strings must be freed 
after use.

# Library API
## Index
[`ss_new_from_raw_len_free`](#ss_new_from_raw_len_free)  
[`ss_new_from_raw_len`](#ss_new_from_raw_len)  
[`ss_new_from_raw`](#ss_new_from_raw)  
[`ss_new_empty_with_free`](#ss_new_empty_with_free)  
[`ss_new_empty`](#ss_new_empty)  
[`ss_clone`](#ss_clone)  
[`ss_set_free_space`](#ss_set_free_space)  
[`ss_reserve_free_space`](#ss_reserve_free_space)  
[`ss_free`](#ss_free)  
[`ss_grow`](#ss_grow)  
[`ss_shrink`](#ss_shrink)  
[`ss_clear`](#ss_clear)  
[`ss_index`](#ss_index)  
[`ss_index_last`](#ss_index_last)  
[`ss_concat_raw_len`](#ss_concat_raw_len)  
[`ss_concat_raw`](#ss_concat_raw)  
[`ss_concat_str`](#ss_concat_str)  
[`ss_prepend_raw_len`](#ss_prepend_raw_len)  
[`ss_prepend_raw`](#ss_prepend_raw)  
[`ss_prepend_str`](#ss_prepend_str)  
[`ss_slice`](#ss_slice)  
[`ss_trim`](#ss_trim)  
[`ss_trim_left`](#ss_trim_left)  
[`ss_trim_right`](#ss_trim_right)  
[`ss_to_lower`](#ss_to_lower)  
[`ss_to_upper`](#ss_to_upper)  
[`ss_split_raw`](#ss_split_raw)  
[`ss_split_str`](#ss_split_str)  
[`ss_join_raw_cat`](#ss_join_raw_cat)  
[`ss_join_raw`](#ss_join_raw)  
[`ss_join_str_cat`](#ss_join_str_cat)  
[`ss_join_str`](#ss_join_str)  
[`ss_list_free`](#ss_list_free)  
[`ss_sprintf_va_cat`](#ss_sprintf_va_cat)  
[`ss_sprintf_va`](#ss_sprintf_va)  
[`ss_sprintf_cat`](#ss_sprintf_cat)  
[`ss_sprintf`](#ss_sprintf)  

### ss_new_from_raw_len_free 
Build a new string copying the provided `init` string of length `len` (the length argument doesn't
include the null terminator) and allocating additional `avail` bytes. If the length of the initial
string is greater than `len`, the exceeding bytes are discarded. Even with `len` and `avail` both
equal to zero, one byte is allocated anyway (for the null terminator). The caller is responsible for
providing the correct values of the three arguments. If the `init` string is NULL a new empty ss
string is built. The newly created string is heap allocated and a pointer to it is returned. The
string must be freed after use with the provided `ss_free` function. Useful shorthands are the
`ss_new_from_raw_len` and `ss_new_from_raw` functions, which are more ergonomic and easier to use.

The returned string has length `len`, but additional `avail` bytes are allocated. This overallocation
is useful because it's reduces the probability of future reallocations when the string is
manipulated.

Returns the newly generated string or NULL if the allocation fails.

```c
ss ss_new_from_raw_len_free(const char *init, size_t len, size_t avail);
```

### ss_new_from_raw_len 
Build a new string copying the provided `init` string of length `len` (the length argument
doesn't include the null terminator). If the length of the initial string is greater than the
provided `len`, the exceeding bytes are discarded. The caller is responsible for providing
valid values for the arguments. If the `init` string is NULL a new empty ss string is built.
The new ss string is heap allocated and a pointer to it is returned. The function is basically
a shorthand for `ss_new_raw_len_cap(init, len, len)`. The string must be freed after use with
the provided `ss_free` function.

The returned string has length `len`, but additional `len` bytes are allocated. This overallocation
is useful because it's reduces the probability of future reallocations when the string is
manipulated.

Returns the newly generated string or NULL if the allocation fails.

```c
ss ss_new_from_raw_len(const char *init, size_t len);
```

### ss_new_from_raw 
Build a new string copying the provided null terminated `init` C string. This function is a
shorthand for `str_new_raw_len(init, strlen(init), strlen(init))`. If the `init` string is
NULL a new empty ss string is built. The returned string must be freed after use with the
provided `ss_free` function.

The returned string has length `strlen(init)`, but additional `strlen(init)` bytes are allocated.
This overallocation is useful because it's reduces the probability of future reallocations when
the string is manipulated.

Returns the newly generated string or NULL if the allocation fails.

```c
ss ss_new_from_raw(const char *init);
```

### ss_new_empty_with_free 
Build and returns a new empty ss string with length zero and `avail` bytes allocated (+ 1
for the null terminator). In any case the string has an implicit null term, so 1 byte is
allocated anyway. The ss must be freed after use with the provided `ss_free` function.

Returns the newly generated string or NULL if the allocation fails.

```c
ss ss_new_empty_with_free(size_t avail);
```

### ss_new_empty 
Build and returns a new empty ss string with length and no free space. Even in this case the
string always has an implicit null term, so 1 byte is allocated anyway. The ss must be freed
after use with the provided `ss_free` function. Note that usually an empty string is built to
be filled, so it could be more convenient to directly preallocate some space via the
`ss_new_empty_with_cap` function.

Returns the newly generated string or NULL if the allocation fails.

```c
ss ss_new_empty(void);
```

### ss_clone 
Build and return a clone of the provided ss string `s`. The new string and the old one are
independent and both of them must be freed after use with the `ss_free` function. Cloning a
string is useful when, for example, we want to mutate a string while also retaining the
original content. The string `s` is not modified.

Returns the cloned string or NULL if the allocation fails.

```c
ss ss_clone(ss s);
```

### ss_set_free_space 
Sets the allocated space not already used for the string `s` to be equal to `avail` bytes. The
operation doesn't change the stored string itself, both in the content and the length. It only
changes the allocated available space to be used for future operations on the string. The function
is useful for example to reserve more space earlier in order to avoid frequent reallocations later.
The string `s` is modified in place.

Returns the modified string `s` if case of success or NULL if any reallocation fails. In case of
failure the ss string `s` is still valid and must be freed after use.

```c
ss_err ss_set_free_space(ss s, size_t avail);
```

### ss_reserve_free_space 
Enlarge the allocated space not already used by the string `s` to be at least `avail` bytes long. The
operation doesn't change the stored string, it only changes the available space beyond the string end.
The function is useful to reserve more space earlier in order to avoid frequent reallocations in the
future. If enough space is already present the function is a no-op. The string `s` is modified in place.

Returns the modified string `s` if case of success or NULL if any reallocation fails. In case of failure
the ss string `s` is still valid and must be freed after use.

```c
ss_err ss_reserve_free_space(ss s, size_t avail);
```

### ss_free 
Deallocate the memory used by the ss string `s`. The string can't be used after being freed
since it will point to deallocated memory.

```c
void ss_free(ss s);
```

## String manipulation

### ss_grow 
Grow the `s` string to have the specified length `len`. Note that here the function enlarges
the string buffer itself, and eventually the allocates space. New bytes inserted will be set
to zero and they will be safe to be written. If the specified length `len` is smaller than the
current length, the function is a no-op. The string `s` is modified in place.

Returns the modified ss string `s` if case of success or NULL if any reallocation fails. In
case of failure the ss string `s` is still valid and must be freed after use.

```c
ss_err ss_grow(ss s, size_t len);
```

### ss_shrink 
Shrink the string `s` to have the provided length `len`. The string is shortened to contain only the
first `len` bytes and a null termination character is written at the position `len` of the string buffer.
The total allocated space is left untouched, while the free space grows. The bytes in the string buffer
after the `len`-th one are not cleaned, they are just considered unused. If the length `len` is greater
than the current string length the function is a no-op. The string `s` is modified in place.

```c
void ss_shrink(ss s, size_t len);
```

### ss_clear 
Erase the content of the string `s`. The length of the string is set to 0 and a null termination
character is written in the first position of the string buffer. The total allocation space is
left untouched, while the free space grows. The old bytes are not cleaned, they are just considered
unused. It is a shorthand for `ss_shrink(s, 0)`. The string `s` is modified in place.

```c
void ss_clear(ss s);
```

### ss_index 
Returns the position (0-indexed) of the starting position of the first occurrence of the substring
`needle` in the ss string `haystack` provided as first argument. Returns -1 if no occurrence is
found or if `needle` is NULL or an empty string. The string `haystack` is not modified.

```c
size_t ss_index(ss s, const char *needle);
```

### ss_index_last 
Returns the position (0-indexed) of the starting position of the last occurrence of the substring
`needle` in the ss string `haystack` provided as first argument. Returns -1 if no occurrence is
found or if `needle` is NULL or an empty string. The string `haystack` is not modified.

```c
size_t ss_index_last(ss haystack, const char *needle);
```

### ss_concat_raw_len 
Concatenate the string `s1` with a C string `s2` of length `s2_len`. If the length of the C string
is greater than `s2_len`, exceeding bytes are discarded. The `s2` C string is appended to the string
`s1`, eventually growing the allocated space for `s1`. The strategy used in concat functions is the
following: if the string `s1` has enough allocated space to contain also the string `s2` the content
of the latter is simply appended, otherwise the `s1` string will be grown in order to have total
allocated space equal to (2*n + 1) bytes, where n is the resulting (concatenated) string length. In
this case, both the final `len` and the free space `free` will be equal to n. The string `s1` is
modified in place.

Returns the string `s1` concatenated with the C string `s2` if case of success or NULL if eventual
reallocations fail. In case of failure the string `s1` is still valid and must be freed after use.

```c
ss_err ss_concat_raw_len(ss s1, const char *s2, size_t s2_len);
```

### ss_concat_raw 
Concatenate the string `s1` with a null terminated C string `s2`. The `s2` string is appended to
s1, eventually growing the allocated space for `s1`. If the `s2` string is NULL the function
simply returns `s1`. The string `s1` is modified in-place. Basically, it is a shorthand for
`str_concat_raw_len(s1, s1, strlen(s2))` (see that function to have more info about the concat
mechanism).

Returns the string `s1` concatenated with the C string `s2` if case of success or NULL if eventual
reallocations fail. In case of failure the string `s1` is still valid and must be freed after use.

```c
ss_err ss_concat_raw(ss s1, const char *s2);
```

### ss_concat_str 
Concatenate the string `s1` with a second ss string `s2`. The `s2` string is appended to s1,
eventually growing the allocated space for `s1`. The string `s1` is modified in-place. Basically
it is a shorthand for `ss_concat_raw_len(s1, s2->buf, s2->len)` (see that function to have more
info about the concat mechanism).Both strings are still valid after the function call and must
be freed separately.

Returns the string `s1` concatenated with the C string `s2` if case of success or NULL if eventual
reallocations fail. In case of failure both strings are still valid and must be freed after use.

```c
ss_err ss_concat_str(ss s1, ss s2);
```

### ss_prepend_raw_len 
Prepend a C string `s1` of length `s1_len` to a ss string `s2`. If the length of the C string is
greater than `s1_len`, exceeding bytes are discarded. The `s1` C string is prepended to the ss
string `s2`, eventually growing the allocated space for the latter. The strategy used in prepend
functions is the following: if the string `s2` has enough allocated space to contain also the string
`s1` the content is simply prepended, otherwise the `s2` string will be grown in order to have
allocated space of (2*n + 1) bytes, where n is the resulting string length. In this case, both the
final `len` and the free space `free` will be equal to n. The string `s2` is modified in place.

Returns the string `s2` with the C string `s1` prepended in case of success or NULL if eventual
reallocations fail. In case of failure the string `s2` is still valid and must be freed after use.

```c
ss_err ss_prepend_raw_len(const char *s1, ss s2, size_t s1_len);
```

### ss_prepend_raw 
Prepend the null terminated C string `s1` to a ss string `s2`. The `s1` string is prepended to `s2`,
eventually growing the allocated space of `s2`. The string `s2` is modified in-place. Basically, it
is a shorthand for `ss_prepend_raw_len(s1, s2, strlen(s1))` (see that function to have more info
about the concat mechanism). If the `s1` string is NULL the function simply returns `s2`.

Returns the string `s2` prepended with the C string `s1` if case of success or NULL if eventual
reallocations fail. In case of failure the string `s2` is still valid and must be freed after use.

```c
ss_err ss_prepend_raw(const char *s1, ss s2);
```

### ss_prepend_str 
Prepend the ss string `s1` to another ss string `s2`. The `s1` string is appended to `s2`, eventually
growing the allocated space of `s2`. Both strings are still valid after the function call and must be
freed separately. The string `s2` is modified in-place. Basically, it is a shorthand for
`ss_prepend_raw_len(s1->buf, s2, s1->len)` (see that function to have more info about the concat
mechanism).

Returns the string `s2` prepended with the C string `s1` if case of success or NULL if eventual
reallocations fail. In case of failure both strings are still valid and must be freed after use.

```c
ss_err ss_prepend_str(ss s1, ss s2);
```

### ss_slice 
Obtain a substring of the ss string `s` slicing it with the provided indexes. The slicing boundaries
are provided via the `str_index` and `end_index` arguments (0-indexed) and the resulting substring
starts from the position `str_index` (inclusive) and ends at `end_index` (not inclusive). If the
`str_index` is >= of the original string length no changes are made. If the end index is < of `str_index`
no changes are made. If `end_index` > of the original string length, `end_index` is reduced to be equal
to the original string length before slicing the string. The string `s` is modified in place.

```c
void ss_slice(ss s, size_t str_index, size_t end_index);
```

### ss_trim 
Removes characters contained in the `cutset` string from both the start and the end of the
ss string `s`. After the trimming operation, the string length is reduced while the allocation
size is left untouched, but more free space will be available. If all characters are trimmed
the result is a valid but empty string. The string `s` is modified in place.

```c
void ss_trim(ss s, const char *cutset);
```

### ss_trim_left 
Removes characters contained in the `cutset` string from the start of the ss string `s`.
After the trimming operation, the string length is reduced while the allocation size
is left untouched, but more free space will be available. If all characters are trimmed
the result is a valid but empty string. The string `s` is modified in place.

```c
void ss_trim_left(ss s, const char *cutset);
```

### ss_trim_right 
Removes characters contained in the `cutset` string from the end of the ss string `s`.
After the trimming operation, the string length is reduced while the allocation size
is left untouched, but more free space will be available. If all characters are trimmed
the result is a valid but empty string. The string `s` is modified in place.

```c
void ss_trim_right(ss s, const char *cutset);
```

### ss_to_lower 
Modify the ss string `s` by turning each character into its lowercase version.
Modifies the string in place.

```c
void ss_to_lower(ss s);
```

### ss_to_upper 
Modify the ss string `s` by turning each character into its uppercase version.
Modifies the string in place.

```c
void ss_to_upper(ss s);
```

## String splitting and joining

### ss_split_raw 
Return all the ss substrings generated from splitting the C string `s` with the delimiter string `del`.
All the returned substrings are heap allocated and returned as an array (`*ss`) of length `n`. The array
of strings must be freed after use with the dedicated `ss_list_free` function. If the delimiter `del` is
empty or it didn't match anywhere in the string `s`, the function returns all the original string (in the
form of a ss string). Consecutive matches of the delimiter are treated as a single delimiter so no empty
strings are returned in this case. Also, if the delimiter matches at the end of the string `s` no empty
string is returned. The value pointed to by the `n` pointer will be the number of strings present in the
returned array.

Returns an array of strings of length `n` in case of success or NULL in case of allocation failures.

```c
ss *ss_split_raw(const char *s, const char *del, int *n);
```

### ss_split_str 
Return all the ss substrings generated from splitting the ss string `s` with the delimiter string `del`.
All the returned substrings are heap allocated and returned as an array (`*ss`) of length `n`. The array
of strings must be freed after use with the dedicated `ss_list_free` function. If the delimiter `del` is
empty or it didn't match anywhere in the string `s`, the function returns all the original string (in the
form of a ss string). Consecutive matches of the delimiter are treated as a single delimiter so no empty
strings are returned in this case. Also, if the delimiter matches at the end of the string `s` no empty
string is returned. The value pointed to by the `n` pointer will be the number of strings present in the
returned array. The `s` string is not modified.

Returns an array of strings of length `n` in case of success or NULL in case of allocation failures.

```c
ss *ss_split_str(ss s, const char *del, int *n);
```

### ss_join_raw_cat 
Join an array of C strings `str` of length `n` using the provided string separator `sep` between them.
The resulting string is concatenated at the end of the provided `s` string. The `s` string is modified
in place and returned.

Returns the modified string `s` if case of success or NULL if any reallocation fails. In case of
failure the ss string `s` is still valid and must be freed after use.

```c
ss_err ss_join_raw_cat(ss s, const char **str, int n, const char *sep);
```

### ss_join_raw 
Join an array of C strings `str` of length `n` using the provided string separator `sep` between them.
The resulting (joined) string is then returned as a new ss string. The returned string must be freed
after use with the provided `ss_free` function.

Returns the joined string in case of success or NULL in case of allocation errors.

```c
ss ss_join_raw(const char **str, int n, const char *sep);
```

### ss_join_str_cat 
Join an array of ss strings `str` of length `n` using the provided string separator `sep` between them.
The resulting string is concatenated at the end of the provided `s` string. The `s` string is modified
in place and returned.

Returns the modified string `s` if case of success or NULL if any reallocation fails. In case of
failure the ss string `s` is still valid and must be freed after use.

```c
ss_err ss_join_str_cat(ss s, ss *str, int n, const char *sep);
```

### ss_join_str 
Join an array of ss strings `str` of length `n` using the provided string separator `sep` between them.
The resulting (joined) string is then returned as a new ss string. The returned string must be freed
after use with the provided `ss_free` function.

Returns the joined string in case of success or NULL in case of allocation errors.

```c
ss ss_join_str(ss *str, int n, const char *sep);
```

### ss_list_free 
Deallocate the memory used by a ss string array `list`. The string array and all the
contained strings can't be used after being freed.

```c
void ss_list_free(ss *list, const int n);
```

## String formatting

### ss_sprintf_va_cat 
Formats the string and concatenates it to the `s` string. Formatting is performed using the usual
C formatting directive. The function accepts a `va_list` to accommodate a variable number of arguments.
The argument list should be started (`va_start`) before providing it to this function and must be ended
(`va_end`) after the function call. The returned string is the modified `s` string. The `s` string is
modified in place.

Returns the formatted string in case of success or NULL in case of allocations errors. In case of
failure the ss string `s` is still valid and must be freed after use.

```c
ss_err ss_sprintf_va_cat(ss s, const char *format, va_list arg_list);
```

### ss_sprintf_va 
Formats and returns a string using the usual C formatting directives. The function accepts
a `va_list` to accommodate a variable number of arguments. In case of success the returned
string must be freed after use with the dedicated `ss_free` function. The argument list
should be started (`va_start`) before providing it to this function and must be ended
(`va_end`) after the function call.

Returns the formatted string in case of success or NULL in case of allocations errors.

```c
ss ss_sprintf_va(const char *format, va_list arg_list);
```

### ss_sprintf_cat 
Formats the string and concatenates it to the `s` string. Formatting is performed using the usual
C formatting directive. The returned string is the modified `s` string. The `s` string is modified
in place.

Returns the formatted string in case of success or NULL in case of allocations errors. In case of
failure the ss string `s` is still valid and must be freed after use.

```c
ss_err ss_sprintf_cat(ss s, const char *format, ...);
```

### ss_sprintf 
Formats and returns a new string using the usual C formatting directive. The returned string
must be freed after use as usual with the dedicated `ss_free` function.

Returns the formatted string in case of success or NULL in case of allocations errors.

```c
ss ss_sprintf(const char *format, ...);
```

