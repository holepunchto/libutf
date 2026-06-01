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

  if (!utf32_validate(input, units)) return -1;

  // Each UTF-32 unit expands to at most 4 UTF-8 bytes.
  utf8_t *utf8 = units > 0 ? malloc(units * 4) : NULL;

  size_t utf8_len = utf32_convert_to_utf8(input, units, utf8);

  assert(utf8_len <= units * 4);

  if (units > 0) {
    // UTF-32 unit count never exceeds UTF-8 byte count.
    utf32_t *utf32 = malloc(utf8_len * sizeof(utf32_t));

    size_t utf32_len = utf8_convert_to_utf32(utf8, utf8_len, utf32);

    assert(utf32_len == units);
    assert(memcmp(utf32, input, units * sizeof(utf32_t)) == 0);

    free(utf32);
  }

  free(utf8);

  return 0;
}
