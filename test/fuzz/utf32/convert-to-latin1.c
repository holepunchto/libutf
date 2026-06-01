#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/utf.h"

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  size_t units = size / sizeof(utf32_t);
  const utf32_t *input = (const utf32_t *) data;

  // Each UTF-32 unit maps to at most one Latin-1 byte.
  latin1_t *latin1 = units > 0 ? malloc(units) : NULL;

  size_t latin1_len = utf32_convert_to_latin1(input, units, latin1);

  assert(latin1_len <= units);

  if (latin1_len > 0) {
    // Each Latin-1 byte maps to one UTF-32 unit.
    utf32_t *utf32 = malloc(latin1_len * sizeof(utf32_t));

    size_t utf32_len = latin1_convert_to_utf32(latin1, latin1_len, utf32);

    assert(utf32_len == units);
    assert(memcmp(utf32, input, units * sizeof(utf32_t)) == 0);

    free(utf32);
  }

  free(latin1);

  return 0;
}
