#include <assert.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main() {
  int e;

  static const utf32_t banana[] = {'b', 'a', 'n', 'a', 'n', 'a', 0};
  static const utf32_t abc[] = {'a', 'b', 'c', 0};

  // Empty string with pos = 0 or pos = (size_t) -1 must return not-found rather
  // than search over a length of SIZE_MAX.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    assert(utf32_string_index_of_character(&s, 0, 'a') == (size_t) -1);
    assert(utf32_string_index_of_character(&s, (size_t) -1, 'a') == (size_t) -1);

    utf32_string_view_t view = utf32_string_view_init(banana, 0);
    assert(utf32_string_view_index_of_character(view, 0, 'a') == (size_t) -1);
    assert(utf32_string_view_index_of_character(view, (size_t) -1, 'a') == (size_t) -1);

    utf32_string_destroy(&s);
  }

  // Search from the start and find the first match.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, banana, (size_t) -1);
    assert(e == 0);

    assert(utf32_string_index_of_character(&s, 0, 'b') == 0);
    assert(utf32_string_index_of_character(&s, 0, 'a') == 1);
    assert(utf32_string_index_of_character(&s, 0, 'n') == 2);

    // Searching from a later position skips the earlier matches.
    assert(utf32_string_index_of_character(&s, 1, 'a') == 1);
    assert(utf32_string_index_of_character(&s, 2, 'a') == 3);
    assert(utf32_string_index_of_character(&s, 4, 'a') == 5);
    assert(utf32_string_index_of_character(&s, 5, 'n') == (size_t) -1);

    utf32_string_view_t view = utf32_string_view(&s);

    assert(utf32_string_view_index_of_character(view, 0, 'b') == 0);
    assert(utf32_string_view_index_of_character(view, 2, 'a') == 3);
    assert(utf32_string_view_index_of_character(view, 5, 'n') == (size_t) -1);

    utf32_string_destroy(&s);
  }

  // A search with no match returns not-found.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_literal(&s, abc, (size_t) -1);
    assert(e == 0);

    assert(utf32_string_index_of_character(&s, 0, 'z') == (size_t) -1);

    // pos at or past the end returns not-found.
    assert(utf32_string_index_of_character(&s, 3, 'a') == (size_t) -1);
    assert(utf32_string_index_of_character(&s, 100, 'a') == (size_t) -1);

    utf32_string_destroy(&s);
  }

  // A null code point is one like any other, both to search for and to search
  // past.
  {
    static const utf32_t with_null[] = {'a', 0, 'b'};

    utf32_string_view_t view = utf32_string_view_init(with_null, 3);

    assert(utf32_string_view_index_of_character(view, 0, 0) == 1);
    assert(utf32_string_view_index_of_character(view, 0, 'b') == 2);
  }

  // A code point outside the basic multilingual plane is found by its value, not
  // by any single byte of it.
  {
    utf32_string_t s;
    utf32_string_init(&s);

    e = utf32_string_append_character(&s, 0x10348);
    assert(e == 0);

    e = utf32_string_append_character(&s, 0x48);
    assert(e == 0);

    assert(utf32_string_index_of_character(&s, 0, 0x10348) == 0);
    assert(utf32_string_index_of_character(&s, 0, 0x48) == 1);

    utf32_string_destroy(&s);
  }
}
