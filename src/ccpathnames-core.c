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
  ccmem_free(P->allocator, P);
}

static ccptn_methods_t const ccptn_methods_for_ccptn_new_nodup_asciiz_stru = {
  .delete	= delete_for_ccptn_new_nodup_asciiz
};

ccptn_t *
ccptn_new_nodup_asciiz (cce_destination_t L, ccmem_allocator_t const * const A, char const * pathname)
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
    ccptn_t *	P = ccmem_malloc(L, A, sizeof(ccptn_t));

    P->methods		= &ccptn_methods_for_ccptn_new_nodup_asciiz_stru;
    P->allocator	= A;
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
  ccmem_free(P->allocator, P);
}

static ccptn_methods_t const ccptn_methods_for_ccptn_new_dup_asciiz_stru = {
  .delete	= delete_for_ccptn_new_dup_asciiz
};

ccptn_t *
ccptn_new_dup_asciiz (cce_destination_t L, ccmem_allocator_t const * const A, char const * pathname)
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

    P			= ccmem_malloc(L, A, sizeof(ccptn_t) + len + 1);
    P->methods		= &ccptn_methods_for_ccptn_new_dup_asciiz_stru;
    P->allocator	= A;
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
  ccmem_free(P->allocator, P);
}

static ccptn_methods_t const ccptn_methods_for_ccptn_new_normal_asciiz_stru = {
  .delete	= delete_for_ccptn_new_normal_asciiz
};

ccptn_t *
ccptn_new_normal_asciiz (cce_destination_t L, ccmem_allocator_t const * const A, char const * pathname)
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
      P			= ccmem_malloc(L, A, sizeof(ccptn_t) + one_len + 1);
      P->methods	= &ccptn_methods_for_ccptn_new_normal_asciiz_stru;
      P->allocator	= A;
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

static ccptn_methods_t const ccptn_methods_for_ccptn_init_nodup_asciiz_stru = {
  .delete	= delete_for_ccptn_init_nodup_asciiz
};

ccptn_t *
ccptn_init_nodup_asciiz (cce_destination_t L CCPTN_UNUSED, ccmem_allocator_t const * const A, ccptn_t * P, char const * pathname)
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
    P->methods		= &ccptn_methods_for_ccptn_init_nodup_asciiz_stru;
    P->allocator	= A;
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
  ccmem_free(P->allocator, P->buf);
}

static ccptn_methods_t const ccptn_methods_for_ccptn_init_dup_asciiz_stru = {
  .delete	= delete_for_ccptn_init_dup_asciiz
};

ccptn_t *
ccptn_init_dup_asciiz (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * P, char const * pathname)
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
    P->methods		= &ccptn_methods_for_ccptn_init_dup_asciiz_stru;
    P->allocator	= A;
    P->len		= len;
    P->absolute		= ('/' == *pathname)? 1 : 0;
    P->normalised	= 0;
    P->realpath		= 0;
    P->buf		= ccmem_malloc(L, A, len + 1);
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
  ccmem_free(P->allocator, P->buf);
}

static ccptn_methods_t const ccptn_methods_for_ccptn_init_normal_asciiz_stru = {
  .delete	= delete_for_ccptn_init_normal_asciiz
};

ccptn_t *
ccptn_init_normal_asciiz (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * P, char const * pathname)
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
      P->methods	= &ccptn_methods_for_ccptn_init_normal_asciiz_stru;
      P->allocator	= A;
      P->len		= one_len;
      P->absolute	= ('/' == one[0])? 1 : 0;
      P->normalised	= 1;
      P->realpath	= 0;
      P->buf		= ccmem_malloc(L, A, one_len + 1);
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
  ccmem_free(P->allocator, P);
}

static ccptn_methods_t const ccptn_methods_for_ccptn_new_dup_ascii_stru = {
  .delete	= delete_for_ccptn_new_dup_ascii
};

ccptn_t *
ccptn_new_dup_ascii (cce_destination_t L, ccmem_allocator_t const * const A, char const * pathname, size_t len)
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
    P = ccmem_malloc(L, A, sizeof(ccptn_t) + len + 1);
    P->methods		= &ccptn_methods_for_ccptn_new_dup_ascii_stru;
    P->allocator	= A;
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
  ccmem_free(P->allocator, P);
}

static ccptn_methods_t const ccptn_methods_for_ccptn_new_normal_ascii_stru = {
  .delete	= delete_for_ccptn_new_normal_ascii
};

ccptn_t *
ccptn_new_normal_ascii (cce_destination_t L, ccmem_allocator_t const * const A, char const * pathname, size_t len)
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
	P = ccmem_malloc(L, A, sizeof(ccptn_t) + one_len + 1);
	P->methods	= &ccptn_methods_for_ccptn_new_normal_ascii_stru;
	P->allocator	= A;
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
  ccmem_free(P->allocator, P->buf);
}

static ccptn_methods_t const ccptn_methods_for_ccptn_init_dup_ascii_stru = {
  .delete	= delete_for_ccptn_init_dup_ascii
};

ccptn_t *
ccptn_init_dup_ascii (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * P, char const * pathname, size_t len)
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
    P->methods		= &ccptn_methods_for_ccptn_init_dup_ascii_stru;
    P->allocator	= A;
    P->len		= len;
    P->absolute		= ('/' == *pathname)? 1 : 0;
    P->normalised	= 0;
    P->realpath		= 0;
    P->buf		= ccmem_malloc(L, A, len + 1);
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
  ccmem_free(P->allocator, P->buf);
}

static ccptn_methods_t const ccptn_methods_for_ccptn_init_normal_ascii_stru = {
  .delete	= delete_for_ccptn_init_normal_ascii
};

