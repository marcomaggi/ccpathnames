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


/** --------------------------------------------------------------------
 ** Last segment.
 ** ----------------------------------------------------------------- */

/* Test for "ccptn_asciiz_find_last_segment()". */
#define TEST_LAST_SEGMENT(FUNCNAME,INPUT_PATHNAME,EXPECTED_SEGMENT)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    ccptn_segment_t	S = ccptn_asciiz_find_last_segment(INPUT_PATHNAME, strlen(INPUT_PATHNAME)); \
    cctests_assert_ascii(upper_L, EXPECTED_SEGMENT, S.ptr, S.len);	\
  }

TEST_LAST_SEGMENT(test_0_1,  "/path/to/file.ext",	"file.ext")
TEST_LAST_SEGMENT(test_0_2,  "/path/to/file",		"file")
TEST_LAST_SEGMENT(test_0_3,  "/path/to/dir/",		"dir")
TEST_LAST_SEGMENT(test_0_4,  "/path/to/dir.ext/",	"dir.ext")
TEST_LAST_SEGMENT(test_0_5,  "file.ext",		"file.ext")
TEST_LAST_SEGMENT(test_0_6,  ".fvwmrc",			".fvwmrc")
TEST_LAST_SEGMENT(test_0_7,  "~/.fvwmrc",		".fvwmrc")
TEST_LAST_SEGMENT(test_0_8,  ".",			".")
TEST_LAST_SEGMENT(test_0_9,  "..",			"..")
TEST_LAST_SEGMENT(test_0_10, "/",			"")


/** --------------------------------------------------------------------
 ** Extensions.
 ** ----------------------------------------------------------------- */

/* Test for "ccptn_extension()". */
#define TEST_EXTENSION(FUNCNAME,INPUT_PATHNAME,EXPECTED_EXTENSION)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    cce_clean_handler_t	P_H[1], Q_H[1];				\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
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
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_EXTENSION(test_1_1,  "/path/to/file.ext",		".ext")
TEST_EXTENSION(test_1_2,  "/path/to/file",		"")
TEST_EXTENSION(test_1_3,  "/path/to/dir/",		"")
TEST_EXTENSION(test_1_4,  "/path/to/dir.ext/",		".ext")
TEST_EXTENSION(test_1_5,  "file.ext",			".ext")
TEST_EXTENSION(test_1_6,  ".fvwmrc",			"")
TEST_EXTENSION(test_1_7,  "~/.fvwmrc",			"")
TEST_EXTENSION(test_1_8,  ".",				"")
TEST_EXTENSION(test_1_9,  "..",				"")
TEST_EXTENSION(test_1_10, "/",				"")


/** --------------------------------------------------------------------
 ** Rootnames.
 ** ----------------------------------------------------------------- */

/* Test for "ccptn_new_rootname_guarded()" with clean handler. */
#define TEST_NEW_ROOTNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_PATHNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    cce_clean_handler_t	P_H[1], Q_H[1], R_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t			*P, *Q, *R;				\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
									\
      if (0) {								\
	fprintf(stderr, "%s: P=%s\n", __func__, ccptn_asciiz(P));	\
	fprintf(stderr, "%s: Q=%s\n", __func__, ccptn_asciiz(Q));	\
      }									\
      									\
      R = ccptn_new_rootname_guarded(L, R_H, Q);			\
									\
      if (0) {								\
	fprintf(stderr, "%s: ", __func__);				\
	ccptn_print(L, stderr, R);					\
	fprintf(stderr, "\n");						\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_PATHNAME, ccptn_asciiz(R));	\
									\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_ROOTNAME(test_2_1_1, "/path/to/file.ext",	"/path/to/file")
TEST_NEW_ROOTNAME(test_2_1_2, "/path/to/file",		"/path/to/file")
TEST_NEW_ROOTNAME(test_2_1_3, "/path/to/dir/",		"/path/to/dir")
TEST_NEW_ROOTNAME(test_2_1_4, "/path/to/dir.ext/",	"/path/to/dir")
TEST_NEW_ROOTNAME(test_2_1_5, "file.ext",		"file")
TEST_NEW_ROOTNAME(test_2_1_6, ".fvwmrc",		".fvwmrc")
TEST_NEW_ROOTNAME(test_2_1_7, "~/.fvwmrc",		"~/.fvwmrc")

