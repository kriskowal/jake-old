/* heap_trial.c
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Heap Trial
 *
 * Adds four words to a heap,
 * and pops them off from greatest
 * to least.
 *
 */

#include "heap.h"

int main() {
	struct heap_t heap;
	struct word_t word_a;
	struct word_t word_b;
	struct word_t word_c;
	struct word_t word_d;

	heap_ctor(&heap);
	word_ctor_sz(&word_a, "a");
	word_ctor_sz(&word_b, "b");
	word_ctor_sz(&word_c, "c");
	word_ctor_sz(&word_d, "d");

	heap_insert(&heap, &word_a);
	heap_print(&heap);
	heap_insert(&heap, &word_b);
	heap_print(&heap);
	heap_insert(&heap, &word_c);
	heap_print(&heap);
	heap_insert(&heap, &word_d);
	heap_print(&heap);

	while (!HEAP_IS_EMPTY(&heap)) {
		heap_remove(&heap);
		heap_print(&heap);
	}

	word_dtor(&word_d);
	word_dtor(&word_c);
	word_dtor(&word_b);
	word_dtor(&word_a);
	heap_dtor(&heap);
	return 0;
}

