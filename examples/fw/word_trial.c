/* word_trial.c
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Word Trial
 *
 * Creates two words with identical strings
 * and demonstrates that their word count
 * incrementer, word count accessor, and
 * word comparator work as both macros and
 * function calls.
 *
 */

#include <stdio.h>
#include "string.h"
#include "word.h"

int main() {
	struct word_t word_1;
	struct word_t word_2;
	struct string_t string;

	string_ctor_sz(&string, "a");
	word_ctor_string(&word_1, &string);
	word_ctor_string(&word_2, &string);

	printf("%d\n", WORD_COUNT(&word_1));
	printf("%d\n", word_count(&word_2));

	WORD_COUNT_INC(&word_1);

	printf("%d\n", WORD_COUNT(&word_1));
	printf("%d\n", word_count(&word_2));

	printf("%d\n", word_cmp_word(&word_1, &word_2));
	printf("%d\n", WORD_CMP_WORD(&word_1, &word_2));

	word_count_inc(&word_2);

	printf("%d\n", WORD_COUNT(&word_1));
	printf("%d\n", word_count(&word_2));

	word_dtor(&word_2);
	word_dtor(&word_1);
	string_dtor(&string);
	return 0;
}

