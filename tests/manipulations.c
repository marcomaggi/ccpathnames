/*
  Part of: CCPathnames
  Contents: test for pathname manipulation functions
  Date: Feb 27, 2018

  Abstract

	Test file for pathname manipulation functions.

  Copyright (C) 2018, 2019, 2020 Marco Maggi <mrc.mgg@gmail.com>

  See the COPYING file.
*/

#include <ccpathnames.h>
#include "cctests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

static char const * progname;
static ccmem_allocator_t const * A;


/** --------------------------------------------------------------------
 ** Composing pathnames.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, concat, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P1_H[1], P2_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "/path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t const	*P1, *P2, *R;

    P1 = ccname_new(ccptn_t, pointer, clean)(L, A, P1_H, pathname_1);
    P2 = ccname_new(ccptn_t, pointer, clean)(L, A, P2_H, pathname_2);
    R  = ccname_new(ccptn_t, concat,  clean)(L, A, R_H, P1, P2);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(R)); }

    cctests_assert_asciiz(L, "/path/to/file.ext", ccptn_ptr(R));

    cce_run_body_handlers(L);
  }
}

void
test_1_2 (cce_destination_t upper_L)
/* Test for  "ccname_new(ccptn_t, concat, clean)()":  if the prefix is  absolute, the
   result is absolute. */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P1_H[1], P2_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "/path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t const	*P1, *P2, *R;

    P1 = ccname_new(ccptn_t, pointer, clean)(L, A, P1_H, pathname_1);
    P2 = ccname_new(ccptn_t, pointer, clean)(L, A, P2_H, pathname_2);
    R  = ccname_new(ccptn_t, concat,  clean)(L, A, R_H, P1, P2);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(R)); }

    cctests_assert(L, true  == ccptn_is_absolute(R));
    cctests_assert(L, false == ccptn_is_relative(R));

    cce_run_body_handlers(L);
  }
}

void
test_1_3 (cce_destination_t upper_L)
/* Test for  "ccname_new(ccptn_t, concat, clean)()":  if the prefix is  relative, the
   result is relative. */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P1_H[1], P2_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "./path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t const	*P1, *P2, *R;

    P1 = ccname_new(ccptn_t, pointer, clean)(L, A, P1_H, pathname_1);
    P2 = ccname_new(ccptn_t, pointer, clean)(L, A, P2_H, pathname_2);
    R  = ccname_new(ccptn_t, concat,  clean)(L, A, R_H, P1, P2);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(R)); }

    cctests_assert(L, false == ccptn_is_absolute(R));
    cctests_assert(L, true  == ccptn_is_relative(R));

    cce_run_body_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_1 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t, concat, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P1_H[1], P2_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "/path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t const	*P1, *P2;
    ccptn_t		R[1];

    P1 = ccname_new(ccptn_t, pointer, clean)(L, A, P1_H, pathname_1);
    P2 = ccname_new(ccptn_t, pointer, clean)(L, A, P2_H, pathname_2);
    ccname_init(ccptn_t, concat, clean)(L, A, R_H, R, P1, P2);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(R)); }

    cctests_assert_asciiz(L, "/path/to/file.ext", ccptn_ptr(R));

    cce_run_body_handlers(L);
  }
}

void
test_2_2 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t,  concat, clean)()": if the prefix  is absolute, the
   result is absolute. */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P1_H[1], P2_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "/path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t const	*P1, *P2;
    ccptn_t		R[1];

    P1 = ccname_new(ccptn_t, pointer, clean)(L, A, P1_H, pathname_1);
    P2 = ccname_new(ccptn_t, pointer, clean)(L, A, P2_H, pathname_2);
    ccname_init(ccptn_t, concat, clean)(L, A, R_H, R, P1, P2);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(R)); }

    cctests_assert(L, true  == ccptn_is_absolute(R));
    cctests_assert(L, false == ccptn_is_relative(R));

    cce_run_body_handlers(L);
  }
}

