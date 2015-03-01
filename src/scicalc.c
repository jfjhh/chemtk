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

static void run_cmd(WINDOW *outwin, struct num_str *numstr)
{
	struct element *e;
	int i;
	char mod, *sym, line[NUMSTR_BUFSIZE];
	WINDOW *infowin = NULL;

	fprintf(stderr, "Running a cmd: %s.\n", numstr->str); /* DEBUG */
	werase(outwin);

	/* Only if this is an element command. */
	if (numstr->str[0] == 'e') {

		/* No errors if strndup fails,
		 * because search may work without this part. */
		if ((sym = strndup((numstr->str) + 2, ELEMENT_SYM_LEN)))
			for (i = 1; i < ELEMENT_SYM_LEN; i++)
				if (!isalpha(sym[i]))
					sym[i] = '\0';
		if (sym)
			sym[0] = toupper(sym[0]);

		/* Get the element */
		if (!(e = find_element_sym((sym) ? sym : ((numstr->str) + 2)))) {
			mvwprintw(outwin, 0, 0, "Cound not find element '%s'!",
					(sym) ? sym : ((numstr->str) + 2));
		} else {
			mod = numstr->str[1];

			if (mod == 'g' || mod == 'n') { /* value of info / data. */
				if (mod == 'g') {
					numstr->data = e->molar_mass;
					mvwprintw(outwin, 0, 0, "%s's molar mass is %f grams.",
							e->symbol, e->molar_mass);
				} else if (mod == 'n') {
					numstr->data = e->atomic_number;
					mvwprintw(outwin, 0, 0, "%s's atomic number is %d.",
							e->symbol, e->atomic_number);
				}

				numstr->type = CONSTANT;

				/* Convert the double to a string and make that numstr->str. */
				snprintf(line, NUMSTR_BUFSIZE, "%Lg", numstr->data);
				strncpy(numstr->str, line, NUMSTR_BUFSIZE);
				for (i = strlen(numstr->str); i < NUMSTR_BUFSIZE - 1; i++)
					numstr->str[i] = ' '; /* Pad the str back out. */
				numstr->str[NUMSTR_BUFSIZE] = '\0';

			} else if (mod == 'i') { /* print info / data */
				if (box(infowin = derwin(outwin,
								getmaxy(outwin), getmaxx(outwin),
								0, 0), 0, 0) == OK) {
					wrefresh(infowin);
					print_element_info(e, derwin(infowin,
								getmaxy(infowin) - 2, getmaxx(infowin) - 3,
								1, 2));

					wmove(infowin, 0, 2);
					page_prompt(infowin, "exit");
					werase(infowin);
				} else {
					print_element_info(e, outwin);

					page_prompt(outwin, "exit");
					werase(outwin);
				}
			}

			/* Print other element information anyway */
			if (mod != 'i') {
				i = strlen(e->name) + strlen(e->symbol) + 16;

				if (box(infowin = derwin(outwin, 5, i, getcury(outwin) + 2, 1),
							0, 0) == OK)
					print_element(e, derwin(infowin,
								getmaxy(infowin) - 2, getmaxx(infowin) - 2,
								1, 1));
				else
					print_element(e, outwin);

			}

			if (infowin)
				wrefresh(infowin);
		}

		if (sym)
			free(sym);

	} else {
		/* Strip trailing whitespace. */
		for (i = 0; i < NUMSTR_BUFSIZE; i++)
			if (!isalpha(numstr->str[i]))
				numstr->str[i] = '\0';

		if ((numstr->data = const_search(numstr->str))) { /* A constant? */

			mvwprintw(outwin, 0, 0, "`%s' is %Le.", numstr->str, numstr->data);

			/* Convert the double to a string and make that numstr->str. */
			snprintf(line, NUMSTR_BUFSIZE, "%Lg", numstr->data);
			strncpy(numstr->str, line, NUMSTR_BUFSIZE);
			for (i = strlen(numstr->str); i < NUMSTR_BUFSIZE - 1; i++)
				numstr->str[i] = ' '; /* Pad the str back out. */
			numstr->str[NUMSTR_BUFSIZE] = '\0';
			numstr->type = CONSTANT;


		} else {
			/* TODO: Parse other commands.
			 * Stack rotations, popping, and actual math. */
			mvwprintw(outwin, 0, 0, "Invalid cmd: '%s'.", numstr->str);
		}
	}

	wrefresh(outwin);
}

static void handle_fields(WINDOW *outwin, struct num_str *numstr,
		int field, struct stack *stack)
{
	switch (field) {
		case 0:
			push_stack(stack, numstr);
			break;

		case 1:
			fprintf(stderr, "Got a cmd: %s.\n", numstr->str); /* DEBUG */

			if (numstr->type == CMD) /* be sure that this is a cmd */
				run_cmd(outwin, numstr);

			if (numstr->type != CMD) /* cmd resulted in a number */
				push_stack(stack, numstr);
			else
				free(numstr);

			break;

		default:
			break;
	}
}

