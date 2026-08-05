#include <assert.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  // Empty string with pos = 0 or pos = (size_t) -1 must return not-found rather
  // than search over a length of SIZE_MAX.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    assert(utf8_string_index_of_character(&s, 0, 'a') == (size_t) -1);
    assert(utf8_string_index_of_character(&s, (size_t) -1, 'a') == (size_t) -1);

    utf8_string_view_t view = utf8_string_view_init((const utf8_t *) "", 0);
    assert(utf8_string_view_index_of_character(view, 0, 'a') == (size_t) -1);
    assert(utf8_string_view_index_of_character(view, (size_t) -1, 'a') == (size_t) -1);

    utf8_string_destroy(&s);
  }

  // Search from the start and find the first match.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "banana", -1);
    assert(e == 0);

    assert(utf8_string_index_of_character(&s, 0, 'b') == 0);
    assert(utf8_string_index_of_character(&s, 0, 'a') == 1);
    assert(utf8_string_index_of_character(&s, 0, 'n') == 2);

    // Searching from a later position skips the earlier matches.
    assert(utf8_string_index_of_character(&s, 1, 'a') == 1);
    assert(utf8_string_index_of_character(&s, 2, 'a') == 3);
    assert(utf8_string_index_of_character(&s, 4, 'a') == 5);
    assert(utf8_string_index_of_character(&s, 5, 'n') == (size_t) -1);

    utf8_string_view_t view = utf8_string_view(&s);

    assert(utf8_string_view_index_of_character(view, 0, 'b') == 0);
    assert(utf8_string_view_index_of_character(view, 2, 'a') == 3);
    assert(utf8_string_view_index_of_character(view, 5, 'n') == (size_t) -1);

    utf8_string_destroy(&s);
  }

  // A search with no match returns not-found.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    e = utf8_string_append_literal(&s, (utf8_t *) "abc", -1);
    assert(e == 0);

    assert(utf8_string_index_of_character(&s, 0, 'z') == (size_t) -1);

    // pos at or past the end returns not-found.
    assert(utf8_string_index_of_character(&s, 3, 'a') == (size_t) -1);
    assert(utf8_string_index_of_character(&s, 100, 'a') == (size_t) -1);

    utf8_string_destroy(&s);
  }

  // A null byte is a byte like any other, both to search for and to search past.
  {
    utf8_string_view_t view = utf8_string_view_init((const utf8_t *) "a\0b", 3);

    assert(utf8_string_view_index_of_character(view, 0, '\0') == 1);
    assert(utf8_string_view_index_of_character(view, 0, 'b') == 2);
  }

  // A search over a stretch long enough to be done more than a byte at a time
  // finds a match at either end of it.
  {
    utf8_string_t s;
    utf8_string_init(&s);

    for (size_t i = 0; i < 100; i++) {
      e = utf8_string_append_character(&s, 'a');
      assert(e == 0);
    }

    e = utf8_string_append_character(&s, 'z');
    assert(e == 0);

    assert(utf8_string_index_of_character(&s, 0, 'z') == 100);
    assert(utf8_string_index_of_character(&s, 1, 'a') == 1);
    assert(utf8_string_index_of_character(&s, 0, 'y') == (size_t) -1);

    utf8_string_destroy(&s);
  }
}
