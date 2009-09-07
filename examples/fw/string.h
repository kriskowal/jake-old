/* string.h
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * String
 * Sequence of Chars
 *
 * Provides transparent memory managment
 * for C strings.
 *
 */

#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <string.h>

typedef struct string_t *string_t;
struct string_t {
	char *begin;
	char *end;
	int size;
};

void string_ctor(string_t string);
void string_ctor_sz(string_t string, const char* sz);
void string_ctor_string(string_t string, const string_t value);
void string_dtor(string_t string);
void string_grow(string_t string, int minimum);
int string_length(const string_t string);
const char *string_sz(string_t string);
void string_assign_sz(string_t string, const char* sz);
void string_assign_string(string_t string, string_t value);
void string_add_sz(string_t string, const char* sz);
void string_add_char(string_t string, char value);
void string_erase(string_t string);
void string_read_line(string_t string);
void string_read_line_file(string_t string, FILE *file);
void string_tolower(string_t string);
int string_cmp_string(const string_t left, const string_t right);

#define STRING_DTOR(string) free((string)->begin)
#define STRING_LENGTH(string) ((string)->end - (string)->begin)
#define STRING_SZ(string) ((string)->begin)
#define STRING_ERASE(string) \
	((string)->end = (string)->begin, *(string)->end = '\0')
#define STRING_READ_LINE(string) string_read_line_file((string), stdin)
#define STRING_CMP_STRING(left, right) \
	(strcmp((left)->begin, (right)->begin))

#endif
