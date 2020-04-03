/*
  Part of: CCPathnames
  Contents: input/output functions
  Date: Fri Mar  2, 2018

  Abstract



  Copyright (C) 2018, 2019, 2020 Marco Maggi <mrc.mgg@gmail.com>

  This is free software; you can redistribute  it and/or modify it under the terms of
  the GNU Lesser General Public License as published by the Free Software Foundation;
  either version 3.0 of the License, or (at your option) any later version.

  This library  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
  Suite 330, Boston, MA 02111-1307 USA.

*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccpathnames-internals.h"
#include <errno.h>


/** --------------------------------------------------------------------
 ** Printing.
 ** ----------------------------------------------------------------- */

void
ccptn_fwrite (cce_destination_t L, FILE * stream, ccptn_t const * P)
{
  int	rv;

  errno = 0;
  rv = fwrite(ccptn_ptr(P), sizeof(char), ccptn_len(P), stream);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}

/* end of file */
