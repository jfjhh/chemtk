AUTOMAKE_OPTIONS = -Wportability gnu dist-xz subdir-objects

# what flags you want to pass to the C compiler & linker
AM_CFLAGS  = -std=c99 -Wall -Wextra -pedantic -rdynamic \
	     -O2 -g -DDATA_DIR=\"$(pkgdatadir)\" \
	     -DCOMMAND_LIB=\"$(pkglibdir)/libsccommands.so\"
AM_LDFLAGS =

SUBDIRS = commands

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS   = chemtk
chemtk_SOURCES = chemtk.c chemtk.h command.c command.h config.h constant.c \
		 constant.h directory.c directory.h dirs.h element.c element.h \
		 ldmath.c ldmath.h Makefile.am operator.c operator.h pager.c \
		 pager.h scicalc.c scicalc.h stack.c stack.h token.c token.h \
		 commands/token_interface.h commands/command_interface.h \
		 input.c input.h parse.c parse.h yaml_macros.h arguments.h
chemtk_LDADD   = commands/libsccommands.la

# testing programs
noinst_PROGRAMS = check_chemtk
TESTS           = check_chemtk

# test sources to build
check_chemtk_SOURCES = check_chemtk.c check_chemtk.h chemtk.h command.c \
		       command.h config.h constant.c constant.h directory.c \
		       directory.h dirs.h element.c element.h ldmath.c \
		       ldmath.h Makefile.am operator.c operator.h pager.c \
		       pager.h scicalc.c scicalc.h stack.c stack.h token.c \
		       token.h commands/token_interface.h \
		       commands/command_interface.h input.c input.h parse.c \
		       parse.h yaml_macros.h arguments.h
check_chemtk_LDADD   = commands/libsccommands.la

CHECK_CFLAGS = -DCHECK_CHEMTK_FLAG=1
check_chemtk_CFLAGS  = $(CFLAGS) $(AM_CFLAGS) $(CHECK_CFLAGS)

