/*
  Part of: CCPathnames
  Contents: core functions for pathnames manipulation
  Date: Sun Feb 25, 2018

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
#include <stdlib.h>


/** --------------------------------------------------------------------
 ** Constructor functions.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
ccptn_delete_nodup_asciiz (ccptn_t * P)
{
  free(P);
}

ccptn_t *
ccptn_new_nodup_asciiz (cce_destination_t L, char const * pathname)
/* Allocate a new "ccptn_t" instance  initialising it with data from the
 * ASCIIZ string "pathname".  The data is *not* duplicated: the returned
 * "ccptn_t" references "pathname" itself.
 *
 * The finalisation function registered in the instance will release the
 * struct itself, but not the data buffer.
 */
{
  ccptn_t *	P = cce_sys_malloc(L, sizeof(ccptn_t));

  P->final	= ccptn_delete_nodup_asciiz;
  P->len	= strlen(pathname);
  P->normalised	= 0;
  P->realpath	= 0;
  P->buf	= (char *)pathname;
  return P;
}

/* ------------------------------------------------------------------ */

__attribute__((__nonnull__(1)))
static void
ccptn_delete_dup_asciiz (ccptn_t * P)
{
  free(P);
}

ccptn_t *
ccptn_new_dup_asciiz (cce_destination_t L, char const * pathname)
/* Allocate a new "ccptn_t" instance  initialising it with data from the
 * ASCIIZ  string "pathname".   The data  *is* duplicated:  the returned
 * "ccptn_t" includes a copy of the data from "pathname".
 *
 * The finalisation function registered in the instance will release the
 * struct itself, and the data buffer.
 */
{
  size_t	len = strlen(pathname);
  ccptn_t *	P;

  P		= cce_sys_malloc(L, sizeof(ccptn_t) + len + 1);
  P->final	= ccptn_delete_dup_asciiz;
  P->len	= len;
  P->normalised	= 0;
  P->realpath	= 0;
  P->buf	= (char *)(((uint8_t *)P) + sizeof(ccptn_t));
  strncpy(P->buf, pathname, len);
  P->buf[len]	= '\0';
  return P;
}

/* ------------------------------------------------------------------ */

ccptn_t *
ccptn_init_nodup_asciiz (cce_destination_t L CCPTN_UNUSED, ccptn_t * P, char const * pathname)
/* Initialise an already allocated "ccptn_t" instance with data from the
 * ASCIIZ string "pathname".  The data is *not* duplicated: the instance
 * references "pathname" itself.
 *
 * The finalisation function registered in the instance will do nothing.
 */
{
  P->final	= NULL;
  P->len	= strlen(pathname);
  P->normalised	= 0;
  P->realpath	= 0;
  P->buf	= (char *)pathname;
  return P;
}

/* ------------------------------------------------------------------ */

__attribute__((__nonnull__(1)))
static void
ccptn_final_dup_asciiz (ccptn_t * P)
{
  free(P->buf);
}

ccptn_t *
ccptn_init_dup_asciiz (cce_destination_t L, ccptn_t * P, char const * pathname)
/* Allocate a new "ccptn_t" instance  initialising it with data from the
 * ASCIIZ  string "pathname".   The data  *is* duplicated:  the instance
 * includes a copy of the data from "pathname".
 *
 * The finalisation function registered in the instance will release the
 * data area but not the struct itself.
 */
{
  size_t	len = strlen(pathname);

  P->final	= ccptn_final_dup_asciiz;
  P->len	= len;
  P->normalised	= 0;
  P->realpath	= 0;
  P->buf	= cce_sys_malloc(L, len + 1);
  strncpy(P->buf, pathname, len);
  P->buf[len]	= '\0';
  return P;
}

/* end of file */
