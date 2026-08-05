#include <assert.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  static const utf32_t banana[] = {'b', 'a', 'n', 'a', 'n', 'a', 0};
  static const utf32_t abc[] = {'a', 'b', 'c', 0};

  // Empty string with pos = (size_t) -1 must return not-found instead of
  // reading data[SIZE_MAX].
  {
    utf32_string_t s;
    utf32_string_init(&s);

    assert(utf32_string_last_index_of_character(&s, (size_t) -1, 'a') == (size_t) -1);

    utf32_string_view_t view = utf32_string_view_init(banana, 0);
    assert(utf32_string_view_last_index_of_character(view, (size_t) -1, 'a') == (size_t) -1);

    utf32_string_destroy(&s);
  }

  // Search from the end and find a match.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, banana, (size_t) -1);
    assert(e == 0);

    assert(utf32_string_last_index_of_character(&s, (size_t) -1, 'a') == 5);
    assert(utf32_string_last_index_of_character(&s, (size_t) -1, 'n') == 4);
    assert(utf32_string_last_index_of_character(&s, (size_t) -1, 'b') == 0);

    utf32_string_destroy(&s);
  }

  // Search with no match must terminate (loop must not run forever or read
  // before the buffer).
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, abc, (size_t) -1);
    assert(e == 0);

    assert(utf32_string_last_index_of_character(&s, (size_t) -1, 'z') == (size_t) -1);
    assert(utf32_string_last_index_of_character(&s, 0, 'z') == (size_t) -1);

    utf32_string_destroy(&s);
  }

  // pos >= string->len returns not-found.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, abc, (size_t) -1);
    assert(e == 0);

    assert(utf32_string_last_index_of_character(&s, 3, 'a') == (size_t) -1);
    assert(utf32_string_last_index_of_character(&s, 100, 'a') == (size_t) -1);

    utf32_string_destroy(&s);
  }
}
