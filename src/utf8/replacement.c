#include <stddef.h>
#include <string.h>

#include "../../include/utf.h"

static const utf8_t replacement[3] = {0xef, 0xbf, 0xbd};

// Returns the width of the well formed sequence at the start of data, or 0 if
// there is none, in which case subpart holds the length of the maximal subpart:
// the longest prefix that could still begin a well formed sequence. One
// replacement stands for one maximal subpart, so a truncated three byte form
// costs one rather than two.
static size_t
utf8__sequence(const utf8_t *data, size_t len, size_t *subpart) {
  utf8_t lead = data[0];

  if (lead < 0x80) return 1;

  size_t width;
  utf8_t low, high;

  if (lead >= 0xc2 && lead <= 0xdf) {
    width = 2, low = 0x80, high = 0xbf;
  } else if (lead == 0xe0) {
    width = 3, low = 0xa0, high = 0xbf;
  } else if (lead == 0xed) {
    width = 3, low = 0x80, high = 0x9f;
  } else if (lead >= 0xe1 && lead <= 0xef) {
    width = 3, low = 0x80, high = 0xbf;
  } else if (lead == 0xf0) {
    width = 4, low = 0x90, high = 0xbf;
  } else if (lead == 0xf4) {
    width = 4, low = 0x80, high = 0x8f;
  } else if (lead >= 0xf1 && lead <= 0xf3) {
    width = 4, low = 0x80, high = 0xbf;
  } else {
    *subpart = 1;

    return 0;
  }

  for (size_t i = 1; i < width; i++) {
    if (i >= len || data[i] < (i == 1 ? low : 0x80) || data[i] > (i == 1 ? high : 0xbf)) {
      *subpart = i;

      return 0;
    }
  }

  return width;
}

size_t
utf8_length_from_utf8_with_replacement(const utf8_t *data, size_t len) {
  size_t counter = 0;

  for (size_t i = 0; i < len;) {
    size_t subpart;
    size_t width = utf8__sequence(&data[i], len - i, &subpart);

    if (width) {
      counter += width;
      i += width;
    } else {
      counter += sizeof(replacement);
      i += subpart;
    }
  }

  return counter;
}

size_t
utf8_convert_to_utf8_with_replacement(const utf8_t *data, size_t len, utf8_t *result) {
  utf8_t *start = result;

  for (size_t i = 0; i < len;) {
    size_t subpart;
    size_t width = utf8__sequence(&data[i], len - i, &subpart);

    if (width) {
      memcpy(result, &data[i], width);
      result += width;
      i += width;
    } else {
      memcpy(result, replacement, sizeof(replacement));
      result += sizeof(replacement);
      i += subpart;
    }
  }

  return (size_t) (result - start);
}
