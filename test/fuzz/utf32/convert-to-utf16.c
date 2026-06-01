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

  // Each UTF-32 unit expands to at most 2 UTF-16 units (surrogate pair).
  utf16_t *utf16 = units > 0 ? malloc(units * 2 * sizeof(utf16_t)) : NULL;

  size_t utf16_len = utf32_convert_to_utf16le(input, units, utf16);

  assert(utf16_len <= units * 2);

  if (units > 0) {
    // UTF-32 unit count never exceeds UTF-16 unit count.
    utf32_t *utf32 = malloc(utf16_len * sizeof(utf32_t));

    size_t utf32_len = utf16le_convert_to_utf32(utf16, utf16_len, utf32);

    assert(utf32_len == units);
    assert(memcmp(utf32, input, units * sizeof(utf32_t)) == 0);

    free(utf32);
  }

  free(utf16);

  return 0;
}
