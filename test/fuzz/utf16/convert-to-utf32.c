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

  // UTF-32 unit count never exceeds UTF-16 unit count.
  utf32_t *utf32 = units > 0 ? malloc(units * sizeof(utf32_t)) : NULL;

  size_t utf32_len = utf16le_convert_to_utf32(input, units, utf32);

  assert(utf32_len <= units);

  if (units > 0) {
    // Each UTF-32 unit expands to at most 2 UTF-16 units (surrogate pair).
    utf16_t *utf16 = malloc(utf32_len * 2 * sizeof(utf16_t));

    size_t utf16_len = utf32_convert_to_utf16le(utf32, utf32_len, utf16);

    assert(utf16_len == units);
    assert(memcmp(utf16, input, units * sizeof(utf16_t)) == 0);

    free(utf16);
  }

  free(utf32);

  return 0;
}
