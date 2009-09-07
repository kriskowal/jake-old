/* word.h
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Word Structure
 *
 * Stores and provides information about the frequency
 * of a given word.  The word stores the string it
 * coresponds to and how often that string has occured.
 * The |word| also stores its index in some heap.
 *
 * The word provides no intelligence: only accessors
 * and mutators.
 *
 */

#ifndef WORD_H
#define WORD_H

#include "string.h"

typedef struct word_t *word_t;
struct word_t {
	struct string_t string;
	int count;
	int heap_at;
};

void word_ctor_string(word_t word, const string_t value);
void word_ctor_sz(word_t word, const char *value);
void word_dtor(word_t word);
int word_count(const word_t word);
string_t word_string(const word_t word);
void word_count_inc(word_t word);
int word_cmp_word(const word_t left, const word_t right);
int word_heap_at(const word_t word);
void word_heap_at_assign(const word_t word, int at);

#define WORD_COUNT(word) ((word)->count)

#define WORD_COUNT_INC(word) ((word)->count++)

#define WORD_STRING(word) (&(word)->string)

#define WORD_CMP_WORD(left, right) (                     \
	((left)->count != (right)->count))?                  \
	(left)->count - (right)->count :                     \
	STRING_CMP_STRING(&(left)->string, &(right)->string)

#define WORD_HEAP_AT(word) ((word)->heap_at)

#define WORD_HEAP_AT_ASSIGN(word, at) ((word)->heap_at = (at))

#endif
