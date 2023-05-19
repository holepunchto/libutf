#include <assert.h>
#include <string.h>

#include "../../include/utf.h"

#define test_validate(string) \
  { \
    size_t string_len = strlen(string); \
    assert(utf8_validate((utf8_t *) string, string_len)); \
  }

int
main () {
  test_validate("a");                                                // Single ASCII character
  test_validate("Hello, World!");                                    // ASCII characters
  test_validate("\xc3\xa9");                                         // Single 2-byte UTF-8 character
  test_validate("\xc3\xa9\xc3\xa8\xc3\xaa\xc3\xab\xc3\xaf");         // String of 2-byte UTF-8 characters
  test_validate("\xe2\x82\xac");                                     // Single 3-byte UTF-8 character
  test_validate("\xe2\x82\xac\xe2\x89\xa1\xe2\x9c\x93");             // String of 3-byte UTF-8 characters
  test_validate("\xf0\x9f\x98\x80");                                 // Single 4-byte UTF-8 character
  test_validate("\xf0\x9f\x98\x80\xf0\x9f\x98\x81\xf0\x9f\x98\x82"); // String of 4-byte UTF-8 characters
  test_validate("a \xc3\xa9 \xe2\x82\xac \xf0\x9f\x98\x80");         // Mix of all
}
