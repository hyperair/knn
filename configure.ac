#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([kNN], [0.1], [hyperair@gmail.com])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/main.cc])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])
AC_CHECK_HEADERS([boost/program_options.hpp], [],
                 [AC_MSG_ERROR([You need the Boost.Program_Options library])])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
