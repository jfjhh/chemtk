#include "scicalc.h"

const char *title = "Scicalc: A scientific calculator utility.";

#define NUM_OPERATORS 16
const char operators[] = {
	'+', '-', '*', '/',
	'=', '<', '>', '&',
	'^', '~', '%', '|',
	'o', 'l', 'r', 'x',
};

long double operate(long double a, long double b, char operation)
{
	long double c;

	switch (operation) {
		case '+':
			c = a + b;
			break;
		case '-':
			c = a - b;
			break;
		case '*':
			c = a * b;
			break;
		case '/':
			c = a / b;
			break;
		case '%':
			c = (int) a % (int) b;
			break;
		case '&':
			c = (int) a & (int) b;
			break;
		case '|':
			c = (int) a | (int) b;
			break;
		case 'x':
			c = (int) a ^ (int) b;
			break;
		case 'l':
			c = (int) a << (int) b;
			break;
		case 'r':
			c = (int) a >> (int) b;
			break;
		case '~':
			c = ~ ((int) a);
			break;
		case '>':
			c = (a > b) ? 1L : 0L;
			break;
		case '<':
			c = (a < b) ? 1L : 0L;
			break;
		case '=':
			c = (a == b) ? 1L : 0L;
			break;
		case '^':
			c = pow(a, b);
			break;
		case 'o':
			c = (logl(a) / logl(b));
			break;
		default:
			c = 0L;
			break;
	}

	return c;
}

/* TODO: write test_scicalc(). */
int test_scicalc(WINDOW *outwin)
{
	wprintw(outwin, "TODO: write test_scicalc().\n");
	return 1;
}

static void refresh_stack(WINDOW *outwin, WINDOW *stackwin, struct stack *stack)
{
	werase(stackwin);

	/* (Re)create stack window */
	if (!(stackwin = derwin(outwin,
					(getmaxy(outwin) - 8),
					(getmaxx(outwin) / SCICALC_NUMLEN) * SCICALC_NUMLEN,
					3, 2)))
		wprintw(outwin, "Could not create a new WINDOW * for the stack!\n");
	box(stackwin, 0, 0);

	/* Print the stack */
	print_stack(stackwin, 0, 1, STACK_WIDTH, stack);
	wrefresh(stackwin);
}

