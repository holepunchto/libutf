#include <assert.h>
#include <string.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  utf8_string_t string;
  utf8_string_init(&string);

  e = utf8_string_append_literal(&string, (utf8_t *) "hello world", -1);
  assert(e == 0);

  utf8_string_view_t view = utf8_string_substring(&string, 2, 8);

  assert(memcmp(view.data, "llo worl", view.len) == 0);

  utf8_string_destroy(&string);

  // Copying a substring out gives a string holding just those bytes.
  {
    utf8_string_t s, copy;
    utf8_string_init(&s);
    utf8_string_init(&copy);

    e = utf8_string_append_literal(&s, (utf8_t *) "hello world", -1);
    assert(e == 0);

    e = utf8_string_substring_copy(&s, 6, (size_t) -1, &copy);
    assert(e == 0);
    assert(copy.len == 5);
    assert(memcmp(copy.data, "world", 5) == 0);

    utf8_string_destroy(&copy);
    utf8_string_destroy(&s);
  }

  // A copy into a string that has already outgrown its inline buffer reuses that
  // allocation rather than leaking it.
  {
    utf8_string_t s, copy;
    utf8_string_init(&s);
    utf8_string_init(&copy);

    e = utf8_string_append_literal(&s, (utf8_t *) "hello world", -1);
    assert(e == 0);

    e = utf8_string_reserve(&copy, sizeof(copy.buf) * 8);
    assert(e == 0);
    assert(copy.data != copy.buf);

    e = utf8_string_view_substring_copy(utf8_string_view(&s), 0, 5, &copy);
    assert(e == 0);
    assert(copy.len == 5);
    assert(memcmp(copy.data, "hello", 5) == 0);

    // Still on the allocation reserved above. Were the result initialized here
    // instead, this would have been dropped back to the inline buffer and the
    // allocation leaked.
    assert(copy.data != copy.buf);

    utf8_string_destroy(&copy);
    utf8_string_destroy(&s);
  }
}
