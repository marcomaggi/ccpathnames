/*
  Part of: CCPathnames
  Contents: test for pathname components functions
  Date: Feb 27, 2018

  Abstract

	Test file for pathname components functions.

  Copyright (C) 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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

static ccmem_allocator_t const * A;


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
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1];				\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t const		*P, *Q;					\
      ccptn_extension_t		E;					\
									\
      P = ccname_new(ccptn_t, pointer,   clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      E = ccptn_extension(L, Q);					\
									\
      if (0) {								\
	fprintf(stderr, "%s: ", __func__);				\
	ccptn_extension_fwrite(L, stderr, E);				\
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

/* Test for "ccname_new(ccptn_t, rootname, clean)". */
#define TEST_NEW_ROOTNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_PATHNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], R_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t const		*P, *Q, *R;				\
									\
      P = ccname_new(ccptn_t, pointer,   clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
									\
      if (0) {								\
	fprintf(stderr, "%s: P=%s\n", __func__, ccptn_ptr(P));		\
	fprintf(stderr, "%s: Q=%s\n", __func__, ccptn_ptr(Q));		\
      }									\
      									\
      R = ccname_new(ccptn_t, rootname, clean)(L, A, R_H, Q);		\
									\
      if (0) {								\
	fprintf(stderr, "%s: ", __func__);				\
	ccptn_fwrite(L, stderr, R);					\
	fprintf(stderr, "\n");						\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_PATHNAME, ccptn_ptr(R));	\
									\
      cce_run_body_handlers(L);						\
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

/* Test for "ccname_init(ccptn_t, rootname, clean)". */
#define TEST_INIT_ROOTNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_PATHNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], R_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t const		*P, *Q;					\
      ccptn_t			R[1];					\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
									\
      if (0) {								\
	fprintf(stderr, "%s: P=%s\n", __func__, ccptn_ptr(P));		\
	fprintf(stderr, "%s: Q=%s\n", __func__, ccptn_ptr(Q));		\
      }									\
      									\
      ccname_init(ccptn_t, rootname, clean)(L, A, R_H, R, Q);		\
									\
      if (0) {								\
	fprintf(stderr, "%s: ", __func__);				\
	ccptn_fwrite(L, stderr, R);					\
	fprintf(stderr, "\n");						\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_PATHNAME, ccptn_ptr(R));	\
									\
      cce_run_body_handlers(L);						\
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

/* Test   for  "ccname_new(ccptn_t,   rootname,  clean)"   and  pathname   having  no
   rootname. */
#define TEST_NEW_ROOTNAME_NO(FUNCNAME, INPUT_PATHNAME)			\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], R_H[1];			\
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
      ccptn_t const		*P, *Q;					\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      if (0) {								\
	fprintf(stderr, "%s: P=%s\n", __func__, ccptn_ptr(P));		\
	fprintf(stderr, "%s: Q=%s\n", __func__, ccptn_ptr(Q));		\
      }									\
      ccname_new(ccptn_t, rootname, clean)(L, A, R_H, Q);		\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);						\
    }									\
  }

TEST_NEW_ROOTNAME_NO(test_2_3_1, ".")
TEST_NEW_ROOTNAME_NO(test_2_3_2, "..")
TEST_NEW_ROOTNAME_NO(test_2_3_3, "/")

/* ------------------------------------------------------------------ */

void
test_2_4_1 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, rootname, error)". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], Q_H[1], S_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t const	*P, *Q, *S;

    P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);
    Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);
    {
      cce_location_t		inner_L[1];
      ccptn_error_handler_t	T_H[1];

      if (cce_location(inner_L)) {
	cce_run_catch_handlers_raise(inner_L, L);
      } else {
	S = ccname_new(ccptn_t, rootname, error)(inner_L, A, T_H, Q);
	cctests_assert_asciiz(inner_L, "/path/to/file", ccptn_ptr(S));
	cce_run_body_handlers(inner_L);
      }
    }
    ccptn_init_and_register_handler(L, S_H, S);
    cctests_assert_asciiz(L, "/path/to/file", ccptn_ptr(S));
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Tailnames.
 ** ----------------------------------------------------------------- */

/* Test for "ccname_new(ccptn_t, tailname, clean)()". */
#define TEST_NEW_TAILNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_TAILNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t const		*P, *Q, *T;				\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      T = ccname_new(ccptn_t, tailname,  clean)(L, A, T_H, Q);		\
									\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(T));		\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_TAILNAME, ccptn_ptr(T));	\
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

