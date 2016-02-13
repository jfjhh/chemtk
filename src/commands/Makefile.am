AUTOMAKE_OPTIONS = -Wportability gnu dist-xz subdir-objects

include $(top_srcdir)/src/Directories.am

# what flags you want to pass to the C compiler & linker
AM_CFLAGS  = -std=c99 -Wall -Wextra -pedantic -rdynamic \
	     -O2 -g -DDATA_DIR=\"$(CT_DATA_DIR)\"
AM_LDFLAGS =

# Common to ChemTK as a 'whole', as well as to libsccommands.
nodist_common_SOURCES    = ../element.c ../element.h ../command.h \
			   ../constant.h ../ldmath.h ../operator.h

# the commands shared library to be dlsym'd.
pkglib_LTLIBRARIES       = libsccommands.la
libsccommands_la_LDFLAGS = -module -avoid-version -shared
libsccommands_la_SOURCES = command_interface.h token_interface.h \
			   command_commands.c command_commands.h \
			   element_commands.c element_commands.h \
			   $(common_SOURCES)


