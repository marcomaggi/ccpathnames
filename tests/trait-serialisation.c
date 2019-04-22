/*
  Part of: CCPathnames
  Contents: test for traits "serialiser" and  "deserialiser"
  Date: Jan 29, 2019

  Abstract

	Test file for traits "serialiser" and "deserialiser".

  Copyright (C) 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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
 ** Serialisation and deserialisation.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Serialise a normal pathname. */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], Q_H[1];
  ccmem_clean_handler_t	storage_block_H[1];

  if (cce_location(L)) {
    if (0) {fprintf(stderr, "%s: exception: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const		*input = "/path/to/file.ext";
    ccmem_block_t	storage_block;
    ccmem_block_t	leftovers_block CCPTN_UNUSED;

    {
      ccptn_t const		*P;
      ccstructs_serialiser_T	IS;

      P  = ccname_new(ccptn_t, pointer, clean)(L, ccmem_standard_allocator, P_H, input);
      IS = ccname_trait_new(ccstructs_serialiser_T, ccptn_t)(P);
      storage_block   = ccmem_block_malloc_guarded(L, storage_block_H, ccmem_standard_allocator, ccstructs_serialiser_required_size(IS));
      leftovers_block = ccstructs_serialiser_write(L, IS, storage_block);
    }

    {
      ccptn_t			*Q;
      ccstructs_deserialiser_T	ID;

      Q  = ccname_new(ccptn_t, deserialisable, clean)(L, ccmem_standard_allocator, Q_H);
      ID = ccname_trait_new(ccstructs_deserialiser_T, ccptn_t)(Q);
      ccstructs_deserialiser_read(L, ID, storage_block);

      cctests_assert_asciiz(L, input, ccptn_ptr(Q));
    }

    cce_run_body_handlers(L);
  }
}

void
test_1_2 (cce_destination_t upper_L)
/* Serialise a normal pathname with length 1. */
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], Q_H[1];
  ccmem_clean_handler_t	storage_block_H[1];

  if (cce_location(L)) {
    if (0) {fprintf(stderr, "%s: exception: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const		*input = "/";
    ccmem_block_t	storage_block;
    ccmem_block_t	leftovers_block CCPTN_UNUSED;

    {
      ccptn_t const		*P;
      ccstructs_serialiser_T	IS;

      P  = ccname_new(ccptn_t, pointer, clean)(L, ccmem_standard_allocator, P_H, input);
      IS = ccname_trait_new(ccstructs_serialiser_T, ccptn_t)(P);
      storage_block   = ccmem_block_malloc_guarded(L, storage_block_H, ccmem_standard_allocator, ccstructs_serialiser_required_size(IS));
      leftovers_block = ccstructs_serialiser_write(L, IS, storage_block);
    }

    {
      ccptn_t			*Q;
      ccstructs_deserialiser_T	ID;

      Q  = ccname_new(ccptn_t, deserialisable, clean)(L, ccmem_standard_allocator, Q_H);
      ID = ccname_trait_new(ccstructs_deserialiser_T, ccptn_t)(Q);
      ccstructs_deserialiser_read(L, ID, storage_block);

      cctests_assert_asciiz(L, input, ccptn_ptr(Q));
    }

    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  ccptn_library_init();

  cctests_init("serialiser/deserialiser traits");
  {
    cctests_begin_group("serialisation and deserialisation");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