/* ------------------------------------------------------------------ */

/* Test for "ccptn_init_rootname_guarded()" with clean handler. */
#define TEST_INIT_ROOTNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_PATHNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    cce_clean_handler_t	P_H[1], Q_H[1], R_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t			*P, *Q, R[1];				\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
									\
      if (0) {								\
	fprintf(stderr, "%s: P=%s\n", __func__, ccptn_asciiz(P));	\
	fprintf(stderr, "%s: Q=%s\n", __func__, ccptn_asciiz(Q));	\
      }									\
      									\
      ccptn_init_rootname_guarded(L, R, R_H, Q);			\
									\
      if (0) {								\
	fprintf(stderr, "%s: ", __func__);				\
	ccptn_print(L, stderr, R);					\
	fprintf(stderr, "\n");						\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_PATHNAME, ccptn_asciiz(R));	\
									\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_INIT_ROOTNAME(test_2_2_1, "/path/to/file.ext",	"/path/to/file")
TEST_INIT_ROOTNAME(test_2_2_2, "/path/to/file",		"/path/to/file")
TEST_INIT_ROOTNAME(test_2_2_3, "/path/to/dir/",		"/path/to/dir")
TEST_INIT_ROOTNAME(test_2_2_4, "/path/to/dir.ext/",	"/path/to/dir")
TEST_INIT_ROOTNAME(test_2_2_5, "file.ext",		"file")
TEST_INIT_ROOTNAME(test_2_2_6, ".fvwmrc",		".fvwmrc")
TEST_INIT_ROOTNAME(test_2_2_7, "~/.fvwmrc",		"~/.fvwmrc")

/* ------------------------------------------------------------------ */

/* Test  for  "ccptn_new_rootname_guarded()"  with clean  handler  and
   pathname having no rootname. */
#define TEST_NEW_ROOTNAME_NO(FUNCNAME, INPUT_PATHNAME)			\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    cce_clean_handler_t	P_H[1], Q_H[1], R_H[1];			\
									\
    if (cce_location(L)) {						\
      if (0) {								\
	fprintf(stderr, "%s: exception: %s\n", __func__,		\
		cce_condition_static_message(cce_condition(L)));	\
      }									\
      if (ccptn_condition_is_no_rootname(cce_condition(L))) {		\
	cce_run_catch_handlers_final(L);				\
      } else {								\
	cce_run_catch_handlers_raise(L, upper_L);			\
      }									\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t		*P, *Q;						\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      if (0) {								\
	fprintf(stderr, "%s: P=%s\n", __func__, ccptn_asciiz(P));	\
	fprintf(stderr, "%s: Q=%s\n", __func__, ccptn_asciiz(Q));	\
      }									\
      ccptn_new_rootname_guarded(L, R_H, Q);				\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_ROOTNAME_NO(test_2_3_1, ".")
TEST_NEW_ROOTNAME_NO(test_2_3_2, "..")
TEST_NEW_ROOTNAME_NO(test_2_3_3, "/")

/* ------------------------------------------------------------------ */

void
test_2_4_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_rootname_guarded()" with error handler. */
{
  cce_location_t	L[1];
  cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t		*P, *Q, *T;

    P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);
    Q = ccptn_new_normalise_guarded(L, Q_H, P);
    {
      cce_location_t		inner_L[1];
      cce_error_handler_t	S_H[1];

      if (cce_location(inner_L)) {
	cce_run_catch_handlers_raise(inner_L, L);
      } else {
	ccptn_t *	S = ccptn_new_rootname_guarded(inner_L, S_H, Q);
	cctests_assert_asciiz(inner_L, "/path/to/file", ccptn_asciiz(S));
	T = S;
	cce_run_body_handlers(inner_L);
      }
    }
    ccptn_handler_ptn_init(L, T_H, T);
    cctests_assert_asciiz(L, "/path/to/file", ccptn_asciiz(T));
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Tailnames.
 ** ----------------------------------------------------------------- */

/* Test for "ccptn_new_tailname_guarded()" with clean handler. */
#define TEST_NEW_TAILNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_TAILNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t		*P, *Q, *T;					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      T = ccptn_new_tailname_guarded(L, T_H, Q);			\
									\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(T));		\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_TAILNAME, ccptn_asciiz(T));	\
									\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_TAILNAME(test_3_1_1, "/path/to/file.ext",	"file.ext")
