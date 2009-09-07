/* words.h
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Words
 *
 * Provides a facility for recording the frequency
 * of given words and reporting those records in
 * proper order.
 *
 * A |words| structure maintains four data structures.
 * Its |heap| contains and owns a collection of all
 * encountered words as yet, in sorted order.  The 
 * structure also contains a collection of |tries|
 * to provide fast lookup of words based on their
 * string.  To accelerate such searches, the |tries|
 * are stored inside a |hash| which provides
 * a trie based on the first four characters of a
 * given string in constant time.
 *
 * Note that reporting the word frequency is a 
 * destructive operation whereby the |words| structure
 * becomes perilous to use, except to destroy.
 *
 */

#ifndef WORDS_H
#define WORDS_H

#include "trie.h"
#include "hash.h"
#include "heap.h"
#include "string.h"

typedef struct words_t *words_t;
struct words_t {
	hash_t hash;
	struct heap_t heap;
};

void words_ctor(words_t words);
void words_dtor(words_t words);
void words_insert_string(words_t words, const string_t value);
void words_report(words_t words, int n);

#endif
