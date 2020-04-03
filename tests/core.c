/*
  Part of: CCPathnames
  Contents: test for core functions
  Date: Feb 25, 2018

  Abstract

	Test file for core functions.

  Copyright (C) 2018, 2019, 2020 Marco Maggi <mrc.mgg@gmail.com>

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

static ccmem_allocator_t const * A;


/** --------------------------------------------------------------------
 ** Constructors: from ASCIIZ pointers.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, pointer, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "/path/to/file.ext";
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, pointer, clean)(L, A, H, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input, ccptn_ptr(P));
    }
    cce_run_body_handlers(L);
  }
}

void
test_1_2 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, pointer, dup, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "/path/to/file.ext";
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, pointer, dup, clean)(L, A, H, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input, ccptn_ptr(P));
    }
    cce_run_body_handlers(L);
  }
}

void
test_1_3 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t, pointer, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "/path/to/file.ext";
    ccptn_t		P[1];

    ccname_init(ccptn_t, pointer, clean)(L, A, H, P, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input, ccptn_ptr(P));
    }
    cce_run_body_handlers(L);
  }
}

void
test_1_4 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t, pointer, dup, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "/path/to/file.ext";
    ccptn_t		P[1];

    ccname_init(ccptn_t, pointer, dup, clean)(L, A, H, P, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input, ccptn_ptr(P));
    }

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Constructors: from ASCIIZ blocks.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, asciiz)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_asciiz_t const	input = ccmem_new_asciiz_from_str("/path/to/file.ext");
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, asciiz, clean)(L, A, H, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input.ptr, ccptn_ptr(P));
    }
    cce_run_body_handlers(L);
  }
}

void
test_2_2 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, asciiz, dup)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_asciiz_t const	input = ccmem_new_asciiz_from_str("/path/to/file.ext");
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, asciiz, dup, clean)(L, A, H, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input.ptr, ccptn_ptr(P));
    }
    cce_run_body_handlers(L);
  }
}

void
test_2_3 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t, asciiz)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_asciiz_t const	input = ccmem_new_asciiz_from_str("/path/to/file.ext");
    ccptn_t		P[1];

    ccname_init(ccptn_t, asciiz, clean)(L, A, H, P, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input.ptr, ccptn_ptr(P));
    }
    cce_run_body_handlers(L);
  }
}

void
test_2_4 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t, asciiz, dup)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_asciiz_t const	input = ccmem_new_asciiz_from_str("/path/to/file.ext");
    ccptn_t		P[1];

    ccname_init(ccptn_t, asciiz, dup, clean)(L, A, H, P, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input.ptr, ccptn_ptr(P));
    }

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Constructors: from ASCII blocks.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, ascii)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_ascii_t const	input = ccmem_new_ascii_from_str("/path/to/file.ext");
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, ascii, clean)(L, A, H, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input.ptr, ccptn_ptr(P));
    }
    cce_run_body_handlers(L);
  }
}

void
test_3_2 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t, ascii)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_ascii_t const	input = ccmem_new_ascii_from_str("/path/to/file.ext");
    ccptn_t		P[1];

    ccname_init(ccptn_t, ascii, clean)(L, A, H, P, input);
    {
      if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_ptr(P)); }
      cctests_assert_asciiz(L, input.ptr, ccptn_ptr(P));
    }
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Predicates.
 ** ----------------------------------------------------------------- */

void
test_4_1_1 (cce_destination_t upper_L)
/* Test for "ccptn_is_absolute()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "/path/to/file.ext";
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, pointer, clean)(L, A, H, input);
    cctests_assert(L, true == ccptn_is_absolute(P));
    cce_run_body_handlers(L);
  }
}

void
test_4_1_2 (cce_destination_t upper_L)
/* Test for "ccptn_is_absolute()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "./path/to/file.ext";
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, pointer, clean)(L, A, H, input);
    cctests_assert(L, false == ccptn_is_absolute(P));
    cce_run_body_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_4_2_1 (cce_destination_t upper_L)
/* Test for "ccptn_is_relative()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "/path/to/file.ext";
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, pointer, clean)(L, A, H, input);
    cctests_assert(L, false == ccptn_is_relative(P));
    cce_run_body_handlers(L);
  }
}

void
test_4_2_2 (cce_destination_t upper_L)
/* Test for "ccptn_is_relative()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "./path/to/file.ext";
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, pointer, clean)(L, A, H, input);
    cctests_assert(L, true == ccptn_is_relative(P));
    cce_run_body_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_4_3_1 (cce_destination_t upper_L)
/* Test for "ccptn_is_realpath()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "/path/to/file.ext";
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, pointer, clean)(L, A, H, input);
    cctests_assert(L, false == ccptn_is_realpath(P));
    cce_run_body_handlers(L);
  }
}

void
test_4_3_2 (cce_destination_t upper_L)
/* Test for "ccptn_is_realpath()". */
{
#ifdef HAVE_REALPATH
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], Q_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	input = progname;
    ccptn_t const *	P;
    ccptn_t const *	Q;

    P = ccname_new(ccptn_t, pointer,  clean)(L, A, P_H, input);
    Q = ccname_new(ccptn_t, realpath, clean)(L, A, Q_H, P);
    cctests_assert(L, false == ccptn_is_realpath(P));
    cctests_assert(L, true  == ccptn_is_realpath(Q));
    cce_run_body_handlers(L);
  }