TEST_NEW_TAILNAME(test_3_1_2, "/path/to/dir/",		"dir/")
TEST_NEW_TAILNAME(test_3_1_3, "file.ext",		"file.ext")
TEST_NEW_TAILNAME(test_3_1_4, ".",			".")
TEST_NEW_TAILNAME(test_3_1_5, "..",			"..")

/* ------------------------------------------------------------------ */

/* Test for "ccptn_init_tailname_guarded()" with clean handler. */
#define TEST_INIT_TAILNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_TAILNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t		*P, *Q, T[1];					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      ccptn_init_tailname_guarded(L, T, T_H, Q);			\
									\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(T));		\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_TAILNAME, ccptn_asciiz(T));	\
									\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_INIT_TAILNAME(test_3_2_1, "/path/to/file.ext",	"file.ext")
TEST_INIT_TAILNAME(test_3_2_2, "/path/to/dir/",		"dir/")
TEST_INIT_TAILNAME(test_3_2_3, "file.ext",		"file.ext")
TEST_INIT_TAILNAME(test_3_2_4, ".",			".")
TEST_INIT_TAILNAME(test_3_2_5, "..",			"..")

/* ------------------------------------------------------------------ */

/* Test  for  "ccptn_new_tailname_guarded()"  with clean  handler  and
   input pathname having no tailname. */
#define TEST_NEW_TAILNAME_NO(FUNCNAME, INPUT_PATHNAME)			\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    cce_clean_handler_t	P_H[1], Q_H[1];				\
									\
    if (cce_location(L)) {						\
      if (0) {								\
	fprintf(stderr, "%s: exception: %s\n", __func__,		\
		cce_condition_static_message(cce_condition(L)));	\
      }									\
      if (ccptn_condition_is_no_tailname(cce_condition(L))) {		\
	cce_run_catch_handlers_final(L);				\
      } else {								\
	cce_run_catch_handlers_raise(L, upper_L);			\
      }									\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t		*P, *Q;						\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      ccptn_new_tailname(L, Q);						\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_TAILNAME_NO(test_3_3_1, "/")

/* ------------------------------------------------------------------ */

void
test_3_4_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_tailname_guarded()" with error handler. */
{
  cce_location_t	L[1];
  cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/dir/";
    ccptn_t		*P, *Q, *T;

    P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);
    Q = ccptn_new_normalise_guarded(L, Q_H, P);
    {
      cce_location_t		inner_L[1];
      cce_error_handler_t	S_H[1];

      if (cce_location(inner_L)) {
	cce_run_catch_handlers_raise(inner_L, L);
      } else {
	ccptn_t *	S = ccptn_new_tailname_guarded(inner_L, S_H, Q);
	cctests_assert_asciiz(inner_L, "dir/", ccptn_asciiz(S));
	T = S;
	cce_run_body_handlers(inner_L);
      }
    }
    ccptn_handler_ptn_init(L, T_H, T);
    cctests_assert_asciiz(L, "dir/", ccptn_asciiz(T));
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Dirnames.
 ** ----------------------------------------------------------------- */

/* Test for "ccptn_new_dirname_guarded()" with clean handler. */
#define TEST_NEW_DIRNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_DIRNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t		*P, *Q, *T;					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      T = ccptn_new_dirname_guarded(L, T_H, Q);				\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(T));		\
      }									\
      cctests_assert_asciiz(L, EXPECTED_DIRNAME, ccptn_asciiz(T));	\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_DIRNAME(test_4_1_1, "/path/to/file.ext",	"/path/to/")
TEST_NEW_DIRNAME(test_4_1_2, "/path/to/dir/",		"/path/to/")
TEST_NEW_DIRNAME(test_4_1_3, ".",			"..")
TEST_NEW_DIRNAME(test_4_1_4, "/",			"/")

/* ------------------------------------------------------------------ */

/* Test for "ccptn_init_dirname_guarded()" with clean handler. */
#define TEST_INIT_DIRNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_DIRNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t		*P, *Q, T[1];					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      ccptn_init_dirname_guarded(L, T, T_H, Q);				\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(T));		\
      }									\
      cctests_assert_asciiz(L, EXPECTED_DIRNAME, ccptn_asciiz(T));	\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_INIT_DIRNAME(test_4_2_1, "/path/to/file.ext",	"/path/to/")
