#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "../../include/utf.h"

/**
 * Modified from https://github.com/simdutf/simdutf
 *
 * Copyright 2020 The simdutf authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

size_t
utf8_convert_to_latin1(const utf8_t *data, size_t len, latin1_t *result) {
  size_t pos = 0;
  latin1_t *start = result;

  while (pos < len) {
    if (pos + 16 <= len) {
      uint64_t v1;
      memcpy(&v1, data + pos, sizeof(uint64_t));
      uint64_t v2;
      memcpy(&v2, data + pos + sizeof(uint64_t), sizeof(uint64_t));
      uint64_t v = v1 | v2;
      if ((v & 0x8080808080808080) == 0) {
        size_t final_pos = pos + 16;
        while (pos < final_pos) {
          *result++ = data[pos];
          pos++;
        }
        continue;
      }
    }
    uint8_t leading_byte = data[pos];
    if (leading_byte < 0b10000000) {
      *result++ = leading_byte;
      pos++;
    } else if ((leading_byte & 0b11100000) == 0b11000000) {
      if (pos + 1 >= len) {
        return 0;
      }
      if ((data[pos + 1] & 0b11000000) != 0b10000000) {
        return 0;
      }
      uint32_t code_point = (uint32_t) (((leading_byte & 0b00011111) << 6) |
                                        (data[pos + 1] & 0b00111111));
      if (code_point < 0x80 || code_point > 0xff) {
        return 0;
      }
      *result++ = (uint8_t) code_point;
      pos += 2;
    } else {
      return 0;
    }
  }

  return (size_t) (result - start);
}
