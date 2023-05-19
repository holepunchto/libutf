#include <assert.h>
#include <string.h>

#include "../../include/utf.h"

#define test_validate_le(string) \
  { \
    size_t string_len = strlen(string) / sizeof(utf16_t); \
    assert(utf16le_validate((utf16_t *) string, string_len)); \
  }

#define test_validate_be(string) \
  { \
    size_t string_len = strlen(string) / sizeof(utf16_t); \
    assert(utf16be_validate((utf16_t *) string, string_len)); \
  }

int
main () {
  // Single ASCII character, 'a'
  test_validate_le("\x61\x00");

  // String of ASCII characters, "Hello"
  test_validate_le("\x48\x00\x65\x00\x6c\x00\x6c\x00\x6f\x00");

  // Single 2-byte UTF-16LE character, 'è'
  test_validate_le("\xe8\x00");

  // String of 2-byte UTF-16LE characters, "èéêë"
  test_validate_le("\xe8\x00\xe9\x00\xea\x00\xeb\x00");

  // Single 4-byte UTF-16LE character (represented as a surrogate pair), '𐐷'
  test_validate_le("\x01\xd8\x37\xdc");

  // String of 4-byte UTF-16LE characters (represented as surrogate pairs), "𐐷𐑏"
  test_validate_le("\x01\xd8\x37\xdc\x3f\xd8\x1f\xdc");

  // A mix of 2-byte and 4-byte UTF-16LE characters, "Hello𐐷"
  test_validate_le("\x48\x00\x65\x00\x6c\x00\x6c\x00\x6f\x00\x01\xd8\x37\xdc");

  // Single ASCII character, 'a'
  test_validate_be("\x00\x61");

  // String of ASCII characters, "Hello"
  test_validate_be("\x00\x48\x00\x65\x00\x6c\x00\x6c\x00\x6f");

  // Single 2-byte UTF-16BE character, 'è'
  test_validate_be("\x00\xe8");

  // String of 2-byte UTF-16BE characters, "èéêë"
  test_validate_be("\x00\xe8\x00\xe9\x00\xea\x00\xeb");

  // Single 4-byte UTF-16BE character (represented as a surrogate pair), '𐐷'
  test_validate_be("\xd8\x01\xdc\x37");

  // String of 4-byte UTF-16BE characters (represented as surrogate pairs), "𐐷𐑏"
  test_validate_be("\xd8\x01\xdc\x37\xd8\x3f\xdc\x1f");

  // A mix of 2-byte and 4-byte UTF-16BE characters, "Hello𐐷"
  test_validate_be("\x00\x48\x00\x65\x00\x6c\x00\x6c\x00\x6f\xd8\x01\xdc\x37");
}
