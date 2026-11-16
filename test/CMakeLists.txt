if(TARGET utf_shared)
  set(library utf::shared)
else()
  set(library utf::static)
endif()

add_test_executables(
  utf8/latin1-convert.c
  utf8/utf16-convert.c
  utf8/utf16-length.c
  utf8/validate.c
  utf8/validate-invalid.c
  utf8/string-append.c
  utf8/string-compare.c
  utf8/string-erase.c
  utf8/string-index.c
  utf8/string-last-index.c
  utf8/string-prepend.c
  utf8/string-replace.c
  utf8/string-reserve.c
  utf8/string-substring.c
  utf16/utf8-convert.c
  utf16/utf8-length.c
  utf16/validate.c
  utf16/validate-invalid.c
  utf32/string-append.c
  utf32/string-compare.c
  utf32/string-erase.c
  utf32/string-index.c
  utf32/string-insert.c
  utf32/string-last-index.c
  utf32/string-prepend.c
  utf32/string-replace.c
  utf32/string-reserve.c
  utf32/string-substring.c
  LINK
    ${library}
)

add_subdirectory(fuzz)
