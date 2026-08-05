#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  static const utf32_t hello_world[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0};
  static const utf32_t hello[] = {'h', 'e', 'l', 'l', 'o', 0};

  {
    static const utf32_t there[] = {' ', 't', 'h', 'e', 'r', 'e', ' ', 0};
    static const utf32_t expected[] = {'h', 'e', 'l', 'l', 'o', ' ', 't', 'h', 'e', 'r', 'e', ' ', 'w', 'o', 'r', 'l', 'd', 0};

    utf32_string_t string;
    utf32_string_init(&string);

    e = utf32_string_append_literal(&string, hello_world, (size_t) -1);
    assert(e == 0);

    e = utf32_string_replace_literal(&string, 5, 1, there, (size_t) -1);
    assert(e == 0);

    assert(utf32_string_compare_literal(&string, expected, (size_t) -1) == 0);

    utf32_string_destroy(&string);
  }

  // replace_character must clamp len to (string->len - pos) so memmove doesn't
  // underflow size_t.
  {
    static const utf32_t expected[] = {'h', 'e', 'X', 0};

    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_replace_character(&s, 2, 100, 'X');
    assert(e == 0);
    assert(s.len == 3);
    assert(utf32_string_compare_literal(&s, expected, (size_t) -1) == 0);

    utf32_string_destroy(&s);
  }

  // replace_character with pos + len overflowing size_t must also clamp.
  {
    static const utf32_t expected[] = {'h', 'Y', 0};

    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_replace_character(&s, 1, SIZE_MAX, 'Y');
    assert(e == 0);
    assert(s.len == 2);
    assert(utf32_string_compare_literal(&s, expected, (size_t) -1) == 0);

    utf32_string_destroy(&s);
  }

  // replace_literal with pos + len overflowing size_t must also clamp.
  {
    static const utf32_t replacement[] = {'i', '!', 0};
    static const utf32_t expected[] = {'h', 'i', '!', 0};

    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_replace_literal(&s, 1, SIZE_MAX, replacement, 2);
    assert(e == 0);
    assert(s.len == 3);
    assert(utf32_string_compare_literal(&s, expected, (size_t) -1) == 0);

    utf32_string_destroy(&s);
  }
}
