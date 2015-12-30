#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([chemtk], [0.0.1], [https://github.com/jfjhh/chemtk/issues])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AM_INIT_AUTOMAKE

# Checks for programs.
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_ARG_VAR([UNXZ],     [Program used to decompress archive of program data.])
AC_CHECK_PROG([UNXZ],  [unxz], [/usr/bin/unxz],  [unxz])
AC_ARG_VAR([TAR],      [Program used to unarchive program data.])
AC_CHECK_PROG([TAR],   [tar],  [/bin/tar],       [tar])

# Check for presence of pdfLaTeX.
AC_CHECK_PROG(PDFLATEX, pdflatex, pdflatex)
AM_CONDITIONAL([HAVE_PDFLATEX], test -n "$PDFLATEX")
if test -z "$PDFLATEX"; then
	AC_MSG_WARN([Unable to create PDF version of documentation.])
fi

# Check for Doxygen.
AC_CHECK_PROG(DOXYGEN, doxygen, doxygen)
AM_CONDITIONAL([HAVE_DOXYGEN], test -n "$DOXYGEN")
if test -z "$DOXYGEN"; then
	AC_MSG_WARN([Unable to create Doxygen documentation.])
fi

# Checks for libraries.
AC_CHECK_LIB([dl],        [dlopen])
AC_CHECK_LIB([m],         [cos])
AC_CHECK_LIB([readline],  [readline])
AC_CHECK_LIB([history],   [using_history])
AC_CHECK_LIB([yaml],      [yaml_parser_parse])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h stdarg.h dlfcn.h string.h ctype.h math.h \
		  float.h unistd.h sys/types.h dirent.h limits.h sys/wait.h \
		  sys/time.h sys/resource.h assert.h stdbool.h yaml.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow strndup snprintf])

# Broken: Doxygen documentation.
# AC_OUTPUT([Makefile src/commands/Makefile src/Makefile doc/Makefile \
# 	   doc/commands/Makefile doc/doxygen/Makefile \
# 	   doc/doxygen/html/Makefile doc/doxygen/html/search/Makefile \
# 	   man/Makefile data/Makefile])

AC_OUTPUT([Makefile src/commands/Makefile src/Makefile doc/Makefile \
	   doc/commands/Makefile man/Makefile data/Makefile])
