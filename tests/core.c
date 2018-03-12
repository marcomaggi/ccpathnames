/*
  Part of: CCPathnames
  Contents: test for core functions
  Date: Feb 25, 2018

  Abstract

	Test file for core functions.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#include <ccpathnames.h>
#include "cctests.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

static char const * progname;


/** --------------------------------------------------------------------
 ** Constructors.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_nodup_asciiz()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t *		P;

    P = ccptn_new_nodup_asciiz(L, pathname);
    if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(P)); }
    ccptn_delete(P);

    cce_run_cleanup_handlers(L);
  }
}

void
test_1_2 (cce_destination_t upper_L)
/* Test for "ccptn_new_dup_asciiz()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t *		P;

    P = ccptn_new_dup_asciiz(L, pathname);
    if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(P)); }
    ccptn_delete(P);

    cce_run_cleanup_handlers(L);
  }
}

void
test_1_3 (cce_destination_t upper_L)
/* Test for "ccptn_init_nodup_asciiz()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t		P[1];

    ccptn_init_nodup_asciiz(L, P, pathname);
    if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(P)); }
    ccptn_delete(P);

    cce_run_cleanup_handlers(L);
  }
}

void
test_1_4 (cce_destination_t upper_L)
/* Test for "ccptn_init_dup_asciiz()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t		P[1];

    ccptn_init_dup_asciiz(L, P, pathname);
    if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(P)); }
    ccptn_delete(P);

    cce_run_cleanup_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_1_5 (cce_destination_t upper_L)
/* Test for "ccptn_handler_ptn_init()". */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t *		P;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);
    if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(P)); }

    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Predicates.
 ** ----------------------------------------------------------------- */

void
test_2_1_1 (cce_destination_t upper_L)
/* Test for "ccptn_is_absolute()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t *		P;

    P = ccptn_new_nodup_asciiz(L, pathname);
    cctests_assert(L, true == ccptn_is_absolute(P));
    ccptn_delete(P);

    cce_run_cleanup_handlers(L);
  }
}

void
test_2_1_2 (cce_destination_t upper_L)
/* Test for "ccptn_is_absolute()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "./path/to/file.ext";
    ccptn_t *		P;

    P = ccptn_new_nodup_asciiz(L, pathname);
    cctests_assert(L, false == ccptn_is_absolute(P));
    ccptn_delete(P);

    cce_run_cleanup_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_2_1 (cce_destination_t upper_L)
/* Test for "ccptn_is_relative()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t *		P;

    P = ccptn_new_nodup_asciiz(L, pathname);
    cctests_assert(L, false == ccptn_is_relative(P));
    ccptn_delete(P);

    cce_run_cleanup_handlers(L);
  }
}

void
test_2_2_2 (cce_destination_t upper_L)
/* Test for "ccptn_is_relative()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "./path/to/file.ext";
    ccptn_t *		P;

    P = ccptn_new_nodup_asciiz(L, pathname);
    cctests_assert(L, true == ccptn_is_relative(P));
    ccptn_delete(P);

    cce_run_cleanup_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_3_1 (cce_destination_t upper_L)
/* Test for "ccptn_is_realpath()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t *		P;

    P = ccptn_new_nodup_asciiz(L, pathname);
    cctests_assert(L, false == ccptn_is_realpath(P));
    ccptn_delete(P);

    cce_run_cleanup_handlers(L);
  }
}

void
test_2_3_2 (cce_destination_t upper_L)
/* Test for "ccptn_is_realpath()". */
{
#ifdef HAVE_REALPATH
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	pathname = progname;
    ccptn_t *		P;
    ccptn_t *		Q;

    P = ccptn_new_nodup_asciiz(L, pathname);
    Q = ccptn_new_realpath(L, P);
    cctests_assert(L, false == ccptn_is_realpath(P));
    cctests_assert(L, true  == ccptn_is_realpath(Q));
    ccptn_delete(P);
    ccptn_delete(Q);

    cce_run_cleanup_handlers(L);
  }
#endif
}


int
main (int argc CCPTN_UNUSED, const char *const argv[])
{
  progname = argv[0];

  ccptn_init();

  cctests_init("core functions");
  {
    cctests_begin_group("constructors and destructors");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
      cctests_run(test_1_4);
      cctests_run(test_1_5);
    }
    cctests_end_group();

    cctests_begin_group("predicates");
    {
      cctests_run(test_2_1_1);
      cctests_run(test_2_1_2);
      cctests_run(test_2_2_1);
      cctests_run(test_2_2_2);
      cctests_run(test_2_3_1);
      cctests_run(test_2_3_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
