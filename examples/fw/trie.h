/* trie.h
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Trie of (String, Word) Pairs
 *
 * Maintains a facility for finding and storing
 * pointers to words based on their contained
 * strings in time proportional
 * only to the length of the string.
 *
 * A trie is a tree where each node contains
 * a branch for every letter.  A trie is a
 * mapping optimized for string type keys.
 *
 * Trie allocation and freeing must be performed
 * using the provided macros or functions.
 * Tries depend on having their tables zeroed
 * before they're used.
 *
 * Trie words are weak references.  That is,
 * tries are not responsible for the allocation
 * and deallocation of the words they contain.
 *
 */

#ifndef TRIE_H
#define TRIE_H

#include "word.h"
#include <stdlib.h>

#define TRIE_LETTER_FIRST 'a'
#define TRIE_LETTER_COUNT 26

typedef struct trie_t *trie_t;
struct trie_t {
	word_t word;
	trie_t letters[TRIE_LETTER_COUNT];
};

trie_t trie_alloc();
void trie_free(trie_t trie);
void trie_ctor(trie_t trie);
void trie_dtor(trie_t trie);
trie_t trie_follow_sz(trie_t trie, const char *sz);
word_t trie_word(trie_t trie);
void trie_word_assign(trie_t trie, word_t word);

#define TRIE_ALLOC() (calloc(1, sizeof(struct trie_t)))

#define TRIE_FREE(trie) free(trie)

#define TRIE_CTOR(trie)

#define TRIE_WORD(trie) ((trie)->word)

#define TRIE_WORD_ASSIGN(trie, value) ((trie)->word = (value))

#define TRIE_IS_LETTER(value) (     \
	TRIE_LETTER_FIRST <= (value) && \
	(value) < (                     \
		TRIE_LETTER_FIRST +         \
		TRIE_LETTER_COUNT           \
	)                               \
)

#endif
