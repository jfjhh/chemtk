#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([chemtk], [0.0.1], [https://github.com/jfjhh/chemtk/issues])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_ARG_VAR([UNXZ], [Program used to decompress archive of program data.])
AC_CHECK_PROG([UNXZ], [unxz], [/usr/bin/unxz], [unxz])
AC_ARG_VAR([TAR], [Program used to unarchive program data.])
AC_CHECK_PROG([TAR], [tar], [/bin/tar], [tar])

# Checks for libraries.
AC_CHECK_LIB([dl], [dlopen])
AC_CHECK_LIB([m], [cos])
AC_CHECK_LIB([readline], [readline])
AC_CHECK_LIB([history], [using_history])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h stdarg.h dlfcn.h string.h ctype.h math.h \
		  float.h unistd.h sys/types.h dirent.h limits.h sys/wait.h \
		  sys/time.h sys/resource.h assert.h stdbool.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow strndup])

AC_OUTPUT([Makefile src/Makefile doc/Makefile man/Makefile data/Makefile])
