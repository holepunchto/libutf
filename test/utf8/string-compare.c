#include <assert.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  // Two empty strings are equal, and neither is compared over any bytes at all.
  {
    utf8_string_t a, b;
    utf8_string_init(&a);
    utf8_string_init(&b);

    assert(utf8_string_compare(&a, &b) == 0);

    utf8_string_view_t empty = utf8_string_view_init((const utf8_t *) "", 0);

    assert(utf8_string_view_compare(empty, empty) == 0);
    assert(utf8_string_compare_literal(&a, (utf8_t *) "", 0) == 0);
    assert(utf8_string_view_compare_literal(empty, (utf8_t *) "", 0) == 0);

    utf8_string_destroy(&a);
    utf8_string_destroy(&b);
  }

  // Ordering follows the bytes, with the shorter of two strings that share a
  // prefix coming first.
  {
    utf8_string_t a;
    utf8_string_init(&a);

    e = utf8_string_append_literal(&a, (utf8_t *) "banana", -1);
    assert(e == 0);

    assert(utf8_string_compare_literal(&a, (utf8_t *) "banana", -1) == 0);
    assert(utf8_string_compare_literal(&a, (utf8_t *) "banan", -1) > 0);
    assert(utf8_string_compare_literal(&a, (utf8_t *) "bananas", -1) < 0);
    assert(utf8_string_compare_literal(&a, (utf8_t *) "apple", -1) > 0);
    assert(utf8_string_compare_literal(&a, (utf8_t *) "cherry", -1) < 0);

    utf8_string_destroy(&a);
  }

  // A null byte is compared like any other byte, rather than ending the
  // comparison as it would were these C strings.
  {
    utf8_string_view_t a = utf8_string_view_init((const utf8_t *) "a\0b", 3);
    utf8_string_view_t b = utf8_string_view_init((const utf8_t *) "a\0c", 3);
    utf8_string_view_t c = utf8_string_view_init((const utf8_t *) "a\0b", 3);

    assert(utf8_string_view_compare(a, b) < 0);
    assert(utf8_string_view_compare(b, a) > 0);
    assert(utf8_string_view_compare(a, c) == 0);

    // The bytes past a null byte also tell apart strings of unequal length.
    utf8_string_view_t d = utf8_string_view_init((const utf8_t *) "a\0", 2);

    assert(utf8_string_view_compare(d, a) < 0);
    assert(utf8_string_view_compare(a, d) > 0);

    assert(utf8_string_view_compare_literal(a, (utf8_t *) "a\0b", 3) == 0);
    assert(utf8_string_view_compare_literal(a, (utf8_t *) "a\0c", 3) < 0);
  }
}