/* Test for "ccname_init(ccptn_t, tailname, clean)()". */
#define TEST_INIT_TAILNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_TAILNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t const		*P, *Q;					\
      ccptn_t			T[1];					\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      ccname_init(ccptn_t, tailname, clean)(L, A, T_H, T, Q);		\
									\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(T));		\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_TAILNAME, ccptn_ptr(T));	\
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

/* Test for  "ccname_new(ccptn_t, tailname,  clean)()" and  input pathname  having no
   tailname. */
#define TEST_NEW_TAILNAME_NO(FUNCNAME, INPUT_PATHNAME)			\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], R_H[1];			\
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
      ccptn_t const		*P, *Q;					\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      ccname_new(ccptn_t, tailname, clean)(L, A, R_H, Q);		\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);						\
    }									\
  }

TEST_NEW_TAILNAME_NO(test_3_3_1, "/")

/* ------------------------------------------------------------------ */

void
test_3_4_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_tailname_guarded()" with error handler. */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], Q_H[1], S_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/dir/";
    ccptn_t const	*P, *Q, *S;

    P = ccname_new(ccptn_t, pointer,   clean)(L, A, P_H, pathname);
    Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);
    {
      cce_location_t		inner_L[1];
      ccptn_error_handler_t	T_H[1];

      if (cce_location(inner_L)) {
	cce_run_catch_handlers_raise(inner_L, L);
      } else {
	S = ccname_new(ccptn_t, tailname, error)(inner_L, A, T_H, Q);
	cctests_assert_asciiz(inner_L, "dir/", ccptn_ptr(S));
	cce_run_body_handlers(inner_L);
      }
    }
    ccptn_init_and_register_handler(L, S_H, S);
    cctests_assert_asciiz(L, "dir/", ccptn_ptr(S));
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Dirnames.
 ** ----------------------------------------------------------------- */

/* Test for "ccname_new(ccptn_t, dirname, clean)()". */
#define TEST_NEW_DIRNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_DIRNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t const		*P, *Q, *T;				\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      T = ccname_new(ccptn_t, dirname,   clean)(L, A, T_H, Q);		\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(T));		\
      }									\
      cctests_assert_asciiz(L, EXPECTED_DIRNAME, ccptn_ptr(T));	\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_DIRNAME(test_4_1_1, "/path/to/file.ext",	"/path/to/")
TEST_NEW_DIRNAME(test_4_1_2, "/path/to/dir/",		"/path/to/")
TEST_NEW_DIRNAME(test_4_1_3, ".",			"..")
TEST_NEW_DIRNAME(test_4_1_4, "/",			"/")

/* ------------------------------------------------------------------ */

/* Test for "ccname_init(ccptn_t, dirname, clean)()". */
#define TEST_INIT_DIRNAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_DIRNAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
    ccptn_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t const		*P, *Q;					\
      ccptn_t			T[1];					\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      ccname_init(ccptn_t, dirname, clean)(L, A, T_H, T, Q);		\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(T));		\
      }									\
      cctests_assert_asciiz(L, EXPECTED_DIRNAME, ccptn_ptr(T));		\
      cce_run_body_handlers(L);						\
    }									\
  }

TEST_INIT_DIRNAME(test_4_2_1, "/path/to/file.ext",	"/path/to/")
TEST_INIT_DIRNAME(test_4_2_2, "/path/to/dir/",		"/path/to/")
TEST_INIT_DIRNAME(test_4_2_3, ".",			"..")
TEST_INIT_DIRNAME(test_4_2_4, "/",			"/")

/* ------------------------------------------------------------------ */

/* Test  for "ccname_new(ccptn_t,  dirname, clean)()"  and input  pathname having  no
   dirname. */
#define TEST_NEW_DIRNAME_NO(FUNCNAME,INPUT_PATHNAME)			\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
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
      ccptn_t const		*P, *Q;					\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      ccname_new(ccptn_t, dirname, clean)(L, A, T_H, Q);		\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);						\
    }									\
  }

TEST_NEW_DIRNAME_NO(test_4_3_1, "file.ext")
TEST_NEW_DIRNAME_NO(test_4_3_2, "..")

/* ------------------------------------------------------------------ */

void
test_4_4_1 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, dirname, error)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], Q_H[1], S_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t const	*P, *Q, *S;

    P = ccname_new(ccptn_t, pointer,   clean)(L, A, P_H, pathname);
    Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);
    {
      cce_location_t		inner_L[1];
      ccptn_error_handler_t	T_H[1];

      if (cce_location(inner_L)) {
	cce_run_catch_handlers_raise(inner_L, L);
      } else {
	S = ccname_new(ccptn_t, dirname, error)(inner_L, A, T_H, Q);
	cctests_assert_asciiz(inner_L, "/path/to/", ccptn_ptr(S));
	cce_run_body_handlers(inner_L);
      }
    }
    ccptn_init_and_register_handler(L, S_H, S);
    cctests_assert_asciiz(L, "/path/to/", ccptn_ptr(S));
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Filenames.
 ** ----------------------------------------------------------------- */

