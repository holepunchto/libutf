#ifndef UTF_H
#define UTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "utf/endianness.h"

typedef uint_least8_t utf8_t;
typedef uint_least16_t utf16_t;

/**
 * UTF-8
 */

bool
utf8_validate (const utf8_t *data, size_t len);

size_t
utf8_length_from_utf16le (const utf16_t *data, size_t len);

size_t
utf8_length_from_utf16be (const utf16_t *data, size_t len);

size_t
utf8_convert_to_utf16le (const utf8_t *data, size_t len, utf16_t *result);

size_t
utf8_convert_to_utf16be (const utf8_t *data, size_t len, utf16_t *result);

/**
 * UTF-16
 */

bool
utf16le_validate (const utf16_t *data, size_t len);

bool
utf16be_validate (const utf16_t *data, size_t len);

size_t
utf16_length_from_utf8 (const utf8_t *data, size_t len);

size_t
utf16le_convert_to_utf8 (const utf16_t *data, size_t len, utf8_t *result);

size_t
utf16be_convert_to_utf8 (const utf16_t *data, size_t len, utf8_t *result);

#ifdef __cplusplus
}
#endif

#endif // UTF_H
