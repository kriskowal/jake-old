/* trie.c
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Trie Implementation
 *
 */

#include "trie.h"
#include "string.h"

trie_t trie_alloc() {
	return calloc(1, sizeof(struct trie_t));
}

void trie_free(trie_t trie) {
	free(trie);
}

void trie_ctor(trie_t trie) {
}

/* destroys all nodes in the tree by simultaneously
 * degenerating the trie into a linked list and iteratively
 * freeing the first elements of the list as they
 * become empty. */
void trie_dtor(trie_t trie) {
	trie_t *letter;
	trie_t begin;
	trie_t end;

	/* seek end */
	end = trie;
	while (end->letters[0]) {
		end = end->letters[0];
	}

	while (trie) {

		/* move all subtrees to the end */
		letter = trie->letters + 1;
		while (letter != trie->letters + TRIE_LETTER_COUNT) {
			if (*letter) {
				end->letters[0] = *letter;

				/* seek end */
				while (end->letters[0]) {
					end = end->letters[0];
				}
			}
			letter++;
		}

		/* destroy the first node and advance */
		begin = trie;
		trie = trie->letters[0];
		TRIE_FREE(begin);
	}
}

trie_t trie_follow_sz(trie_t trie, const char* sz) {
	trie_t *next;

	while (*sz) {
		next = &trie->letters[*sz - TRIE_LETTER_FIRST];

		if (!*next) {
			*next = TRIE_ALLOC();
			TRIE_CTOR(*next);
		}

		trie = *next;
		sz++;
	}

	return trie;
}

word_t trie_word(trie_t trie) {
	return trie->word;
}

void trie_word_assign(trie_t trie, word_t word) {
	trie->word = word;
}