#endif
}


/** --------------------------------------------------------------------
 ** Constructors for deserialisable instances.
 ** ----------------------------------------------------------------- */

void
test_5_1 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t, deserialisable, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccptn_t	P[1];

    ccname_init(ccptn_t, deserialisable, clean)(L, A, H, P);

    {
      ccstructs_dumpable_T	I;

      I = ccname_trait_new(ccstructs_dumpable_T, ccptn_t)(P);
      ccstructs_dumpable_dump(L, I);
      fprintf(stderr, "\n");
    }
    cce_run_body_handlers(L);
  }
}

void
test_5_2 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, deserialisable, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccptn_t const *	P;

    P = ccname_new(ccptn_t, deserialisable, clean)(L, A, H);

    {
      ccstructs_dumpable_T	I;

      I = ccname_trait_new(ccstructs_dumpable_T, ccptn_t)(P);
      ccstructs_dumpable_dump(L, I);
      fprintf(stderr, "\n");
    }

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Constructors for clone instances.
 ** ----------------------------------------------------------------- */

void
test_6_1 (cce_destination_t upper_L)
/* Test for "ccname_init(ccptn_t, clone, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	dst_H[1], src_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "/path/to/file.ext";
    ccptn_t	dst[1], src[1];

    ccname_init(ccptn_t, pointer, clean)(L, A, src_H, src, input);
    ccname_init(ccptn_t, clone,   clean)(L, A, dst_H, dst, src);
    if (0) {
      ccstructs_dumpable_T	I;

      I = ccname_trait_new(ccstructs_dumpable_T, ccptn_t)(dst);
      ccstructs_dumpable_dump(L, I);
      fprintf(stderr, "\n");
    }
    cctests_assert_asciiz(L, ccptn_ptr(src), ccptn_ptr(dst));
    cctests_assert_equal_size(L, ccptn_len(src), ccptn_len(dst));
    cce_run_body_handlers(L);
  }
}

void
test_6_2 (cce_destination_t upper_L)
/* Test for "ccname_new(ccptn_t, clone, clean)()". */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	dst_H[1], src_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const *	input = "/path/to/file.ext";
    ccptn_t const	*dst, *src;

    src = ccname_new(ccptn_t, pointer, clean)(L, A, src_H, input);
    dst = ccname_new(ccptn_t, clone,   clean)(L, A, dst_H, src);

    if (0) {
      ccstructs_dumpable_T	I;

      I = ccname_trait_new(ccstructs_dumpable_T, ccptn_t)(dst);
      ccstructs_dumpable_dump(L, I);
      fprintf(stderr, "\n");
    }
    cctests_assert_asciiz(L, ccptn_ptr(src), ccptn_ptr(dst));
    cctests_assert_equal_size(L, ccptn_len(src), ccptn_len(dst));
    cce_run_body_handlers(L);
  }
}


int
main (int argc CCPTN_UNUSED, const char *const argv[])
{
  progname = argv[0];
  A = ccmem_standard_allocator;

  ccptn_library_init();

  cctests_init("core functions");
  {
    cctests_begin_group("constructors from ASCIIZ pointer");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
      cctests_run(test_1_4);
    }
    cctests_end_group();

    cctests_begin_group("constructors from ASCIIZ blocks");
    {
      cctests_run(test_2_1);
      cctests_run(test_2_2);
      cctests_run(test_2_3);
      cctests_run(test_2_4);
    }
    cctests_end_group();

    cctests_begin_group("constructors from ASCII blocks");
    {
      cctests_run(test_3_1);
      cctests_run(test_3_2);
    }
    cctests_end_group();

    cctests_begin_group("predicates");
    {
      cctests_run(test_4_1_1);
      cctests_run(test_4_1_2);
      cctests_run(test_4_2_1);
      cctests_run(test_4_2_2);
      cctests_run(test_4_3_1);
      cctests_run(test_4_3_2);
    }
    cctests_end_group();

    cctests_begin_group("constructors for deserialisable instances");
    {
      cctests_run(test_5_1);
      cctests_run(test_5_2);
    }
    cctests_end_group();

    cctests_begin_group("constructors for clone instances");
    {
      cctests_run(test_6_1);
      cctests_run(test_6_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
