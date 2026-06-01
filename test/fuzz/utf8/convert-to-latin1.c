#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/utf.h"

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Latin-1 output is never longer than the UTF-8 input.
  latin1_t *latin1 = size > 0 ? malloc(size) : NULL;

  size_t latin1_len = utf8_convert_to_latin1((const utf8_t *) data, size, latin1);

  assert(latin1_len <= size);

  if (latin1_len > 0) {
    // Each Latin-1 byte expands to at most 2 UTF-8 bytes.
    utf8_t *utf8 = malloc(latin1_len * 2);

    size_t utf8_len = latin1_convert_to_utf8(latin1, latin1_len, utf8);

    assert(utf8_len == size);
    assert(memcmp(utf8, data, size) == 0);

    free(utf8);
  }

  free(latin1);

  return 0;
}
