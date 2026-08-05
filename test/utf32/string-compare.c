#include <assert.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  // Two empty strings are equal, and neither is compared over any code points at
  // all.
  {
    utf32_string_t a, b;
    utf32_string_init(&a);
    utf32_string_init(&b);

    assert(utf32_string_compare(&a, &b) == 0);

    static const utf32_t nothing[] = {0};

    utf32_string_view_t empty = utf32_string_view_init(nothing, 0);

    assert(utf32_string_view_compare(empty, empty) == 0);
    assert(utf32_string_compare_literal(&a, nothing, 0) == 0);
    assert(utf32_string_view_compare_literal(empty, nothing, 0) == 0);

    utf32_string_destroy(&a);
    utf32_string_destroy(&b);
  }

  // Ordering follows the code points, with the shorter of two strings that share
  // a prefix coming first.
  {
    static const utf32_t banana[] = {'b', 'a', 'n', 'a', 'n', 'a', 0};
    static const utf32_t banan[] = {'b', 'a', 'n', 'a', 'n', 0};
    static const utf32_t bananas[] = {'b', 'a', 'n', 'a', 'n', 'a', 's', 0};
    static const utf32_t apple[] = {'a', 'p', 'p', 'l', 'e', 0};
    static const utf32_t cherry[] = {'c', 'h', 'e', 'r', 'r', 'y', 0};

    utf32_string_t a;
    utf32_string_init(&a);

    e = utf32_string_append_literal(&a, banana, (size_t) -1);
    assert(e == 0);
    assert(a.len == 6);

    assert(utf32_string_compare_literal(&a, banana, (size_t) -1) == 0);
    assert(utf32_string_compare_literal(&a, banan, (size_t) -1) > 0);
    assert(utf32_string_compare_literal(&a, bananas, (size_t) -1) < 0);
    assert(utf32_string_compare_literal(&a, apple, (size_t) -1) > 0);
    assert(utf32_string_compare_literal(&a, cherry, (size_t) -1) < 0);

    utf32_string_destroy(&a);
  }

  // Ordering is by the value of a code point rather than by its bytes. A
  // `memcmp` would put U+0100 before U+00FF on a little endian platform, its
  // first byte being the lesser of the two.
  {
    static const utf32_t high[] = {0x100};
    static const utf32_t low[] = {0xff};

    utf32_string_view_t a = utf32_string_view_init(high, 1);
    utf32_string_view_t b = utf32_string_view_init(low, 1);

    assert(utf32_string_view_compare(a, b) > 0);
    assert(utf32_string_view_compare(b, a) < 0);
  }

  // The same holds above the basic multilingual plane, where a `memcmp` would
  // put U+10000 before U+FFFF.
  {
    static const utf32_t supplementary[] = {0x10000};
    static const utf32_t basic[] = {0xffff};

    utf32_string_view_t a = utf32_string_view_init(supplementary, 1);
    utf32_string_view_t b = utf32_string_view_init(basic, 1);

    assert(utf32_string_view_compare(a, b) > 0);
    assert(utf32_string_view_compare(b, a) < 0);
  }

  // A null code point is compared like any other, rather than ending the
  // comparison as it would were these C strings.
  {
    static const utf32_t abc[] = {'a', 0, 'b'};
    static const utf32_t acc[] = {'a', 0, 'c'};
    static const utf32_t ab[] = {'a', 0};

    utf32_string_view_t a = utf32_string_view_init(abc, 3);
    utf32_string_view_t b = utf32_string_view_init(acc, 3);
    utf32_string_view_t c = utf32_string_view_init(abc, 3);

    assert(utf32_string_view_compare(a, b) < 0);
    assert(utf32_string_view_compare(b, a) > 0);
    assert(utf32_string_view_compare(a, c) == 0);

    // The code points past a null one also tell apart strings of unequal length.
    utf32_string_view_t d = utf32_string_view_init(ab, 2);

    assert(utf32_string_view_compare(d, a) < 0);
    assert(utf32_string_view_compare(a, d) > 0);

    assert(utf32_string_view_compare_literal(a, abc, 3) == 0);
    assert(utf32_string_view_compare_literal(a, acc, 3) < 0);
  }
}
