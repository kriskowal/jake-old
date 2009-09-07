/* string.c
 * 2004-10-24-13-06 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * String Implementation
 *
 */

#include "string.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define STRING_SIZE_DEFAULT 6
/* note: this value must be at least 2 */

void string_ctor(string_t string) {
	string->size = STRING_SIZE_DEFAULT;
	string->begin = (char*)malloc(string->size);
	if (!string->begin) {
		perror("malloc");
		exit(-1);
	}
	string->end = string->begin;
	*string->end = '\0';
}

void string_ctor_sz(string_t string, const char* value) {
	string->size = 1 + strlen(value);
	string->begin = (char*)malloc(string->size);
	if (!string->begin) {
		perror("malloc");
		exit(-1);
	}
	strcpy(string->begin, value);
	string->end = string->begin + string->size - 1;
}

void string_ctor_string(string_t string, const string_t value) {
	string->size = value->size;
	string->begin = (char*)malloc(string->size);
	if (!string->begin) {
		perror("malloc");
		exit(-1);
	}
	strcpy(string->begin, value->begin);
	string->end = string->begin + string->size - 1;
}

void string_dtor(string_t string) {
	free(string->begin);
}

void string_grow(string_t string, int minimum) {
	int size, length;
	length = STRING_LENGTH(string);

	size = string->size;
	while (size < minimum) {
		size <<= 1;
	}
	string->size = size;

	string->begin = (char*)realloc(string->begin, size);
	if (!string->begin) {
		perror("malloc");
		exit(-1);
	}

	string->end = string->begin + length;
}

int string_length(const string_t string) {
	return string->end - string->begin;
}

const char *string_sz(string_t string) {
	return string->begin;
}

void string_assign_sz(string_t string, const char* sz) {
	int size;

	size = 1 + strlen(sz);
	if (string->size < size) {
		string_grow(string, size);
	}

	strcpy(string->begin, sz);
	string->end = string->begin + size - 1;
}

void string_assign_string(string_t string, string_t value) {
	int size;

	size = 1 + STRING_LENGTH(value);
	if (string->size < size) {
		string_grow(string, size);
	}

	strcpy(string->begin, value->begin);
	string->end = string->begin + size - 1;
}

void string_add_sz(string_t string, const char* sz) {
	int size;

	size = 1 + strlen(string->begin) + strlen(sz);
	if (string->size < size) {
		string_grow(string, size);
	}

	strcat(string->begin, sz);
	string->end = string->begin + size - 1;
}

void string_add_char(string_t string, char value) {
	if (string->size == STRING_LENGTH(string) + 1) {
		string_grow(string, string->size + 1);
	}

	*string->end++ = value;
	*string->end = '\0';
}

void string_erase(string_t string) {
	string->end = string->begin;
	*string->end = '\0';
}

void string_read_line(string_t string) {
	string_read_line_file(string, stdin);
}

void string_read_line_file(string_t string, FILE *file) {
	int got;
	char buffer[STRING_SIZE_DEFAULT];
	char *at;
	char *end;

	at = buffer;
	end = buffer + STRING_SIZE_DEFAULT - 1;
	*end = '\0';

	string_erase(string);

	if (!feof(file)) {
		while(1) {
			got = fgetc(file);

			if (got == EOF || got == '\n') {
				*at++ = '\0';
				break;
			} else {
				*at++ = (char)got;
			}

			if (at == end) {
				string_add_sz(string, buffer);
				at = buffer;
			}
		}
	}

	at = '\0';
	string_add_sz(string, buffer);
}

void string_tolower(string_t string) {
	char *at;

	for (at = string->begin; *at; at++) {
		*at = tolower(*at);
	}
}

int string_cmp_string(const string_t left, const string_t right) {
	return strcmp(left->begin, right->begin);
}

