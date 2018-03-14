/*
  Part of: CCPathnames
  Contents: test for pathname components functions
  Date: Feb 27, 2018

  Abstract

	Test file for pathname components functions.

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
 ** Extensions and rootname.
 ** ----------------------------------------------------------------- */

/* Test for "ccptn_extension()". */
#define TEST_EXTENSION(FUNCNAME,INPUT_PATHNAME,EXPECTED_EXTENSION)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    cce_cleanup_handler_t	P_H[1], Q_H[1];				\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_error_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t			*P, *Q;					\
      ccptn_extension_t		E;					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      E = ccptn_extension(L, Q);					\
									\
      if (0) {								\
	fprintf(stderr, "%s: ", __func__);				\
	ccptn_extension_print(L, stderr, E);				\
	fprintf(stderr, "\n");						\
      }									\
									\
      cctests_assert_ascii(L, EXPECTED_EXTENSION, E.ptr, E.len);	\
									\
      cce_run_cleanup_handlers(L);					\
    }									\
  }

TEST_EXTENSION(test_1_1, "/path/to/file.ext",		".ext")
TEST_EXTENSION(test_1_2, "/path/to/file",		"")
TEST_EXTENSION(test_1_3, "/path/to/dir/",		"")
TEST_EXTENSION(test_1_4, "/path/to/dir.ext/",		".ext")
TEST_EXTENSION(test_1_5, "file.ext",			".ext")
TEST_EXTENSION(test_1_6, ".fvwmrc",			"")
TEST_EXTENSION(test_1_7, "~/.fvwmrc",			"")
TEST_EXTENSION(test_1_8, ".",				"")
TEST_EXTENSION(test_1_9, "..",				"")


/** --------------------------------------------------------------------
 ** Tailnames.
 ** ----------------------------------------------------------------- */

/* Test for "ccptn_new_tailname()". */
#define TEST_TAILNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_TAILNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    cce_cleanup_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_error_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t		*P, *Q, *T;					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
									\
      Q = ccptn_new_normalise(L, P);					\
      ccptn_handler_ptn_init(L, Q_H, Q);				\
									\
      T = ccptn_new_tailname(L, Q);					\
      ccptn_handler_ptn_init(L, T_H, T);				\
									\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(T));		\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_TAILNAME, ccptn_asciiz(T));	\
									\
      cce_run_cleanup_handlers(L);					\
    }									\
  }

TEST_TAILNAME(test_3_1_1, "/path/to/file.ext",	"file.ext")
TEST_TAILNAME(test_3_1_2, "/path/to/dir/",	"dir/")
TEST_TAILNAME(test_3_1_3, "file.ext",		"file.ext")
TEST_TAILNAME(test_3_1_4, ".",			".")
TEST_TAILNAME(test_3_1_5, "..",			"..")

/* ------------------------------------------------------------------ */

void
test_3_2_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_tailname_guarded_cleanup()". */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], Q_H[1], T_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/dir/";
    ccptn_t		*P, *Q, *T;

    P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);

    Q = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, Q_H, Q);

    T = ccptn_new_tailname_guarded(L, T_H, Q);

    if (0) {
      fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(T));
    }

    cctests_assert_asciiz(L, "dir/", ccptn_asciiz(T));

    cce_run_cleanup_handlers(L);
  }
}

void
test_3_2_2 (cce_destination_t upper_L)
/* Test for "ccptn_new_tailname_guarded_error()". */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], Q_H[1], T_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/dir/";
    ccptn_t		*P, *Q, *T;

    P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);

    Q = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, Q_H, Q);

    {
      cce_location_t		inner_L[1];
      cce_error_handler_t	S_H[1];

      if (cce_location(inner_L)) {
	cce_run_error_handlers_raise(inner_L, L);
      } else {
	ccptn_t *	S = ccptn_new_tailname_guarded(inner_L, S_H, Q);
	cctests_assert_asciiz(inner_L, "dir/", ccptn_asciiz(S));
	T = S;
	cce_run_cleanup_handlers(inner_L);
      }
    }

    ccptn_handler_ptn_init(L, T_H, T);
    cctests_assert_asciiz(L, "dir/", ccptn_asciiz(T));
    cce_run_cleanup_handlers(L);
  }
}


int
main (int argc CCPTN_UNUSED, const char *const argv[])
{
  progname = argv[0];

  ccptn_init();

  cctests_init("pathname components");
  {
    cctests_begin_group("extensions");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
      cctests_run(test_1_4);
      cctests_run(test_1_5);
      cctests_run(test_1_6);
      cctests_run(test_1_7);
      cctests_run(test_1_8);
      cctests_run(test_1_9);
    }
    cctests_end_group();

    cctests_begin_group("rootnames");
    {
      //cctests_run(test_2_1);
    }
    cctests_end_group();

    cctests_begin_group("tailnames");
    {
      cctests_run(test_3_1_1);
      cctests_run(test_3_1_2);
      cctests_run(test_3_1_3);
      cctests_run(test_3_1_4);
      cctests_run(test_3_1_5);

      cctests_run(test_3_2_1);
      cctests_run(test_3_2_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
