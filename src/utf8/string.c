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

int
utf8_string_clear (utf8_string_t *string) {
  utf8_string_destroy(string);

  return utf8_string_init(string);
}

bool
utf8_string_empty (utf8_string_t *string) {
  return string->len == 0;
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
utf8_string_append_characters (utf8_string_t *string, const utf8_t *characters, size_t len) {
  int err;

  if (len == (size_t) -1) len = strlen((const char *) characters);

  err = utf8_string_reserve(string, string->len + len);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], characters, len);

  string->len += len;

  string->data[string->len] = 0;

  return 0;
}

int
utf8_string_append_string (utf8_string_t *string, const utf8_string_t *other) {
  int err;

  err = utf8_string_reserve(string, string->len + other->len);
  if (err < 0) return -1;

  memcpy(&string->data[string->len], other->data, other->len);

  string->len += other->len;

  string->data[string->len] = 0;

  return 0;
}

int
utf8_string_compare (const utf8_string_t *string, const utf8_string_t *other) {
  return strncmp((const char *) string->data, (const char *) other->data, string->len < other->len ? string->len : other->len);
}

int
utf8_string_view_compare (const utf8_string_view_t view, const utf8_string_view_t other) {
  return strncmp((const char *) view.data, (const char *) other.data, view.len < other.len ? view.len : other.len);
}

int
utf8_string_compare_literal (const utf8_string_t *string, const char *literal) {
  return strncmp((const char *) string->data, literal, string->len);
}

int
utf8_string_view_compare_literal (const utf8_string_view_t view, const char *literal) {
  return strncmp((const char *) view.data, literal, view.len);
}
