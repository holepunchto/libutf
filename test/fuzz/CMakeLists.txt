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

  add_fuzzer(fuzz_${name} ${fuzzer}.c)

  target_link_libraries(
    fuzz_${name}
    PRIVATE
      utf_static
  )
endforeach()
