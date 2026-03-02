#include <assert.h>
#include <string.h>

#include "../../include/utf.h"

#define test_convert(string, len, expected, written) \
  { \
    latin1_t result[written]; \
    assert(utf8_convert_to_latin1((utf8_t *) string, len, result) == written); \
    assert(memcmp(expected, result, written) == 0); \
  }

int
main() {
  // ASCII character 'A'
  test_convert("\x41", 1, "\x41", 1);

  // ø Character
  test_convert("\xc3\xb8", 2, "\xf8", 1);

  // Ending with an invalid character 'AAAД'
  test_convert("\x41\x41\x41\xd0\x94", 4, "", 0);
}
