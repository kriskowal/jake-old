/* fw.c
 * 2004-10-24-12-57 PDT
 * by Kris Kowal
 * for CPE X317 section 2
 * with Phil Nico
 *
 * Frequent Words
 *
 * Reports the top n most frequent words in given files,
 * albeit standard input.
 *
 */

#include "words.h"
#include "string.h"
#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

/* reads words from given |file| and increments their
 * count in the |words| structure. */
void fw(words_t words, FILE *file) {
	struct string_t string; /* a buffer */
	int got; /* character recieved from |file| */

	string_ctor(&string);

	if (!feof(file)) {
		while ((got = fgetc(file)) != EOF) {
			if (isalpha(got)) {
				/* add the lowercase form of the read
				 * character to the end of the buffer */
				string_add_char(&string, tolower(got));
			} else {
				/* flush */
				if (STRING_LENGTH(&string)) {
					words_insert_string(words, &string);
					STRING_ERASE(&string);
				}
			}
		}
		/* flush */
		if (STRING_LENGTH(&string)) {
			words_insert_string(words, &string);
			STRING_ERASE(&string);
		}
	}

	STRING_DTOR(&string);
}

/* display usage information for fw */
void usage() {
	printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
}

/* parse arguments */
int main(int argc, char **argv) {
	FILE *file;
	struct words_t words;
	int i; /* current argument index */
	int	n; /* number of top most frequent words to report */

	words_ctor(&words);

	/* check for user specified report
	 * length */
	i = 1;
	n = 10; /* default report length */
	if (i < argc) {
		if (strcmp(argv[i], "-n") == 0) {
			i++;
			if (i < argc) {
				n = atoi(argv[i]);
				i++;
				if (!n) {
					usage();
					exit(-1);
				}
			} else {
				usage();
				exit(-1);
			}
		}
	}

	/* read remaining arguments as files */
	if (i < argc) {
		do {
			file = fopen(argv[i], "r");
			if (!file) {
				perror(argv[i]);
			} else {
				/* fw words from file */
				fw(&words, file);
				if (fclose(file) == EOF) {
					perror(argv[i]);
				}
			}
			i++;
		} while (i < argc);
	} else {
		/* if the user specified no file,
		 * read words from standard input */
		fw(&words, stdin);
	}

	/* report n most frequent words to stdout */
	words_report(&words, n);

	words_dtor(&words);
	return 0;
}

