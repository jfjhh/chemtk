#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([chemtk], [1.0], [https://github.com/jfjhh/chemtk/issues])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
AC_CHECK_LIB([m], [cos])
AC_CHECK_LIB([ncurses], [wprintw])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h stdio.h string.h unistd.h dirent.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow strndup])

AC_OUTPUT([Makefile src/Makefile doc/Makefile man/Makefile data/Makefile])
