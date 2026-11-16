cmake_minimum_required(VERSION 4.0)

find_package(cmake-harden REQUIRED PATHS node_modules/cmake-harden)
find_package(cmake-fuzz REQUIRED PATHS node_modules/cmake-fuzz)
find_package(cmake-prelude REQUIRED PATHS node_modules/cmake-prelude)
find_package(cmake-test REQUIRED PATHS node_modules/cmake-test)

project(utf LANGUAGES C VERSION 0.0.0)

add_library_targets(
  utf
  HEADERS
    include/utf.h
    include/utf/string.h
  FEATURES
    c_std_11
  SOURCES
    src/endianness.c
    src/endianness.h
    src/ascii/validate.c
    src/latin1/convert-to-utf8.c
    src/latin1/convert-to-utf16.c
    src/latin1/convert-to-utf32.c
    src/latin1/length-from-utf8.c
    src/latin1/length-from-utf16.c
    src/latin1/length-from-utf32.c
    src/utf8/convert-to-latin1.c
    src/utf8/convert-to-utf16.c
    src/utf8/convert-to-utf32.c
    src/utf8/length-from-latin1.c
    src/utf8/length-from-utf16.c
    src/utf8/length-from-utf32.c
    src/utf8/string.c
    src/utf8/validate.c
    src/utf16/convert-to-latin1.c
    src/utf16/convert-to-utf8.c
    src/utf16/convert-to-utf32.c
    src/utf16/length-from-latin1.c
    src/utf16/length-from-utf8.c
    src/utf16/length-from-utf32.c
    src/utf16/validate.c
    src/utf32/convert-to-latin1.c
    src/utf32/convert-to-utf8.c
    src/utf32/convert-to-utf16.c
    src/utf32/length-from-latin1.c
    src/utf32/length-from-utf8.c
    src/utf32/length-from-utf16.c
    src/utf32/string.c
    src/utf32/validate.c
)

harden(utf)

add_test_subdirectory(test)
