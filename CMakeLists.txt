cmake_minimum_required(VERSION 3.25)

project(utf C)

add_library(utf OBJECT)

set_target_properties(
  utf
  PROPERTIES
  C_STANDARD 99
  POSITION_INDEPENDENT_CODE ON
)

target_sources(
  utf
  INTERFACE
    include/utf.h
    include/utf/endianness.h
    include/utf/string.h
  PRIVATE
    src/endianness.c
    src/utf8/utf16-convert.c
    src/utf8/utf16-length.c
    src/utf8/validate.c
    src/utf8/string.c
    src/utf16/utf8-convert.c
    src/utf16/utf8-length.c
    src/utf16/validate.c
)

target_include_directories(
  utf
  PUBLIC
    include
)

add_library(utf_shared SHARED)

set_target_properties(
  utf_shared
  PROPERTIES
  OUTPUT_NAME utf
  WINDOWS_EXPORT_ALL_SYMBOLS ON
)

target_link_libraries(
  utf_shared
  PUBLIC
    utf
)

add_library(utf_static STATIC)

set_target_properties(
  utf_static
  PROPERTIES
  OUTPUT_NAME utf
  PREFIX lib
)

target_link_libraries(
  utf_static
  PUBLIC
    utf
)

install(TARGETS utf_shared utf_static)

install(FILES include/utf.h DESTINATION include)

install(DIRECTORY include/utf DESTINATION include)

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()
  add_subdirectory(test)
endif()
