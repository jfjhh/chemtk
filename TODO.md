# TODO

- Everything.
- Fix autoconf to define where installed data is and have files include paths
  relative to that; e.g. `#define SCICALC_DOCFILE DATA_DIR "/doc/scicalc.txt"`,
  or something similar, where `DATA_DIR` is from some autoconf magic.
- Line-oriented number entry.
- Stack display.
- Calculate with something like this:
```
push_stack(stack, calculate(pop_stack(stack), pop_stack(stack), op));
```
- Right arrow key to switch between number entry and operating.

