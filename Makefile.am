## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL			\
			  meta/slackstuff/config.in	\
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README COPYING
AM_CFLAGS		= -I$(srcdir)/src $(AX_CFLAGS)

## --------------------------------------------------------------------

EXTRA_DIST		+= meta/scripts/ccpathnames.pc.in
pkgconfigdir		= $(libdir)/pkgconfig
nodist_pkgconfig_DATA	= meta/scripts/ccpathnames.pc

## --------------------------------------------------------------------

AM_MAKEINFOFLAGS	= --no-split

info_TEXINFOS		= doc/ccpathnames.texi
doc_ccpathnames_TEXINFOS	= \
	doc/macros.texi		\
	doc/lgpl-3.0.texi	\
	doc/fdl-1.3.texi


#### libraries

ccpathnames_CURRENT		= @ccpathnames_VERSION_INTERFACE_CURRENT@
ccpathnames_REVISION		= @ccpathnames_VERSION_INTERFACE_REVISION@
ccpathnames_AGE			= @ccpathnames_VERSION_INTERFACE_AGE@

include_HEADERS			= src/ccpathnames.h

lib_LTLIBRARIES			= libccpathnames.la
libccpathnames_la_CPPFLAGS	= $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS)
libccpathnames_la_LDFLAGS	= -version-info $(ccpathnames_CURRENT):$(ccpathnames_REVISION):$(ccpathnames_AGE) \
				  $(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS)
libccpathnames_la_SOURCES	= \
	src/ccpathnames.h			\
	src/ccpathnames-internals.h		\
	src/ccpathnames-version.c		\
	src/ccpathnames-conditions.c		\
	src/ccpathnames-input-representation.c


# src/ccpathnames-core.c			\
# src/ccpathnames-manipulation.c		\
# src/ccpathnames-components.c		\
# src/ccpathnames-input-output.c


#### tests

check_PROGRAMS	= \
	tests/version			\
	tests/conditions		\
	tests/input-representation


# tests/core			\
# tests/manipulations		\
# tests/components

if MMUX_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccpathnames_tests_cppflags	= -I$(srcdir)/src $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS) $(CCTESTS_CFLAGS)
ccpathnames_tests_ldadd		= libccpathnames.la $(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS) $(CCTESTS_LIBS)

## --------------------------------------------------------------------

tests_version_CPPFLAGS			= $(ccpathnames_tests_cppflags)
tests_version_LDADD			= $(ccpathnames_tests_ldadd)

tests_conditions_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_conditions_LDADD			= $(ccpathnames_tests_ldadd)

tests_input_representation_CPPFLAGS	= $(ccpathnames_tests_cppflags)
tests_input_representation_LDADD	= $(ccpathnames_tests_ldadd)

tests_core_CPPFLAGS			= $(ccpathnames_tests_cppflags)
tests_core_LDADD			= $(ccpathnames_tests_ldadd)

tests_manipulations_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_manipulations_LDADD		= $(ccpathnames_tests_ldadd)

tests_components_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_components_LDADD			= $(ccpathnames_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### testing under GDB

$(builddir)/.gdbinit:
	ln -sv "$(top_srcdir)/gdbinit" "$(builddir)/.gdbinit"

# This rule is meant to be used as:
#
#   $ make all gdb-test FILE=tests/inspection CFLAGS='-g'
#
# after having created an appropriate file "$(top_srcdir)/.gdbinit".
#
gdb-test: $(builddir)/.gdbinit
	libtool --mode=execute gdb $$FILE


#### Static analysis with Clang's Static Analyzer
#
# See the documentation for the command line tool at:
#
#    <http://clang-analyzer.llvm.org/scan-build.html>
#
# To run the tool we must do:
#
#    $ make clean
#    $ make clang-static-analysis
#
# The  program  "scan-build"  works  by  overriding  the  CC  and  CXX
# environment variables.  Automake defines CXX as:
#
#    CXX = g++ -std=c++11
#
# so we override CXXFLAGS to include the language specification.
#

.PHONY: clang-static-analysis

clang-static-analysis:
	scan-build make CXXFLAGS='-std=c++11'

### end of file
