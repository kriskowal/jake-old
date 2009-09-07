#include "line.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LINE_SIZE_DEFAULT 2
/* note: this value must be at least 2 */
/* todo: crank this value up to something practical */

void line_ctor(line_t line) {
	line->size = LINE_SIZE_DEFAULT;
	line->sz = (char*)malloc(line->size);
	if (!line->sz) {
		perror("malloc");
		exit(-1);
	}
}

void line_ctor_sz(line_t line, const char* value) {
	line->size = strlen(value);
	line->sz = (char*)malloc(line->size);
	if (!line->sz) {
		perror("malloc");
		exit(-1);
	}
	strcpy(line->sz, value);
}

void line_ctor_line(line_t line, const line_t value) {
	line_ctor_sz(line, value->sz);
}

void line_dtor(line_t line) {
	free(line->sz);
}

int line_length(const line_t line) {
	return strlen(line->sz);
}

const char *line_sz(line_t line) {
	return line->sz;
}

void line_assign_sz(line_t line, const char* sz) {
	int size;

	size = strlen(sz);
	if (line->size < size) {
		line_grow(line, size);
	}

	strcpy(line->sz, sz);
}

void line_assign_line(line_t line, line_t rhs) {
	line_assign_sz(line, rhs->sz);
}

void line_add_sz(line_t line, const char* sz) {
	int size;

	size = strlen(line->sz) + strlen(sz);
	if (line->size < size) {
		line_grow(line, size);
	}

	strcat(line->sz, sz);
}

void line_erase(line_t line) {
	*line->sz = '\0';
}

void line_read(line_t line) {
	line_read_file(line, stdin);
}

void line_read_file(line_t line, FILE *file) {
	int got;
	char buffer[LINE_SIZE_DEFAULT];
	char *at;
	char *end;

	at = buffer;
	end = buffer + LINE_SIZE_DEFAULT - 1;
	*end = '\0';

	line_erase(line);

	while(1) {
		got = fgetc(file);

		if (got == EOF || got == '\n') {
			*at++ = '\0';
			break;
		} else {
			*at++ = (char)got;
		}

		if (at == end) {
			line_add_sz(line, buffer);
			at = buffer;
		}
	}

	at = '\0';
	line_add_sz(line, buffer);
}

void line_grow(line_t line, int minimum) {
	int size;

	while (line->size < minimum) {
		size = line->size << 1;
		if (!(line->size < size)) {
			fprintf(stderr, "line_grow: error: string too large");
			exit(-1);
		}
		line->size = size;
	}

	line->size = size;
	line->sz = (char*)realloc(line->sz, size);
	if (!line->sz) {
		perror("malloc");
		exit(-1);
	}
}

void line_tolower(line_t line) {
	char *at;

	for (at = line->sz; *at; at++) {
		*at = tolower(*at);
	}
}

int line_cmp_line(const line_t left, const line_t right) {
	return strcmp(left->sz, right->sz);
}