/* Test for "ccname_new(ccptn_t, filename,  clean)()". */
#define TEST_NEW_FILENAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_FILENAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t const		*P, *Q, *T;				\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      T = ccname_new(ccptn_t, filename,  clean)(L, A, T_H, Q);		\
									\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(T));		\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_FILENAME, ccptn_ptr(T));	\
									\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_FILENAME(test_5_1_1, "/path/to/file.ext",	"file.ext")
TEST_NEW_FILENAME(test_5_1_2, "file.ext",		"file.ext")
TEST_NEW_FILENAME(test_5_1_3, ".fvwmrc",		".fvwmrc")
TEST_NEW_FILENAME(test_5_1_4, "~/.fvwmrc",		".fvwmrc")

/* ------------------------------------------------------------------ */

/* Test for "ccname_init(ccptn_t, filename, clean)()". */
#define TEST_INIT_FILENAME(FUNCNAME,INPUT_PATHNAME,EXPECTED_FILENAME)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
									\
    if (cce_location(L)) {						\
      fprintf(stderr, "%s: exception: %s\n", __func__,			\
	      cce_condition_static_message(cce_condition(L)));		\
      cce_run_catch_handlers_raise(L, upper_L);				\
    } else {								\
      static char const *	pathname = INPUT_PATHNAME;		\
      ccptn_t const		*P, *Q;					\
      ccptn_t			T[1];					\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      ccname_init(ccptn_t, filename, clean)(L, A, T_H, T, Q);		\
									\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(T));		\
      }									\
									\
      cctests_assert_asciiz(L, EXPECTED_FILENAME, ccptn_ptr(T));	\
									\
      cce_run_body_handlers(L);					\
    }									\
  }

TEST_NEW_FILENAME(test_5_2_1, "/path/to/file.ext",	"file.ext")
TEST_NEW_FILENAME(test_5_2_2, "file.ext",		"file.ext")
TEST_NEW_FILENAME(test_5_2_3, ".fvwmrc",		".fvwmrc")
TEST_NEW_FILENAME(test_5_2_4, "~/.fvwmrc",		".fvwmrc")

/* ------------------------------------------------------------------ */

/* Test  for  "ccname_new(ccptn_t,  filename,   clean)()"  with  pathname  having  no
   filename. */
#define TEST_NEW_FILENAME_NO(FUNCNAME, INPUT_PATHNAME)			\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t		L[1];					\
    ccptn_clean_handler_t	P_H[1], Q_H[1], T_H[1];			\
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
      ccptn_t const		*P, *Q;					\
									\
      P = ccname_new(ccptn_t, pointer,    clean)(L, A, P_H, pathname);	\
      Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);		\
      if (0) {								\
	fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(Q));		\
      }									\
      ccname_new(ccptn_t, filename, clean)(L, A, T_H, Q);		\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);						\
    }									\
  }

TEST_NEW_FILENAME_NO(test_5_3_1, ".")
TEST_NEW_FILENAME_NO(test_5_3_2, "..")
TEST_NEW_FILENAME_NO(test_5_3_3, "/path/to/dir/")
TEST_NEW_FILENAME_NO(test_5_3_4, "/")

/* ------------------------------------------------------------------ */

void
test_5_4_1 (cce_destination_t upper_L)
/* Test for "ccptn_new_filename_guarded()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], Q_H[1], S_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t const	*P, *Q, *S;

    P = ccname_new(ccptn_t, pointer,   clean)(L, A, P_H, pathname);
    Q = ccname_new(ccptn_t, normalise, clean)(L, A, Q_H, P);
    {
      cce_location_t		inner_L[1];
      ccptn_error_handler_t	T_H[1];

      if (cce_location(inner_L)) {
	cce_run_catch_handlers_raise(inner_L, L);
      } else {
	S = ccname_new(ccptn_t, filename, error)(inner_L, A, T_H, Q);
	cctests_assert_asciiz(inner_L, "file.ext", ccptn_ptr(S));
	cce_run_body_handlers(inner_L);
      }
    }
    ccptn_init_and_register_handler(L, S_H, S);
    cctests_assert_asciiz(L, "file.ext", ccptn_ptr(S));
    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  ccptn_library_init();

  A = ccmem_standard_allocator;

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
