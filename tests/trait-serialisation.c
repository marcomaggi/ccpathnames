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

static ccmem_allocator_t const * A;


/** --------------------------------------------------------------------
 ** Serialisation and deserialisation.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	P_H[1], Q_H[1];
  ccmem_clean_handler_t	B_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const		*input = "/path/to/file.ext";
    ccmem_block_t	B;

    {
      ccptn_t const		*P;
      ccstructs_serialiser_T	IS;

      P  = ccname_new(ccptn_t, pointer, clean)(L, A, P_H, input);
      IS = ccname_trait_new(ccstructs_serialiser_T, ccptn_t)(P);
      B  = ccmem_block_malloc_guarded(L, B_H, A, ccstructs_serialiser_required_size(IS));
      ccstructs_serialiser_write(L, IS, B);
    }

    {
      ccptn_t			*Q;
      ccstructs_deserialiser_T	ID;

      Q  = ccname_new(ccptn_t, deserialisable, clean)(L, A, Q_H);
      ID = ccname_trait_new(ccstructs_deserialiser_T, ccptn_t)(Q);
      ccstructs_deserialiser_read(L, ID, B);

      cctests_assert_asciiz(L, input, ccptn_ptr(Q));
    }

    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  ccptn_library_init();

  A = ccmem_standard_allocator;

  cctests_init("serialiser/deserialiser traits");
  {
    cctests_begin_group("serialisation and deserialisation");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
