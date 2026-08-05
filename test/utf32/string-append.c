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

  // A length of (size_t) -1 takes the length of a null terminated sequence of
  // code points, standing in for the `strlen` of the UTF-8 string.
  {
    utf32_string_t string;
    utf32_string_init(&string);

    e = utf32_string_append_literal(&string, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_append_literal(&string, world, (size_t) -1);
    assert(e == 0);

    assert(string.len == 11);
    assert(utf32_string_compare_literal(&string, expected, (size_t) -1) == 0);

    utf32_string_destroy(&string);
  }

  // Appending code point by code point grows past the inline buffer, keeping
  // everything that came before.
  {
    utf32_string_t string;
    utf32_string_init(&string);

    for (size_t i = 0; i < UTF32_STRING_INLINE_CAPACITY * 4; i++) {
      e = utf32_string_append_character(&string, (utf32_t) (0x10000 + i));
      assert(e == 0);
    }

    assert(string.len == UTF32_STRING_INLINE_CAPACITY * 4);
    assert(string.data != string.buf);

    for (size_t i = 0; i < string.len; i++) {
      assert(string.data[i] == 0x10000 + i);
    }

    utf32_string_destroy(&string);
  }

  // Appending a string and a view leaves the same result.
  {
    utf32_string_t a, b;
    utf32_string_init(&a);
    utf32_string_init(&b);

    e = utf32_string_append_literal(&a, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_append(&b, &a);
    assert(e == 0);

    e = utf32_string_append_view(&b, utf32_string_view_init(world, 6));
    assert(e == 0);

    assert(utf32_string_compare_literal(&b, expected, (size_t) -1) == 0);

    utf32_string_destroy(&a);
    utf32_string_destroy(&b);
  }
}
