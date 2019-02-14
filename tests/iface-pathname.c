/*
  Part of: CCPathnames
  Contents: test for interface "pathname"
  Date: Jan 29, 2019

  Abstract

	Test file for interface "pathname".

  Copyright (C) 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#include "ccpathnames.h"
#include <cctests.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ccmem_allocator_t const * A;


/** --------------------------------------------------------------------
 ** Tests for the "destructor" interface.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Pathname instance built with "ccname_new". */
{
  cce_location_t		L[1];
  ccstructs_clean_handler_t	H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const		*input = "/path/to/file.ext";
    ccptn_t const		*P;
    ccstructs_pathname_I	IPTN;
    ccstructs_dtor_I		IDTOR;

    P     = ccname_new(ccptn_t, pointer)(L, A, input);
    IPTN  = ccname_iface_new(ccstructs_pathname_I, ccptn_t)(P);
    IDTOR = ccname_iface_new(ccstructs_dtor_I, ccstructs_pathname_I)(IPTN);
    ccstructs_handler_init(L, H, IDTOR);

    {
      ccstructs_dumpable_I	IDUMP = ccname_iface_new(ccstructs_dumpable_I, ccstructs_pathname_I)(L, IPTN);

      ccstructs_dumpable_dump(L, IDUMP);
      fprintf(stderr, "\n");
    }
    cce_run_body_handlers(L);
  }
}

void
test_1_2 (cce_destination_t upper_L)
/* Pathname instance built with "ccname_init". */
{
  cce_location_t		L[1];
  ccstructs_clean_handler_t	H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const		*input = "/path/to/file.ext";
    ccptn_t			P[1];
    ccstructs_pathname_I	IPTN;
    ccstructs_dtor_I		IDTOR;

    ccname_init(ccptn_t, pointer)(L, A, P, input);
    IPTN  = ccname_iface_new(ccstructs_pathname_I, ccptn_t)(P);
    IDTOR = ccname_iface_new(ccstructs_dtor_I, ccstructs_pathname_I)(IPTN);
    ccstructs_handler_init(L, H, IDTOR);

    {
      ccstructs_dumpable_I	IDUMP = ccname_iface_new(ccstructs_dumpable_I, ccstructs_pathname_I)(L, IPTN);

      ccstructs_dumpable_dump(L, IDUMP);
      fprintf(stderr, "\n");
    }
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Tests for predicates.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
{
  cce_location_t		L[1];
  ccstructs_clean_handler_t	H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const		*input = "/path/to/file.ext";
    ccptn_t const		*P;
    ccstructs_pathname_I	IPTN;
    ccstructs_dtor_I		IDTOR;

    P     = ccname_new(ccptn_t, pointer)(L, A, input);
    IPTN  = ccname_iface_new(ccstructs_pathname_I, ccptn_t)(P);
    IDTOR = ccname_iface_new(ccstructs_dtor_I, ccstructs_pathname_I)(IPTN);
    ccstructs_handler_init(L, H, IDTOR);

    cctests_assert(L, false == ccstructs_pathname_is_static(IPTN));
    cctests_assert(L, true  == ccstructs_pathname_is_absolute(L, IPTN));
    cctests_assert(L, false == ccstructs_pathname_is_relative(L, IPTN));
    cctests_assert(L, false == ccstructs_pathname_is_normalised(L, IPTN));
    cctests_assert(L, false == ccstructs_pathname_is_realpath(L, IPTN));
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Tests for the "serialiser" and "deserialiser" interfaces.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L)
{
  cce_location_t		L[1];
  ccstructs_clean_handler_t	P_H[1], Q_H[1];
  ccmem_block_t			M, M_leftover;
  ccmem_clean_handler_t		M_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccstructs_pathname_I	P_IPTN;

    {
      static char const		*input = "/path/to/file.ext";
      ccptn_t const		*P;
      ccstructs_dtor_I		P_IDTOR;

      P     = ccname_new(ccptn_t, pointer)(L, A, input);
      P_IPTN  = ccname_iface_new(ccstructs_pathname_I, ccptn_t)(P);
      P_IDTOR = ccname_iface_new(ccstructs_dtor_I, ccstructs_pathname_I)(P_IPTN);
      ccstructs_handler_init(L, P_H, P_IDTOR);

      {
	/* Build the "serialiser" interface. */
	ccstructs_serialiser_I P_ISER = ccname_iface_new(ccstructs_serialiser_I, ccstructs_pathname_I)(L, P_IPTN);

	/* Allocate memory for the serialisation. */
	M = ccmem_block_malloc_guarded(L, M_H, A, ccstructs_serialiser_required_size(P_ISER));

	/* Serialise the struct. */
	M_leftover = ccstructs_serialiser_write(L, P_ISER, M);
      }
    }

    {
      ccptn_t			*Q;
      ccstructs_pathname_I	Q_IPTN;
      ccstructs_dtor_I		Q_IDTOR;
      ccstructs_deserialiser_I	Q_IDES;

      /* Build a struct to be target of deserialisation. */
      Q       = ccname_new(ccptn_t, deserialisable)(L, A);
      Q_IPTN  = ccname_iface_new(ccstructs_pathname_I, ccptn_t)(Q);
      Q_IDTOR = ccname_iface_new(ccstructs_dtor_I, ccstructs_pathname_I)(Q_IPTN);
      ccstructs_handler_init(L, Q_H, Q_IDTOR);

      /* Build the "deserialiser" interface. */
      Q_IDES = ccname_iface_new(ccstructs_deserialiser_I, ccstructs_pathname_I)(L, Q_IPTN);

      /* Deserialise the struct. */
      M_leftover = ccstructs_deserialiser_read(L, Q_IDES, M);

      /* Check the deserialisation results. */
      {
	ccmem_asciiz_t		rep_P = ccstructs_pathname_asciiz(L, P_IPTN);
	ccmem_asciiz_t		rep_Q = ccstructs_pathname_asciiz(L, Q_IPTN);

	cctests_assert_equal_size(L, rep_P.len, rep_Q.len);
	cctests_assert_asciiz(L, rep_P.ptr, rep_Q.ptr);
	cctests_assert(L, M_leftover.ptr > M.ptr);
	cctests_assert(L, M_leftover.len < M.len);
      }
    }

    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  ccptn_library_init();

  A = ccmem_standard_allocator;

  cctests_init("pathname interface");
  {
    cctests_begin_group("destructor and dumpable interfaces");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
    }
    cctests_end_group();

    cctests_begin_group("predicates");
    {
      cctests_run(test_2_1);
    }
    cctests_end_group();

    cctests_begin_group("serialiser/deserialiser interfaces");
    {
      cctests_run(test_3_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