void
test_2_3 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t,  concat, clean)()": if the prefix  is relative, the
   result is relative. */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P1_H[1], P2_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "./path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t const	*P1, *P2;
    ccptn_t		R[1];

    P1 = ccname_new(ccptn_t, pointer, clean)(L, A, P1_H, pathname_1);
    P2 = ccname_new(ccptn_t, pointer, clean)(L, A, P2_H, pathname_2);
    ccname_init(ccptn_t, concat, clean)(L, A, R_H, R, P1, P2);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(R)); }

    cctests_assert(L, false == ccptn_is_absolute(R));
    cctests_assert(L, true  == ccptn_is_relative(R));

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Normalisation: realpath.
 ** ----------------------------------------------------------------- */

void
test_3_1_1 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, realpath, clean)()". */
{
#ifdef HAVE_REALPATH
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = progname;
    ccptn_t const	*P, *R;

    P = ccname_new(ccptn_t, pointer,  clean)(L, A, P_H, pathname);
    R = ccname_new(ccptn_t, realpath, clean)(L, A, R_H, P);

    cctests_assert(L, false == ccptn_is_realpath(P));
    cctests_assert(L, false == ccptn_is_normalised(P));

    cctests_assert(L, true  == ccptn_is_realpath(R));
    cctests_assert(L, true  == ccptn_is_normalised(R));

    cce_run_body_handlers(L);
  }
#endif
}

void
test_3_1_2 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t, realpath, clean)()". */
{
#ifdef HAVE_REALPATH
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = progname;
    ccptn_t const	*P;
    ccptn_t		R[1];

    P = ccname_new(ccptn_t, pointer,  clean)(L, A, P_H, pathname);
    ccname_init(ccptn_t, realpath, clean)(L, A, R_H, R, P);

    cctests_assert(L, false == ccptn_is_realpath(P));
    cctests_assert(L, false == ccptn_is_normalised(P));

    cctests_assert(L, true  == ccptn_is_realpath(R));
    cctests_assert(L, true  == ccptn_is_normalised(R));

    cce_run_body_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Normalisation: normalise.
 ** ----------------------------------------------------------------- */

/* Test for "ccname_new(ccptn_t, normalise, clean)()". */
#define NEW_NORMALISE_TEST(FUNCNAME,INPUT_PATHNAME,EXPECTED_PATHNAME,VERBOSE) \
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], R_H[1];				\
									\
    if (cce_location(L)) {						\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      char const *	pathname = INPUT_PATHNAME;			\
      ccptn_t const	*P, *R;						\
									\
      P = ccname_new(ccptn_t, pointer,   clean)(L, A, P_H, pathname);	\
      R = ccname_new(ccptn_t, normalise, clean)(L, A, R_H, P);		\
									\
      if (VERBOSE) {							\
	fprintf(stderr,							\
		"%s: input_pathname: \"%s\", normal_pathname=\"",	\
		__func__, pathname);					\
	ccptn_fwrite(L, stderr, R);					\
	fprintf(stderr,							\
		"\", expected_pathname=\"%s\"\n",			\
		EXPECTED_PATHNAME);					\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_PATHNAME, ccptn_ptr(R));	\
      cctests_assert(L, true == ccptn_is_normalised(R));		\
									\
      cce_run_body_handlers(L);					\
    }									\
  }

/* ------------------------------------------------------------------ */

/* Absolute pathnames: already normalised pathnames. */
NEW_NORMALISE_TEST(test_3_2_1_1,	"/",				"/",			false)
NEW_NORMALISE_TEST(test_3_2_1_2,	"/file.ext",			"/file.ext",		false)
NEW_NORMALISE_TEST(test_3_2_1_3,	"/path/to/file.ext",		"/path/to/file.ext",	false)

/* Relative pathnames: already normalised pathnames. */
NEW_NORMALISE_TEST(test_3_2_2_1,	".",				".",			false)
NEW_NORMALISE_TEST(test_3_2_2_2,	"file.ext",			"file.ext",		false)
NEW_NORMALISE_TEST(test_3_2_2_3,	"path/to/file.ext",		"path/to/file.ext",	false)

/* ------------------------------------------------------------------ */

/* Absolute pathnames: useless slashes removal. */
NEW_NORMALISE_TEST(test_3_2_3_1,	"/path///to////file.ext",	"/path/to/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_3_2,	"/path/to/dir///",		"/path/to/dir/",	false)

/* Relative pathnames: useless slashes removal. */
NEW_NORMALISE_TEST(test_3_2_4_1,	"path///to////file.ext",	"path/to/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_4_2,	"path/to/dir///",		"path/to/dir/",		false)

/* ------------------------------------------------------------------ */

/* Absolute pathnames: single-dot removal. */
NEW_NORMALISE_TEST(test_3_2_5_1,	"/path/./to/././file.ext",	"/path/to/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_5_2,	"/path/to/dir/.",		"/path/to/dir/",	false)
NEW_NORMALISE_TEST(test_3_2_5_3,	"/.",				"/",			false)
NEW_NORMALISE_TEST(test_3_2_5_4,	"/./././.",			"/",			false)

/* Relative pathnames: single-dot removal. */
NEW_NORMALISE_TEST(test_3_2_6_1,	"./",				".",			false)
NEW_NORMALISE_TEST(test_3_2_6_2,	"./.",				".",			false)
NEW_NORMALISE_TEST(test_3_2_6_3,	"./././.",			".",			false)
NEW_NORMALISE_TEST(test_3_2_6_4,	"path/",			"path/",		false)
NEW_NORMALISE_TEST(test_3_2_6_5,	"path/.",			"path/",		false)
NEW_NORMALISE_TEST(test_3_2_6_6,	"./path",			"./path",		false)
NEW_NORMALISE_TEST(test_3_2_6_7,	"./path/file.ext",		"path/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_6_8,	"./file.ext",			"./file.ext",		false)
NEW_NORMALISE_TEST(test_3_2_6_9,	"./path/to/file.ext",		"path/to/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_6_10,	"./path///to////file.ext",	"path/to/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_6_11,	"./path/to/dir///",		"path/to/dir/",		false)
NEW_NORMALISE_TEST(test_3_2_6_12,	"~/.fvwmrc",			"~/.fvwmrc",		false)
NEW_NORMALISE_TEST(test_3_2_6_13,	".fvwmrc",			".fvwmrc",		false)
NEW_NORMALISE_TEST(test_3_2_6_14,	"/path/to/.fvwmrc",		"/path/to/.fvwmrc",	false)

/* ------------------------------------------------------------------ */

/* Absolute pathnames: double-dot removal. */
NEW_NORMALISE_TEST(test_3_2_7_1,	"/path/..",			"/",			false)
NEW_NORMALISE_TEST(test_3_2_7_2,	"/path/to/../..",		"/",			false)
NEW_NORMALISE_TEST(test_3_2_7_3,	"/path/to/../file.ext",		"/path/file.ext",	false)

/* Relative pathnames: double-dot removal. */
NEW_NORMALISE_TEST(test_3_2_8_1,	"..",				"..",			false)
NEW_NORMALISE_TEST(test_3_2_8_2,	"../",				"..",			false)
NEW_NORMALISE_TEST(test_3_2_8_3,	"../path",			"../path",		false)
NEW_NORMALISE_TEST(test_3_2_8_4,	"path/..",			".",			false)
NEW_NORMALISE_TEST(test_3_2_8_5,	"./path/..",			".",			false)
NEW_NORMALISE_TEST(test_3_2_8_6,	"path/to/../..",		".",			false)
NEW_NORMALISE_TEST(test_3_2_8_7,	"./path/to/../..",		".",			false)
NEW_NORMALISE_TEST(test_3_2_8_8,	"path/../file.ext",		"./file.ext",		false)
NEW_NORMALISE_TEST(test_3_2_8_9,	"./path/../file.ext",		"./file.ext",		false)
NEW_NORMALISE_TEST(test_3_2_8_10,	"path/to/../file.ext",		"path/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_8_11,	"./path/to/../file.ext",	"path/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_8_12,	"path/to/../../../file.ext",	"../file.ext",		false)
NEW_NORMALISE_TEST(test_3_2_8_13,	"./path/to/../../../file.ext",	"../file.ext",		false)
NEW_NORMALISE_TEST(test_3_2_8_14,	"path/../../../file.ext",	"../../file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_8_15,	"./path/../../../file.ext",	"../../file.ext",	false)

/* ------------------------------------------------------------------ */

void
test_3_2_9_1 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, normalise, clean)()".  Invalid pathname. */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    if (ccptn_condition_is_invalid_pathname(cce_condition(L))) {
      cce_run_body_handlers(L);
    } else {
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	pathname = "/..";
    ccptn_t const	*P;

    P = ccname_new(ccptn_t, pointer,   clean)(L, A, P_H, pathname);
    ccname_new(ccptn_t, normalise, clean)(L, A, R_H, P);
    cctests_assert(L, false);
  }
}

void
test_3_2_9_2 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, realpath, clean)()".  Invalid pathname. */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    if (ccptn_condition_is_invalid_pathname(cce_condition(L))) {
      cce_run_body_handlers(L);
    } else {
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	pathname = "/path/to/../../..";
    ccptn_t const	*P;

    P = ccname_new(ccptn_t, pointer,   clean)(L, A, P_H, pathname);
    ccname_new(ccptn_t, normalise, clean)(L, A, R_H, P);
    cctests_assert(L, false);
  }
}


