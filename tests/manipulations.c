/*
  Part of: CCPathnames
  Contents: test for pathname manipulation functions
  Date: Feb 27, 2018

  Abstract

	Test file for pathname manipulation functions.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

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


/** --------------------------------------------------------------------
 ** Composing pathnames.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_concat()". */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P1_H[1], P2_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "/path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t		*P1, *P2, *R;

    P1 = ccptn_new_nodup_asciiz(L, pathname_1);
    ccptn_handler_ptn_init(L, P1_H, P1);

    P2 = ccptn_new_nodup_asciiz(L, pathname_2);
    ccptn_handler_ptn_init(L, P2_H, P2);

    R = ccptn_new_concat(L, P1, P2);
    ccptn_handler_ptn_init(L, R_H, R);

    if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(R)); }

    cctests_assert_asciiz(L, "/path/to/file.ext", ccptn_asciiz(R));

    cce_run_cleanup_handlers(L);
  }
}

void
test_1_2 (cce_destination_t upper_L)
/* Test for "ccptn_new_concat()": if the  prefix is absolute, the result
   is absolute. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P1_H[1];
  cce_cleanup_handler_t	P2_H[1];
  cce_cleanup_handler_t	R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "/path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t		*P1, *P2, *R;

    P1 = ccptn_new_nodup_asciiz(L, pathname_1);
    ccptn_handler_ptn_init(L, P1_H, P1);

    P2 = ccptn_new_nodup_asciiz(L, pathname_2);
    ccptn_handler_ptn_init(L, P2_H, P2);

    R = ccptn_new_concat(L, P1, P2);
    ccptn_handler_ptn_init(L, R_H, R);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(R)); }

    cctests_assert(L, true  == ccptn_is_absolute(R));
    cctests_assert(L, false == ccptn_is_relative(R));

    cce_run_cleanup_handlers(L);
  }
}

void
test_1_3 (cce_destination_t upper_L)
/* Test for "ccptn_new_concat()": if the  prefix is relative, the result
   is relative. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P1_H[1];
  cce_cleanup_handler_t	P2_H[1];
  cce_cleanup_handler_t	R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "./path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t		*P1, *P2, *R;

    P1 = ccptn_new_nodup_asciiz(L, pathname_1);
    ccptn_handler_ptn_init(L, P1_H, P1);

    P2 = ccptn_new_nodup_asciiz(L, pathname_2);
    ccptn_handler_ptn_init(L, P2_H, P2);

    R = ccptn_new_concat(L, P1, P2);
    ccptn_handler_ptn_init(L, R_H, R);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(R)); }

    cctests_assert(L, false == ccptn_is_absolute(R));
    cctests_assert(L, true  == ccptn_is_relative(R));

    cce_run_cleanup_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_1 (cce_destination_t upper_L)
/* Test for "ccptn_init_concat()". */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P1_H[1];
  cce_cleanup_handler_t	P2_H[1];
  cce_cleanup_handler_t	R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "/path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t		*P1, *P2, R[1];

    P1 = ccptn_new_nodup_asciiz(L, pathname_1);
    ccptn_handler_ptn_init(L, P1_H, P1);

    P2 = ccptn_new_nodup_asciiz(L, pathname_2);
    ccptn_handler_ptn_init(L, P2_H, P2);

    ccptn_init_concat(L, R, P1, P2);
    ccptn_handler_ptn_init(L, R_H, R);

    if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(R)); }

    cctests_assert_asciiz(L, "/path/to/file.ext", ccptn_asciiz(R));

    cce_run_cleanup_handlers(L);
  }
}

void
test_2_2 (cce_destination_t upper_L)
/* Test for "ccptn_init_concat()": if the  prefix is absolute, the result
   is absolute. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P1_H[1];
  cce_cleanup_handler_t	P2_H[1];
  cce_cleanup_handler_t	R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "/path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t		*P1, *P2, R[1];

    P1 = ccptn_new_nodup_asciiz(L, pathname_1);
    ccptn_handler_ptn_init(L, P1_H, P1);

    P2 = ccptn_new_nodup_asciiz(L, pathname_2);
    ccptn_handler_ptn_init(L, P2_H, P2);

    ccptn_init_concat(L, R, P1, P2);
    ccptn_handler_ptn_init(L, R_H, R);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(R)); }

    cctests_assert(L, true  == ccptn_is_absolute(R));
    cctests_assert(L, false == ccptn_is_relative(R));

    cce_run_cleanup_handlers(L);
  }
}

void
test_2_3 (cce_destination_t upper_L)
/* Test for "ccptn_init_concat()": if the  prefix is relative, the result
   is relative. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P1_H[1];
  cce_cleanup_handler_t	P2_H[1];
  cce_cleanup_handler_t	R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "./path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t		*P1, *P2, R[1];

    P1 = ccptn_new_nodup_asciiz(L, pathname_1);
    ccptn_handler_ptn_init(L, P1_H, P1);

    P2 = ccptn_new_nodup_asciiz(L, pathname_2);
    ccptn_handler_ptn_init(L, P2_H, P2);

    ccptn_init_concat(L, R, P1, P2);
    ccptn_handler_ptn_init(L, R_H, R);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(R)); }

    cctests_assert(L, false == ccptn_is_absolute(R));
    cctests_assert(L, true  == ccptn_is_relative(R));

    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Normalisation: realpath.
 ** ----------------------------------------------------------------- */

void
test_3_1_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_realpath()". */
{
#ifdef HAVE_REALPATH
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = progname;
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_realpath(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    cctests_assert(L, false == ccptn_is_realpath(P));
    cctests_assert(L, true  == ccptn_is_realpath(R));

    cce_run_cleanup_handlers(L);
  }
#endif
}

