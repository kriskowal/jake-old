
#include "string.h"

int main() {
	struct string_t string;

	string_ctor(&string);
	string_assign_sz(&string, "HELLO, WORLD!");
	string_tolower(&string);
	printf("%s\n", string_sz(&string));
	string_dtor(&string);

	return 0;
}

