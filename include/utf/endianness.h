#ifndef UTF_ENDIANNESS_H
#define UTF_ENDIANNESS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum utf_endianness_e utf_endianness_t;

enum utf_endianness_e {
  utf_le,
  utf_be
};

inline utf_endianness_t
utf_endianness (void) {
  const union {
    uint8_t u8[2];
    uint16_t u16;
  } byte_order = {{1, 0}};

  return byte_order.u16 == 1 ? utf_le : utf_be;
}

inline bool
utf_is_le (void) {
  return utf_endianness() == utf_le;
}

inline bool
utf_is_be (void) {
  return utf_endianness() == utf_be;
}

#endif // UTF_ENDIANNESS_H