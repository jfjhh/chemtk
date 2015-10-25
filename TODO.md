# TODO

### commands.yml parsing procedure

- Ugh, maybe Unicode support all the things in the future.

### Do it now!

- Fix scicalc's operate() function to recalculate other significant figures upon
  evaluation.

- Update scicalc documentation file (also remove silly ops that override normal
  typing.

#### New Design

- Input goes in a `char *`, which is immediately tokenized into a `sc_token`
  as a part of the input mechanism.

- Type `CMD` is "executed" immediately, either replacing itself with
  type `NUMBER`, `CONSTANT` or `OPERATOR`, or with NONE if the command has
  no stack-related output.

- Display of numbers is in scientific notation with correct significant
  figures, thanks to printf's `%- .*e` format specifier.
	* Use a `sc_stack` print function.

- `sc_token`s should not have a `char *` field, unless they are commands;
   the data needed to reproduce
   the original string of the token should be present in the `sc_token`.

### Maybe later...

- Unit conversion interface (e.g. from `mol` to `liters`), with drawing of the
  dimensional analysis bars and correct units. Maybe expand to generic problems
  like stoichiometry given a balanced equation, reagents, and desired output.

- History with up/down arrows (libreadline).

