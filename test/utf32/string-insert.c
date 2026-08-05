#include <assert.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  static const utf32_t hello[] = {'h', 'e', 'l', 'l', 'o', 0};

  // Inserting in the middle shifts the tail along.
  {
    static const utf32_t there[] = {' ', 't', 'h', 'e', 'r', 'e', 0};
    static const utf32_t expected[] = {'h', 'e', 'l', ' ', 't', 'h', 'e', 'r', 'e', 'l', 'o', 0};

    utf32_string_t string;
    utf32_string_init(&string);

    e = utf32_string_append_literal(&string, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_insert_literal(&string, 3, there, (size_t) -1);
    assert(e == 0);

    assert(utf32_string_compare_literal(&string, expected, (size_t) -1) == 0);

    utf32_string_destroy(&string);
  }

  // Inserting at either end is a prepend and an append respectively.
  {
    utf32_string_t string;
    utf32_string_init(&string);

    e = utf32_string_append_literal(&string, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_insert_character(&string, 0, '<');
    assert(e == 0);

    e = utf32_string_insert_character(&string, string.len, '>');
    assert(e == 0);

    assert(string.len == 7);
    assert(string.data[0] == '<');
    assert(string.data[6] == '>');

    utf32_string_destroy(&string);
  }

  // Inserting past the end is an error, leaving the string untouched.
  {
    utf32_string_t string;
    utf32_string_init(&string);

    e = utf32_string_append_literal(&string, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_insert_character(&string, 6, 'x');
    assert(e == -1);
    assert(string.len == 5);

    e = utf32_string_insert_character(&string, (size_t) -1, 'x');
    assert(e == -1);
    assert(string.len == 5);

    utf32_string_destroy(&string);
  }

  // Repeatedly inserting at a position grows the string past the inline buffer
  // without disturbing the order of what is already there.
  {
    utf32_string_t string;
    utf32_string_init(&string);

    for (size_t i = 0; i < UTF32_STRING_INLINE_CAPACITY * 2; i++) {
      e = utf32_string_insert_character(&string, i, (utf32_t) (0x30000 + i));
      assert(e == 0);
    }

    assert(string.len == UTF32_STRING_INLINE_CAPACITY * 2);
    assert(string.data != string.buf);

    for (size_t i = 0; i < string.len; i++) {
      assert(string.data[i] == 0x30000 + i);
    }

    utf32_string_destroy(&string);
  }
}
