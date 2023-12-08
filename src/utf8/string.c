#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/utf/string.h"

int
utf8_string_init (utf8_string_t *string) {
  string->len = 0;
  string->cap = 0;
  string->data = NULL;

  return 0;
}

void
utf8_string_destroy (utf8_string_t *string) {
  free(string->data);
}

int
utf8_string_reserve (utf8_string_t *string, size_t len) {
  size_t cap = string->cap;

  if (len < cap) return 0;

  if (cap == 0) cap = sizeof(utf8_string_t) - 1;

  while (cap <= len) {
    cap = cap * 2 + 1;
  }

  void *data = realloc(string->data, cap);

  if (data == NULL) return -1;

  string->cap = cap;
  string->data = data;

  return 0;
}

int
utf8_string_shrink_to_fit (utf8_string_t *string) {
  size_t cap = string->len + 1;

  void *data = realloc(string->data, cap);

  if (data == NULL) return -1;

  string->cap = cap;
  string->data = data;

  return 0;
}

void
utf8_string_clear (utf8_string_t *string) {
  string->len = 0;
}

bool
utf8_string_empty (utf8_string_t *string) {
  return string->len == 0;
}

int
utf8_string_append (utf8_string_t *string, const utf8_string_t *other) {
  int err;

  err = utf8_string_reserve(string, string->len + other->len);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], other->data, other->len);

  string->len += other->len;

  string->data[string->len] = 0;

  return 0;
}

int
utf8_string_append_view (utf8_string_t *string, const utf8_string_view_t view) {
  return utf8_string_append_literal(string, view.data, view.len);
}

int
utf8_string_append_character (utf8_string_t *string, utf8_t character) {
  int err;

  err = utf8_string_reserve(string, string->len + 1);
  if (err < 0) return -1;

  string->data[string->len] = character;

  string->len += 1;

  string->data[string->len] = 0;

  return 0;
}

int
utf8_string_append_literal (utf8_string_t *string, const utf8_t *literal, size_t len) {
  int err;

  if (len == (size_t) -1) len = strlen((const char *) literal);

  err = utf8_string_reserve(string, string->len + len);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], literal, len);

  string->len += len;

  string->data[string->len] = 0;

  return 0;
}

int
utf8_string_prepend (utf8_string_t *string, const utf8_string_t *other) {
  int err;

  err = utf8_string_reserve(string, string->len + other->len);
  if (err < 0) return -1;

  memmove(&string->data[other->len], string->data, string->len);

  memcpy(string->data, other->data, other->len);

  string->len += other->len;

  string->data[string->len] = 0;

  return 0;
}

int
utf8_string_prepend_view (utf8_string_t *string, const utf8_string_view_t view) {
  return utf8_string_prepend_literal(string, view.data, view.len);
}

int
utf8_string_prepend_character (utf8_string_t *string, utf8_t character) {
  int err;

  err = utf8_string_reserve(string, string->len + 1);
  if (err < 0) return -1;

  memmove(&string->data[1], string->data, string->len);

  string->data[0] = character;

  string->len += 1;

  string->data[string->len] = 0;

  return 0;
}

int
utf8_string_prepend_literal (utf8_string_t *string, const utf8_t *literal, size_t len) {
  int err;

  if (len == (size_t) -1) len = strlen((const char *) literal);

  err = utf8_string_reserve(string, string->len + len);
  if (err < 0) return -1;

  memmove(&string->data[len], string->data, string->len);

  memcpy(string->data, literal, len);

  string->len += len;

  string->data[string->len] = 0;

  return 0;
}

int
utf8_string_compare (const utf8_string_t *string, const utf8_string_t *other) {
  size_t a_len = string->len;
  size_t b_len = other->len;

  int result = strncmp((const char *) string->data, (const char *) other->data, a_len < b_len ? a_len : b_len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

int
utf8_string_view_compare (const utf8_string_view_t view, const utf8_string_view_t other) {
  size_t a_len = view.len;
  size_t b_len = other.len;

  int result = strncmp((const char *) view.data, (const char *) other.data, a_len < b_len ? a_len : b_len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

int
utf8_string_compare_literal (const utf8_string_t *string, const char *literal, size_t len) {
  if (len == (size_t) -1) len = strlen((const char *) literal);

  size_t a_len = string->len;
  size_t b_len = len;

  int result = strncmp((const char *) string->data, literal, a_len < b_len ? a_len : b_len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

int
utf8_string_view_compare_literal (const utf8_string_view_t view, const char *literal, size_t len) {
  if (len == (size_t) -1) len = strlen((const char *) literal);

  size_t a_len = view.len;
  size_t b_len = len;

  int result = strncmp((const char *) view.data, literal, a_len < b_len ? a_len : b_len);

  if (result == 0) return a_len < b_len
                            ? -1
                          : a_len > b_len ? 1
                                          : 0;

  return result;
}

utf8_string_view_t
utf8_string_substring (const utf8_string_t *string, size_t start, size_t end) {
  if (end > string->len) end = string->len;
  if (start > end) start = end;

  return (utf8_string_view_t){
    .data = &string->data[start],
    .len = end - start,
  };
}

utf8_string_view_t
utf8_string_view_substring (const utf8_string_view_t view, size_t start, size_t end) {
  if (end > view.len) end = view.len;
  if (start > end) start = end;

  return (utf8_string_view_t){
    .data = &view.data[start],
    .len = end - start,
  };
}

int
utf8_string_substring_copy (const utf8_string_t *string, size_t start, size_t end, utf8_string_t *result) {
  int err;

  if (end > string->len) end = string->len;
  if (start > end) start = end;

  size_t len = end - start;

  err = utf8_string_reserve(result, len);
  if (err < 0) return -1;

  memcpy(result->data, &string->data[start], len);

  result->len = len;

  return 0;
}

int
utf8_string_view_substring_copy (const utf8_string_view_t view, size_t start, size_t end, utf8_string_t *result) {
  int err;

  if (end > view.len) end = view.len;
  if (start > end) start = end;

  size_t len = end - start;

  err = utf8_string_reserve(result, len);
  if (err < 0) return -1;

  memcpy(result->data, &view.data[start], len);

  result->len = len;

  return 0;
}
