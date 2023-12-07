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

int
utf8_string_clear (utf8_string_t *string);

bool
utf8_string_empty (utf8_string_t *string);

int
utf8_string_append_character (utf8_string_t *string, utf8_t character);

int
utf8_string_append_characters (utf8_string_t *string, const utf8_t *characters, size_t len);

int
utf8_string_append_string (utf8_string_t *string, const utf8_string_t *other);

int
utf8_string_compare (utf8_string_t *string, utf8_string_t *other);

#ifdef __cplusplus
}
#endif

#endif // UTF_STRING_H