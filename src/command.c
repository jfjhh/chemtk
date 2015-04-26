#include "command.h"

int run_cmd(const char *line, sc_token *token, FILE *logfile)
{
	struct element *e;
	int i;
	char mod, *sym;

	/* Token defaults. */
	token->type = NONE;
	memset(token->data.cmd, '\0', SC_TOKEN_INLEN);

	if (line[0] == 'e') { /* Only if this is an element command. */

		/* No errors if strndup fails,
		 * because search may work without this part. */
		if ((sym = strndup((line) + 2, ELEMENT_SYM_LEN))) {
			for (i = 1; i < ELEMENT_SYM_LEN; i++)
				if (!isalpha(sym[i]))
					sym[i] = '\0';
			sym[0] = toupper(sym[0]);
		} else {
			sym = NULL; /* Future free on NULL is safe. */
		}

		/* Get the element */
		if (!(e = find_element_sym((sym) ? sym : ((line) + 2)))) {
			fprintf(logfile, "CMD: Cound not find element '%s'!",
					(sym) ? sym : ((line) + 2));

		} else { /* Success. */
			mod = line[1];

			if (mod == 'g' || mod == 'n') { /* Value of info / data. */
				token->type = CONSTANT;

				if (mod == 'g') {
					token->data.flt = (double) e->molar_mass;
					fprintf(logfile, "CMD: %s's molar mass is %f grams.",
							e->symbol, e->molar_mass);
				} else if (mod == 'n') {
					token->data.flt = (double) e->atomic_number;
					fprintf(logfile, "CMD: %s's atomic number is %d.",
							e->symbol, e->atomic_number);
				}

			} else if (mod == 'i') { /* Print info / data */
				print_element_info(e, logfile);
			} else { /* Print other element information anyway */
				print_element(e, logfile);
			}
		}

		free(sym);

	} else if (0) {
		/* Parse out if it is a different type of command other than an element
		 * command. */
	} else { /* Not a command; fail. */
		return 0;
	}

	return 1; /* Command successfully "executed." */
}