TEST_INIT_DIRNAME(test_4_2_2, "/path/to/dir/",		"/path/to/")
TEST_INIT_DIRNAME(test_4_2_3, ".",			"..")
TEST_INIT_DIRNAME(test_4_2_4, "/",			"/")

/* ------------------------------------------------------------------ */

/* Test for "ccptn_new_dirname_guarded()" with clean handler and input
   pathname having no dirname. */
#define TEST_NEW_DIRNAME_NO(FUNCNAME,INPUT_PATHNAME)			\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      if (0) {								\
	fprintf(stderr, "%s: exception: %s\n", __func__,		\
		cce_condition_static_message(cce_condition(L)));	\
      }									\
      if (ccptn_condition_is_no_dirname(cce_condition(L))) {		\
	cce_run_catch_handlers_final(L);				\
      } else {								\
	cce_run_catch_handlers_raise(L, upper_L);			\
      }									\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t			*P, *Q;					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      ccptn_new_dirname_guarded(L, T_H, Q);				\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_DIRNAME_NO(test_4_3_1, "file.ext")
TEST_NEW_DIRNAME_NO(test_4_3_2, "..")


/* ------------------------------------------------------------------ */

void
test_4_4_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_dirname_guarded()" with error handler. */
{
  cce_location_t	L[1];
  cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t		*P, *Q, *T;

    P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);
    Q = ccptn_new_normalise_guarded(L, Q_H, P);
    {
      cce_location_t		inner_L[1];
      cce_error_handler_t	S_H[1];

      if (cce_location(inner_L)) {
	cce_run_catch_handlers_raise(inner_L, L);
      } else {
	ccptn_t *	S = ccptn_new_dirname_guarded(inner_L, S_H, Q);
	cctests_assert_asciiz(inner_L, "/path/to/", ccptn_asciiz(S));
	T = S;
	cce_run_body_handlers(inner_L);
      }
    }
    ccptn_handler_ptn_init(L, T_H, T);
    cctests_assert_asciiz(L, "/path/to/", ccptn_asciiz(T));
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Filenames.
 ** ----------------------------------------------------------------- */

/* Test for "ccptn_new_filename_guarded()" with clean handler. */
#define TEST_NEW_FILENAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_FILENAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t		*P, *Q, *T;					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      T = ccptn_new_filename_guarded(L, T_H, Q);			\
									\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(T));		\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_FILENAME, ccptn_asciiz(T));	\
									\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_FILENAME(test_5_1_1, "/path/to/file.ext",	"file.ext")
TEST_NEW_FILENAME(test_5_1_2, "file.ext",		"file.ext")
TEST_NEW_FILENAME(test_5_1_3, ".fvwmrc",		".fvwmrc")
TEST_NEW_FILENAME(test_5_1_4, "~/.fvwmrc",		".fvwmrc")

/* ------------------------------------------------------------------ */

/* Test for "ccptn_init_filename_guarded()" with clean handler. */
#define TEST_INIT_FILENAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_FILENAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t		*P, *Q, T[1];					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      ccptn_init_filename_guarded(L, T, T_H, Q);			\
									\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(T));		\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_FILENAME, ccptn_asciiz(T));	\
									\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_FILENAME(test_5_2_1, "/path/to/file.ext",	"file.ext")
TEST_NEW_FILENAME(test_5_2_2, "file.ext",		"file.ext")
TEST_NEW_FILENAME(test_5_2_3, ".fvwmrc",		".fvwmrc")
TEST_NEW_FILENAME(test_5_2_4, "~/.fvwmrc",		".fvwmrc")

/* ------------------------------------------------------------------ */

/* Test  for  "ccptn_new_filename_guarded()"  with  pathname  having  no
   filename. */
#define TEST_NEW_FILENAME_NO(FUNCNAME, INPUT_PATHNAME)			\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      if (0) {								\
	fprintf(stderr, "%s: exception: %s\n", __func__,		\
		cce_condition_static_message(cce_condition(L)));	\
      }									\
      if (ccptn_condition_is_no_filename(cce_condition(L))) {		\
	cce_run_catch_handlers_final(L);				\
      } else {								\
	cce_run_catch_handlers_raise(L, upper_L);			\
      }									\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t			*P, *Q;					\
									\
      P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);		\
      Q = ccptn_new_normalise_guarded(L, Q_H, P);			\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(Q));		\
      }									\
      ccptn_new_filename_guarded(L, T_H, Q);				\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_FILENAME_NO(test_5_3_1, ".")
