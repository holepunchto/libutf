#include <assert.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  // Empty string with pos = (size_t) -1 must return not-found instead of
  // reading data[SIZE_MAX].
  {
    utf8_string_t s;
    utf8_string_init(&s);

    assert(utf8_string_last_index_of_character(&s, (size_t) -1, 'a') == (size_t) -1);

    utf8_string_view_t view = utf8_string_view_init((const utf8_t *) "", 0);
    assert(utf8_string_view_last_index_of_character(view, (size_t) -1, 'a') == (size_t) -1);

    utf8_string_destroy(&s);
  }

  // Search from the end and find a match.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "banana", -1);
    assert(e == 0);

    assert(utf8_string_last_index_of_character(&s, (size_t) -1, 'a') == 5);
    assert(utf8_string_last_index_of_character(&s, (size_t) -1, 'n') == 4);
    assert(utf8_string_last_index_of_character(&s, (size_t) -1, 'b') == 0);

    utf8_string_destroy(&s);
  }

  // Search with no match must terminate (loop must not run forever or read
  // before the buffer).
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "abc", -1);
    assert(e == 0);

    assert(utf8_string_last_index_of_character(&s, (size_t) -1, 'z') == (size_t) -1);
    assert(utf8_string_last_index_of_character(&s, 0, 'z') == (size_t) -1);

    utf8_string_destroy(&s);
  }

  // pos >= string->len returns not-found.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "abc", -1);
    assert(e == 0);

    assert(utf8_string_last_index_of_character(&s, 3, 'a') == (size_t) -1);
    assert(utf8_string_last_index_of_character(&s, 100, 'a') == (size_t) -1);

    utf8_string_destroy(&s);
  }
}
