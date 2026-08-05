#ifndef UTF_STRING_H
#define UTF_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utf.h"

/**
 * UTF-8
 */

typedef struct utf8_string_s utf8_string_t;
typedef struct utf8_string_view_s utf8_string_view_t;

struct utf8_string_s {
  utf8_t *data;
  size_t len;
  union {
    size_t cap;
    utf8_t buf[8];
  };
};

struct utf8_string_view_s {
  const utf8_t *data;
  size_t len;
};

inline void
utf8_string_init(utf8_string_t *string) {
  string->data = string->buf;
  string->len = 0;
}

inline utf8_string_view_t
utf8_string_view_init(const utf8_t *data, size_t len) {
  utf8_string_view_t view = {data, len};
  return view;
}

inline void
utf8_string_destroy(utf8_string_t *string) {
  if (string->data != string->buf) free(string->data);
}

inline int
utf8_string_reserve(utf8_string_t *string, size_t len) {
  size_t cap = string->data == string->buf ? sizeof(string->buf) : string->cap;

  if (len <= cap) return 0;

  // Round up to next power of two; bail out if the rounding would overflow.
  if (len > (SIZE_MAX >> 1) + 1) return -1;

  // https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
  len--;
  len |= len >> 1;
  len |= len >> 2;
  len |= len >> 4;
  len |= len >> 8;
  len |= len >> 16;
#if SIZE_MAX == UINT64_MAX
  len |= len >> 32;
#endif
  len++;

  cap = len;

  utf8_t *data;

  if (string->data == string->buf) {
    data = (utf8_t *) malloc(cap);

    if (data == NULL) return -1;

    memcpy(data, string->data, string->len);
  } else {
    data = (utf8_t *) realloc(string->data, cap);

    if (data == NULL) return -1;
  }

  string->data = data;
  string->cap = cap;

  return 0;
}

inline int
utf8_string_shrink_to_fit(utf8_string_t *string) {
  if (string->data == string->buf) return 0;

  size_t cap = string->len;

  if (cap <= sizeof(string->buf)) {
    memcpy(string->buf, string->data, cap);

    free(string->data);

    string->data = string->buf;
  } else {
    utf8_t *data = (utf8_t *) realloc(string->data, cap);

    if (data == NULL) return -1;

    string->data = data;
    string->cap = cap;
  }

  return 0;
}

inline utf8_string_view_t
utf8_string_view(const utf8_string_t *string) {
  return utf8_string_view_init(string->data, string->len);
}

inline void
utf8_string_clear(utf8_string_t *string) {
  string->len = 0;
}

inline bool
utf8_string_empty(const utf8_string_t *string) {
  return string->len == 0;
}

inline bool
utf8_string_view_empty(const utf8_string_view_t view) {
  return view.len == 0;
}

inline int
utf8_string_copy(const utf8_string_t *string, utf8_string_t *result) {
  int err;

  err = utf8_string_reserve(result, string->len);
  if (err < 0) return err;

  memcpy(result->data, string->data, string->len);

  result->len = string->len;

  return 0;
}

inline int
utf8_string_view_copy(const utf8_string_view_t view, utf8_string_t *result) {
  int err;

  err = utf8_string_reserve(result, view.len);
  if (err < 0) return err;

  memcpy(result->data, view.data, view.len);

  result->len = view.len;

  return 0;
}

inline int
utf8_string_append(utf8_string_t *string, const utf8_string_t *other) {
  int err;

  err = utf8_string_reserve(string, string->len + other->len);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], other->data, other->len);

  string->len += other->len;

  return 0;
}

inline int
utf8_string_append_view(utf8_string_t *string, const utf8_string_view_t view) {
  int err;

  err = utf8_string_reserve(string, string->len + view.len);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], view.data, view.len);

  string->len += view.len;

  return 0;
}

inline int
utf8_string_append_character(utf8_string_t *string, utf8_t c) {
  int err;

  err = utf8_string_reserve(string, string->len + 1);
  if (err < 0) return -1;

  string->data[string->len] = c;

  string->len += 1;

  return 0;
}

inline int
utf8_string_append_literal(utf8_string_t *string, const utf8_t *literal, size_t n) {
  int err;

  if (n == (size_t) -1) n = strlen((const char *) literal);

  err = utf8_string_reserve(string, string->len + n);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], literal, n);

  string->len += n;

  return 0;
}

inline int
utf8_string_prepend(utf8_string_t *string, const utf8_string_t *other) {
  int err;

  err = utf8_string_reserve(string, string->len + other->len);
  if (err < 0) return -1;

  memmove(&string->data[other->len], string->data, string->len);

  memcpy(string->data, other->data, other->len);

  string->len += other->len;

  return 0;
}

inline int
utf8_string_prepend_view(utf8_string_t *string, const utf8_string_view_t view) {
  int err;

  err = utf8_string_reserve(string, string->len + view.len);
  if (err < 0) return -1;

  memmove(&string->data[view.len], string->data, string->len);

  memcpy(string->data, view.data, view.len);

  string->len += view.len;

  return 0;
}

