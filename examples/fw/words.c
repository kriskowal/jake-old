/* words.c
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Words Implementation
 *
 */

#include "words.h"
#include <stdlib.h>

void words_ctor(words_t words) {
	heap_ctor(&words->heap);
	words->hash = hash_alloc();
	HASH_CTOR(hash);
}

void words_dtor(words_t words) {
	word_t word;

	while (!HEAP_IS_EMPTY(&words->heap)) {
		word = heap_remove(&words->heap);
		word_dtor(word);
		free(word);
	}

	HEAP_DTOR(&words->heap);
	hash_dtor(words->hash);
	HASH_FREE(words->hash);
}

/* pre: value is lowercase */
void words_insert_string(words_t words, const string_t string) {
	trie_t trie;
	word_t word;

	trie = hash_follow_sz(words->hash, STRING_SZ(string));
	if ((word = TRIE_WORD(trie))) {
		/* if the word already exists */

		word_count_inc(word);
		HEAP_RAISE_WORD(&words->heap, word);

	} else {
		/* if this is a new word */

		/* create a new word */
		word = malloc(sizeof(struct word_t));
		if (!word) {
			perror("malloc");
			exit(-1);
		}
		word_ctor_string(word, string);

		/* insert into trie */
		TRIE_WORD_ASSIGN(trie, word);

		/* insert into heap */
		heap_insert(&words->heap, word);

	}

}

void words_report(words_t words, int n) {
	word_t word;
	int i;

	/* heap_sort(&words->heap); */

	printf(
		"The top %d words (out of %d) are:\n",
		n,
		HEAP_LENGTH(&words->heap)
	);

	i = 0;
	while (i < n && !HEAP_IS_EMPTY(&words->heap)) {
		word = heap_remove(&words->heap);
		printf(
				"%9d %s\n",
				WORD_COUNT(word),
				STRING_SZ(word_string(word))
		);
		word_dtor(word);
		free(word);
		i++;
	}
}

