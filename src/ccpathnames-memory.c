/*
  Part of: CCPathnames
  Contents: memory allocator
  Date: Mon Mar 19, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you  can redistribute it and/or modify it under
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

#include "ccpathnames-internals.h"


/** --------------------------------------------------------------------
 ** Default allocator.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void *
ccptn_default_malloc (ccptn_allocator_t const * const A CCPTN_UNUSED, cce_destination_t L, size_t const size)
{
  return cce_sys_malloc(L, size);
}

__attribute__((__nonnull__(1,2)))
static void
ccptn_default_free (ccptn_allocator_t const * const A CCPTN_UNUSED, void * ptr)
{
  free(ptr);
}

static ccptn_allocator_t const ccptn_default_allocator_stru = {
  .malloc	= ccptn_default_malloc,
  .free		= ccptn_default_free,
};


/** --------------------------------------------------------------------
 ** Public API.
 ** ----------------------------------------------------------------- */

ccptn_allocator_t const * const ccptn_default_allocator = &ccptn_default_allocator_stru;
ccptn_allocator_t const *       ccptn_current_allocator = &ccptn_default_allocator_stru;

void *
ccptn_malloc (cce_destination_t L, size_t size)
{
  return ccptn_current_allocator->malloc(ccptn_current_allocator, L, size);
}

void
ccptn_free (void * ptr)
{
  ccptn_current_allocator->free(ccptn_current_allocator, ptr);
}

/* end of file */
