# ChemTK

![Build Status](https://travis-ci.org/jfjhh/chemtk.svg?branch=master)

A toolkit of a few little programs for chemistry.
They include things like a scientific calculator, and element info lookup.

## Legal Note

Thanks to [The ASCII Periodic Table](http://ascii.periodni.com/index.html) for
individual element information (See [fair use
document](doc/legal/fair_use_periodni.pdf). The content provided by them is used
freely due to their [disclaimer](http://www.periodni.com/disclaimer.html):

> The copyright for any material created by the author is reserved.
> This material may be freely used and copied for educational and other
> non-commercial purposes, provided that any reproduction of data be accompanied
> by an acknowledgement of www.periodni.com as the source.

## TODO

### Do it now!

- Fix scicalc's operate() function to recalculate other significant figures upon evaluation.
	* Add some utility functions to `ldmath.c`.

- Make installation of data files not require the ridiculous archive,
  `data.tar.xz`, it's annoying to update and needs to be decompressed and
  unarchived twice in the process of installation, which would be bad if it grew
  to a bigger size.
	* Re-write `data/Makefile.am`.

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

