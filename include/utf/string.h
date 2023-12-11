#ifndef UTF_STRING_H
#define UTF_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include "../utf.h"

/**
 * UTF-8
 */

typedef struct utf8_string_s utf8_string_t;
typedef struct utf8_string_view_s utf8_string_view_t;

struct utf8_string_s {
  utf8_t *data;
  size_t len;
  size_t cap;
};

struct utf8_string_view_s {
  const utf8_t *data;
  size_t len;
};

int
utf8_string_init (utf8_string_t *string);

void
utf8_string_destroy (utf8_string_t *string);

int
utf8_string_reserve (utf8_string_t *string, size_t len);

int
utf8_string_shrink_to_fit (utf8_string_t *string);

inline void
utf8_string_clear (utf8_string_t *string) {
  string->len = 0;
}

inline bool
utf8_string_empty (utf8_string_t *string) {
  return string->len == 0;
}

inline bool
utf8_string_view_empty (const utf8_string_view_t view) {
  return view.len == 0;
}

int
utf8_string_copy (const utf8_string_t *string, utf8_string_t *result);

int
utf8_string_view_copy (const utf8_string_view_t view, utf8_string_t *result);

int
utf8_string_append (utf8_string_t *string, const utf8_string_t *other);

int
utf8_string_append_view (utf8_string_t *string, const utf8_string_view_t view);

int
utf8_string_append_character (utf8_string_t *string, utf8_t c);

int
utf8_string_append_literal (utf8_string_t *string, const utf8_t *literal, size_t n);

int
utf8_string_prepend (utf8_string_t *string, const utf8_string_t *other);

int
utf8_string_prepend_view (utf8_string_t *string, const utf8_string_view_t view);

int
utf8_string_prepend_character (utf8_string_t *string, utf8_t c);

int
utf8_string_prepend_literal (utf8_string_t *string, const utf8_t *literal, size_t n);

int
utf8_string_replace (utf8_string_t *string, size_t pos, size_t len, const utf8_string_t *replacement);

int
utf8_string_replace_view (utf8_string_t *string, size_t pos, size_t len, const utf8_string_view_t replacement);

int
utf8_string_replace_character (utf8_string_t *string, size_t pos, size_t len, utf8_t c);

int
utf8_string_replace_literal (utf8_string_t *string, size_t pos, size_t len, const utf8_t *literal, size_t n);

int
utf8_string_compare (const utf8_string_t *string, const utf8_string_t *other);

int
utf8_string_view_compare (const utf8_string_view_t view, const utf8_string_view_t other);

int
utf8_string_compare_literal (const utf8_string_t *string, const utf8_t *literal, size_t n);

int
utf8_string_view_compare_literal (const utf8_string_view_t view, const utf8_t *literal, size_t n);

utf8_string_view_t
utf8_string_substring (const utf8_string_t *string, size_t start, size_t end);

utf8_string_view_t
utf8_string_view_substring (const utf8_string_view_t view, size_t start, size_t end);

int
utf8_string_substring_copy (const utf8_string_t *string, size_t start, size_t end, utf8_string_t *result);

int
utf8_string_view_substring_copy (const utf8_string_view_t view, size_t start, size_t end, utf8_string_t *result);

size_t
utf8_string_index_of_character (const utf8_string_t *string, utf8_t c, size_t pos);

size_t
utf8_string_view_index_of_character (const utf8_string_view_t view, utf8_t c, size_t pos);

size_t
utf8_string_last_index_of_character (const utf8_string_t *string, utf8_t c, size_t pos);

size_t
utf8_string_view_last_index_of_character (const utf8_string_view_t view, utf8_t c, size_t pos);

#ifdef __cplusplus
}
#endif

#endif // UTF_STRING_H
