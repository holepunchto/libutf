#include <stdbool.h>
#include <stddef.h>

#include "../../include/utf.h"
#include "../../include/utf/string.h"

extern void
utf32_string_init(utf32_string_t *string);

extern utf32_string_view_t
utf32_string_view_init(const utf32_t *data, size_t len);

extern void
utf32_string_destroy(utf32_string_t *string);

extern int
utf32_string_reserve(utf32_string_t *string, size_t len);

extern int
utf32_string_shrink_to_fit(utf32_string_t *string);

extern utf32_string_view_t
utf32_string_view(const utf32_string_t *string);

extern void
utf32_string_clear(utf32_string_t *string);

extern bool
utf32_string_empty(const utf32_string_t *string);

extern bool
utf32_string_view_empty(const utf32_string_view_t view);

extern int
utf32_string_copy(const utf32_string_t *string, utf32_string_t *result);

extern int
utf32_string_view_copy(const utf32_string_view_t view, utf32_string_t *result);

extern int
utf32_string_append(utf32_string_t *string, const utf32_string_t *other);

extern int
utf32_string_append_view(utf32_string_t *string, const utf32_string_view_t view);

extern int
utf32_string_append_character(utf32_string_t *string, utf32_t c);

extern int
utf32_string_append_literal(utf32_string_t *string, const utf32_t *literal, size_t n);

extern int
utf32_string_prepend(utf32_string_t *string, const utf32_string_t *other);

extern int
utf32_string_prepend_view(utf32_string_t *string, const utf32_string_view_t view);

extern int
utf32_string_prepend_character(utf32_string_t *string, utf32_t c);

extern int
utf32_string_prepend_literal(utf32_string_t *string, const utf32_t *literal, size_t n);

extern int
utf32_string_insert(utf32_string_t *string, size_t pos, const utf32_string_t *other);

extern int
utf32_string_insert_view(utf32_string_t *string, size_t pos, const utf32_string_view_t other);

extern int
utf32_string_insert_character(utf32_string_t *string, size_t pos, utf32_t c);

extern int
utf32_string_insert_literal(utf32_string_t *string, size_t pos, const utf32_t *literal, size_t n);

extern int
utf32_string_replace(utf32_string_t *string, size_t pos, size_t len, const utf32_string_t *replacement);

extern int
utf32_string_replace_view(utf32_string_t *string, size_t pos, size_t len, const utf32_string_view_t replacement);

extern int
utf32_string_replace_character(utf32_string_t *string, size_t pos, size_t len, utf32_t c);

extern int
utf32_string_replace_literal(utf32_string_t *string, size_t pos, size_t len, const utf32_t *literal, size_t n);

extern int
utf32_string_erase(utf32_string_t *string, size_t pos, size_t len);

extern int
utf32_string_concat(const utf32_string_t *string, const utf32_string_t *other, utf32_string_t *result);

extern int
utf32_string_view_concat(const utf32_string_view_t view, const utf32_string_t *other, utf32_string_t *result);

extern int
utf32_string_concat_view(const utf32_string_t *string, const utf32_string_view_t other, utf32_string_t *result);

extern int
utf32_string_view_concat_view(const utf32_string_view_t view, const utf32_string_view_t other, utf32_string_t *result);

extern int
utf32_string_concat_character(const utf32_string_t *string, utf32_t c, utf32_string_t *result);

extern int
utf32_string_view_concat_character(const utf32_string_view_t view, utf32_t c, utf32_string_t *result);

extern int
utf32_string_concat_literal(const utf32_string_t *string, const utf32_t *literal, size_t n, utf32_string_t *result);

extern int
utf32_string_view_concat_literal(const utf32_string_view_t view, const utf32_t *literal, size_t n, utf32_string_t *result);

extern int
utf32_string_compare(const utf32_string_t *string, const utf32_string_t *other);

extern int
utf32_string_view_compare(const utf32_string_view_t view, const utf32_string_view_t other);

extern int
utf32_string_compare_literal(const utf32_string_t *string, const utf32_t *literal, size_t n);

extern int
utf32_string_view_compare_literal(const utf32_string_view_t view, const utf32_t *literal, size_t n);

extern utf32_string_view_t
utf32_string_substring(const utf32_string_t *string, size_t start, size_t end);

extern utf32_string_view_t
utf32_string_view_substring(const utf32_string_view_t view, size_t start, size_t end);

extern int
utf32_string_substring_copy(const utf32_string_t *string, size_t start, size_t end, utf32_string_t *result);

extern int
utf32_string_view_substring_copy(const utf32_string_view_t view, size_t start, size_t end, utf32_string_t *result);

extern size_t
utf32_string_index_of_character(const utf32_string_t *string, size_t pos, utf32_t c);

extern size_t
utf32_string_view_index_of_character(const utf32_string_view_t view, size_t pos, utf32_t c);

extern size_t
utf32_string_last_index_of_character(const utf32_string_t *string, size_t pos, utf32_t c);

extern size_t
utf32_string_view_last_index_of_character(const utf32_string_view_t view, size_t pos, utf32_t c);
