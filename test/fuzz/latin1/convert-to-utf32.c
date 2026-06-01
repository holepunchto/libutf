#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/utf.h"

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Each Latin-1 byte maps to one UTF-32 unit.
  utf32_t *utf32 = size > 0 ? malloc(size * sizeof(utf32_t)) : NULL;

  size_t utf32_len = latin1_convert_to_utf32((const latin1_t *) data, size, utf32);

  assert(utf32_len == size);

  if (size > 0) {
    latin1_t *latin1 = malloc(size);

    size_t latin1_len = utf32_convert_to_latin1(utf32, utf32_len, latin1);

    assert(latin1_len == size);
    assert(memcmp(latin1, data, size) == 0);

    free(latin1);
  }

  free(utf32);

  return 0;
}
