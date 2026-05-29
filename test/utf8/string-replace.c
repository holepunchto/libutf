#include <assert.h>
#include <stddef.h>
#include <stdint.h>
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

  e = utf8_string_replace_literal(&string, 5, 1, (utf8_t *) " there ", -1);
  assert(e == 0);

  assert(memcmp(string.data, "hello there world", string.len) == 0);

  utf8_string_destroy(&string);

  // replace_character must clamp len to (string->len - pos) so memmove doesn't
  // underflow size_t.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "hello", -1);
    assert(e == 0);

    e = utf8_string_replace_character(&s, 2, 100, 'X');
    assert(e == 0);
    assert(s.len == 3);
    assert(memcmp(s.data, "heX", 3) == 0);

    utf8_string_destroy(&s);
  }

  // replace_character with pos + len overflowing size_t must also clamp.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "hello", -1);
    assert(e == 0);

    e = utf8_string_replace_character(&s, 1, SIZE_MAX, 'Y');
    assert(e == 0);
    assert(s.len == 2);
    assert(memcmp(s.data, "hY", 2) == 0);

    utf8_string_destroy(&s);
  }

  // replace_literal with pos + len overflowing size_t must also clamp.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "hello", -1);
    assert(e == 0);

    e = utf8_string_replace_literal(&s, 1, SIZE_MAX, (utf8_t *) "i!", 2);
    assert(e == 0);
    assert(s.len == 3);
    assert(memcmp(s.data, "hi!", 3) == 0);

    utf8_string_destroy(&s);
  }
}
