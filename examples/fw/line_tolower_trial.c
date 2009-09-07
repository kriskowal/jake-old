
#include "line.h"

int main() {
	struct line_t line;

	line_ctor(&line);
	line_assign_sz(&line, "HELLO, WORLD!");
	line_tolower(&line);
	printf("%s\n", line_sz(&line));
	line_dtor(&line);

	return 0;
}