inline int
utf8_string_prepend_character(utf8_string_t *string, utf8_t c) {
  int err;

  err = utf8_string_reserve(string, string->len + 1);
  if (err < 0) return -1;

  memmove(&string->data[1], string->data, string->len);

  string->data[0] = c;

  string->len += 1;

  return 0;
}

inline int
utf8_string_prepend_literal(utf8_string_t *string, const utf8_t *literal, size_t n) {
  int err;

  if (n == (size_t) -1) n = strlen((const char *) literal);

  err = utf8_string_reserve(string, string->len + n);
  if (err < 0) return -1;

  memmove(&string->data[n], string->data, string->len);

  memcpy(string->data, literal, n);

  string->len += n;

  return 0;
}

inline int
utf8_string_insert(utf8_string_t *string, size_t pos, const utf8_string_t *other) {
  int err;

  if (pos > string->len) return -1;

  size_t inserted_len = string->len + other->len;

  err = utf8_string_reserve(string, inserted_len);
  if (err < 0) return err;

  memmove(&string->data[pos + other->len], &string->data[pos], string->len - pos);

  memcpy(&string->data[pos], other->data, other->len);

  string->len = inserted_len;

  return 0;
}

inline int
utf8_string_insert_view(utf8_string_t *string, size_t pos, const utf8_string_view_t other) {
  int err;

  if (pos > string->len) return -1;

  size_t inserted_len = string->len + other.len;

  err = utf8_string_reserve(string, inserted_len);
  if (err < 0) return err;

  memmove(&string->data[pos + other.len], &string->data[pos], string->len - pos);

  memcpy(&string->data[pos], other.data, other.len);

  string->len = inserted_len;

  return 0;
}

inline int
utf8_string_insert_character(utf8_string_t *string, size_t pos, utf8_t c) {
  int err;

  if (pos > string->len) return -1;

  size_t inserted_len = string->len + 1;

  err = utf8_string_reserve(string, inserted_len);
  if (err < 0) return err;

  memmove(&string->data[pos + 1], &string->data[pos], string->len - pos);

  string->data[pos] = c;

  string->len = inserted_len;

  return 0;
}

inline int
utf8_string_insert_literal(utf8_string_t *string, size_t pos, const utf8_t *literal, size_t n) {
  int err;

  if (pos > string->len) return -1;

  if (n == (size_t) -1) n = strlen((const char *) literal);

  size_t inserted_len = string->len + n;

  err = utf8_string_reserve(string, inserted_len);
  if (err < 0) return err;

  memmove(&string->data[pos + n], &string->data[pos], string->len - pos);

  memcpy(&string->data[pos], literal, n);

  string->len = inserted_len;

  return 0;
}

inline int
utf8_string_replace(utf8_string_t *string, size_t pos, size_t len, const utf8_string_t *replacement) {
  int err;

  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  size_t replaced_len = string->len + replacement->len - len;

  err = utf8_string_reserve(string, replaced_len);
  if (err < 0) return err;

  memmove(&string->data[pos + replacement->len], &string->data[pos + len], string->len - pos - len);

  memcpy(&string->data[pos], replacement->data, replacement->len);

  string->len = replaced_len;

  return 0;
}

inline int
utf8_string_replace_view(utf8_string_t *string, size_t pos, size_t len, const utf8_string_view_t replacement) {
  int err;

  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  size_t replaced_len = string->len + replacement.len - len;

  err = utf8_string_reserve(string, replaced_len);
  if (err < 0) return err;

  memmove(&string->data[pos + replacement.len], &string->data[pos + len], string->len - pos - len);

  memcpy(&string->data[pos], replacement.data, replacement.len);

  string->len = replaced_len;

  return 0;
}

inline int
utf8_string_replace_character(utf8_string_t *string, size_t pos, size_t len, utf8_t c) {
  int err;

  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  size_t replaced_len = string->len + 1 - len;

  err = utf8_string_reserve(string, replaced_len);
  if (err < 0) return err;

  memmove(&string->data[pos + 1], &string->data[pos + len], string->len - pos - len);

  string->data[pos] = c;

  string->len = replaced_len;

  return 0;
}

inline int
utf8_string_replace_literal(utf8_string_t *string, size_t pos, size_t len, const utf8_t *literal, size_t n) {
  int err;

  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  if (n == (size_t) -1) n = strlen((const char *) literal);

  size_t replaced_len = string->len + n - len;

  err = utf8_string_reserve(string, replaced_len);
  if (err < 0) return err;

  memmove(&string->data[pos + n], &string->data[pos + len], string->len - pos - len);

  memcpy(&string->data[pos], literal, n);

  string->len = replaced_len;

  return 0;
}

inline int
utf8_string_erase(utf8_string_t *string, size_t pos, size_t len) {
  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  memmove(&string->data[pos], &string->data[pos + len], string->len - pos - len);

  string->len -= len;

  return 0;
}

