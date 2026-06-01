#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  // Normal reserve grows past the inline buffer.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_reserve(&s, 64);
    assert(e == 0);
    assert(s.data != s.buf);
    assert(s.cap >= 64);

    utf8_string_destroy(&s);
  }

  // Reserve within the inline buffer is a no-op.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_reserve(&s, sizeof(s.buf));
    assert(e == 0);
    assert(s.data == s.buf);

    utf8_string_destroy(&s);
  }

  // Reserve must reject sizes that would overflow the power-of-two rounding,
  // rather than wrapping to cap = 0 and returning success.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_reserve(&s, SIZE_MAX);
    assert(e == -1);

    e = utf8_string_reserve(&s, (SIZE_MAX >> 1) + 2);
    assert(e == -1);

    utf8_string_destroy(&s);
  }
}
