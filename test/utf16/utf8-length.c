#include <assert.h>

#include "../../include/utf.h"

#define test_length(string, len, expected) \
  { \
    assert(utf8_length_from_utf16le((utf16_t *) string, len) == expected); \
  }

int
main() {
  // ASCII character 'A'
  test_length("\x41\x00", 1, 1);

  // Greek capital letter 'Ω'
  test_length("\xa9\x03", 1, 2);

  // Snowman symbol '☃'
  test_length("\x03\x26", 1, 3);

  // Chinese character '中'
  test_length("\x2d\x4e", 1, 3);

  // Face with tears of joy emoji '😂'
  test_length("\x3d\xd8\x02\xde", 2, 4);

  // Multiple characters 'AΩ☃中😂'
  test_length("\x41\x00\xa9\x03\x03\x26\x2d\x4e\x3d\xd8\x02\xde", 6, 13);

  // Lone high surrogate followed by ASCII, surrogate contributes 0
  test_length("\x00\xd8\x41\x00", 2, 1);

  // Lone low surrogate followed by ASCII, surrogate contributes 0
  test_length("\x00\xdc\x41\x00", 2, 1);

  // Lone trailing high surrogate, contributes 0
  test_length("\x00\xd8", 1, 0);

  // High surrogate not followed by low surrogate, first contributes 0, pair contributes 4
  test_length("\x00\xd8\x00\xd8\x37\xdc", 3, 4);
}
