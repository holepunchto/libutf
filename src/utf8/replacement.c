#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "../../include/utf.h"

static const utf8_t replacement[3] = {0xef, 0xbf, 0xbd};

// Reports whether data starts with a well formed sequence. If it does not,
// width is the length of its maximal subpart, which one replacement stands for,
// as in "U+FFFD Substitution of Maximal Subparts" of Unicode section 3.9.
static bool
utf8__sequence(const utf8_t *data, size_t len, size_t *width) {
  utf8_t lead = data[0];

  if (lead < 0x80) {
    *width = 1;

    return true;
  }

  size_t expected;
  utf8_t low, high;

  if (lead >= 0xc2 && lead <= 0xdf) {
    expected = 2, low = 0x80, high = 0xbf;
  } else if (lead == 0xe0) {
    expected = 3, low = 0xa0, high = 0xbf;
  } else if (lead == 0xed) {
    expected = 3, low = 0x80, high = 0x9f;
  } else if (lead >= 0xe1 && lead <= 0xef) {
    expected = 3, low = 0x80, high = 0xbf;
  } else if (lead == 0xf0) {
    expected = 4, low = 0x90, high = 0xbf;
  } else if (lead == 0xf4) {
    expected = 4, low = 0x80, high = 0x8f;
  } else if (lead >= 0xf1 && lead <= 0xf3) {
    expected = 4, low = 0x80, high = 0xbf;
  } else {
    *width = 1;

    return false;
  }

  for (size_t i = 1; i < expected; i++, low = 0x80, high = 0xbf) {
    if (i >= len || data[i] < low || data[i] > high) {
      *width = i;

      return false;
    }
  }

  *width = expected;

  return true;
}

size_t
utf8_length_with_replacement(const utf8_t *data, size_t len) {
  size_t counter = 0;

  for (size_t i = 0; i < len;) {
    size_t width;

    counter += utf8__sequence(&data[i], len - i, &width) ? width : sizeof(replacement);
    i += width;
  }

  return counter;
}

size_t
utf8_with_replacement(const utf8_t *data, size_t len, utf8_t *result) {
  utf8_t *start = result;

  for (size_t i = 0; i < len;) {
    size_t width;

    if (utf8__sequence(&data[i], len - i, &width)) {
      memcpy(result, &data[i], width);
      result += width;
    } else {
      memcpy(result, replacement, sizeof(replacement));
      result += sizeof(replacement);
    }

    i += width;
  }

  return (size_t) (result - start);
}
