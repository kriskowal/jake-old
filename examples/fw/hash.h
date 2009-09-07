/* hash.h
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Hash of (String, Trie) Pairs
 *
 * Accelerates word searches by mapping
 * the first four characters of all possible
 * key strings to their associated value trie
 * in constant time.
 *
 */

#ifndef HASH_H
#define HASH_H

#include "trie.h"
#include <stdlib.h>

/* This hash's key guarantees a unique position
 * for all lower-case strings of length less
 * than or equal to four.  The hash table
 * keys strings based on this, three-character,
 * substring and resolves conflicts between
 * strings with equivalent initial substrings
 * with a trie.
 */

/* 26 possible characters
 * 1 additional for premature null terminators
 * 4 characters long
 */

/* 2 ** (ceil(lg(26 + 1)) * 4) */
/* 2 ** (5 * 4) */
/* 2 ** 20 */
/* 1048576 */
#define HASH_SIZE 1048576
/* note: must be a power of two */

/*
 * Bit masks:
 *
 *           1         
 * 0123456789_123456789 key
 * 01234                character 3
 *      01234           character 2
 *           01234      character 1
 *                01234 character 0
 */

typedef struct hash_t *hash_t;
struct hash_t {
	trie_t tries[HASH_SIZE];
};

hash_t hash_alloc();
void hash_free(hash_t hash);
void hash_ctor(hash_t hash);
void hash_dtor(hash_t hash);
trie_t hash_follow_sz(hash_t, const char *sz);

#define HASH_FREE(hash) free(hash)

#define HASH_CTOR(hash)

#endif
