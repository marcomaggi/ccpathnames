/*
  Part of: CCPathnames
  Contents: test for trait "dumpable"
  Date: Jan 29, 2019

  Abstract

	Test file for trait "dumpable".

  Copyright (C) 2019, 2020 Marco Maggi <mrc.mgg@gmail.com>

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
 ** Just dump it.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  ccptn_clean_handler_t	H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__,
	    cce_condition_static_message(cce_condition(L)));
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    static char const		*input = "/path/to/file.ext";
    ccptn_t const		*P;
    ccstructs_dumpable_T	ID;

    P  = ccname_new(ccptn_t, pointer, clean)(L, A, H, input);
    ID = ccname_trait_new(ccstructs_dumpable_T, ccptn_t)(P);

    ccstructs_dumpable_dump(L, ID);
    fprintf(stderr, "\n");

    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  ccptn_library_init();

  A = ccmem_standard_allocator;

  cctests_init("dumpable trait");
  {
    cctests_begin_group("just dump it");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
