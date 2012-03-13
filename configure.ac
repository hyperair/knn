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
AX_BOOST_BASE([1.46])
AX_BOOST_PROGRAM_OPTIONS

AC_ARG_ENABLE([warnings],
              [AS_HELP_STRING([--enable-warnings], [Enable extra warning flags])],,
              [enable_warnings=no])

AM_CONDITIONAL([ENABLE_WARNINGS], [test "$enable_warnings" = "yes"])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
