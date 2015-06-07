# TODO

### Do it now!

- Document all header files with Doxygen / code reveiew.

#### New Design

- Use prefix `sc_`.

- Move from `num_str` to `sc_token` (using similar to long `num_str`
  implementation below).

- Change `stack` to `sc_stack`, and have it accept `void *` type data.

- General design aspects:

	* Input goes in a `char *`, which is immediately tokenized into a `sc_token`
	  as a part of the input mechanism.

	* Type `CMD` is "executed" immediately, generating a `sc_token`.

		+ Maybe have a separate *thing*, like `commands.{c,h}` or similar to
		  process commands into `sc_token`s.

	* Display of numbers is in scientific notation with correct significant
	  figures, thanks to printf's `%- .*e` format specifier.

	* `sc_token`s should not have a `char *` field; the data needed to reproduce
	  the original string of the token should be present in the `sc_token`.

- Possibly change `num_str` to use a sort of tagged union:

```c
#define NUMSTR_BUFSIZE 81

/* Previous num_str structure. */
enum num_str_type {
	NONE = 0,
	NUMBER,
	CMD,
	CONSTANT,
	OPERATOR
};

struct num_str {
	long double data;
	int sig_figs;
	enum num_str_type type;
	char str[NUMSTR_BUFSIZE];
};

struct stack {
	struct num_str *data;
	struct stack *next;
};
```
```c
#define NUMSTR_BUFSIZE 81

/* Proposed num_str structure. */
struct num_str {
	enum type {
		INT,
		LONG,
		FLOAT,
		DOUBLE,
		NUM_STR,
		CMD,
		OPERATOR,
		NEXT,
		NONE
	};
	int sig_figs[3]; /* [0] == sig_figs,
					  * [1] == sig_before,
					  * [2] == sig_after. */
	union number {
		int;
		long;
		double;
		float;
	};
	char str[81];
	struct num_str *next;
};
```

- Test number validity with a regex.
- Actual math.
- Tab mode switching.
- Fix user-constants (e.g. `23.2.`).
- Update scicalc documentation file (also remove silly ops that override normal
  typing.

### Maybe later...

- Unit conversion interface (e.g. from `mol` to `liters`), with drawing of the
  dimensional analysis bars and correct units. Maybe expand to generic problems
  like stoichiometry given a balanced equation, reagents, and desired output.
- Menu system for data in infowin.
- Make the interface clickable (menus and click-number-entry);
- User-variables or scratch stack (to store values).
- History with up/down arrows.

