#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/utf.h"

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Each Latin-1 byte maps to one UTF-16 unit.
  utf16_t *utf16 = size > 0 ? malloc(size * sizeof(utf16_t)) : NULL;

  size_t utf16_len = latin1_convert_to_utf16le((const latin1_t *) data, size, utf16);

  assert(utf16_len == size);

  if (size > 0) {
    latin1_t *latin1 = malloc(size);

    size_t latin1_len = utf16le_convert_to_latin1(utf16, utf16_len, latin1);

    assert(latin1_len == size);
    assert(memcmp(latin1, data, size) == 0);

    free(latin1);
  }

  free(utf16);

  return 0;
}
