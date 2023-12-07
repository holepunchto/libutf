#include <assert.h>
#include <string.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

int
main () {
  int e;
  {
    utf8_string_t string;
    e = utf8_string_init(&string);
    assert(e == 0);

    e = utf8_string_append_characters(&string, (utf8_t *) "hello", -1);
    assert(e == 0);

    e = utf8_string_append_characters(&string, (utf8_t *) " world", -1);
    assert(e == 0);

    assert(memcmp(string.data, "hello world", string.len) == 0);

    utf8_string_destroy(&string);
  }
}
