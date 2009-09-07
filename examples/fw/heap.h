/* heap.h
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Heap of Words
 *
 * Maintains a collection of words in sorted order.
 *
 * The heap owns its words.  That is, the heap
 * is responsible for the allocation and deallocation
 * of all words it contains.
 *
 */

#ifndef HEAP_H
#define HEAP_H

#include "word.h"

typedef struct heap_t *heap_t;
struct heap_t {
	word_t *begin;
	word_t *end;
	int size;
};

void heap_ctor(heap_t heap);
void heap_dtor(heap_t heap);
void heap_grow(heap_t heap);
int heap_is_empty(const heap_t heap);
int heap_length(const heap_t heap);
void heap_raise(heap_t heap, int self_at);
void heap_raise_word(heap_t heap, word_t word);
void heap_lower(heap_t heap, int self_at);
void heap_sort(heap_t heap);
void heap_insert(heap_t heap, word_t word);
word_t heap_remove(heap_t heap);
void heap_print(heap_t heap);

#define HEAP_DTOR(heap) (free((heap)->begin))

#define HEAP_IS_EMPTY(heap) ((heap)->begin == (heap)->end)

#define HEAP_LENGTH(heap) ((heap)->end - (heap)->begin)

#define HEAP_RAISE_WORD(heap, word) heap_raise((heap), WORD_HEAP_AT(word))

#endif
