# what flags you want to pass to the C compiler & linker
AM_CFLAGS  = --std=c99 -pedantic -Wall -Wextra -O2 -DDATA_DIR=\"$(pkgdatadir)\"
AM_LDFLAGS =

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS   = chemtk
chemtk_SOURCES = chemtk.c chemtk.h command.c command.h config.h constant.c \
		 constant.h directory.c directory.h dirs.h element.c element.h \
		 ldmath.c ldmath.h Makefile.am operator.c operator.h pager.c \
		 pager.h scicalc.c scicalc.h stack.c stack.h tests.c tests.h \
		 token.c token.h

