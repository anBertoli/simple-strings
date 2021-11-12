# Simple Strings

Simple Strings (SS) is a string library for C designed to augment the libc string functionalities by adding dynamic and heap allocated 
strings. Those strings are easier to use and resemble the dynamic string types of other programming languages (like the String type in 
Rust).

# Library API

ss_new_from_raw_len_cap
---

Build a new string copying the provided init string of length len (the len argument
doesn't include the null terminator) and making total space for cap + 1 bytes (plus
one for the null terminator). If the length of the init string is greater than len,
the exceeding bytes are discarded. If cap < len, cap is adjusted to be equal to len.
The caller is responsible for providing the correct values of len, cap and init string.
The ss string is heap allocated and a pointer to it is returned. The string must be
freed after use with the provided ss_free function. Useful shorthands are the
ss_new_raw_len and ss_new_raw functions, which are more ergonomic and easier to use.

Returns the newly generated string or NULL if the allocation fails.

```c
ss *ss_new_from_raw_len_cap(const char *init, const size_t len, const size_t cap)
```
ss_new_from_raw_len
---

Build a new string copying the provided init string of length len (the len argument
doesn't include the null terminator). If the length of the init string is greater than
len, the exceeding bytes are discarded. The caller is responsible for providing the
correct values of len and init string. The ss string is heap allocated and a pointer
to it is returned. The ss must be freed after use with the provided ss_free function.
The ss_new_raw_len function is a shorthand for ss_new_raw_len_cap(init, len, len * 2).

The returned string has length len, but (len * 2 + 1) bytes are allocated (the cap
value) and +1 is added for the null terminator. This overallocation is often useful
because it's reduces the probability of future reallocations when the string is
manipulated.

Returns the newly generated string or NULL if the allocation fails.

```c
ss *ss_new_from_raw_len(const char *init, const size_t len)
```
ss_new_from_raw
---

Build a new string copying the provided null terminated init string. The ss_new_raw
function is a shorthand for str_new_raw_len(init, len), where len here is calculated
with strlen. If the init pointer is NULL a new empty ss string is built. The returned
ss string must be freed after use with the provided ss_free function.

Returns the newly generated string or NULL if the allocation fails.

```c
ss *ss_new_from_raw(const char *init)
```
ss_new_empty_with_cap
---

Build and returns a new empty ss string with length and the provided cap. In any case
the string always has an implicit null term, so 1 byte is allocated anyway. The ss must
be freed after use with the provided ss_free function.

Returns the new empty string or NULL if the allocation fails.

```c
ss *ss_new_empty_with_cap(const size_t cap)
```
ss_new_empty
---

Build and returns a new empty ss string with length and cap zero. Even in this case the
string always has an implicit null term, so 1 byte is allocated anyway. The ss must be
freed after use with the provided ss_free function. Note that usually an empty string
is built to be filled, so it could be a better idea in several situations to directly
preallocate some space via ss_new_empty_with_cap().

Returns the new empty string or NULL if the allocation fails.

```c
ss *ss_new_empty(void)
```
ss_clone
---

Build and return a clone of the provided ss string. The new string and the old one are
independent and both of them must be freed after use. Cloning a string is useful when,
for example, we want to mutate a string while also retaining the original content.

Returns the new string in case of success or NULL if the allocation fails.

```c
ss *ss_clone(ss *s)
```
ss_grow
---

Grow the string to have the specified length. New bytes inserted will be set to
zero. If the specified length is smaller than the current length, the function
is a no-op.

Returns the enlarged substring in case of success or NULL if any allocation fails.

```c
ss *ss_grow(ss *s, size_t len)
```
ss_cut
---

Cut the string at the provided length len. The string is shortened to contain the
first len bytes. The allocation space (cap) is left untouched. The bytes after the
number len are not cleaned, they are just marked as 'unused' since they are beyond
the length of the string. If the length len is greater than the string s length
the function is a no-op.

```c
void ss_cut(ss *s, size_t len)
```
ss_clear
---

Erase the string. The length of the string is set to 0 and a null terminator
is written in the first byte of the string buffer. The allocation space (cap)
is left untouched, so future string manipulations can be performed with fewer
reallocations. The bytes after the number len are not cleaned, they are just
marked as 'unused' since they are beyond the length of the string.

```c
void ss_clear(ss *s)
```
ss_set_free_space
---

Changes the allocated space for the string. The operation doesn't change the
stored string itself, both in the content and the length. It only changes the
available (allocated) space beyond the string len. The function is useful to
reserve more space earlier in order to avoid frequent reallocations later.

Returns the ss string s if the function succeeded or NULL if the eventual
reallocation fails. In case of failure the ss string s is still valid and
must be freed after use.

```c
ss *ss_set_free_space(ss *s, size_t free_space)
```
ss_reserve_free_space
---

Enlarge the free allocated space (the space after the string itself) to at least
'free_space' bytes. The operation doesn't change the stored string buffer itself,
both in the content and the length. It only changes the available space beyond the
string length. The function is useful to reserve more space earlier in order to
avoid frequent reallocations. If enough space is already present the function is
a no-op.

Returns the ss string s if the function succeeded or NULL if the eventual
reallocation fails. In case of failure the ss string s is still valid and
must be freed after use.

```c
ss *ss_reserve_free_space(ss *s, size_t free_space)
```
ss_free
---

Deallocate the memory used by a ss string s. The string can't be used after
being freed.

```c
void ss_free(ss *s)
```
ss_index
---

Returns the position (0-indexed) of the first occurrence of the substring needle in the
ss string provided as first argument. Returns -1 if no occurrence is found.

```c
int ss_index(ss *s, const char *needle)
```
ss_concat_raw_len
---

Concatenate a ss string s1 with a C string s2 of length s2_len. If the length of the C
string is greater than s2_len, the exceeding bytes are discarded. The s2 C string is
appended to the ss string s1, eventually growing s1. The strategy used in concat
functions is: if the string s1 has enough allocated space to contain also the string s2
the content is simply appended, otherwise the s1 string will be grown in order to have
a capacity (cap) of (2*n + 1) bytes, where n is the resulting (concatenated) string length.

Returns the concatenated string s1 if the function succeeded or NULL if the eventual
reallocation fails. In case of failure the string s1 is still valid and must be
freed after use.

```c
ss *ss_concat_raw_len(ss *s1, const char *s2, const size_t s2_len)
```
ss_concat_raw
---

Concatenate a ss string s1 with a null terminated C string s2. The s2 string is
appended to s1, eventually growing the s1 string. Basically, it is a shorthand for
str_concat_raw(s1, s1, strlen(s2)).

Returns the concatenated string s1 if the function succeeded or NULL if the eventual
reallocation fails. In case of failure the ss string s1 is still valid and must be
freed after use.

```c
ss *ss_concat_raw(ss *s1, const char *s2)
```
ss_concat_str
---

Concatenate a ss string s1 with another ss string s2. The s2 string is appended to
s1, eventually growing the s1 string. Both strings are still valid after the
function call and must be freed separately.

Returns the concatenated string s1 if the function succeeded or NULL if the eventual
reallocation fails. In case of failure the ss string s1 is still valid and must be
freed after use.

```c
ss *ss_concat_str(ss *s1, ss *s2)
```
ss_prepend_raw_len
---

Prepend a C string s1 of length s1_len to a ss string s2. If the length of the C
string is greater than s1_len, the exceeding bytes are discarded. The s1 C string is
prepended to the ss string s2, eventually growing s2. The strategy used in prepend
functions is: if the string s2 has enough allocated space to contain also the string s1
the content is simply prepended, otherwise the s2 string will be grown in order to have
a capacity (cap) of (2*n + 1) bytes, where n is the resulting string length. The s2 string
buffer is memmoved ahead in any case.

Returns the string s2 with the appended C string in case of success or NULL if the eventual
reallocation fails. In case of failure the string s2 is still valid and must be
freed after use.

```c
ss *ss_prepend_raw_len(const char *s1, ss *s2, const size_t s1_len)
```
ss_prepend_raw
---

Prepend a null terminated C string s1 to a ss string s2. The s1 string is prepended to
s2, eventually growing the s2 string. Basically, it is a shorthand for
ss_prepend_raw_len(s1, s2, strlen(s1)).

Returns the string s2 with the prepended C string in case of success or NULL if the eventual
reallocation fails. In case of failure the string s2 is still valid and must be
freed after use.

```c
ss *ss_prepend_raw(const char *s1, ss *s2)
```
ss_prepend_str
---

Prepend a ss string s1 to another ss string s2. The s1 string is appended to
s2, eventually growing the s2 string. Both strings are still valid after the
function call and must be freed separately.

Returns the string s2 with the prepended s1 string in case of success or NULL if the eventual
reallocation fails. In case of failure the string s2 is still valid and must be
freed after use.

```c
ss *ss_prepend_str(ss *s1, ss *s2)
```
ss_slice
---

Substring the original ss string slicing it with the provided index. The string is modified
in-place. The slicing boundaries must be provided via the str_index and end_index (0-indexed)
and the resulting range is [str_index, end_index) (right boundary not inclusive). If the str_index
is >= of the original string length no changes are made. If the end index is < of str_index no
changes are made. If end_index > of the original string length, end_index is reduced to original
string length before slicing the string.

```c
void ss_slice(ss *s, const int str_index, const int end_index)
```
ss_trim
---

Removes characters contained in the cutset string from both the start and the end of the
ss string s. After the trimming operation, the string length is reduced while the allocation
size (the capacity cap) is left untouched. If all characters are trimmed the result is a
valid but empty ss string.

```c
void ss_trim(ss *s, const char *cutset)
```
ss_trim_left
---

Removes characters contained in the cutset string from the start of the ss string s.
After the trimming operation, the string length is reduced while the allocation size
(the capacity cap) is left untouched. If all characters are trimmed the result is a
valid but empty string.

```c
void ss_trim_left(ss *s, const char *cutset)
```
ss_trim_right
---

Removes characters contained in the cutset string from the end of the ss string s.
After the trimming operation, the string length is reduced while the allocation
size is left untouched. If all characters are trimmed the result is a valid but
empty ss string.

```c
void ss_trim_right(ss *s, const char *cutset)
```
ss_to_lower
---

Turn the ss string by turning each char into its lowercase version.
Modifies the string in place.

```c
void ss_to_lower(ss *s)
```
ss_to_upper
---

Turn the ss string by turning each char into its uppercase version.
Modifies the string in place.

```c
void ss_to_upper(ss *s)
```
ss_sprintf_va
---

Format a string using the usual C formatting directives and return it. The function
accepts s va_list to accommodate a variable number of arguments. In case of success
the returned string must be freed after use with the dedicated ss_free function().
The argument list should be started (va_start) before providing it to this function
and must be ended (va_end) after the function call.

Returns the ss string s in case of success or NULL in case of allocations errors.

```c
ss *ss_sprintf_va(const char *format, va_list arg_list)
```
ss_sprintf
---

Format a string using the usual C formatting directive and returns a formatted string.
The returned string must be freed after use with the dedicated ss_free function().

Returns a formatted ss string in case of success or NULL in case of allocations errors.

```c
ss *ss_sprintf(const char *format, ...)
```
ss_split_raw_to_iter
---

Build a new string iterator from a raw string and a string delimiter. The string
iterator is used to split the string into substrings, each of them separated from
others by occurrences of the delimiter. The string iterator should be used with
the ss_iter_next function in order to yield subsequent substring.

The string iterator is heap allocated along with a copy of the passed string. The
string iterator must be freed after use, but note that if a call to ss_iter_next
returns END_ITER (i.e. iterator exhausted) the string iter has been automatically
freed.

Returns ss_iter* or NULL in case of allocation failures.

```c
ss_iter *ss_split_raw_to_iter(const char *s, const char *del)
```
ss_split_str_to_iter
---

Build a new string iterator from a string and a string delimiter. It is a shorthand
for ss_split_raw(s->buf, del). Look at the documentation of ss_split_raw_to_iter for
further details. The string iterator should be used with the ss_iter_next function
in order to yield subsequent substring.

The string iterator is heap allocated along with a copy of the passed string. The
string iterator must be freed after use, but note that if a call to ss_iter_next
returns END_ITER (i.e. iterator exhausted) the string iter have been automatically
freed.

Returns ss_iter* or NULL in case of allocation failures.

```c
ss_iter *ss_split_str_to_iter(ss *s, const char *del)
```
ss_iter_next
---

Advances the string iterator and returns the next substring. The returned substring
starts at the end of the previous delimiter (or the start of the original string in
the first iteration) and ends at the start of the next delimiter (or at the end of
the original string in the last iteration). Those ss substrings are heap allocated
and must be freed after use (with ss_free). If no delimiters are present, all the
original string is returned. If the iterator contains an empty delimiter all the
original string is returned. Multiple consecutive delimiters are treated as one
delimiter, and in general no empty strings are returned from this function.

When the iterator is exhausted the function returns END_ITER. The caller can test
for this condition to stop the iteration. When END_ITER is returned, ss_iter_next
has already taken care of freeing the string iterator. The function returns NULL
in case of allocation errors. Also in this case the iterator is automatically freed.

Returns a ss* in case of success, NULL in case of allocation failures or END_ITER
when the iterator is exhausted.

```c
ss *ss_iter_next(ss_iter *s_iter)
```
ss_iter_collect
---

Collect and return all the ss substrings that a ss_iter could produce. The function is
useful when the caller doesn't want/need to iterate and collect substrings manually;
ss_split_raw and ss_split_str have similar functions but ss_iter_collect is more flexible
since it could be used also when some substrings were already obtained from an iterator.
All substrings are heap allocated and returned as an array of *ss that must be freed after
use with the dedicated ss_list_free function.

Returns an array of strings (ss*) of length n in case of success or NULL in case of
allocation failures. In case of errors the strings list is freed along with the iterator.

```c
ss **ss_iter_collect(ss_iter *s_iter, int *n)
```
ss_split_raw
---

Return all the ss substrings generated from splitting the raw_str argument with the
delimiter string. The function is useful when the caller doesn't want/need to
create a string iterator and collect substrings manually. All substrings are heap
allocated and returned as an array of ss (ss**) that must be freed after use with
the dedicated ss_list_free function.

Returns an array of strings (ss**) of length n in case of success or NULL in case of
allocation failures. In case of errors the strings list is freed along with the iterator.

```c
ss **ss_split_raw(const char *raw_str, const char *del, int *n)
```
ss_split_str
---

Return all the ss substrings generated from splitting the ss string argument with
the delimiter string. The function is useful when the caller doesn't want/need to
create a string iterator and collect substrings manually. All substrings are heap
allocated and returned as an array of *ss that must be freed after use with the
dedicated ss_list_free function.

Returns an array of strings (ss**) of length n in case of success or NULL in case of
allocation failures. In case of errors the strings list is freed along with the iterator.

```c
ss **ss_split_str(ss *s, const char *del, int *n)
```
ss_join_raw
---

Join an array of C strings using the provided separator (also a C string) between them.
The returned ss string must be freed after use. As an example, if the provided strings
are "how", "are", "you?" and the delimiter is a hash (#) the resulting string is:
"how#are#you?".

Returns the joined ss string in case of success or NULL in case of allocation errors.

```c
ss *ss_join_raw(char **s, const int n, const char *sep)
```
ss_join_str
---

Join an array of ss strings using the provided separator (a C string) between them.
The returned ss string must be freed after use. As an example, if the provided strings
are "how", "are", "you?" and the delimiter is a hash (#) the resulting string is:
"how#are#you?".

Returns the joined ss string in case of success or NULL in case of allocation errors.

```c
ss *ss_join_str(ss **s, const int n, const char *sep)
```
ss_iter_free
---

Deallocate the memory used by a string iterator. The string iterator can't be
used after being freed. Note that some functions automatically consumes and
frees the iterator, so it shouldn't be freed again.

```c
void ss_iter_free(ss_iter *s_iter)
```
ss_list_free
---

Deallocate the memory used by a ss string array. The string array and all the
strings can't be used after being freed.

```c
void ss_list_free(ss **s_list, const int n)
```




Utilities used in iterator and lists tests.



