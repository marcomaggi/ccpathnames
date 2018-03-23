dnl @configure_input@
dnl

AC_PREREQ([2.69])
AC_INIT([CCPathnames],[0.1.0-beta.0],
  [marco.maggi-ipsu@poste.it],
  [ccpathnames],[http://marcomaggi.github.com/ccpathnames])
dnl This is the revision number of this configure.ac
AC_REVISION([0.1])
AC_COPYRIGHT([Copyright (c) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

This program is free software: you  can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free  Software Foundation, either version  3 of the License,  or (at
your option) any later version.

This program  is distributed  in the  hope that it  will be  useful, but
WITHOUT   ANY   WARRANTY;  without   even   the   implied  warranty   of
MERCHANTABILITY  or  FITNESS FOR  A  PARTICULAR  PURPOSE.  See  the  GNU
General Public License for more details.

You should have received a copy  of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_MACRO_DIR([meta/autotools])
AC_CONFIG_AUX_DIR([meta/autotools])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign dist-xz no-dist-gzip subdir-objects])
AM_MAINTAINER_MODE

AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

LT_PREREQ([2.4])
LT_INIT

dnl page
#### configuration

AS_VAR_SET([CCSYS_ON_LINUX], [0])
AS_VAR_SET([CCSYS_ON_BSD],   [0])
AS_VAR_SET([CCSYS_ON_CYGWIN],[0])
AS_VAR_SET([CCSYS_ON_DARWIN],[0])

AS_CASE("$target_os",
  [*linux*],
  [AS_VAR_SET([CCSYS_ON_LINUX],[1])
   AC_MSG_NOTICE([detected OS: linux])],
  [*bsd*],
  [AS_VAR_SET([CCSYS_ON_BSD],[1])
   AC_MSG_NOTICE([detected OS: BSD])],
  [*cygwin*],
  [AS_VAR_SET([CCSYS_ON_CYGWIN],[1])
   AC_MSG_NOTICE([detected OS: CYGWIN])],
  [*darwin*],
  [AS_VAR_SET([CCSYS_ON_CYGWIN],[1])
   AC_MSG_NOTICE([detected OS: DARWIN])])
AM_CONDITIONAL([ON_LINUX], [test "x$CCSYS_ON_LINUX"  = x1])
AM_CONDITIONAL([ON_BSD],   [test "x$CCSYS_ON_BSD"    = x1])
AM_CONDITIONAL([ON_CYGWIN],[test "x$CCSYS_ON_CYGWIN" = x1])
AM_CONDITIONAL([ON_DARWIN],[test "x$CCSYS_ON_DARWIN" = x1])

AC_SUBST([CCSYS_ON_LINUX], [$CCSYS_ON_LINUX])
AC_SUBST([CCSYS_ON_BSD],   [$CCSYS_ON_BSD])
AC_SUBST([CCSYS_ON_CYGWIN],[$CCSYS_ON_CYGWIN])
AC_SUBST([CCSYS_ON_DARWIN],[$CCSYS_ON_DARWIN])

dnl page
#### libraries interface version

AS_VAR_SET([SLACKSTUFF_PACKAGE_VERSION],[0.1.0])
AC_SUBST([SLACKSTUFF_PACKAGE_VERSION])

dnl This is the version stored in the pkg-config data file.
AC_SUBST([CCPATHNAMES_PKG_CONFIG_VERSION],[0.1])

MM_LIBTOOL_LIBRARY_VERSIONS([ccpathnames],1,0,0)

dnl page
#### external libraries

dnl Testing presence of pkg-config
AC_MSG_CHECKING([availability of pkg-config m4 macros])
AS_IF([test m4_ifdef([PKG_CHECK_MODULES],[yes],[no]) == yes],
  [AC_MSG_RESULT([yes])],
  [AC_MSG_RESULT([no])
   AC_MSG_ERROR([pkg-config is required.  See pkg-config.freedesktop.org])])

PKG_CHECK_MODULES([CCEXCEPTIONS],[ccexceptions >= 0.6.4])

AC_CACHE_SAVE

dnl page
#### basic system inspection

AC_LANG([C])
AC_PROG_CC_C99
AX_CHECK_COMPILE_FLAG([-std=c11],[],[AC_MSG_ERROR([*** Compiler does not support -std=c11])],[-pedantic])

AS_VAR_SET(AX_CFLAGS,[-std=c11])
AS_VAR_APPEND(AX_CFLAGS,[" -Wall -Wextra"])
AX_GCC_VERSION
AS_IF([test "x$GCC" = "xyes"],
  [AS_CASE("$GCC_VERSION",
     [7.*],[AS_VAR_APPEND(AX_CFLAGS,[" -pedantic -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference -Wjump-misses-init -Wdouble-promotion -Wshadow -Wformat=2 -Wmisleading-indentation"])])])
AC_SUBST(AX_CFLAGS)

AM_PROG_AS
AM_PROG_CC_C_O
AC_HEADER_STDC
AC_CHECK_HEADERS([assert.h string.h limits.h errno.h unistd.h])

AC_CACHE_SAVE

dnl page
#### checks for library functions

AC_CHECK_FUNCS([realpath])

dnl This is used by CCTests.
AC_CHECK_FUNCS([isatty])

AC_CACHE_SAVE

dnl page
#### finish

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile]
   [meta/scripts/ccpathnames.pc]
   [meta/slackstuff/config]
   [meta/slackware/slack-desc])
AC_OUTPUT

### end of file
# Local Variables:
# mode: autoconf
# page-delimiter: "^dnl page"
# End:
