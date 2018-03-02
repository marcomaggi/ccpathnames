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

    cctests_assert(L, 0 == strcmp("/path/to/file.ext", ccptn_asciiz(R)));

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

    cctests_assert(L, 0 == strcmp("/path/to/file.ext", ccptn_asciiz(R)));

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
 ** Normalisation.
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

/* ------------------------------------------------------------------ */

void
test_3_2_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_normalise()": normal pathname. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = "/";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    cctests_assert(L, 0 == strcmp(pathname, ccptn_asciiz(R)));

    cce_run_cleanup_handlers(L);
  }
}

void
test_3_2_2 (cce_destination_t upper_L)
/* Test for "ccptn_new_normalise()": normal pathname. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = "/path/to/file.ext";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    cctests_assert(L, 0 == strcmp(pathname, ccptn_asciiz(R)));

    cce_run_cleanup_handlers(L);
  }
}

void
test_3_2_3 (cce_destination_t upper_L)
/* Test for "ccptn_new_normalise()": multiple slashes. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = "/path///to////file.ext";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    cctests_assert(L, 0 == strcmp("/path/to/file.ext", ccptn_asciiz(R)));

    cce_run_cleanup_handlers(L);
  }
}


void
test_3_2_4 (cce_destination_t upper_L)
/* Test for "ccptn_new_normalise()": multiple slashes at the end. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = "/path/to/dir///";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    cctests_assert(L, 0 == strcmp("/path/to/dir", ccptn_asciiz(R)));

    cce_run_cleanup_handlers(L);
  }
}

void
test_3_2_5 (cce_destination_t upper_L)
/* Test for "ccptn_new_normalise()": single-dot components. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = "/path/./to/././file.ext";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    cctests_assert(L, 0 == strcmp("/path/to/file.ext", ccptn_asciiz(R)));

    cce_run_cleanup_handlers(L);
  }
}

void
test_3_2_6 (cce_destination_t upper_L)
/* Test for "ccptn_new_normalise()": single-dot components at the end. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = "/path/to/dir/.";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    if (0) {
      ccptn_print(L, stderr, R);
      fprintf(stderr, "\n");
    }

    cctests_assert_asciiz(L, "/path/to/dir", ccptn_asciiz(R));

    cce_run_cleanup_handlers(L);
  }
}

void
test_3_2_7 (cce_destination_t upper_L)
/* Test for "ccptn_new_normalise()": single-dot components at the end as
   the only segment after a slash. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = "/.";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    if (0) {
      ccptn_print(L, stderr, R);
      fprintf(stderr, "\n");
    }

    cctests_assert_asciiz(L, "/", ccptn_asciiz(R));

    cce_run_cleanup_handlers(L);
  }
}

void
test_3_2_8 (cce_destination_t upper_L)
/* Test  for  "ccptn_new_normalise()":   single-dot  components  at  the
   end. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = "a/.";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    if (0) {
      ccptn_print(L, stderr, R);
      fprintf(stderr, "\n");
    }

    cctests_assert_asciiz(L, "a", ccptn_asciiz(R));

    cce_run_cleanup_handlers(L);
  }
}

void
test_3_2_9 (cce_destination_t upper_L)
/* Test for  "ccptn_new_normalise()": single-dot  component as  the full
   pathname. */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = ".";
    ccptn_t *		P;
    ccptn_t *		R;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    R = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, R_H, R);

    if (0) {
      ccptn_print(L, stderr, R);
      fprintf(stderr, "\n");
    }

    cctests_assert_asciiz(L, ".", ccptn_asciiz(R));

    cce_run_cleanup_handlers(L);
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
    }
    cctests_end_group();

  }
  cctests_final();
}

/* end of file */
