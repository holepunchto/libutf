#include <assert.h>
#include <string.h>

#include <utf.h>

#define test_replacement(string, len, expected, written) \
  { \
    utf8_t result[(len) * 3]; \
    assert(utf8_length_from_utf8_with_replacement((utf8_t *) string, len) == written); \
    assert(utf8_convert_to_utf8_with_replacement((utf8_t *) string, len, result) == written); \
    assert(memcmp(expected, result, written) == 0); \
  }

#define FFFD "\xef\xbf\xbd"

int
main() {
  // Well formed input passes through untouched
  test_replacement("abc", 3, "abc", 3);
  test_replacement("\xc3\xa9", 2, "\xc3\xa9", 2);
  test_replacement("\xe2\x82\xac", 3, "\xe2\x82\xac", 3);
  test_replacement("\xf0\x9f\x98\x80", 4, "\xf0\x9f\x98\x80", 4);

  // A lead byte whose continuation never arrives is one maximal subpart
  test_replacement("\xc3\x28", 2, FFFD "\x28", 4);

  // An overlong form is ill formed at its lead byte, and the continuation that
  // follows is then stranded
  test_replacement("\xc0\xaf", 2, FFFD FFFD, 6);
  test_replacement("\xf0\x80\x80\x80", 4, FFFD FFFD FFFD FFFD, 12);

  // A surrogate has no UTF-8 form
  test_replacement("\xed\xa0\x80", 3, FFFD FFFD FFFD, 9);

  // Above the maximum code point
  test_replacement("\xf4\x90\x80\x80", 4, FFFD FFFD FFFD FFFD, 12);

  // Two bytes of a three byte form, completable, so one subpart covers both
  test_replacement("\xe1\x80\x61", 3, FFFD "a", 4);

  // Two bytes of a three byte form that could never complete, so the second
  // byte is a subpart of its own
  test_replacement("\xe0\x80\x41", 3, FFFD FFFD "A", 7);

  // Continuation bytes with no lead
  test_replacement("\x80\x80\x41", 3, FFFD FFFD "A", 7);

  // Truncated at the end of the input rather than by a bad byte
  test_replacement("\xe1\x80", 2, FFFD, 3);

  // Nothing at all
  assert(utf8_length_from_utf8_with_replacement((utf8_t *) "", 0) == 0);
}
