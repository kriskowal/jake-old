/* heap.c
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Heap Implementation
 *
 */

#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define HEAP_SIZE_DEFAULT 1024
/* note: this value must be at least 2 */

void heap_ctor(heap_t heap) {
	heap->size = HEAP_SIZE_DEFAULT;
	/* operates on the premise that
	 * pointers are 4 bytes wide */
	heap->begin = malloc(heap->size * sizeof(word_t));
	if (!heap->begin) {
		perror("malloc");
		exit(-1);
	}
	heap->end = heap->begin;
}

void heap_dtor(heap_t heap) {
	free(heap->begin);
}

/* doubles the size of the heap */
void heap_grow(heap_t heap) {
	int end_at;

	end_at = heap->end - heap->begin;
	heap->size <<= 1;
	heap->begin = realloc(heap->begin, heap->size * sizeof(word_t));
	if (!heap->begin) {
		perror("malloc");
		exit(-1);
	}
	heap->end = heap->begin + end_at;
}

int heap_is_empty(const heap_t heap) {
	return heap->begin == heap->end;
}

int heap_length(const heap_t heap) {
	return heap->end - heap->begin;
}

void heap_raise(heap_t heap, int self_at) {
	int parent_at;
	int cmp;
	word_t self, parent;

	/* bubble toward top of heap as long
	 * as parent is greater than self */
	while (self_at) {
		parent_at = (self_at - 1) >> 1;
		self = heap->begin[self_at];
		parent = heap->begin[parent_at];
		cmp = WORD_COUNT(self) - WORD_COUNT(parent);
		if (
			(
			 	cmp? cmp:
				STRING_CMP_STRING(
					WORD_STRING(self),
					WORD_STRING(parent)
				)
			) > 0
		) {
			/* swap */
			heap->begin[self_at] = parent;
			heap->begin[parent_at] = self;
			WORD_HEAP_AT_ASSIGN(parent, self_at);
			WORD_HEAP_AT_ASSIGN(self, parent_at);

			self_at = parent_at;
			continue;

		} else {
			break;
		}
	}
}

void heap_raise_word(heap_t heap, word_t word) {
	heap_raise(heap, WORD_HEAP_AT(word));
}

void heap_lower(heap_t heap, int self_at) {
	word_t self, child, left, right;
	int child_at, left_at, right_at, end_at;
	int cmp;

	/* bubble top toward bottom of heap */
	end_at = heap->end - heap->begin;
	while (1) {
		left_at = (self_at << 1) + 1;
		right_at = (self_at + 1) << 1;

		/* find greatest child */
		if (right_at < end_at) {
			/* two children */
			left = heap->begin[left_at];
			right = heap->begin[right_at];
			cmp = WORD_COUNT(left) - WORD_COUNT(right);
			if (
				(
					cmp? cmp:
					STRING_CMP_STRING(
						WORD_STRING(left),
						WORD_STRING(right)
					)
				) > 0
			) {
				child_at = left_at;
			} else {
				child_at = right_at;
			}
		} else if (left_at < end_at ) {
			/* one child */
			child_at = left_at;
		} else {
			/* no children */
			break;
		}

		self = heap->begin[self_at];
		child = heap->begin[child_at];
		cmp = WORD_COUNT(self) - WORD_COUNT(child);
		if (
			(
				cmp? cmp:
				STRING_CMP_STRING(
					WORD_STRING(self),
					WORD_STRING(child)
				)
			) < 0
		) {
			/* self less than child */

			/* swap */
			heap->begin[child_at] = self;
			heap->begin[self_at] = child;
			WORD_HEAP_AT_ASSIGN(self, child_at);
			WORD_HEAP_AT_ASSIGN(child, self_at);

			self_at = child_at;
		} else {
			/* greater than child */
			/* stop */
			break;
		}
	}

}

void heap_sort(heap_t heap) {
	int i;
	for (i = 0; i != HEAP_LENGTH(heap); i++) {
		heap_raise(heap, i);
	}
}

void heap_insert(heap_t heap, word_t word) {
	int self_at;

	/* insert new word at end */
	if (heap->begin + heap->size == heap->end) {
		heap_grow(heap);
	}

	/* put on bottom */
	self_at = heap->end - heap->begin;
	WORD_HEAP_AT_ASSIGN(word, self_at);
	*heap->end++ = word;

	/* if the heap does not need to be in
	 * persistant sorted order, this call
	 * is not necessary and heap_sort
	 * could be called just before order
	 * is needed. */
	heap_raise(heap, self_at);
}

word_t heap_remove(heap_t heap) {
	word_t begin;

	/* store top and put bottom on top */
	begin = *heap->begin;
	*heap->begin = *--heap->end;
	WORD_HEAP_AT_ASSIGN(*heap->begin, 0);

	heap_lower(heap, 0);

	return begin;
}

void heap_print(heap_t heap) {
	word_t *at = heap->begin;
	for (; at != heap->end; at++) {
		printf(
			"%d:%s, ",
			WORD_HEAP_AT(*at),
			STRING_SZ(WORD_STRING(*at))
		);
	}
	printf("\n");
}

