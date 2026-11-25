#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <utf.h>

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size == 0) return -1;

  // Each input byte expands to at most one 3 byte replacement.
  utf8_t *result = malloc(size * 3);

  size_t len = utf8_length_with_replacement((const utf8_t *) data, size);

  size_t written = utf8_with_replacement((const utf8_t *) data, size, result);

  assert(written == len);
  assert(utf8_validate(result, written));

  if (utf8_validate((const utf8_t *) data, size)) {
    assert(written == size);
    assert(memcmp(result, data, size) == 0);
  }

  free(result);

  return 0;
}
