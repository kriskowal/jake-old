/* word.c
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Word Implementation
 *
 */

#include "word.h"
#include <stdlib.h>

void word_ctor_string(word_t word, const string_t value) {
	string_ctor_string(&word->string, value);
	word->count = 1;
}

void word_ctor_sz(word_t word, const char *value) {
	string_ctor_sz(&word->string, value);
	word->count = 1;
}

void word_dtor(word_t word) {
	STRING_DTOR(&word->string);
}

int word_count(const word_t word) {
	return word->count;
}

void word_count_inc(const word_t word) {
	word->count++;
}

string_t word_string(const word_t word) {
	return &word->string;
}

int word_cmp_word(const word_t left, const word_t right) {
	return WORD_CMP_WORD(left, right);
}

int word_heap_at(const word_t word) {
	return word->heap_at;
}

void word_heap_at_assign(const word_t word, int at) {
	word->heap_at = at;
}

