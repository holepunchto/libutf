#include <assert.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  static const utf32_t hello[] = {'h', 'e', 'l', 'l', 'o', 0};
  static const utf32_t world[] = {' ', 'w', 'o', 'r', 'l', 'd', 0};
  static const utf32_t expected[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0};

  {
    utf32_string_t string;
    utf32_string_init(&string);

    e = utf32_string_prepend_literal(&string, world, (size_t) -1);
    assert(e == 0);

    e = utf32_string_prepend_literal(&string, hello, (size_t) -1);
    assert(e == 0);

    assert(string.len == 11);
    assert(utf32_string_compare_literal(&string, expected, (size_t) -1) == 0);

    utf32_string_destroy(&string);
  }

  // Prepending a single code point shifts the rest along, including once the
  // string has outgrown the inline buffer.
  {
    utf32_string_t string;
    utf32_string_init(&string);

    for (size_t i = 0; i < UTF32_STRING_INLINE_CAPACITY * 2; i++) {
      e = utf32_string_prepend_character(&string, (utf32_t) (0x20000 + i));
      assert(e == 0);
    }

    assert(string.len == UTF32_STRING_INLINE_CAPACITY * 2);

    // The first code point prepended ended up last.
    for (size_t i = 0; i < string.len; i++) {
      assert(string.data[i] == 0x20000 + (string.len - 1 - i));
    }

    utf32_string_destroy(&string);
  }
}
