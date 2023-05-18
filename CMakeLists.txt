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
  PRIVATE
    src/endianness.c
)

target_include_directories(
  utf
  PUBLIC
    include
)

add_library(utf_shared SHARED $<TARGET_OBJECTS:utf>)

set_target_properties(
  utf_shared
  PROPERTIES
  OUTPUT_NAME utf
  WINDOWS_EXPORT_ALL_SYMBOLS ON
)

add_library(utf_static STATIC $<TARGET_OBJECTS:utf>)

set_target_properties(
  utf_static
  PROPERTIES
  OUTPUT_NAME utf
  PREFIX lib
)

install(TARGETS utf_shared utf_static)

install(FILES include/utf.h DESTINATION include)

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()
  add_subdirectory(test)
endif()
