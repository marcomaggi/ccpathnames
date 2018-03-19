## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL configure.sh		\
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

#page
#### libraries

ccpathnames_CURRENT		= @ccpathnames_VERSION_INTERFACE_CURRENT@
ccpathnames_REVISION		= @ccpathnames_VERSION_INTERFACE_REVISION@
ccpathnames_AGE			= @ccpathnames_VERSION_INTERFACE_AGE@

include_HEADERS			= src/ccpathnames.h

lib_LTLIBRARIES			= libccpathnames.la
libccpathnames_la_CPPFLAGS	= $(CCEXCEPTIONS_CFLAGS)
libccpathnames_la_LDFLAGS	= -version-info $(ccpathnames_CURRENT):$(ccpathnames_REVISION):$(ccpathnames_AGE) \
				  $(CCEXCEPTIONS_LIBS)
libccpathnames_la_SOURCES	= \
	src/ccpathnames.h		\
	src/ccpathnames-internals.h	\
	src/ccpathnames-version.c	\
	src/ccpathnames-conditions.c	\
	src/ccpathnames-memory.c	\
	src/ccpathnames-core.c		\
	src/ccpathnames-manipulation.c	\
	src/ccpathnames-components.c	\
	src/ccpathnames-input-output.c

#page
#### libraries

check_LTLIBRARIES	= libcctests.la
libcctests_la_CPPFLAGS	= $(CCEXCEPTIONS_CFLAGS) \
	-Dcctests_VERSION_INTERFACE_STRING='"0.0.0"'	\
	-Dcctests_VERSION_INTERFACE_CURRENT=0		\
	-Dcctests_VERSION_INTERFACE_REVISION=0		\
	-Dcctests_VERSION_INTERFACE_AGE=0
libcctests_la_LDFLAGS	= $(CCEXCEPTIONS_LIBS)
libcctests_la_SOURCES	= \
	cctests/cctests.h			\
	cctests/cctests-internals.h		\
	cctests/cctests-version.c		\
	cctests/cctests-conditions.c		\
	cctests/cctests-exception-handlers.c	\
	cctests/cctests-system.c		\
	cctests/cctests-driver.c		\
	cctests/cctests-assertions.c		\
	cctests/cctests-process.c		\
	cctests/cctests-helpers.c

#page
#### tests

check_PROGRAMS	= \
	tests/version		\
	tests/conditions	\
	tests/core		\
	tests/manipulations	\
	tests/components

TESTS				= $(check_PROGRAMS)

ccpathnames_tests_cppflags	= -I$(srcdir)/src $(CCEXCEPTIONS_CFLAGS) -I$(srcdir)/cctests
ccpathnames_tests_ldadd		= libccpathnames.la $(CCEXCEPTIONS_LIBS) libcctests.la

tests_version_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_version_LDADD		= $(ccpathnames_tests_ldadd)

tests_conditions_CPPFLAGS	= $(ccpathnames_tests_cppflags)
tests_conditions_LDADD		= $(ccpathnames_tests_ldadd)

tests_core_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_core_LDADD		= $(ccpathnames_tests_ldadd)

tests_manipulations_CPPFLAGS	= $(ccpathnames_tests_cppflags)
tests_manipulations_LDADD	= $(ccpathnames_tests_ldadd)

tests_components_CPPFLAGS	= $(ccpathnames_tests_cppflags)
tests_components_LDADD		= $(ccpathnames_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done

#page
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

#page
#### Coverity support
#
# We can visit the project site at:
#
#    <https://scan.coverity.com/projects/marcomaggi-ccpathnames>
#
# Remember to  put in  the PATH  the directory  in which  the Coverity
# tools are installed; for example:
#
#    $ export PATH=/opt/coverity/8.7.0/cov-analysis-linux64-8.7.0/bin:$PATH
#
# The tool  will use  a temporary directory  "$(builddir)/cov-int"; we
# must *not* change this pathname.
#
# To generate and submit a report we must do:
#
#    $ make clean
#    $ make coverity-scan
#    $ make coverity-archive
#    $ make coverity-upload
#
# after "make  coverity-scan" we should  check that the last  lines of
# the file "cov-int/build-log.txt" report a successful build:
#
#    $ tail cov-int/build-log.txt
#
# NOTE If  your Linux kernel  is at version 4.8  or above you  have to
# first issue the command:
#
#    $ sudo /sbin/sysctl vsyscall=emulate
#
# to make the Coverity tools work.
#

CCPATHNAMES_COVERITY_TOKEN_FILE = /home/unsafe-users/marco-devel/.coverity-ccpathnames-token

.PHONY: coverity-scan coverity-archive coverity-upload

coverity-scan:
	cov-build --dir cov-int make -j 4

coverity-archive:
	tar caf ccpathnames.xz cov-int

coverity-upload:
	curl --form token=$$(<$(CCPATHNAMES_COVERITY_TOKEN_FILE)) \
	     --form email='marco.maggi-ipsu@poste.it' \
	     --form file='@ccpathnames.xz' \
	     --form version="$(VERSION)" \
	     --form description="A build description" \
	     'https://scan.coverity.com/builds?project=marcomaggi%2Fccpathnames'

#page
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
