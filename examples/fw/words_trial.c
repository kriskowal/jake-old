/* words_trial.c
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Words Trial
 *
 * Allows the user to add lines
 * to the words collection (possibly
 * invalid words) and reports
 * their frequencies.
 *
 */

#include "words.h"

int main() {
	struct words_t words;
	struct string_t string;

	words_ctor(&words);
	string_ctor(&string);

	do {
		string_read_line(&string);
		words_insert_string(&words, &string);
	} while (string_length(&string));

	words_report(&words, 10);

	string_dtor(&string);
	words_dtor(&words);

	return 0;
}

