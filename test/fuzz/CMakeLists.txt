add_fuzz_executables(
  latin1/convert-to-utf8.c
  latin1/convert-to-utf16.c
  latin1/convert-to-utf32.c
  utf8/convert-to-latin1.c
  utf8/convert-to-utf16.c
  utf8/convert-to-utf32.c
  utf8/replacement.c
  utf16/convert-to-latin1.c
  utf16/convert-to-utf8.c
  utf16/convert-to-utf32.c
  utf32/convert-to-latin1.c
  utf32/convert-to-utf8.c
  utf32/convert-to-utf16.c
  PREFIX fuzz_
  LINK
    utf::static
)
