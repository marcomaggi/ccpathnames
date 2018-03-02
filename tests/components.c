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

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccptn_extension()". */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P_H[1], Q_H[1];

  if (cce_location(L)) {
    fprintf(stderr, "%s: exception: %s\n", __func__, cce_condition_static_message(cce_condition(L)));
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname = "/path/to/file.ext";
    ccptn_t		*P, *Q;
    ccptn_extension_t	E;

    P = ccptn_new_nodup_asciiz(L, pathname);
    ccptn_handler_ptn_init(L, P_H, P);

    Q = ccptn_new_normalise(L, P);
    ccptn_handler_ptn_init(L, Q_H, Q);

    E = ccptn_extension(L, Q);

    if (0) { fprintf(stderr, "%s: %s\n", __func__, E.ptr); }

    cctests_assert_asciiz(L, ".ext", E.ptr);

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
    cctests_begin_group("extensions and rootname");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