void scicalc(WINDOW *outwin)
{
	int ok, num, i, frows, fcols, fy, fx, push;
	int oldpos[2];
	struct num_str *numstr;
	struct stack *stack = NULL;
	char *fdata = NULL;
	FORM *form = NULL;
	FIELD *number[3];
	WINDOW *stackwin = NULL;

	/* Positions for the number prompt. */
	fy = getmaxy(outwin) - 5;
	fx = (getmaxx(outwin) - SCICALC_NUMLEN) / 2;

	/* Allocate fields. */
	if (!(number[0] = new_field(1, SCICALC_NUMLEN, fy + 1, fx + 1, 0, 0))) {
		wprintw(outwin, "Could not create a new FIELD *! (0)\n");
		ok = 0;
	}
	if (!(number[1] = new_field(1, SCICALC_NUMLEN, fy + 2, fx + 1, 0, 0))) {
		wprintw(outwin, "Could not create a new FIELD * (1)!\n");
		ok = 0;
	}
	number[2] = NULL; /* NULL-terminate the field array. */

	/* Allocate stack. */
	if (!(stack = new_stack())) {
		wprintw(outwin, "Could not create a new struct stack *! (0)\n");
		ok = 0;
	}

	/* Configure the input fields. */

	/* 0 */
	set_field_just(number[0], JUSTIFY_LEFT);
	/* set_field_type(number[0], TYPE_NUMERIC, 0, -INFINITY, INFINITY); */
	set_field_type(number[0], TYPE_REGEXP, SCICALC_NUM_REGEXP);

	field_opts_off(number[0], O_AUTOSKIP);

	set_field_fore(number[0], COLOR_PAIR(SCIWIN_COLOR_PROMPT) | A_STANDOUT);
	set_field_back(number[0], A_UNDERLINE);

	fdata = field_buffer(number[0], 0); /* Use for number data. */

	/* 1 */
	set_field_just(number[1], JUSTIFY_LEFT);
	set_field_type(number[1], TYPE_NUMERIC, 0, -INFINITY, INFINITY);

	field_opts_off(number[1], O_AUTOSKIP);

	set_field_fore(number[1], COLOR_PAIR(SCIWIN_COLOR_PROMPT) | A_STANDOUT);
	set_field_back(number[1], A_UNDERLINE);

	/* Configure the input form. */
	if (!(form = new_form(number))) {
		wprintw(outwin, "Could not create a new FORM *!\n");
		ok = 0;
	}

	scale_form(form, &frows, &fcols);
	set_form_win(form, outwin);
	set_form_sub(form, derwin(outwin, frows, fcols, fy + 1, fx + 1));

	/* Form */
	post_form(form);
	box(derwin(outwin, 4, SCICALC_NUMLEN + 6, fy, fx - 2), 0, 0);
	wmove(outwin, fy + 1, fx + 1);

	/* Stack */
	refresh_stack(outwin, stackwin, stack);

	/* Title */
	mvwprintw(outwin, 1, (getmaxx(outwin) - strlen(title)) / 2, "%s", title);

	/* Done with init */
	wmove(outwin, fy + 1, fx + 1); /* Go to start of form */
	wrefresh(outwin);

	num = ' ';
	ok = 1;
	i = push = 0;

	while (ok && (num = wgetch(outwin))) {
		if (num == 'Q' || num == KEY_F(1))
			break;

		switch (num) { /* Field controls */
			case KEY_BTAB: /* Change mode */
			case '\t':
				/* TODO: Mode changing and operations and lookup. */
				break;

			case KEY_ENTER: /* Toggle field */
			case '\n':
				if (form_driver(form, REQ_VALIDATION) == E_INVALID_FIELD) {
					getyx(outwin, oldpos[0], oldpos[1]);
					mvwprintw(outwin, getmaxy(outwin) - 1, 3,
							"Invalid number! (%s).", fdata);
					wmove(outwin, oldpos[0], oldpos[1]);
					wrefresh(outwin);
				} else {
					getyx(outwin, oldpos[0], oldpos[1]);
					wmove(outwin, getmaxy(outwin) - 1, 0);
					wclrtoeol(outwin);
					wmove(outwin, oldpos[0], oldpos[1]);
					form_driver(form, REQ_DEL_LINE);
					push = 1;
				}
				break;

			case KEY_DOWN: /* Down */
				form_driver(form, REQ_NEXT_FIELD);
				form_driver(form, REQ_END_LINE);
				break;

			case KEY_UP: /* Up */
				form_driver(form, REQ_PREV_FIELD);
				form_driver(form, REQ_END_LINE);
				break;

			case '/': /* Left */
			case KEY_LEFT:
				form_driver(form, REQ_PREV_CHAR);
				break;

			case '*': /* Right */
			case KEY_RIGHT:
				form_driver(form, REQ_NEXT_CHAR);
				break;

			case KEY_BACKSPACE: /* Backspace */
			case '\b':
				form_driver(form, REQ_DEL_CHAR);
				form_driver(form, REQ_PREV_CHAR);
				break;

			default: /* Otherwise just print the char */
				form_driver(form, num);
				break;
		}

		if (push) {
			push = 0;
			if (!(numstr = get_num_str(fdata))) {
				getyx(outwin, oldpos[0], oldpos[1]);
				mvwprintw(outwin, 0, 0,
						"Unable to allocate memory for a num_str!\n");
				wmove(outwin, oldpos[0], oldpos[1]);
			} else {
				push_stack(stack, numstr);
			}
		}

		refresh_stack(outwin, stackwin, stack);
		i++;
	}

	/* Exit. */
	unpost_form(form);

	if (form)
		free_form(form);
	if (number[0])
		free_field(number[0]);
	if (number[1])
		free_field(number[1]);
	if (stack)
		delete_stack(stack);

	werase(outwin);
}

