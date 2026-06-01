#include <assert.h>
#include <string.h>

#include "../../include/utf.h"

#define test_convert(string, len, expected, written) \
  { \
    utf8_t result[written]; \
    assert(utf16le_convert_to_utf8((utf16_t *) string, len, result) == written); \
    assert(memcmp(expected, result, written) == 0); \
  }

int
main() {
  // ASCII only
  test_convert("\x61\x00\x62\x00\x63\x00", 3, "abc", 3);

  // Greek alphabet
  test_convert("\xb1\x03\xb2\x03\xb3\x03", 3, "\xce\xb1\xce\xb2\xce\xb3", 6);

  // Chinese characters
  test_convert("\x4e\x67\xf3\x97\x2d\x4e", 3, "\xe6\x9d\x8e\xe9\x9f\xb3\xe4\xb8\xad", 9);

  // Mix of ASCII, Greek and Chinese
  test_convert("\x61\x00\xb1\x03\x4e\x67", 3, "a\xce\xb1\xe6\x9d\x8e", 6);

  // Surrogate pair
  test_convert("\x00\xd8\x37\xdc", 2, "\xf0\x90\x80\xb7", 4);

  // Lone high surrogate followed by ASCII, must skip the surrogate (not consume the ASCII)
  test_convert("\x00\xd8\x41\x00", 2, "\x41", 1);

  // Lone low surrogate followed by ASCII, must skip the surrogate
  test_convert("\x00\xdc\x41\x00", 2, "\x41", 1);

  // High surrogate followed by another high surrogate that begins a valid pair
  test_convert("\x00\xd8\x00\xd8\x37\xdc", 3, "\xf0\x90\x80\xb7", 4);

  // Lone trailing high surrogate (truncated), must not OOB-read
  {
    utf8_t result[1] = {0xaa};
    assert(utf16le_convert_to_utf8((utf16_t *) "\x00\xd8", 1, result) == 0);
    assert(result[0] == 0xaa);
  }
}
