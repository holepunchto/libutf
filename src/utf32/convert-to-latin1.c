#include <stddef.h>
#include <stdint.h>

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
utf32_convert_to_latin1 (const utf32_t *data, size_t len, latin1_t *result) {
  size_t pos = 0;
  uint32_t word, overflow = 0;
  latin1_t *start = result;

  while (pos < len) {
    word = data[pos];
    overflow |= word;
    *result++ = word & 0xff;
    pos++;
  }

  if (overflow & 0xffffff00) return 0;

  return result - start;
}
