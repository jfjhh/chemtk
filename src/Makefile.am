# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -pedantic -Wall -Wextra -O2 -DDATA_DIR=\"$(pkgdatadir)\"
AM_LDFLAGS =

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = chemtk
chemtk_SOURCES = chemtk.c chemtk.h constants.c constants.h directory.c \
		 directory.h elements.c elements.h num_str.c num_str.h \
		 pager.c pager.h runtests.c runtests.h scicalc.c \
		 scicalc.h stack.c stack.h

