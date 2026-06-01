#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/utf.h"

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  size_t units = size / sizeof(utf16_t);
  const utf16_t *input = (const utf16_t *) data;

  // Each UTF-16 unit maps to at most one Latin-1 byte.
  latin1_t *latin1 = units > 0 ? malloc(units) : NULL;

  size_t latin1_len = utf16le_convert_to_latin1(input, units, latin1);

  assert(latin1_len <= units);

  if (latin1_len > 0) {
    // Each Latin-1 byte maps to one UTF-16 unit.
    utf16_t *utf16 = malloc(latin1_len * sizeof(utf16_t));

    size_t utf16_len = latin1_convert_to_utf16le(latin1, latin1_len, utf16);

    assert(utf16_len == units);
    assert(memcmp(utf16, input, units * sizeof(utf16_t)) == 0);

    free(utf16);
  }

  free(latin1);

  return 0;
}
