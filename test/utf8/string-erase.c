#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  // Plain erase in the middle.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "hello world", -1);
    assert(e == 0);

    e = utf8_string_erase(&s, 5, 6);
    assert(e == 0);
    assert(s.len == 5);
    assert(memcmp(s.data, "hello", 5) == 0);

    utf8_string_destroy(&s);
  }

  // Erase with len > string->len - pos must clamp.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "hello", -1);
    assert(e == 0);

    e = utf8_string_erase(&s, 2, 100);
    assert(e == 0);
    assert(s.len == 2);
    assert(memcmp(s.data, "he", 2) == 0);

    utf8_string_destroy(&s);
  }

  // Erase with pos + len overflowing size_t must also clamp.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "hello", -1);
    assert(e == 0);

    e = utf8_string_erase(&s, 1, SIZE_MAX);
    assert(e == 0);
    assert(s.len == 1);
    assert(memcmp(s.data, "h", 1) == 0);

    utf8_string_destroy(&s);
  }

  // Erase with pos > string->len is an error.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "hello", -1);
    assert(e == 0);

    e = utf8_string_erase(&s, 10, 1);
    assert(e == -1);
    assert(s.len == 5);

    utf8_string_destroy(&s);
  }
}
