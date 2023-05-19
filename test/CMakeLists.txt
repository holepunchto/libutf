list(APPEND tests
  utf8/utf16-convert
  utf8/utf16-length
  utf8/validate
  utf8/validate-invalid
  utf16/utf8-convert
  utf16/utf8-length
  utf16/validate
  utf16/validate-invalid
)

foreach(test IN LISTS tests)
  string(REPLACE "/" "_" name ${test})

  add_executable(${name} ${test}.c)

  target_link_libraries(
    ${name}
    PRIVATE
      utf_static
  )

  target_include_directories(
    ${name}
    PRIVATE
      $<TARGET_PROPERTY:utf,INTERFACE_INCLUDE_DIRECTORIES>
  )

  add_test(
    NAME ${name}
    COMMAND ${name}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )

  set_tests_properties(
    ${name}
    PROPERTIES
    TIMEOUT 30
  )
endforeach()
