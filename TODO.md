# TODO

### Do it now!

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