TEST_NEW_FILENAME_NO(test_5_3_2, "..")
TEST_NEW_FILENAME_NO(test_5_3_3, "/path/to/dir/")
TEST_NEW_FILENAME_NO(test_5_3_4, "/")


/* ------------------------------------------------------------------ */

void
test_5_4_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_filename_guarded()" with error handler. */
{
  cce_location_t	L[1];
  cce_clean_handler_t	P_H[1], Q_H[1], T_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t		*P, *Q, *T;

    P = ccptn_new_nodup_asciiz_guarded(L, P_H, pathname);
    Q = ccptn_new_normalise_guarded(L, Q_H, P);
    {
      cce_location_t		inner_L[1];
      cce_error_handler_t	S_H[1];

      if (cce_location(inner_L)) {
	cce_run_catch_handlers_raise(inner_L, L);
      } else {
	ccptn_t *	S = ccptn_new_filename_guarded(inner_L, S_H, Q);
	cctests_assert_asciiz(inner_L, "file.ext", ccptn_asciiz(S));
	T = S;
	cce_run_body_handlers(inner_L);
      }
    }
    ccptn_handler_ptn_init(L, T_H, T);
    cctests_assert_asciiz(L, "file.ext", ccptn_asciiz(T));
    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  ccptn_library_init();

  cctests_init("pathname components");
  {
    cctests_begin_group("last segment");
    {
      cctests_run(test_0_1);
      cctests_run(test_0_2);
      cctests_run(test_0_3);
      cctests_run(test_0_4);
      cctests_run(test_0_5);
      cctests_run(test_0_6);
      cctests_run(test_0_7);
      cctests_run(test_0_8);
      cctests_run(test_0_9);
      cctests_run(test_0_10);
    }
    cctests_end_group();

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
      cctests_run(test_1_10);
    }
    cctests_end_group();

    cctests_begin_group("rootnames");
    {
      cctests_run(test_2_1_1);
      cctests_run(test_2_1_2);
      cctests_run(test_2_1_3);
      cctests_run(test_2_1_4);
      cctests_run(test_2_1_5);
      cctests_run(test_2_1_6);
      cctests_run(test_2_1_7);

      cctests_run(test_2_2_1);
      cctests_run(test_2_2_2);
      cctests_run(test_2_2_3);
      cctests_run(test_2_2_4);
      cctests_run(test_2_2_5);
      cctests_run(test_2_2_6);
      cctests_run(test_2_2_7);

      cctests_run(test_2_3_1);
      cctests_run(test_2_3_2);
      cctests_run(test_2_3_3);

      cctests_run(test_2_4_1);
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
      cctests_run(test_3_2_3);
      cctests_run(test_3_2_4);
      cctests_run(test_3_2_5);

      cctests_run(test_3_3_1);

      cctests_run(test_3_4_1);
    }
    cctests_end_group();

    cctests_begin_group("dirnames");
    {
      cctests_run(test_4_1_1);
      cctests_run(test_4_1_2);
      cctests_run(test_4_1_3);
      cctests_run(test_4_1_4);

      cctests_run(test_4_2_1);
      cctests_run(test_4_2_2);
      cctests_run(test_4_2_3);
      cctests_run(test_4_2_4);

      cctests_run(test_4_3_1);
      cctests_run(test_4_3_2);

      cctests_run(test_4_4_1);
    }
    cctests_end_group();

    cctests_begin_group("filenames");
    {
      cctests_run(test_5_1_1);
      cctests_run(test_5_1_2);
      cctests_run(test_5_1_3);
      cctests_run(test_5_1_4);

      cctests_run(test_5_2_1);
      cctests_run(test_5_2_2);
      cctests_run(test_5_2_3);
      cctests_run(test_5_2_4);

      cctests_run(test_5_3_1);
      cctests_run(test_5_3_2);
      cctests_run(test_5_3_3);
      cctests_run(test_5_3_4);

      cctests_run(test_5_4_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
