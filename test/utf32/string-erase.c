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
  static const utf32_t he[] = {'h', 'e', 0};
  static const utf32_t h[] = {'h', 0};

  // Plain erase in the middle.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, hello_world, (size_t) -1);
    assert(e == 0);

    e = utf32_string_erase(&s, 5, 6);
    assert(e == 0);
    assert(s.len == 5);
    assert(utf32_string_compare_literal(&s, hello, (size_t) -1) == 0);

    utf32_string_destroy(&s);
  }

  // Erase with len > string->len - pos must clamp.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_erase(&s, 2, 100);
    assert(e == 0);
    assert(s.len == 2);
    assert(utf32_string_compare_literal(&s, he, (size_t) -1) == 0);

    utf32_string_destroy(&s);
  }

  // Erase with pos + len overflowing size_t must also clamp.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_erase(&s, 1, SIZE_MAX);
    assert(e == 0);
    assert(s.len == 1);
    assert(utf32_string_compare_literal(&s, h, (size_t) -1) == 0);

    utf32_string_destroy(&s);
  }

  // Erase with pos > string->len is an error.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, hello, (size_t) -1);
    assert(e == 0);

    e = utf32_string_erase(&s, 10, 1);
    assert(e == -1);
    assert(s.len == 5);

    utf32_string_destroy(&s);
  }
}
