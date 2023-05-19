#include <assert.h>
#include <string.h>

#include "../../include/utf.h"

#define test_validate(string) \
  { \
    size_t string_len = strlen(string) / sizeof(utf16_t); \
    assert(!utf16le_validate((utf16_t *) string, string_len)); \
  }

int
main () {
  // An isolated high surrogate
  test_validate("\x01\xd8");

  // An isolated low surrogate
  test_validate("\x37\xdc");

  // A high surrogate followed by another high surrogate
  test_validate("\x01\xd8\x02\xd8");

  // A low surrogate followed by another low surrogate
  test_validate("\x37\xdc\x38\xdc");

  // A high surrogate followed by a non-surrogate
  test_validate("\x01\xd8\xe8\x00");

  // A low surrogate followed by a non-surrogate
  test_validate("\x37\xdc\xe8\x00");

  // A non-surrogate followed by a low surrogate
  // test_validate("\xe8\x00\x37\xdc");

  // A non-surrogate followed by a high surrogate
  // test_validate("\xe8\x00\x01\xd8");

  // A surrogate pair in the wrong order
  test_validate("\x37\xdc\x01\xd8");

  // Two high surrogates followed by a low surrogate
  test_validate("\x01\xd8\x02\xd8\x37\xdc");

  // A high surrogate followed by two low surrogates
  test_validate("\x01\xd8\x37\xdc\x38\xdc");

  // Two high surrogates in a row
  test_validate("\x01\xd8\x02\xd8");

  // Two low surrogates in a row
  test_validate("\x37\xdc\x38\xdc");

  // A surrogate pair that decodes to an invalid Unicode character
  test_validate("\x46\xd8\x00\xdc");
}
