#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/utf.h"

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (!utf8_validate((const utf8_t *) data, size)) return -1;

  // UTF-32 unit count never exceeds UTF-8 byte count.
  utf32_t *utf32 = size > 0 ? malloc(size * sizeof(utf32_t)) : NULL;

  size_t utf32_len = utf8_convert_to_utf32((const utf8_t *) data, size, utf32);

  assert(utf32_len <= size);

  if (size > 0) {
    // Each UTF-32 unit expands to at most 4 UTF-8 bytes.
    utf8_t *utf8 = malloc(utf32_len * 4);

    size_t utf8_len = utf32_convert_to_utf8(utf32, utf32_len, utf8);

    assert(utf8_len == size);
    assert(memcmp(utf8, data, size) == 0);

    free(utf8);
  }

  free(utf32);

  return 0;
}
