/*
  Part of: CCPathnames
  Contents: test for pathname manipulation functions
  Date: Feb 27, 2018

  Abstract

	Test file for pathname manipulation functions.

  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#include <ccpathnames.h>
#include "cctests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char const * progname;


/** --------------------------------------------------------------------
 ** Composing pathnames.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
/* Test for "ccptn_append()". */
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	P1_H[1];
  cce_cleanup_handler_t	P2_H[1];
  cce_cleanup_handler_t	R_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    static char const *	pathname_1 = "/path/to";
    static char const *	pathname_2 = "file.ext";
    ccptn_t		*P1, *P2, *R;

    P1 = ccptn_new_nodup_asciiz(L, pathname_1);
    ccptn_handler_ptn_init(L, P1_H, P1);

    P2 = ccptn_new_nodup_asciiz(L, pathname_2);
    ccptn_handler_ptn_init(L, P2_H, P2);

    R = ccptn_append(L, P1, P2);
    ccptn_handler_ptn_init(L, R_H, R);

    if (1) { fprintf(stderr, "%s: %s\n", __func__, ccptn_asciiz(R)); }

    cctests_assert(L, 0 == strcmp("/path/to/file.ext", ccptn_asciiz(R)));

    cce_run_cleanup_handlers(L);
  }
}


int
main (int argc CCPTN_UNUSED, const char *const argv[])
{
  progname = argv[0];

  ccptn_init();

  cctests_init("manipulation functions");
  {
    cctests_begin_group("composing pathnames");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
