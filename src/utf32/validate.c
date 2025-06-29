#include <stdbool.h>
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

bool
utf32_validate(const utf32_t *data, size_t len) {
  uint64_t pos = 0;
  uint32_t word;

  while (pos < len) {
    word = data[pos];
    if (word > 0x10ffff || (word >= 0xd800 && word <= 0xdfff)) {
      return false;
    }
    pos++;
  }

  return true;
}
