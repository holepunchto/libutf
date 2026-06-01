#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/utf.h"

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (!utf8_validate((const utf8_t *) data, size)) return -1;

  // UTF-16 unit count never exceeds UTF-8 byte count.
  utf16_t *utf16 = size > 0 ? malloc(size * sizeof(utf16_t)) : NULL;

  size_t utf16_len = utf8_convert_to_utf16le((const utf8_t *) data, size, utf16);

  assert(utf16_len <= size);

  if (size > 0) {
    // Each UTF-16 unit expands to at most 3 UTF-8 bytes.
    utf8_t *utf8 = malloc(utf16_len * 3);

    size_t utf8_len = utf16le_convert_to_utf8(utf16, utf16_len, utf8);

    assert(utf8_len == size);
    assert(memcmp(utf8, data, size) == 0);

    free(utf8);
  }

  free(utf16);

  return 0;
}