int
main (int argc CCLIB_UNUSED, const char *const argv[])
{
  progname = argv[0];
  A = ccmem_standard_allocator;

  ccptn_library_init();

  cctests_init("manipulation functions");
  {
    cctests_begin_group("composing pathnames");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
      cctests_run(test_2_1);
      cctests_run(test_2_2);
      cctests_run(test_2_3);
    }
    cctests_end_group();

    cctests_begin_group("normalisation");
    {
      cctests_run(test_3_1_1);
      cctests_run(test_3_1_2);

      /* Absolute pathnames: already normalised pathnames. */
      cctests_run(test_3_2_1_1);
      cctests_run(test_3_2_1_2);
      cctests_run(test_3_2_1_3);

      /* Relative pathnames: already normalised pathnames. */
      cctests_run(test_3_2_2_1);
      cctests_run(test_3_2_2_2);
      cctests_run(test_3_2_2_3);

      /* Absolute pathnames: useless slashes removal. */
      cctests_run(test_3_2_3_1);
      cctests_run(test_3_2_3_2);

      /* Relative pathnames: useless slashes removal. */
      cctests_run(test_3_2_4_1);
      cctests_run(test_3_2_4_2);

      /* Absolute pathnames: single-dot removal. */
      cctests_run(test_3_2_5_1);
      cctests_run(test_3_2_5_2);
      cctests_run(test_3_2_5_3);
      cctests_run(test_3_2_5_4);

      /* Relative pathnames: single-dot removal. */
      cctests_run(test_3_2_6_1);
      cctests_run(test_3_2_6_2);
      cctests_run(test_3_2_6_3);
      cctests_run(test_3_2_6_4);
      cctests_run(test_3_2_6_5);
      cctests_run(test_3_2_6_6);
      cctests_run(test_3_2_6_7);
      cctests_run(test_3_2_6_8);
      cctests_run(test_3_2_6_9);
      cctests_run(test_3_2_6_10);
      cctests_run(test_3_2_6_11);
      cctests_run(test_3_2_6_12);
      cctests_run(test_3_2_6_13);
      cctests_run(test_3_2_6_14);

      /* Absolute pathnames: double-dot removal. */
      cctests_run(test_3_2_7_1);
      cctests_run(test_3_2_7_2);
      cctests_run(test_3_2_7_3);

      /* Relative pathnames: double-dot removal. */
      cctests_run(test_3_2_8_1);
      cctests_run(test_3_2_8_2);
      cctests_run(test_3_2_8_3);
      cctests_run(test_3_2_8_4);
      cctests_run(test_3_2_8_5);
      cctests_run(test_3_2_8_6);
      cctests_run(test_3_2_8_7);
      cctests_run(test_3_2_8_8);
      cctests_run(test_3_2_8_9);
      cctests_run(test_3_2_8_10);
      cctests_run(test_3_2_8_11);
      cctests_run(test_3_2_8_12);
      cctests_run(test_3_2_8_13);
      cctests_run(test_3_2_8_14);
      cctests_run(test_3_2_8_15);

      /* Invalid pathnames. */
      cctests_run(test_3_2_9_1);
      cctests_run(test_3_2_9_2);
    }
    cctests_end_group();

  }
  cctests_final();
}

/* end of file */