void
test_3_1_2 (cce_destination_t upper_L)
/* Test for "ccptn_init_realpath()". */
{
#ifdef HAVE_REALPATH
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = progname;
    ccptn_t		*P, R[1];

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    ccptn_init_realpath(L, R, P);
    ccptn_handler_ptn_init(L, R_H, R);

    cctests_assert(L, false == ccptn_is_realpath(P));
    cctests_assert(L, true  == ccptn_is_realpath(R));

    cce_run_cleanup_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Normalisation: normalise.
 ** ----------------------------------------------------------------- */

/* Test for "ccptn_new_normalise()". */
#define NEW_NORMALISE_TEST(FUNCNAME,INPUT_PATHNAME,EXPECTED_PATHNAME,VERBOSE) \
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    cce_cleanup_handler_t	P_H[1], R_H[1];				\
									\
    if (cce_location(L)) {						\
      cce_run_error_handlers_raise(L, upper_L);				\
    } else {								\
      char const *	pathname = INPUT_PATHNAME;			\
      ccptn_t *		P;						\
      ccptn_t *		R;						\
									\
      P = ccptn_new_nodup_asciiz(L, pathname);				\
      ccptn_handler_ptn_init(L, P_H, P);				\
									\
      R = ccptn_new_normalise(L, P);					\
      ccptn_handler_ptn_init(L, R_H, R);				\
									\
      if (VERBOSE) {							\
	fprintf(stderr,							\
		"%s: input_pathname: \"%s\", normal_pathname=\"",	\
		__func__, pathname);					\
	ccptn_print(L, stderr, R);					\
	fprintf(stderr,							\
		"\", expected_pathname=\"%s\"\n",			\
		EXPECTED_PATHNAME);					\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_PATHNAME, ccptn_asciiz(R));	\
									\
      cce_run_cleanup_handlers(L);					\
    }									\
  }

/* Absolute pathnames. */
NEW_NORMALISE_TEST(test_3_2_1,  "/",				"/",			false)
NEW_NORMALISE_TEST(test_3_2_2,  "/path/to/file.ext",		"/path/to/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_3,  "/path///to////file.ext",	"/path/to/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_4,  "/path/to/dir///",		"/path/to/dir",		false)
NEW_NORMALISE_TEST(test_3_2_5,  "/path/./to/././file.ext",	"/path/to/file.ext",	false)
NEW_NORMALISE_TEST(test_3_2_6,  "/path/to/dir/.",		"/path/to/dir",		false)
NEW_NORMALISE_TEST(test_3_2_7,  "/.",				"/",			false)
NEW_NORMALISE_TEST(test_3_2_8,  "/path/..",			"/",			false)
NEW_NORMALISE_TEST(test_3_2_9,  "/path/to/../..",		"/",			false)
NEW_NORMALISE_TEST(test_3_2_10, "/path/to/../file.ext",		"/path/file.ext",	false)

/* Relative pathnames. */
NEW_NORMALISE_TEST(test_3_3_1,  "a/.",				"a",			false)
NEW_NORMALISE_TEST(test_3_3_2,  ".",				".",			false)
NEW_NORMALISE_TEST(test_3_3_3,  "./",				".",			false)
NEW_NORMALISE_TEST(test_3_3_4,  "./path",			"path",			false)
NEW_NORMALISE_TEST(test_3_3_5,  "..",				"..",			false)
NEW_NORMALISE_TEST(test_3_3_6,  "../path",			"../path",		false)
NEW_NORMALISE_TEST(test_3_3_7,  "path/..",			".",			false)
NEW_NORMALISE_TEST(test_3_3_8,  "path/to/../..",		".",			false)
NEW_NORMALISE_TEST(test_3_3_9,  "path/to/../file.ext",		"path/file.ext",	false)
NEW_NORMALISE_TEST(test_3_3_10, "path/to/../../../file.ext",	"../file.ext",		false)

/* ------------------------------------------------------------------ */

void
test_3_4_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_normalise()".  Invalid pathname. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    if (ccptn_condition_is_invalid_pathname(cce_condition(L))) {
      cce_run_cleanup_handlers(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	pathname = "/..";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    cctests_assert(L, false);
  }
}

void
test_3_4_2 (cce_destination_t upper_L)
/* Test for "ccptn_new_normalise()".  Invalid pathname. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    if (ccptn_condition_is_invalid_pathname(cce_condition(L))) {
      cce_run_cleanup_handlers(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	pathname = "/path/to/../../..";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    cctests_assert(L, false);
  }
}


int
main (int argc CCPTN_UNUSED, const char *const argv[])
{
  progname = argv[0];

  ccptn_init();

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

      cctests_run(test_3_2_1);
      cctests_run(test_3_2_2);
      cctests_run(test_3_2_3);
      cctests_run(test_3_2_4);
      cctests_run(test_3_2_5);
      cctests_run(test_3_2_6);
      cctests_run(test_3_2_7);
      cctests_run(test_3_2_8);
      cctests_run(test_3_2_9);
      cctests_run(test_3_2_10);

      cctests_run(test_3_3_1);
      cctests_run(test_3_3_2);
      cctests_run(test_3_3_3);
      cctests_run(test_3_3_4);
      cctests_run(test_3_3_5);
      cctests_run(test_3_3_6);
      cctests_run(test_3_3_7);
      cctests_run(test_3_3_8);
      cctests_run(test_3_3_9);
      cctests_run(test_3_3_10);

      cctests_run(test_3_4_1);
      cctests_run(test_3_4_2);
    }
    cctests_end_group();

  }
  cctests_final();
}

/* end of file */
