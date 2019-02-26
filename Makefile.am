## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL			\
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README COPYING
AM_CPPFLAGS		= -I$(srcdir)/src
AM_CFLAGS		= $(MMUX_CFLAGS)

## --------------------------------------------------------------------

nodist_pkgconfig_DATA	= meta/scripts/ccpathnames.pc


#### documentation

AM_MAKEINFOFLAGS	= --no-split
AM_MAKEINFOHTMLFLAGS	= --split=node -c WORDS_IN_PAGE=0 \
	-c PRE_BODY_CLOSE="<p>This document describes version <tt>$(PACKAGE_VERSION)</tt> of <em>$(PACKAGE_NAME)</em>.</p>"

info_TEXINFOS		= doc/ccpathnames.texi
doc_ccpathnames_TEXINFOS	= \
	doc/macros.texi			\
	doc/overview-linking.texi	\
	doc/overview-ccnames.texi	\
	doc/overview-ccexceptions.texi	\
	doc/overview-ccmemory.texi	\
	doc/lgpl-3.0.texi		\
	doc/fdl-1.3.texi


#### libraries

ccpathnames_CURRENT		= @ccpathnames_VERSION_INTERFACE_CURRENT@
ccpathnames_REVISION		= @ccpathnames_VERSION_INTERFACE_REVISION@
ccpathnames_AGE			= @ccpathnames_VERSION_INTERFACE_AGE@
ccpathnames_LINKER_VERSION	= -version-info $(ccpathnames_CURRENT):$(ccpathnames_REVISION):$(ccpathnames_AGE)

include_HEADERS			= src/ccpathnames.h

lib_LTLIBRARIES			= libccpathnames.la
libccpathnames_la_CPPFLAGS	= $(AM_CPPFLAGS)
libccpathnames_la_CFLAGS	= $(AM_CFLAGS) $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS)
libccpathnames_la_LDFLAGS	= $(ccpathnames_LINKER_VERSION) $(AM_LDFLAGS) $(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS)
libccpathnames_la_SOURCES	= \
	src/ccpathnames.h			\
	src/ccpathnames-internals.h		\
	src/ccpathnames-version.c		\
	src/ccpathnames-conditions.c		\
	src/ccpathnames-core.c			\
	src/ccpathnames-manipulation.c		\
	src/ccpathnames-components.c		\
	src/ccpathnames-input-output.c


#### tests

check_PROGRAMS	= \
	tests/version			\
	tests/conditions		\
	tests/core			\
	tests/manipulations		\
	tests/components		\
	tests/iface-destructor		\
	tests/iface-dumpable		\
	tests/iface-serialisation	\
	tests/iface-pathname

if MMUX_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccpathnames_tests_cppflags	= $(AM_CPPFLAGS)
ccpathnames_tests_cflags	= $(AM_CFLAGS) $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS) $(CCTESTS_CFLAGS)
ccpathnames_tests_ldadd		= $(AM_LDFLAGS) libccpathnames.la $(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS) $(CCTESTS_LIBS)

## --------------------------------------------------------------------

tests_version_CPPFLAGS			= $(ccpathnames_tests_cppflags)
tests_version_CFLAGS			= $(ccpathnames_tests_cflags)
tests_version_LDADD			= $(ccpathnames_tests_ldadd)

tests_conditions_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_conditions_CFLAGS			= $(ccpathnames_tests_cflags)
tests_conditions_LDADD			= $(ccpathnames_tests_ldadd)

tests_core_CPPFLAGS			= $(ccpathnames_tests_cppflags)
tests_core_CFLAGS			= $(ccpathnames_tests_cflags)
tests_core_LDADD			= $(ccpathnames_tests_ldadd)

tests_manipulations_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_manipulations_CFLAGS		= $(ccpathnames_tests_cflags)
tests_manipulations_LDADD		= $(ccpathnames_tests_ldadd)

tests_components_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_components_CFLAGS			= $(ccpathnames_tests_cflags)
tests_components_LDADD			= $(ccpathnames_tests_ldadd)

tests_iface_destructor_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_iface_destructor_CFLAGS		= $(ccpathnames_tests_cflags)
tests_iface_destructor_LDADD		= $(ccpathnames_tests_ldadd)

tests_iface_dumpable_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_iface_dumpable_CFLAGS		= $(ccpathnames_tests_cflags)
tests_iface_dumpable_LDADD		= $(ccpathnames_tests_ldadd)

tests_iface_serialisation_CPPFLAGS	= $(ccpathnames_tests_cppflags)
tests_iface_serialisation_CFLAGS	= $(ccpathnames_tests_cflags)
tests_iface_serialisation_LDADD		= $(ccpathnames_tests_ldadd)

tests_iface_pathname_CPPFLAGS		= $(ccpathnames_tests_cppflags)
tests_iface_pathname_CFLAGS		= $(ccpathnames_tests_cflags)
tests_iface_pathname_LDADD		= $(ccpathnames_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### common rules

include $(top_srcdir)/meta/automake/mmux-makefile-rules

### end of file
