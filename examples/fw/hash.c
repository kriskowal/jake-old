/* hash.c
 * 2004-10-24-13-06 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Hash Implementation
 *
 */

#include "hash.h"

hash_t hash_alloc() {
	hash_t hash = calloc(1, sizeof(struct hash_t));
	if (!hash) {
		perror("calloc");
		exit(-1);
	}
	return hash;
}

void hash_free(hash_t hash) {
	free(hash);
}

void hash_ctor(hash_t hash) {
}

void hash_dtor(hash_t hash) {
	trie_t *trie;
	trie = hash->tries;
	while (trie != hash->tries + HASH_SIZE) {
		if (*trie) {
			trie_dtor(*trie);
			TRIE_FREE(*trie);
		}
		trie++;
	}
}

trie_t hash_follow_sz(hash_t hash, const char *sz) {
	trie_t *next;
	int key;

	/* construct key based on first characters
	 */
	key = 0;
	if (*sz) {
		key |= *sz - TRIE_LETTER_FIRST;
		sz++;
	} else {
		key |= 26;
	}
	key <<= 5;
	if (*sz) {
		key |= *sz - TRIE_LETTER_FIRST;
		sz++;
	} else {
		key |= 26;
	}
	key <<= 5;
	if (*sz) {
		key |= *sz - TRIE_LETTER_FIRST;
		sz++;
	} else {
		key |= 26;
	}
	key <<= 5;
	if (*sz) {
		key |= *sz - TRIE_LETTER_FIRST;
		sz++;
	} else {
		key |= 26;
	}
	
	next = &hash->tries[key];
	if (!*next) {
		*next = TRIE_ALLOC();
		TRIE_CTOR(*next);
	}

	return trie_follow_sz(*next, sz);
}

