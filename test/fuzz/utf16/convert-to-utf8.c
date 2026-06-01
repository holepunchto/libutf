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

  if (!utf16le_validate(input, units)) return -1;

  // Each UTF-16 unit expands to at most 3 UTF-8 bytes.
  utf8_t *utf8 = units > 0 ? malloc(units * 3) : NULL;

  size_t utf8_len = utf16le_convert_to_utf8(input, units, utf8);

  assert(utf8_len <= units * 3);

  if (units > 0) {
    // UTF-16 unit count never exceeds UTF-8 byte count.
    utf16_t *utf16 = malloc(utf8_len * sizeof(utf16_t));

    size_t utf16_len = utf8_convert_to_utf16le(utf8, utf8_len, utf16);

    assert(utf16_len == units);
    assert(memcmp(utf16, input, units * sizeof(utf16_t)) == 0);

    free(utf16);
  }

  free(utf8);

  return 0;
}
