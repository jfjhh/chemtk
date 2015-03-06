#include "scicalc.h"

const char *title = "Scicalc: A scientific calculator utility.";

static int least(int a, int b)
{
	if (a < b)
		return a; /* a is least. */
	else if (a > b)
		return b; /* b is least. */
	else
		return a; /* Same, so a or b doesn't matter. */
}

int operate(WINDOW *outwin, struct stack *stack)
{
	char operation;
	struct num_str *a, *b, *c, *op;
	int strbool;
	strbool = 0;

	/* Get the op from the stack and free it */
	if (!(op = pop_stack(stack)))
		return 0;

	operation = op->str[0];
	free(op);

	if (!(c = (struct num_str *) malloc(sizeof(struct num_str))))
		return 0;

	/* Pop from the stack. */
	if (operation != '~') {
		/* This is the only unary op, so all the others are binary and need two
		 * num_strs to be popped. */
		b = pop_stack(stack);
		a = pop_stack(stack);
	} else { /* Operation is unary. */
		a = pop_stack(stack);
		b = a;
	}

	/* Check pop(s) went alright. */
	if (!(a && b))
		return 0;

	/* Get sig. figs. for result based on operation. */
	switch (operation) {
		case '+':
		case '-':
			c->sig_figs = least(sig_after(a->str), sig_after(b->str));
			break;
		default: /* Same as for multiply/divide, and other ops. */
			c->sig_figs = least(a->sig_figs, b->sig_figs);
			break;
	}

	switch (operation) {
		case '+':
			c->data = a->data + b->data;
			break;
		case '-':
			c->data = a->data - b->data;
			break;
		case '*':
			c->data = a->data * b->data;
			break;
		case '/':
			c->data = a->data / b->data;
			break;
		case '%':
			c->data = LDTOI(a->data) % LDTOI(b->data);
			break;
		case '&':
			c->data = LDTOI(a->data) & LDTOI(b->data);
			break;
		case '|':
			c->data = LDTOI(a->data) | LDTOI(b->data);
			break;
		case 'x':
			c->data = LDTOI(a->data) ^ LDTOI(b->data);
			break;
		case 'l':
			c->data = LDTOI(a->data) << LDTOI(b->data);
			break;
		case 'r':
			c->data = LDTOI(a->data) >> LDTOI(b->data);
			break;
		case '>':
			c->data = (a->data > b->data) ? 1.0L : 0.0L;
			break;
		case '<':
			c->data = (a->data < b->data) ? 1.0L : 0.0L;
			break;
		case '=':
			c->data = (a->data == b->data) ? 1.0L : 0.0L;
			break;
		case '^':
			c->data = powl(a->data, b->data);
			break;
		case 'o':
			c->data = logl(a->data);
			break;
		default:
			c->data = 0.0L;
			break;
	}

	/* Free a and b */
	if (b == a) {
		free(a);
	} else {
		free(a);
		free(b);
	}

	/* Fix c->str */
	if (strbool) {
		if (c->data == 1.0L)
			strncpy(c->str, "True", NUMSTR_BUFSIZE);
		else
			strncpy(c->str, "False", NUMSTR_BUFSIZE);
	} else {
		sscanf(c->str, "%Le", &(c->data));
	}

	wprintw(outwin, "Got: %s (%Le)", c->str, c->data);
	push_stack(stack, c);

	return 1;
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

static int run_cmd(WINDOW *outwin, struct num_str *numstr, struct stack *stack)
{
	struct element *e;
	int i;
	char mod, *sym, line[NUMSTR_BUFSIZE];
	WINDOW *infowin = NULL;

	werase(outwin);

	if (is_operator(numstr->str[0])) {
		/* The command is to perform a (math) operation on the stack. */
		wprintw(outwin, "Running an operation.\n");
		if (!(operate(outwin, stack)))
			return 0; /* Operation failed. */
	} else if (numstr->str[0] == 'e') { /* Only if this is an element command. */

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

	} else { /* Not an element cmd */
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

			wrefresh(outwin);

		} else {
			/* TODO: Parse other commands.
			 * Stack rotations, popping, and actual math. */
			return 0;
		}
	}

	return 1;
}

static int handle_fields(WINDOW *outwin, struct num_str *numstr,
		struct stack *stack)
{
	int status = 1;

	/* Be sure that this is a cmd or operation. */
	if (numstr->type == CMD || numstr->type == OPERATOR) {
		wprintw(outwin, "handle_fields got a CMD or OPERATOR type numstr.\n");
		status = run_cmd(outwin, numstr, stack);
	} else {
		/* TODO: Check for a valid number using regexp. */
	}

