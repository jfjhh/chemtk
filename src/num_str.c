#include "num_str.h"

struct num_str *get_num_str(WINDOW *outwin, const char *prompt)
{
	struct num_str *read = malloc(sizeof(struct num_str));
	char *buf = (char *) malloc(sizeof(char) * NUMSTR_BUFSIZE);
	int i, is_num;
	long double tmp;

	wprintw(outwin, "%s", prompt);
	wscanw(outwin, "%s", buf);

	is_num = 0;
	read->type = NONE;
	for (i = 0; i < 10; i++) {
		if (buf[0] == '0' + i) {
			sscanf(buf, "%Le", &(read->data));

			if (buf[NUMSTR_BUFSIZE - 2] == NUMSTR_CONST_CHAR)
				read->type = CONSTANT;
			else
				read->type = NUMBER;

			is_num = 1;
			break;
		}
	}

	if (!is_num) {
		if ((tmp = const_search(buf)) != 0) {
			read->type = CONSTANT;
		} else {
			read->type = CMD;
		}
		read->data = tmp;
	}

	strncpy(buf, read->str, NUMSTR_BUFSIZE);

	free(buf);
	return read;
}

/* TODO: write test_num_str(). */
int test_num_str(WINDOW *outwin)
{
	wprintw(outwin, "TODO: write test_num_str().\n");
	return 1;
}