void scicalc(WINDOW *outwin, WINDOW *infowin)
{
	int ok, num, i, frows, fcols, fy, fx, push, field;
	int oldpos[2];
	struct num_str *numstr;
	struct stack *stack = NULL;
	char *fdata, *fcmd;
	FORM *form = NULL;
	FIELD *number[3];
	WINDOW *stackwin = NULL;
	fdata = fcmd = NULL;

	/* Positions for the number prompt. */
	fy = getmaxy(outwin) - 5;
	fx = (getmaxx(outwin) - SCICALC_NUMLEN) / 2;

	/* Allocate fields. */
	if (!(number[0] = new_field(1, SCICALC_NUMLEN, fy + 1, fx + 1, 0, 0))) {
		wprintw(outwin, "Could not create a new FIELD *! (0)\n");
		ok = 0;
	} if (!(number[1] = new_field(1, SCICALC_NUMLEN, fy + 3, fx + 1, 0, 0))) {
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

	/* 0: num */
	set_field_just(number[0], JUSTIFY_LEFT);
	set_field_type(number[0], TYPE_REGEXP, SCICALC_NUM_REGEXP);

	field_opts_off(number[0], O_AUTOSKIP);
	field_opts_off(number[0], O_NULLOK);
	field_opts_on(number[0], O_BLANK);

	set_field_fore(number[0], COLOR_PAIR(SCIWIN_COLOR_PROMPT) | A_STANDOUT);
	set_field_back(number[0], A_UNDERLINE);

	if ((fdata = field_buffer(number[0], 0)) != E_OK) /* Use for cmd data. */
		wprintw(outwin, "Bad field buffer: 1st field, buf 0.");

	/* 1: cmd */
	set_field_just(number[1], JUSTIFY_LEFT);
	/* set_field_type(number[1], TYPE_ALPHA, 0); */

	field_opts_off(number[1], O_AUTOSKIP);
	field_opts_off(number[1], O_NULLOK);
	field_opts_on(number[1], O_BLANK);

	set_field_fore(number[1], COLOR_PAIR(SCIWIN_COLOR_PROMPT) | A_STANDOUT);
	set_field_back(number[1], A_UNDERLINE);

	if ((fcmd = field_buffer(number[1], 0)) != E_OK) /* Use for cmd data. */
		wprintw(outwin, "Bad field buffer: 2nd field, buf 0.");

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
	box(derwin(outwin, 5, SCICALC_NUMLEN + 6, fy, fx - 2), 0, 0);
	wmove(outwin, fy + 1, fx + 1);

	/* Stack */
	refresh_stack(outwin, stackwin, stack);

	/* Title */
	mvwprintw(outwin, 1, (getmaxx(outwin) - strlen(title)) / 2, "%s", title);

	/* Elements in memory */
	init_elements();

	/* Done with init */
	wmove(outwin, fy + 1, fx + 1); /* Go to start of form */
	wrefresh(outwin);

	num = ' ';
	ok = 1;
	i = push = field = 0;

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
				push = 1;
				break;

			case KEY_DOWN: /* Down */
				form_driver(form, REQ_NEXT_FIELD);
				form_driver(form, REQ_BEG_LINE);
				field = 1;
				break;

			case KEY_UP: /* Up */
				form_driver(form, REQ_PREV_FIELD);
				form_driver(form, REQ_BEG_LINE);
				field = 0;
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
				form_driver(form, REQ_PREV_CHAR);
				form_driver(form, REQ_DEL_CHAR);
				break;

			default: /* Otherwise just print the char */
				form_driver(form, num);
				break;
		}

		/* Validate form */
		if (form_driver(form, REQ_VALIDATION) != E_INVALID_FIELD /* Good number */
				&& !(field == 1 && fcmd[0] == ' ')) { /* No blank cmds */
			getyx(outwin, oldpos[0], oldpos[1]);
			mvwprintw(outwin, fy + 2, fx + 5, "        ");
			wmove(outwin, oldpos[0], oldpos[1]);

			/* Maybe push the form */
			if (push) {
				push = 0;
				if (!(numstr = get_num_str((field == 0) ? fdata : fcmd))) {
					getyx(outwin, oldpos[0], oldpos[1]);
					mvwprintw(outwin, 0, 0,
							"Unable to allocate memory for a num_str!\n");
					wmove(outwin, oldpos[0], oldpos[1]);
				} else {
					getyx(outwin, oldpos[0], oldpos[1]);
					mvwprintw(outwin, 0, 0,
							"(f:%d) Got num_str: %s (fields are %s // %s).",
							field, numstr->str, fdata, fcmd);
					wmove(outwin, oldpos[0], oldpos[1]);
					handle_fields(infowin, numstr, field, stack);
				}
			}
		} else { /* Form is invalid */
			getyx(outwin, oldpos[0], oldpos[1]);
			mvwprintw(outwin, fy + 2, fx + 5, "Invalid!");
			wmove(outwin, oldpos[0], oldpos[1]);
		}

		refresh_stack(outwin, stackwin, stack);
		wrefresh(outwin);

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

	end_elements();
	werase(outwin);
}

