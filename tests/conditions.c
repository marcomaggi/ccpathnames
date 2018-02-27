/*
  Part of: CCPathnames
  Contents: tests for exceptional condition objects
  Date: Feb 27, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you can  redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the
  Free Software  Foundation; either version  3.0 of the License,  or (at
  your option) any later version.

  This library  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  Lesser General Public License for more details.

  You  should have  received a  copy of  the GNU  Lesser  General Public
  License along  with this library; if  not, write to  the Free Software
  Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
  USA.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccpathnames.h"
#include <cctests.h>


/** --------------------------------------------------------------------
 ** Condition objects: invalid pathnames.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccptn_condition_invalid_pathname_t". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (cce_condition_is_runtime_error(cce_condition(L))
	&& ccptn_condition_is_invalid_pathname(cce_condition(L))) {
      cce_run_cleanup_handlers_final(L);
    } else {
      cce_run_cleanup_handlers_raise(L, upper_L);
    }
  } else {
    cce_raise(L, cce_condition(ccptn_condition_new_invalid_pathname()));
    cce_run_cleanup_handlers(L);
  }
}


int
main (void)
{
  ccptn_init();

  cctests_init("condition objects");
  {
    cctests_begin_group("invalid pathname");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
