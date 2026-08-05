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
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_reserve(&s, 512);
    assert(e == 0);
    assert(s.data != s.buf);
    assert(s.cap >= 512);

    utf32_string_destroy(&s);
  }

  // Reserve within the inline buffer is a no-op. The capacity counts code
  // points, so the whole of the buffer is available rather than a quarter of it.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_reserve(&s, UTF32_STRING_INLINE_CAPACITY);
    assert(e == 0);
    assert(s.data == s.buf);

    utf32_string_destroy(&s);
  }

  // One code point past the inline buffer does allocate.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_reserve(&s, UTF32_STRING_INLINE_CAPACITY + 1);
    assert(e == 0);
    assert(s.data != s.buf);

    utf32_string_destroy(&s);
  }

  // Reserve must reject sizes that would overflow the power-of-two rounding,
  // rather than wrapping to cap = 0 and returning success.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_reserve(&s, SIZE_MAX);
    assert(e == -1);

    e = utf32_string_reserve(&s, (SIZE_MAX >> 1) + 2);
    assert(e == -1);

    utf32_string_destroy(&s);
  }

  // Reserve must also reject a count of code points that survives the rounding
  // but overflows once scaled to the number of bytes that they occupy. This has
  // no counterpart for UTF-8, where a code unit is a single byte.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_reserve(&s, SIZE_MAX / sizeof(utf32_t));
    assert(e == -1);

    utf32_string_destroy(&s);
  }

  // Shrinking back to within the inline buffer returns to it, keeping the
  // contents.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_reserve(&s, 512);
    assert(e == 0);
    assert(s.data != s.buf);

    e = utf32_string_append_character(&s, 0x10348);
    assert(e == 0);

    e = utf32_string_shrink_to_fit(&s);
    assert(e == 0);
    assert(s.data == s.buf);
    assert(s.len == 1);
    assert(s.data[0] == 0x10348);

    utf32_string_destroy(&s);
  }
}
