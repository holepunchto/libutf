#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/utf.h"

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Each Latin-1 byte expands to at most 2 UTF-8 bytes.
  utf8_t *utf8 = size > 0 ? malloc(size * 2) : NULL;

  size_t utf8_len = latin1_convert_to_utf8((const latin1_t *) data, size, utf8);

  assert(utf8_len >= size);
  assert(utf8_len <= size * 2);

  if (size > 0) {
    latin1_t *latin1 = malloc(size);

    size_t latin1_len = utf8_convert_to_latin1(utf8, utf8_len, latin1);

    assert(latin1_len == size);
    assert(memcmp(latin1, data, size) == 0);

    free(latin1);
  }

  free(utf8);

  return 0;
}
