#ifndef LINE_H
#define LINE_H

#include <stdio.h>

typedef struct line_t *line_t;
struct line_t {
	char *sz;
	int size;
};

void line_ctor(line_t line);
void line_ctor_sz(line_t line, const char* sz);
void line_ctor_line(line_t line, const line_t value);
void line_dtor(line_t line);
int line_length(const line_t line);
const char *line_sz(line_t line);
void line_assign_sz(line_t line, const char* sz);
void line_assign_line(line_t line, line_t value);
void line_add_sz(line_t line, const char* sz);
void line_erase(line_t line);
void line_read(line_t line);
void line_read_file(line_t line, FILE *file);
void line_grow(line_t line, int minimum);
void line_tolower(line_t line);
int line_cmp_line(const line_t left, const line_t right);

#define LINE_READ(line) line_read_file((line), stdin)
#define LINE_SZ(line) (line->sz)
#define LINE_CMP_LINE(left, right) (strcmp((left)->sz, (right)->sz))

#endif
