try_compile(
  supports_fuzzing
  SOURCE_FROM_CONTENT
    entry.c
      "
      #include <stddef.h>
      #include <stdint.h>

      int
      LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {}
      "
  LINK_OPTIONS -fsanitize=fuzzer
)

if(NOT supports_fuzzing)
  return()
endif()

option(FUZZ "Register fuzzers as tests" OFF)

set(FUZZ_DURATION 120 CACHE STRING "Seconds to run each fuzzer")

math(EXPR FUZZ_TIMEOUT "${FUZZ_DURATION} + 60")

list(APPEND fuzzers
  latin1/convert-to-utf8
  latin1/convert-to-utf16
  latin1/convert-to-utf32
  utf8/convert-to-latin1
  utf8/convert-to-utf16
  utf8/convert-to-utf32
  utf16/convert-to-latin1
  utf16/convert-to-utf8
  utf16/convert-to-utf32
  utf32/convert-to-latin1
  utf32/convert-to-utf8
  utf32/convert-to-utf16
)

foreach(fuzzer IN LISTS fuzzers)
  string(REPLACE "/" "_" name ${fuzzer})

  set(target fuzz_${name})

  add_executable(${target} ${fuzzer}.c)

  set_target_properties(
    ${target}
    PROPERTIES
    OUTPUT_NAME ${name}
  )

  target_link_libraries(
    ${target}
    PRIVATE
      utf_static
  )

  target_compile_options(
    ${target}
    PRIVATE
      -fsanitize=fuzzer
  )

  target_link_options(
    ${target}
    PRIVATE
      -fsanitize=fuzzer
  )

  if(FUZZ)
    set(corpus ${PROJECT_SOURCE_DIR}/test/fuzz/corpus/${name})
    set(crashes ${PROJECT_SOURCE_DIR}/test/fuzz/crashes/${name})

    file(MAKE_DIRECTORY ${corpus})
    file(MAKE_DIRECTORY ${crashes})

    add_test(
      NAME ${target}
      COMMAND ${target}
        ${corpus}
        -max_total_time=${FUZZ_DURATION}
        -artifact_prefix=${crashes}/
    )

    set_tests_properties(
      ${target}
      PROPERTIES
      LABELS fuzz
      TIMEOUT ${FUZZ_TIMEOUT}
    )
  endif()
endforeach()