	if (status) {
		if (numstr->type != CMD) /* cmd resulted in a number */
			push_stack(stack, numstr);
		else
			free(numstr);
	}

	return status;
}

void scicalc(WINDOW *outwin, WINDOW *infowin)
{
	int ok, num, i, frows, fcols, fy, fx, push, valid, op;
	int oldpos[2];
	struct num_str *numstr;
	struct stack *stack = NULL;
	char *fstr, opstr[2];
	FORM *form = NULL;
	FIELD *number[2];
	WINDOW *stackwin = NULL;
	fstr = NULL;
	opstr[1] = '\0';

	/* Positions for the number prompt. */
	fy = getmaxy(outwin) - 5;
	fx = (getmaxx(outwin) - SCICALC_NUMLEN) / 2;

	/* Allocate fields. */
	if (!(number[0] = new_field(1, SCICALC_NUMLEN, fy + 1, fx + 1, 0, 0))) {
		wprintw(outwin, "Could not create a new FIELD *! (0)\n");
		ok = 0;
	}
	number[1] = NULL; /* NULL-terminate the field array. */

	/* Allocate stack. */
	if (!(stack = new_stack())) {
		wprintw(outwin, "Could not create a new struct stack *! (0)\n");
		ok = 0;
	}

	/* Configure the input fields. */
	/* 0: fstr */
	set_field_just(number[0], JUSTIFY_LEFT);

	field_opts_off(number[0], O_AUTOSKIP);
	field_opts_off(number[0], O_NULLOK);
	field_opts_on(number[0], O_BLANK);

	set_field_fore(number[0], COLOR_PAIR(SCIWIN_COLOR_PROMPT) | A_STANDOUT);
	set_field_back(number[0], A_UNDERLINE);

	if ((fstr = field_buffer(number[0], 0)) != E_OK) /* Use for cmd data. */
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
	box(derwin(outwin, 4, SCICALC_NUMLEN + 6, fy, fx - 2), 0, 0);
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
	i = push = valid = op = 0;

	while (ok && (num = wgetch(outwin))) {
		if (num == 'Q' || num == KEY_F(1))
			break;

		switch (num) { /* Field controls */
			case KEY_ENTER: /* Toggle field */
			case '\n':
				push = 1;
				break;

			case KEY_LEFT:/* Left */
				form_driver(form, REQ_PREV_CHAR);
				break;

			case KEY_RIGHT: /* Right */
				form_driver(form, REQ_NEXT_CHAR);
				break;

			case KEY_BACKSPACE: /* Backspace */
			case '\b':
				form_driver(form, REQ_PREV_CHAR);
				form_driver(form, REQ_DEL_CHAR);
				break;

			default: /* Otherwise check if it is an op or print the char */
				if (!(op = is_operator(num))) {
					form_driver(form, num);
					op = 0;
				} else {
					wprintw(infowin, "Form read an op.\n");
					op = 1;
					opstr[0] = num;
				}
				break;
		}

		/* Validate form */
		if (form_driver(form, REQ_VALIDATION) != E_INVALID_FIELD /* Good number */
				&& isalnum(fstr[0])) { /* No blank cmds */
			valid = 1;

			getyx(outwin, oldpos[0], oldpos[1]);
			mvwprintw(outwin, fy + 2, fx + 5, "        ");
			wmove(outwin, oldpos[0], oldpos[1]);

			/* Maybe push the form */
			if (push) {
				/* Either an op str or the normal field str. This is needed so
				 * that an operator can be processed without the press of return
				 * by the user. When this happens, fstr is blank, and causes
				 * errors by get_num_str() in the future. */
				if (!(numstr = get_num_str((op) ? opstr : fstr))) {
					getyx(outwin, oldpos[0], oldpos[1]);
					mvwprintw(outwin, 0, 0,
							"Unable to allocate memory for a num_str!\n");
					wmove(outwin, oldpos[0], oldpos[1]);
				} else {
					getyx(outwin, oldpos[0], oldpos[1]);
					mvwprintw(outwin, 0, 0,
							"Got num_str: %s (field is %s).",
							numstr->str, (op) ? opstr : fstr);
					wmove(outwin, oldpos[0], oldpos[1]);
					valid = handle_fields(infowin, numstr, stack);
				}

				push = 0;
				form_driver(form, REQ_CLR_FIELD);
			}
		} else { /* Form is invalid */
			valid = 0;
		}

		getyx(outwin, oldpos[0], oldpos[1]);
		mvwprintw(outwin, fy + 2, fx + 1,
				((valid) ? "      OK        " : "                "));
		wmove(outwin, oldpos[0], oldpos[1]);

		refresh_stack(outwin, stackwin, stack);
		wrefresh(outwin);
		wrefresh(infowin);

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