inline int
utf8_string_concat(const utf8_string_t *string, const utf8_string_t *other, utf8_string_t *result) {
  int err;

  utf8_string_init(result);

  err = utf8_string_reserve(result, string->len + other->len);
  if (err < 0) return err;

  err = utf8_string_append(result, string);
  assert(err == 0);

  err = utf8_string_append(result, other);
  assert(err == 0);

  return 0;
}

inline int
utf8_string_view_concat(const utf8_string_view_t view, const utf8_string_t *other, utf8_string_t *result) {
  int err;

  utf8_string_init(result);

  err = utf8_string_reserve(result, view.len + other->len);
  if (err < 0) return err;

  err = utf8_string_append_view(result, view);
  assert(err == 0);

  err = utf8_string_append(result, other);
  assert(err == 0);

  return 0;
}

inline int
utf8_string_concat_view(const utf8_string_t *string, const utf8_string_view_t other, utf8_string_t *result) {
  int err;

  utf8_string_init(result);

  err = utf8_string_reserve(result, string->len + other.len);
  if (err < 0) return err;

  err = utf8_string_append(result, string);
  assert(err == 0);

  err = utf8_string_append_view(result, other);
  assert(err == 0);

  return 0;
}

inline int
utf8_string_view_concat_view(const utf8_string_view_t view, const utf8_string_view_t other, utf8_string_t *result) {
  int err;

  utf8_string_init(result);

  err = utf8_string_reserve(result, view.len + other.len);
  if (err < 0) return err;

  err = utf8_string_append_view(result, view);
  assert(err == 0);

  err = utf8_string_append_view(result, other);
  assert(err == 0);

  return 0;
}

inline int
utf8_string_concat_character(const utf8_string_t *string, utf8_t c, utf8_string_t *result) {
  int err;

  utf8_string_init(result);

  err = utf8_string_reserve(result, string->len + 1);
  if (err < 0) return err;

  err = utf8_string_append(result, string);
  assert(err == 0);

  err = utf8_string_append_character(result, c);
  assert(err == 0);

  return 0;
}

inline int
utf8_string_view_concat_character(const utf8_string_view_t view, utf8_t c, utf8_string_t *result) {
  int err;

  utf8_string_init(result);

  err = utf8_string_reserve(result, view.len + 1);
  if (err < 0) return err;

  err = utf8_string_append_view(result, view);
  assert(err == 0);

  err = utf8_string_append_character(result, c);
  assert(err == 0);

  return 0;
}

inline int
utf8_string_concat_literal(const utf8_string_t *string, const utf8_t *literal, size_t n, utf8_string_t *result) {
  int err;

  utf8_string_init(result);

  err = utf8_string_reserve(result, string->len + n);
  if (err < 0) return err;

  err = utf8_string_append(result, string);
  assert(err == 0);

  err = utf8_string_append_literal(result, literal, n);
  assert(err == 0);

  return 0;
}

inline int
utf8_string_view_concat_literal(const utf8_string_view_t view, const utf8_t *literal, size_t n, utf8_string_t *result) {
  int err;

  utf8_string_init(result);

  err = utf8_string_reserve(result, view.len + n);
  if (err < 0) return err;

  err = utf8_string_append_view(result, view);
  assert(err == 0);

  err = utf8_string_append_literal(result, literal, n);
  assert(err == 0);

  return 0;
}

