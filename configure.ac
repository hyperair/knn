#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([kNN], [0.1], [hyperair@gmail.com])
AC_CONFIG_SRCDIR([main.cc])

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])
AC_CHECK_HEADERS([boost/program_options.hpp], [],
                 [AC_MSG_ERROR([You need the Boost.Program_Options library])])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