ccptn_t *
ccptn_init_normal_ascii (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * P, char const * pathname, size_t len)
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
      P->methods	= &ccptn_methods_for_ccptn_init_normal_ascii_stru;
      P->allocator	= A;
      P->len		= one_len;
      P->absolute	= ('/' == one[0])? 1 : 0;
      P->normalised	= 1;
      P->realpath	= 0;
      P->buf		= ccmem_malloc(L, A, one_len + 1);
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
ccptn_clean_handler_ptn_init (cce_location_t * L, ccptn_clean_handler_t * P_H, ccptn_t * P)
{
  P_H->handler.handler.function	= ccptn_handler_ptn_function;
  P_H->handler.handler.pointer	= P;
  cce_register_clean_handler(L, &(P_H->handler));
}

void
ccptn_error_handler_ptn_init (cce_location_t * L, ccptn_error_handler_t * P_H, ccptn_t * P)
{
  P_H->handler.handler.function	= ccptn_handler_ptn_function;
  P_H->handler.handler.pointer	= P;
  cce_register_error_handler(L, &(P_H->handler));
}


/** --------------------------------------------------------------------
 ** Pathnames: guarded constructors.
 ** ----------------------------------------------------------------- */

ccptn_t *
ccptn_init_nodup_asciiz_guarded_error (cce_destination_t L, ccmem_allocator_t const * const A,
				       ccptn_t * P, ccptn_error_handler_t * R_H, char const * pathname)
{
  ccptn_init_nodup_asciiz(L, A, P, pathname);
  ccptn_error_handler_ptn_init(L, R_H, P);
  return P;
}

ccptn_t *
ccptn_init_dup_asciiz_guarded_error (cce_destination_t L, ccmem_allocator_t const * const A,
				     ccptn_t * R, ccptn_error_handler_t * R_H, char const * pathname)
{
  ccptn_init_dup_asciiz(L, A, R, pathname);
  ccptn_error_handler_ptn_init(L, R_H, R);
  return R;
}

ccptn_t *
ccptn_init_normal_asciiz_guarded_error (cce_destination_t L, ccmem_allocator_t const * const A,
					ccptn_t * R, ccptn_error_handler_t * R_H, char const * pathname)
{
  ccptn_init_normal_asciiz(L, A, R, pathname);
  ccptn_error_handler_ptn_init(L, R_H, R);
  return R;
}

ccptn_t *
ccptn_new_nodup_asciiz_guarded_error (cce_destination_t L, ccmem_allocator_t const * const A,
				      ccptn_error_handler_t * R_H, char const * pathname)
{
  ccptn_t *	R = ccptn_new_nodup_asciiz(L, A, pathname);
  ccptn_error_handler_ptn_init(L, R_H, R);
  return R;
}

ccptn_t *
ccptn_new_dup_asciiz_guarded_error (cce_destination_t L, ccmem_allocator_t const * const A,
				    ccptn_error_handler_t * R_H, char const * pathname)
{
  ccptn_t *	R = ccptn_new_dup_asciiz(L, A, pathname);
  ccptn_error_handler_ptn_init(L, R_H, R);
  return R;
}

ccptn_t *
ccptn_new_normal_asciiz_guarded_error (cce_destination_t L, ccmem_allocator_t const * const A,
				       ccptn_error_handler_t * R_H, char const * pathname)
{
  ccptn_t *	R = ccptn_new_normal_asciiz(L, A, pathname);
  ccptn_error_handler_ptn_init(L, R_H, R);
  return R;
}

/* ------------------------------------------------------------------ */

ccptn_t *
ccptn_init_nodup_asciiz_guarded_clean (cce_destination_t L, ccmem_allocator_t const * const A,
				       ccptn_t * R, ccptn_clean_handler_t * R_H, char const * pathname)
{
  ccptn_init_nodup_asciiz(L, A, R, pathname);
  ccptn_clean_handler_ptn_init(L, R_H, R);
  return R;
}

ccptn_t *
ccptn_init_dup_asciiz_guarded_clean (cce_destination_t L, ccmem_allocator_t const * const A,
				     ccptn_t * R, ccptn_clean_handler_t * R_H, char const * pathname)
{
  ccptn_init_dup_asciiz(L, A, R, pathname);
  ccptn_clean_handler_ptn_init(L, R_H, R);
  return R;
}

ccptn_t *
ccptn_init_normal_asciiz_guarded_clean (cce_destination_t L, ccmem_allocator_t const * const A,
					ccptn_t * R, ccptn_clean_handler_t * R_H, char const * pathname)
{
  ccptn_init_normal_asciiz(L, A, R, pathname);
  ccptn_clean_handler_ptn_init(L, R_H, R);
  return R;
}

ccptn_t *
ccptn_new_nodup_asciiz_guarded_clean (cce_destination_t L, ccmem_allocator_t const * const A,
				      ccptn_clean_handler_t * R_H, char const * pathname)
{
  ccptn_t *	R = ccptn_new_nodup_asciiz(L, A, pathname);
  ccptn_clean_handler_ptn_init(L, R_H, R);
  return R;
}

ccptn_t *
ccptn_new_dup_asciiz_guarded_clean (cce_destination_t L, ccmem_allocator_t const * const A,
				    ccptn_clean_handler_t * R_H, char const * pathname)
{
  ccptn_t *	R = ccptn_new_dup_asciiz(L, A, pathname);
  ccptn_clean_handler_ptn_init(L, R_H, R);
  return R;
}

ccptn_t *
ccptn_new_normal_asciiz_guarded_clean (cce_destination_t L, ccmem_allocator_t const * const A,
				       ccptn_clean_handler_t * R_H, char const * pathname)
{
  ccptn_t *	R = ccptn_new_normal_asciiz(L, A, pathname);
  ccptn_clean_handler_ptn_init(L, R_H, R);
  return R;
}

/* end of file */
