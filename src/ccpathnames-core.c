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
 ** Helpers.
 ** ----------------------------------------------------------------- */

static void
scan_for_non_terminating_zeros (cce_destination_t L, char const * pathname, size_t len)
/* Scan for zero octets in side the pathname. */
{
  for (size_t i=0; i<len; ++i) {
    if ('\0' == pathname[i]) {
      cce_raise(L, ccptn_condition_new_invalid_length());
    }
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCIIZ input, allocated struct, nodup data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_new_nodup_asciiz (ccptn_t * P)
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
  size_t	len = strlen(pathname);

  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    ccptn_t *	P = cce_sys_malloc(L, sizeof(ccptn_t));

    P->delete		= delete_for_ccptn_new_nodup_asciiz;
    P->len		= len;
    P->absolute		= ('/' == *pathname)? 1 : 0;
    P->normalised	= 0;
    P->realpath		= 0;
    P->buf		= (char *)pathname;
    return P;
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCIIZ input, allocated struct, dup data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_new_dup_asciiz (ccptn_t * P)
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

  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    ccptn_t *	P;

    P			= cce_sys_malloc(L, sizeof(ccptn_t) + len + 1);
    P->delete		= delete_for_ccptn_new_dup_asciiz;
    P->len		= len;
    P->absolute		= ('/' == *pathname)? 1 : 0;
    P->normalised	= 0;
    P->realpath		= 0;
    P->buf		= (char *)(((uint8_t *)P) + sizeof(ccptn_t));
    strncpy(P->buf, pathname, len);
    P->buf[len]		= '\0';
    return P;
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCIIZ input, allocated struct, normal data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_new_normal_asciiz (ccptn_t * P)
{
  free(P);
}

ccptn_t *
ccptn_new_normal_asciiz (cce_destination_t L, char const * pathname)
/* Allocate a new "ccptn_t" instance  initialising it with data from the
 * ASCIIZ string "pathname".  The data  *is* duplicated and the pathname
 * is normalised.
 *
 * The finalisation function registered in the instance will release the
 * struct itself, and the data buffer.
 */
{
  size_t	len = strlen(pathname);

  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    char	one[1 + len];
    size_t	one_len;

    one_len = ccptn_normal_pass_remove_useless_slashes(one, pathname, len);
    {
      char	two[1 + one_len];
      size_t	two_len;
      ccptn_t *	P;

      two_len = ccptn_normal_pass_remove_single_dot_segments(two, one, one_len);
      one_len = ccptn_normal_pass_remove_double_dot_segments(L, one, two, two_len);
      P			= cce_sys_malloc(L, sizeof(ccptn_t) + one_len + 1);
      P->delete		= delete_for_ccptn_new_normal_asciiz;
      P->len		= one_len;
      P->absolute	= ('/' == one[0])? 1 : 0;
      P->normalised	= 1;
      P->realpath	= 0;
      P->buf		= (char *)(((uint8_t *)P) + sizeof(ccptn_t));
      strncpy(P->buf, one, one_len);
      P->buf[one_len]	= '\0';
      return P;
    }
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCIIZ input, existent struct, nodup data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_init_nodup_asciiz (ccptn_t * P CCPTN_UNUSED)
{
}

ccptn_t *
ccptn_init_nodup_asciiz (cce_destination_t L CCPTN_UNUSED, ccptn_t * P, char const * pathname)
/* Initialise an already allocated "ccptn_t" instance with data from the
 * ASCIIZ string "pathname".  The data is *not* duplicated: the instance
 * references "pathname" itself.
 *
 * The finalisation function registered in the instance will do nothing.
 */
{
  size_t	len = strlen(pathname);

  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    P->delete		= delete_for_ccptn_init_nodup_asciiz;
    P->len		= strlen(pathname);
    P->absolute		= ('/' == *pathname)? 1 : 0;
    P->normalised	= 0;
    P->realpath		= 0;
    P->buf		= (char *)pathname;
    return P;
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCIIZ input, existent struct, dup data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_init_dup_asciiz (ccptn_t * P)
{
  free(P->buf);
}

ccptn_t *
ccptn_init_dup_asciiz (cce_destination_t L, ccptn_t * P, char const * pathname)
/* Initialise an already allocted "ccptn_t"  instance with data from the
 * ASCIIZ  string "pathname".   The data  *is* duplicated:  the instance
 * includes a copy of the data from "pathname".
 *
 * The finalisation function registered in the instance will release the
 * data area but not the struct itself.
 */
{
  size_t	len = strlen(pathname);

  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    P->delete		= delete_for_ccptn_init_dup_asciiz;
    P->len		= len;
    P->absolute		= ('/' == *pathname)? 1 : 0;
    P->normalised	= 0;
    P->realpath		= 0;
    P->buf		= cce_sys_malloc(L, len + 1);
    strncpy(P->buf, pathname, len);
    P->buf[len]		= '\0';
    return P;
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCIIZ input, existent struct, normal data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_init_normal_asciiz (ccptn_t * P)
{
  free(P->buf);
}

ccptn_t *
ccptn_init_normal_asciiz (cce_destination_t L, ccptn_t * P, char const * pathname)
/* Initialise an already allocted "ccptn_t"  instance with data from the
 * ASCIIZ string "pathname".   The data *is* duplicated  the pathname is
 * normalised.
 *
 * The finalisation function registered in the instance will release the
 * data area but not the struct itself.
 */
{
  size_t	len = strlen(pathname);

  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    char	one[1 + len];
    size_t	one_len;

    one_len = ccptn_normal_pass_remove_useless_slashes(one, pathname, len);
    {
      char	two[1 + one_len];
      size_t	two_len;

      two_len = ccptn_normal_pass_remove_single_dot_segments(two, one, one_len);
      one_len = ccptn_normal_pass_remove_double_dot_segments(L, one, two, two_len);
      P->delete		= delete_for_ccptn_init_normal_asciiz;
      P->len		= one_len;
      P->absolute	= ('/' == one[0])? 1 : 0;
      P->normalised	= 1;
      P->realpath	= 0;
      P->buf		= cce_sys_malloc(L, one_len + 1);
      strncpy(P->buf, one, one_len);
      P->buf[one_len]	= '\0';
      return P;
    }
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCII input, allocated struct, dup data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_new_dup_ascii (ccptn_t * P)
{
  free(P);
}

ccptn_t *
ccptn_new_dup_ascii (cce_destination_t L, char const * pathname, size_t len)
/* Allocate a new "ccptn_t" instance  initialising it with data from the
 * ASCII string "pathname" which  holds "len" octets without terminating
 * zero.  The  data *is* duplicated:  the returned "ccptn_t"  includes a
 * copy of the data from "pathname".
 *
 * The finalisation function registered in the instance will release the
 * struct itself, and the data buffer.
 */
{
  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    ccptn_t *	P;

    scan_for_non_terminating_zeros(L, pathname, len);
    P = cce_sys_malloc(L, sizeof(ccptn_t) + len + 1);
    P->delete		= delete_for_ccptn_new_dup_ascii;
    P->len		= len;
    P->absolute		= ('/' == *pathname)? 1 : 0;
    P->normalised	= 0;
    P->realpath		= 0;
    P->buf		= (char *)(((uint8_t *)P) + sizeof(ccptn_t));
    strncpy(P->buf, pathname, len);
    P->buf[len]		= '\0';
    return P;
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCII input, allocated struct, normal data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_new_normal_ascii (ccptn_t * P)
{
  free(P);
}

ccptn_t *
ccptn_new_normal_ascii (cce_destination_t L, char const * pathname, size_t len)
/* Allocate a new "ccptn_t" instance  initialising it with data from the
 * ASCII string "pathname" which  holds "len" octets without terminating
 * zero.  The  data *is* duplicated:  the returned "ccptn_t"  includes a
 * copy of the data from "pathname".
 *
 * The finalisation function registered in the instance will release the
 * struct itself, and the data buffer.
 */
{
  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    scan_for_non_terminating_zeros(L, pathname, len);

    char	one[1 + len];
    size_t	one_len;

    one_len = ccptn_normal_pass_remove_useless_slashes(one, pathname, len);
    {
      char	two[1 + one_len];
      size_t	two_len;

      two_len = ccptn_normal_pass_remove_single_dot_segments(two, one, one_len);
      one_len = ccptn_normal_pass_remove_double_dot_segments(L, one, two, two_len);
      {
	ccptn_t *	P;
	P = cce_sys_malloc(L, sizeof(ccptn_t) + one_len + 1);
	P->delete	= delete_for_ccptn_new_normal_ascii;
	P->len		= one_len;
	P->absolute	= ('/' == one[0])? 1 : 0;
	P->normalised	= 1;
	P->realpath	= 0;
	P->buf		= (char *)(((uint8_t *)P) + sizeof(ccptn_t));
	strncpy(P->buf, one, one_len);
	P->buf[one_len]	= '\0';
	return P;
      }
    }
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCII input, existent struct, dup data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_init_dup_ascii (ccptn_t * P)
{
  free(P->buf);
}

ccptn_t *
ccptn_init_dup_ascii (cce_destination_t L, ccptn_t * P, char const * pathname, size_t len)
/* Initialise an already allocted "ccptn_t"  instance with data from the
 * ASCII string "pathname" which  holds "len" octets without terminating
 * zero.  The data *is* duplicated: the  instance includes a copy of the
 * data from "pathname".
 *
 * The finalisation function registered in the instance will release the
 * data area but not the struct itself.
 */
{
  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    scan_for_non_terminating_zeros(L, pathname, len);
    P->delete		= delete_for_ccptn_init_dup_ascii;
    P->len		= len;
    P->absolute		= ('/' == *pathname)? 1 : 0;
    P->normalised	= 0;
    P->realpath		= 0;
    P->buf		= cce_sys_malloc(L, len + 1);
    strncpy(P->buf, pathname, len);
    P->buf[len]		= '\0';
    return P;
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions: ASCII input, existent struct, normal data.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void
delete_for_ccptn_init_normal_ascii (ccptn_t * P)
{
  free(P->buf);
}

ccptn_t *
ccptn_init_normal_ascii (cce_destination_t L, ccptn_t * P, char const * pathname, size_t len)
/* Initialise an already allocted "ccptn_t"  instance with data from the
 * ASCII string "pathname" which  holds "len" octets without terminating
 * zero.  The data *is* duplicated and the pathname normalised.
 *
 * The finalisation function registered in the instance will release the
 * data area but not the struct itself.
 */
{
  if (CCPTN_PATH_MAX < len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < len) {
    scan_for_non_terminating_zeros(L, pathname, len);

    char	one[1 + len];
    size_t	one_len;

    one_len = ccptn_normal_pass_remove_useless_slashes(one, pathname, len);
    {
      char	two[1 + one_len];
      size_t	two_len;

      two_len = ccptn_normal_pass_remove_single_dot_segments(two, one, one_len);
      one_len = ccptn_normal_pass_remove_double_dot_segments(L, one, two, two_len);
      P->delete		= delete_for_ccptn_init_normal_ascii;
      P->len		= one_len;
      P->absolute	= ('/' == one[0])? 1 : 0;
      P->normalised	= 1;
      P->realpath	= 0;
      P->buf		= cce_sys_malloc(L, one_len + 1);
      strncpy(P->buf, one, one_len);
      P->buf[one_len]	= '\0';
      return P;
    }
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}


/** --------------------------------------------------------------------
 ** Exception handlers.
 ** ----------------------------------------------------------------- */

__attribute__((nonnull(1,2)))
static void
ccptn_handler_ptn_function (const cce_condition_t * C CCE_UNUSED, cce_handler_t * H)
{
  ccptn_delete(H->pointer);
  if (0) { fprintf(stderr, "%s: done releasing '%p'\n", __func__, (void *)(H->pointer)); }
}

void
ccptn_cleanup_handler_ptn_init (cce_location_t * L, cce_handler_t * H, ccptn_t * P)
{
  H->function	= ccptn_handler_ptn_function;
  H->pointer	= P;
  cce_register_cleanup_handler(L, H);
}

void
ccptn_error_handler_ptn_init (cce_location_t * L, cce_handler_t * H, ccptn_t * P)
{
  H->function	= ccptn_handler_ptn_function;
  H->pointer	= P;
  cce_register_error_handler(L, H);
}

/* end of file */
