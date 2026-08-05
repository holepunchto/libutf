#include <assert.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  static const utf32_t hello_world[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0};

  {
    static const utf32_t expected[] = {'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 0};

    utf32_string_t string;
    utf32_string_init(&string);

    e = utf32_string_append_literal(&string, hello_world, (size_t) -1);
    assert(e == 0);

    utf32_string_view_t view = utf32_string_substring(&string, 2, 10);

    assert(view.len == 8);
    assert(utf32_string_view_compare_literal(view, expected, (size_t) -1) == 0);

    utf32_string_destroy(&string);
  }

  // An end of (size_t) -1, or one past the length, runs to the end of the
  // string, and a start past the end gives an empty view.
  {
    utf32_string_t string;
    utf32_string_init(&string);

    e = utf32_string_append_literal(&string, hello_world, (size_t) -1);
    assert(e == 0);

    assert(utf32_string_substring(&string, 6, (size_t) -1).len == 5);
    assert(utf32_string_substring(&string, 6, 100).len == 5);
    assert(utf32_string_substring(&string, 100, 100).len == 0);
    assert(utf32_string_substring(&string, 8, 4).len == 0);

    utf32_string_destroy(&string);
  }

  // Copying a substring out gives a string holding just those code points.
  {
    static const utf32_t expected[] = {'w', 'o', 'r', 'l', 'd', 0};

    utf32_string_t string, copy;
    utf32_string_init(&string);
    utf32_string_init(&copy);

    e = utf32_string_append_literal(&string, hello_world, (size_t) -1);
    assert(e == 0);

    e = utf32_string_substring_copy(&string, 6, (size_t) -1, &copy);
    assert(e == 0);
    assert(copy.len == 5);
    assert(utf32_string_compare_literal(&copy, expected, (size_t) -1) == 0);

    utf32_string_destroy(&copy);
    utf32_string_destroy(&string);
  }

  // A copy into a string that has already outgrown its inline buffer reuses that
  // allocation rather than leaking it.
  {
    utf32_string_t string, copy;
    utf32_string_init(&string);
    utf32_string_init(&copy);

    e = utf32_string_append_literal(&string, hello_world, (size_t) -1);
    assert(e == 0);

    e = utf32_string_reserve(&copy, UTF32_STRING_INLINE_CAPACITY * 8);
    assert(e == 0);
    assert(copy.data != copy.buf);

    e = utf32_string_view_substring_copy(utf32_string_view(&string), 0, 5, &copy);
    assert(e == 0);
    assert(copy.len == 5);

    // Still on the allocation reserved above. Were the result initialized here
    // instead, this would have been dropped back to the inline buffer and the
    // allocation leaked.
    assert(copy.data != copy.buf);

    utf32_string_destroy(&copy);
    utf32_string_destroy(&string);
  }
}
