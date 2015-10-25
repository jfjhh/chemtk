AUTOMAKE_OPTIONS = -Wportability gnu dist-xz subdir-objects

# what flags you want to pass to the C compiler & linker
AM_CFLAGS  = -std=c99 -Wall -Wextra -pedantic -rdynamic \
	     -O2 -g -DDATA_DIR=\"$(pkgdatadir)\"
AM_LDFLAGS =

# the commands shared library to be dlsym'd.
pkglib_LTLIBRARIES       = libsccommands.la
libsccommands_la_LDFLAGS = -module -avoid-version -shared
libsccommands_la_SOURCES = command_interface.h token_interface \
			   command_commands.c command_commands.h \
			   element_commands.c element_commands.h \
			   ../element.c ../element.h \ ../command.h \
			   ../constant.h ../ldmath.h \ ../operator.h