inline int
utf8_string_compare(const utf8_string_t *string, const utf8_string_t *other) {
  size_t a_len = string->len;
  size_t b_len = other->len;

  // The shorter of the two lengths, over which the contents are compared. The
  // comparison is one of bytes rather than of C strings, a length delimited
  // string being free to hold a null byte.
  size_t len = a_len < b_len ? a_len : b_len;

  int result = len == 0 ? 0 : memcmp(string->data, other->data, len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

inline int
utf8_string_view_compare(const utf8_string_view_t view, const utf8_string_view_t other) {
  size_t a_len = view.len;
  size_t b_len = other.len;

  // The shorter of the two lengths, over which the contents are compared. The
  // comparison is one of bytes rather than of C strings, a length delimited
  // string being free to hold a null byte.
  size_t len = a_len < b_len ? a_len : b_len;

  int result = len == 0 ? 0 : memcmp(view.data, other.data, len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

inline int
utf8_string_compare_literal(const utf8_string_t *string, const utf8_t *literal, size_t n) {
  if (n == (size_t) -1) n = strlen((const char *) literal);

  size_t a_len = string->len;
  size_t b_len = n;

  // The shorter of the two lengths, over which the contents are compared. The
  // comparison is one of bytes rather than of C strings, a length delimited
  // string being free to hold a null byte.
  size_t len = a_len < b_len ? a_len : b_len;

  int result = len == 0 ? 0 : memcmp(string->data, literal, len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

inline int
utf8_string_view_compare_literal(const utf8_string_view_t view, const utf8_t *literal, size_t n) {
  if (n == (size_t) -1) n = strlen((const char *) literal);

  size_t a_len = view.len;
  size_t b_len = n;

  // The shorter of the two lengths, over which the contents are compared. The
  // comparison is one of bytes rather than of C strings, a length delimited
  // string being free to hold a null byte.
  size_t len = a_len < b_len ? a_len : b_len;

  int result = len == 0 ? 0 : memcmp(view.data, literal, len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

inline utf8_string_view_t
utf8_string_substring(const utf8_string_t *string, size_t start, size_t end) {
  if (end == (size_t) -1 || end > string->len) end = string->len;
  if (start > end) start = end;

  return utf8_string_view_init(&string->data[start], end - start);
}

inline utf8_string_view_t
utf8_string_view_substring(const utf8_string_view_t view, size_t start, size_t end) {
  if (end == (size_t) -1 || end > view.len) end = view.len;
  if (start > end) start = end;

  return utf8_string_view_init(&view.data[start], end - start);
}

inline int
utf8_string_substring_copy(const utf8_string_t *string, size_t start, size_t end, utf8_string_t *result) {
  int err;

  if (end == (size_t) -1 || end > string->len) end = string->len;
  if (start > end) start = end;

  utf8_string_init(result);

  size_t len = end - start;

  err = utf8_string_reserve(result, len);
  if (err < 0) return -1;

  memcpy(result->data, &string->data[start], len);

  result->len = len;

  return 0;
}

inline int
utf8_string_view_substring_copy(const utf8_string_view_t view, size_t start, size_t end, utf8_string_t *result) {
  int err;

  if (end == (size_t) -1 || end > view.len) end = view.len;
  if (start > end) start = end;

  size_t len = end - start;

  err = utf8_string_reserve(result, len);
  if (err < 0) return -1;

  memcpy(result->data, &view.data[start], len);

  result->len = len;

  return 0;
}

inline size_t
utf8_string_index_of_character(const utf8_string_t *string, size_t pos, utf8_t c) {
  // Also catches a pos of (size_t) -1, which must not be turned into a length.
  if (pos >= string->len) return (size_t) -1;

  const utf8_t *found = (const utf8_t *) memchr(&string->data[pos], c, string->len - pos);

  return found == NULL ? (size_t) -1 : (size_t) (found - string->data);
}

inline size_t
utf8_string_view_index_of_character(const utf8_string_view_t view, size_t pos, utf8_t c) {
  // Also catches a pos of (size_t) -1, which must not be turned into a length.
  if (pos >= view.len) return (size_t) -1;

  const utf8_t *found = (const utf8_t *) memchr(&view.data[pos], c, view.len - pos);

  return found == NULL ? (size_t) -1 : (size_t) (found - view.data);
}

inline size_t
utf8_string_last_index_of_character(const utf8_string_t *string, size_t pos, utf8_t c) {
  if (string->len == 0) return (size_t) -1;
  if (pos == (size_t) -1) pos = string->len - 1;
  else if (pos >= string->len) return (size_t) -1;

  for (size_t i = pos; i <= pos; i--) {
    if (string->data[i] == c) {
      return i;
    }
  }

  return (size_t) -1;
}

inline size_t
utf8_string_view_last_index_of_character(const utf8_string_view_t view, size_t pos, utf8_t c) {
  if (view.len == 0) return (size_t) -1;
  if (pos == (size_t) -1) pos = view.len - 1;
  else if (pos >= view.len) return (size_t) -1;

  for (size_t i = pos; i <= pos; i--) {
    if (view.data[i] == c) {
      return i;
    }
  }

  return (size_t) -1;
}

/**
 * UTF-32
 */

typedef struct utf32_string_s utf32_string_t;
typedef struct utf32_string_view_s utf32_string_view_t;

/**
 * The number of code points that the inline buffer of a string holds. Unlike
 * for UTF-8, where the two coincide, the capacity of a string counts code
 * points rather than the bytes that they occupy.
 *
 * A code point takes four times the room of a UTF-8 code unit, so a buffer of
 * the same length as that of `utf8_string_t` would grow the string by more than
 * it tends to save in allocations. This is kept short enough that a string
 * remains cheap to hold an array of, on the understanding that a caller who
 * knows how much it needs is better served by reserving that upfront.
 */
#define UTF32_STRING_INLINE_CAPACITY 8

struct utf32_string_s {
  utf32_t *data;
  size_t len;
  union {
    size_t cap;
    utf32_t buf[UTF32_STRING_INLINE_CAPACITY];
  };
};

struct utf32_string_view_s {
  const utf32_t *data;
  size_t len;
};

/**
 * Returns the length of a null terminated sequence of code points. This stands
 * in for the `strlen` that the UTF-8 string uses, there being no equivalent for
 * code points in the standard library.
 */
static inline size_t
utf32__string_literal_length(const utf32_t *literal) {
  size_t len = 0;

  while (literal[len] != 0) {
    len++;
  }

  return len;
}

inline void
utf32_string_init(utf32_string_t *string) {
  string->data = string->buf;
  string->len = 0;
}

inline utf32_string_view_t
utf32_string_view_init(const utf32_t *data, size_t len) {
  utf32_string_view_t view = {data, len};
  return view;
}

inline void
utf32_string_destroy(utf32_string_t *string) {
  if (string->data != string->buf) free(string->data);
}

inline int
utf32_string_reserve(utf32_string_t *string, size_t len) {
  size_t cap = string->data == string->buf ? UTF32_STRING_INLINE_CAPACITY : string->cap;

  if (len <= cap) return 0;

  // Round up to next power of two; bail out if the rounding would overflow.
  if (len > (SIZE_MAX >> 1) + 1) return -1;

  // https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
  len--;
  len |= len >> 1;
  len |= len >> 2;
  len |= len >> 4;
  len |= len >> 8;
  len |= len >> 16;
#if SIZE_MAX == UINT64_MAX
  len |= len >> 32;
#endif
  len++;

  cap = len;

  // The capacity counts code points, and so must also fit once scaled to the
  // number of bytes that they occupy.
  if (cap > SIZE_MAX / sizeof(utf32_t)) return -1;

  utf32_t *data;

  if (string->data == string->buf) {
    data = (utf32_t *) malloc(cap * sizeof(utf32_t));

    if (data == NULL) return -1;

    memcpy(data, string->data, string->len * sizeof(utf32_t));
  } else {
    data = (utf32_t *) realloc(string->data, cap * sizeof(utf32_t));

    if (data == NULL) return -1;
  }

  string->data = data;
  string->cap = cap;

  return 0;
}

inline int
utf32_string_shrink_to_fit(utf32_string_t *string) {
  if (string->data == string->buf) return 0;

  size_t cap = string->len;

  if (cap <= UTF32_STRING_INLINE_CAPACITY) {
    memcpy(string->buf, string->data, cap * sizeof(utf32_t));

    free(string->data);

    string->data = string->buf;
  } else {
    utf32_t *data = (utf32_t *) realloc(string->data, cap * sizeof(utf32_t));

    if (data == NULL) return -1;

    string->data = data;
    string->cap = cap;
  }

  return 0;
}

inline utf32_string_view_t
utf32_string_view(const utf32_string_t *string) {
  return utf32_string_view_init(string->data, string->len);
}

inline void
utf32_string_clear(utf32_string_t *string) {
  string->len = 0;
}

inline bool
utf32_string_empty(const utf32_string_t *string) {
  return string->len == 0;
}

inline bool
utf32_string_view_empty(const utf32_string_view_t view) {
  return view.len == 0;
}

inline int
utf32_string_copy(const utf32_string_t *string, utf32_string_t *result) {
  int err;

  err = utf32_string_reserve(result, string->len);
  if (err < 0) return err;

  memcpy(result->data, string->data, string->len * sizeof(utf32_t));

  result->len = string->len;

  return 0;
}

inline int
utf32_string_view_copy(const utf32_string_view_t view, utf32_string_t *result) {
  int err;

  err = utf32_string_reserve(result, view.len);
  if (err < 0) return err;

  memcpy(result->data, view.data, view.len * sizeof(utf32_t));

  result->len = view.len;

  return 0;
}

inline int
utf32_string_append(utf32_string_t *string, const utf32_string_t *other) {
  int err;

  err = utf32_string_reserve(string, string->len + other->len);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], other->data, other->len * sizeof(utf32_t));

  string->len += other->len;

  return 0;
}

inline int
utf32_string_append_view(utf32_string_t *string, const utf32_string_view_t view) {
  int err;

  err = utf32_string_reserve(string, string->len + view.len);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], view.data, view.len * sizeof(utf32_t));

  string->len += view.len;

  return 0;
}

inline int
utf32_string_append_character(utf32_string_t *string, utf32_t c) {
  int err;

  err = utf32_string_reserve(string, string->len + 1);
  if (err < 0) return -1;

  string->data[string->len] = c;

  string->len += 1;

  return 0;
}

inline int
utf32_string_append_literal(utf32_string_t *string, const utf32_t *literal, size_t n) {
  int err;

  if (n == (size_t) -1) n = utf32__string_literal_length(literal);

  err = utf32_string_reserve(string, string->len + n);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], literal, n * sizeof(utf32_t));

  string->len += n;

  return 0;
}

inline int
utf32_string_prepend(utf32_string_t *string, const utf32_string_t *other) {
  int err;

  err = utf32_string_reserve(string, string->len + other->len);
  if (err < 0) return -1;

  memmove(&string->data[other->len], string->data, string->len * sizeof(utf32_t));

  memcpy(string->data, other->data, other->len * sizeof(utf32_t));

  string->len += other->len;

  return 0;
}

inline int
utf32_string_prepend_view(utf32_string_t *string, const utf32_string_view_t view) {
  int err;

  err = utf32_string_reserve(string, string->len + view.len);
  if (err < 0) return -1;

  memmove(&string->data[view.len], string->data, string->len * sizeof(utf32_t));

  memcpy(string->data, view.data, view.len * sizeof(utf32_t));

  string->len += view.len;

  return 0;
}

inline int
utf32_string_prepend_character(utf32_string_t *string, utf32_t c) {
  int err;

  err = utf32_string_reserve(string, string->len + 1);
  if (err < 0) return -1;

  memmove(&string->data[1], string->data, string->len * sizeof(utf32_t));

  string->data[0] = c;

  string->len += 1;

  return 0;
}

inline int
utf32_string_prepend_literal(utf32_string_t *string, const utf32_t *literal, size_t n) {
  int err;

  if (n == (size_t) -1) n = utf32__string_literal_length(literal);

  err = utf32_string_reserve(string, string->len + n);
  if (err < 0) return -1;

  memmove(&string->data[n], string->data, string->len * sizeof(utf32_t));

  memcpy(string->data, literal, n * sizeof(utf32_t));

  string->len += n;

  return 0;
}

inline int
utf32_string_insert(utf32_string_t *string, size_t pos, const utf32_string_t *other) {
  int err;

  if (pos > string->len) return -1;

  size_t inserted_len = string->len + other->len;

  err = utf32_string_reserve(string, inserted_len);
  if (err < 0) return err;

  memmove(&string->data[pos + other->len], &string->data[pos], (string->len - pos) * sizeof(utf32_t));

  memcpy(&string->data[pos], other->data, other->len * sizeof(utf32_t));

  string->len = inserted_len;

  return 0;
}

inline int
utf32_string_insert_view(utf32_string_t *string, size_t pos, const utf32_string_view_t other) {
  int err;

  if (pos > string->len) return -1;

  size_t inserted_len = string->len + other.len;

  err = utf32_string_reserve(string, inserted_len);
  if (err < 0) return err;

  memmove(&string->data[pos + other.len], &string->data[pos], (string->len - pos) * sizeof(utf32_t));

  memcpy(&string->data[pos], other.data, other.len * sizeof(utf32_t));

  string->len = inserted_len;

  return 0;
}

inline int
utf32_string_insert_character(utf32_string_t *string, size_t pos, utf32_t c) {
  int err;

  if (pos > string->len) return -1;

  size_t inserted_len = string->len + 1;

  err = utf32_string_reserve(string, inserted_len);
  if (err < 0) return err;

  memmove(&string->data[pos + 1], &string->data[pos], (string->len - pos) * sizeof(utf32_t));

  string->data[pos] = c;

  string->len = inserted_len;

  return 0;
}

inline int
utf32_string_insert_literal(utf32_string_t *string, size_t pos, const utf32_t *literal, size_t n) {
  int err;

  if (pos > string->len) return -1;

  if (n == (size_t) -1) n = utf32__string_literal_length(literal);

  size_t inserted_len = string->len + n;

  err = utf32_string_reserve(string, inserted_len);
  if (err < 0) return err;

  memmove(&string->data[pos + n], &string->data[pos], (string->len - pos) * sizeof(utf32_t));

  memcpy(&string->data[pos], literal, n * sizeof(utf32_t));

  string->len = inserted_len;

  return 0;
}

inline int
utf32_string_replace(utf32_string_t *string, size_t pos, size_t len, const utf32_string_t *replacement) {
  int err;

  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  size_t replaced_len = string->len + replacement->len - len;

  err = utf32_string_reserve(string, replaced_len);
  if (err < 0) return err;

  memmove(&string->data[pos + replacement->len], &string->data[pos + len], (string->len - pos - len) * sizeof(utf32_t));

  memcpy(&string->data[pos], replacement->data, replacement->len * sizeof(utf32_t));

  string->len = replaced_len;

  return 0;
}

inline int
utf32_string_replace_view(utf32_string_t *string, size_t pos, size_t len, const utf32_string_view_t replacement) {
  int err;

  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  size_t replaced_len = string->len + replacement.len - len;

  err = utf32_string_reserve(string, replaced_len);
  if (err < 0) return err;

  memmove(&string->data[pos + replacement.len], &string->data[pos + len], (string->len - pos - len) * sizeof(utf32_t));

  memcpy(&string->data[pos], replacement.data, replacement.len * sizeof(utf32_t));

  string->len = replaced_len;

  return 0;
}

inline int
utf32_string_replace_character(utf32_string_t *string, size_t pos, size_t len, utf32_t c) {
  int err;

  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  size_t replaced_len = string->len + 1 - len;

  err = utf32_string_reserve(string, replaced_len);
  if (err < 0) return err;

  memmove(&string->data[pos + 1], &string->data[pos + len], (string->len - pos - len) * sizeof(utf32_t));

  string->data[pos] = c;

  string->len = replaced_len;

  return 0;
}

inline int
utf32_string_replace_literal(utf32_string_t *string, size_t pos, size_t len, const utf32_t *literal, size_t n) {
  int err;

  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  if (n == (size_t) -1) n = utf32__string_literal_length(literal);

  size_t replaced_len = string->len + n - len;

  err = utf32_string_reserve(string, replaced_len);
  if (err < 0) return err;

  memmove(&string->data[pos + n], &string->data[pos + len], (string->len - pos - len) * sizeof(utf32_t));

  memcpy(&string->data[pos], literal, n * sizeof(utf32_t));

  string->len = replaced_len;

  return 0;
}

inline int
utf32_string_erase(utf32_string_t *string, size_t pos, size_t len) {
  if (pos > string->len) return -1;
  if (len > string->len - pos) len = string->len - pos;

  memmove(&string->data[pos], &string->data[pos + len], (string->len - pos - len) * sizeof(utf32_t));

  string->len -= len;

  return 0;
}

inline int
utf32_string_concat(const utf32_string_t *string, const utf32_string_t *other, utf32_string_t *result) {
  int err;

  utf32_string_init(result);

  err = utf32_string_reserve(result, string->len + other->len);
  if (err < 0) return err;

  err = utf32_string_append(result, string);
  assert(err == 0);

  err = utf32_string_append(result, other);
  assert(err == 0);

  return 0;
}

inline int
utf32_string_view_concat(const utf32_string_view_t view, const utf32_string_t *other, utf32_string_t *result) {
  int err;

  utf32_string_init(result);

  err = utf32_string_reserve(result, view.len + other->len);
  if (err < 0) return err;

  err = utf32_string_append_view(result, view);
  assert(err == 0);

  err = utf32_string_append(result, other);
  assert(err == 0);

  return 0;
}

inline int
utf32_string_concat_view(const utf32_string_t *string, const utf32_string_view_t other, utf32_string_t *result) {
  int err;

  utf32_string_init(result);

  err = utf32_string_reserve(result, string->len + other.len);
  if (err < 0) return err;

  err = utf32_string_append(result, string);
  assert(err == 0);

  err = utf32_string_append_view(result, other);
  assert(err == 0);

  return 0;
}

inline int
utf32_string_view_concat_view(const utf32_string_view_t view, const utf32_string_view_t other, utf32_string_t *result) {
  int err;

  utf32_string_init(result);

  err = utf32_string_reserve(result, view.len + other.len);
  if (err < 0) return err;

  err = utf32_string_append_view(result, view);
  assert(err == 0);

  err = utf32_string_append_view(result, other);
  assert(err == 0);

  return 0;
}

inline int
utf32_string_concat_character(const utf32_string_t *string, utf32_t c, utf32_string_t *result) {
  int err;

  utf32_string_init(result);

  err = utf32_string_reserve(result, string->len + 1);
  if (err < 0) return err;

  err = utf32_string_append(result, string);
  assert(err == 0);

  err = utf32_string_append_character(result, c);
  assert(err == 0);

  return 0;
}

inline int
utf32_string_view_concat_character(const utf32_string_view_t view, utf32_t c, utf32_string_t *result) {
  int err;

  utf32_string_init(result);

  err = utf32_string_reserve(result, view.len + 1);
  if (err < 0) return err;

  err = utf32_string_append_view(result, view);
  assert(err == 0);

  err = utf32_string_append_character(result, c);
  assert(err == 0);

  return 0;
}

inline int
utf32_string_concat_literal(const utf32_string_t *string, const utf32_t *literal, size_t n, utf32_string_t *result) {
  int err;

  utf32_string_init(result);

  err = utf32_string_reserve(result, string->len + n);
  if (err < 0) return err;

  err = utf32_string_append(result, string);
  assert(err == 0);

  err = utf32_string_append_literal(result, literal, n);
  assert(err == 0);

  return 0;
}

inline int
utf32_string_view_concat_literal(const utf32_string_view_t view, const utf32_t *literal, size_t n, utf32_string_t *result) {
  int err;

  utf32_string_init(result);

  err = utf32_string_reserve(result, view.len + n);
  if (err < 0) return err;

  err = utf32_string_append_view(result, view);
  assert(err == 0);

  err = utf32_string_append_literal(result, literal, n);
  assert(err == 0);

  return 0;
}

/**
 * Compares the code points of `a` and `b`, of which the first `len` are
 * compared. This stands in for the `memcmp` that the UTF-8 string uses, which
 * would order the code points by their bytes rather than by their value.
 */
static inline int
utf32__string_compare(const utf32_t *a, const utf32_t *b, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
  }

  return 0;
}

inline int
utf32_string_compare(const utf32_string_t *string, const utf32_string_t *other) {
  size_t a_len = string->len;
  size_t b_len = other->len;

  // The shorter of the two lengths, over which the contents are compared. The
  // comparison is one of code points rather than of C strings, a length
  // delimited string being free to hold a null code point.
  size_t len = a_len < b_len ? a_len : b_len;

  int result = len == 0 ? 0 : utf32__string_compare(string->data, other->data, len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

inline int
utf32_string_view_compare(const utf32_string_view_t view, const utf32_string_view_t other) {
  size_t a_len = view.len;
  size_t b_len = other.len;

  // The shorter of the two lengths, over which the contents are compared. The
  // comparison is one of code points rather than of C strings, a length
  // delimited string being free to hold a null code point.
  size_t len = a_len < b_len ? a_len : b_len;

  int result = len == 0 ? 0 : utf32__string_compare(view.data, other.data, len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

inline int
utf32_string_compare_literal(const utf32_string_t *string, const utf32_t *literal, size_t n) {
  if (n == (size_t) -1) n = utf32__string_literal_length(literal);

  size_t a_len = string->len;
  size_t b_len = n;

  // The shorter of the two lengths, over which the contents are compared. The
  // comparison is one of code points rather than of C strings, a length
  // delimited string being free to hold a null code point.
  size_t len = a_len < b_len ? a_len : b_len;

  int result = len == 0 ? 0 : utf32__string_compare(string->data, literal, len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

inline int
utf32_string_view_compare_literal(const utf32_string_view_t view, const utf32_t *literal, size_t n) {
  if (n == (size_t) -1) n = utf32__string_literal_length(literal);

  size_t a_len = view.len;
  size_t b_len = n;

  // The shorter of the two lengths, over which the contents are compared. The
  // comparison is one of code points rather than of C strings, a length
  // delimited string being free to hold a null code point.
  size_t len = a_len < b_len ? a_len : b_len;

  int result = len == 0 ? 0 : utf32__string_compare(view.data, literal, len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

inline utf32_string_view_t
utf32_string_substring(const utf32_string_t *string, size_t start, size_t end) {
  if (end == (size_t) -1 || end > string->len) end = string->len;
  if (start > end) start = end;

  return utf32_string_view_init(&string->data[start], end - start);
}

inline utf32_string_view_t
utf32_string_view_substring(const utf32_string_view_t view, size_t start, size_t end) {
  if (end == (size_t) -1 || end > view.len) end = view.len;
  if (start > end) start = end;

  return utf32_string_view_init(&view.data[start], end - start);
}

inline int
utf32_string_substring_copy(const utf32_string_t *string, size_t start, size_t end, utf32_string_t *result) {
  int err;

  if (end == (size_t) -1 || end > string->len) end = string->len;
  if (start > end) start = end;

  size_t len = end - start;

  err = utf32_string_reserve(result, len);
  if (err < 0) return -1;

  memcpy(result->data, &string->data[start], len * sizeof(utf32_t));

  result->len = len;

  return 0;
}

inline int
utf32_string_view_substring_copy(const utf32_string_view_t view, size_t start, size_t end, utf32_string_t *result) {
  int err;

  if (end == (size_t) -1 || end > view.len) end = view.len;
  if (start > end) start = end;

  size_t len = end - start;

  err = utf32_string_reserve(result, len);
  if (err < 0) return -1;

  memcpy(result->data, &view.data[start], len * sizeof(utf32_t));

  result->len = len;

  return 0;
}

inline size_t
utf32_string_index_of_character(const utf32_string_t *string, size_t pos, utf32_t c) {
  // Also catches a pos of (size_t) -1, which must not be turned into a length.
  if (pos >= string->len) return (size_t) -1;

  for (size_t i = pos; i < string->len; i++) {
    if (string->data[i] == c) {
      return i;
    }
  }

  return (size_t) -1;
}

inline size_t
utf32_string_view_index_of_character(const utf32_string_view_t view, size_t pos, utf32_t c) {
  // Also catches a pos of (size_t) -1, which must not be turned into a length.
  if (pos >= view.len) return (size_t) -1;

  for (size_t i = pos; i < view.len; i++) {
    if (view.data[i] == c) {
      return i;
    }
  }

  return (size_t) -1;
}

inline size_t
utf32_string_last_index_of_character(const utf32_string_t *string, size_t pos, utf32_t c) {
  if (string->len == 0) return (size_t) -1;
  if (pos == (size_t) -1) pos = string->len - 1;
  else if (pos >= string->len) return (size_t) -1;

  for (size_t i = pos; i <= pos; i--) {
    if (string->data[i] == c) {
      return i;
    }
  }

  return (size_t) -1;
}

inline size_t
utf32_string_view_last_index_of_character(const utf32_string_view_t view, size_t pos, utf32_t c) {
  if (view.len == 0) return (size_t) -1;
  if (pos == (size_t) -1) pos = view.len - 1;
  else if (pos >= view.len) return (size_t) -1;

  for (size_t i = pos; i <= pos; i--) {
    if (view.data[i] == c) {
      return i;
    }
  }

  return (size_t) -1;
}

#ifdef __cplusplus
}
#endif

#endif // UTF_STRING_H
