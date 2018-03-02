/*
  Part of: CCPathnames
  Contents: manipulation functions
  Date: Feb 26, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it  under the  terms  of  the GNU  Lesser  General  Public License  as
  published by  the Free  Software Foundation, either  version 3  of the
  License, or (at your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccpathnames-internals.h"
#include <stdlib.h>
#ifdef HAVE_ERRNO_H
#  include <errno.h>
#endif
#ifdef HAVE_LIMITS_H
#  include <limits.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif


/** --------------------------------------------------------------------
 ** Normalisation: realpath.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_REALPATH
ccptn_t *
ccptn_new_realpath (cce_destination_t upper_L, ccptn_t const * const P)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	rv_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	rv;

    errno = 0;
    rv = realpath(ccptn_asciiz(P), NULL);
    if (NULL == rv) {
      cce_raise(L, cce_condition_new_errno_clear());
    } else {
      cce_handler_malloc_init(L, rv_H, (void *)rv);
      {
	size_t	len = strlen(rv);
	if (CCPTN_PATH_MAX < len) {
	  cce_raise(L, ccptn_condition_new_exceeded_length());
	} else {
	  ccptn_t *	Q = ccptn_new_dup_asciiz(L, rv);

	  Q->realpath	= 1;
	  Q->normalised	= 1;
	  cce_run_cleanup_handlers(L);
	  return Q;
	}
      }
    }
  }
}

ccptn_t *
ccptn_init_realpath (cce_destination_t upper_L, ccptn_t * R, ccptn_t const * const P)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	rv_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    char const *	rv;

    errno = 0;
    rv = realpath(ccptn_asciiz(P), NULL);
    if (NULL == rv) {
      cce_raise(L, cce_condition_new_errno_clear());
    } else {
      cce_handler_malloc_init(L, rv_H, (void *)rv);
      {
	size_t	len = strlen(rv);
	if (CCPTN_PATH_MAX < len) {
	  cce_raise(L, ccptn_condition_new_exceeded_length());
	} else {
	  ccptn_init_dup_asciiz(L, R, rv);
	  R->realpath	= 1;
	  R->normalised	= 1;
	  cce_run_cleanup_handlers(L);
	  return R;
	}
      }
    }
  }
}
#endif


/** --------------------------------------------------------------------
 ** Normalisation: helper functions.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1,2)))
static size_t
normalise_remove_multiple_slashes (char * output_ptr, char const * input_ptr, size_t input_len)
/* Copy  a  pathname  from  "input_ptr"  to  "output_ptr"  performing  a
   normalisation pass it in the process: removal of multiple slashes and
   terminating slashes.

   The array referenced  by "input_ptr" must represent  an ASCIIZ string
   with  at least  "input_len" octets,  terminating zero  excluded.  The
   array referenced  by "output_ptr"  must be at  least "1  + input_len"
   octets wide.

   Return  the  number of  octets  stored  in  the array  referenced  by
   "output_ptr", terminating null excluded. */
{
  char const * const	end = input_ptr + input_len;
  char const *		in  = input_ptr;
  char *		ou  = output_ptr;

  while (in < end) {
    if ('/' == *in) {
      /* Skip repeated slashes. */
      while ((in < end) && ('/' == *in)) {
	++in;
      }
      if ((in < end) || ((input_ptr + 1) == in)) {
	/* Either  the  slash is  not  the  last  octet in  the  input
	   pathname, or a single slash is the whole pathname.  Store a
	   single slash in the output. */
	*ou++ = '/';
      }
    } else {
      *ou++ = *in++;
    }
  }

  *ou = '\0';
  return (ou - output_ptr);
}

__attribute__((__nonnull__(1,2)))
static size_t
normalise_remove_single_dot_segments (char * output_ptr, char const * input_ptr, size_t input_len)
/* Copy  a  pathname  from  "input_ptr"  to  "output_ptr"  performing  a
   normalisation pass it in the process: removal of single-dot segments.

   The array referenced  by "input_ptr" must represent  an ASCIIZ string
   with  at least  "input_len" octets,  terminating zero  excluded.  The
   array referenced  by "output_ptr"  must be at  least "1  + input_len"
   octets wide.

   Return  the  number of  octets  stored  in  the array  referenced  by
   "output_ptr", terminating null excluded. */
{
  if ((1 == input_len) && ('.' == input_ptr[0])) {
    /* The full pathname is a single-dot component. */
    output_ptr[0] = '.';
    output_ptr[1] = '\0';
    return 1;
  } else {
    char const * const	end = input_ptr + input_len;
    char const *	in  = input_ptr;
    char *		ou  = output_ptr;

    while (in < end) {
      if ('/' == *in) {
	/* Copy the slash to the output. */
	*ou++ = *in++;
      } else {
	ccptn_segment_t	S = ccptn_segment_next(in, end - in);

	if (ccptn_segment_is_dot(S)) {
	  /* Skip this segment.  If after the dot there is a slash: skip
	     the slash too. */
	  ++in;
	  if ((in < end) && ('/' == *in)) {
	    ++in;
	  }
	} else {
	  /* Copy the segment to the output. */
	  strncpy(ou, S.ptr, S.len);
	  in += S.len;
	  ou += S.len;
	}
      }
    }
    /* Here it may be that the  last octet represents a slash: remove it
       if it is not the first octet in the pathname. */
    if (((output_ptr + 1) < ou) && ('/' == *(ou-1))) {
      --ou;
    }

    *ou = '\0';
    if (0) { fprintf(stderr, "%s: out=%s\n", __func__, output_ptr); }
    return (ou - output_ptr);
  }
}


/** --------------------------------------------------------------------
 ** Normalisation: pathname normalisation.
 ** ----------------------------------------------------------------- */

ccptn_t *
ccptn_new_normalise (cce_destination_t L, ccptn_t const * const P)
{
  char		one[1 + ccptn_len(P)];
  size_t	one_len;

  one_len = normalise_remove_multiple_slashes(one, ccptn_asciiz(P), ccptn_len(P));
  {
    char	two[1 + one_len];

    normalise_remove_single_dot_segments(two, one, one_len);
    if (0) { fprintf(stderr, "%s: out=%s\n", __func__, two); }
    {
      ccptn_t *	R = ccptn_new_dup_asciiz(L, two);
      R->normalised = 1;
      return R;
    }
  }
}

ccptn_t *
ccptn_init_normalise (cce_destination_t L, ccptn_t * R, ccptn_t const * const P)
{
  char		one[1 + ccptn_len(P)];
  size_t	one_len;

  one_len = normalise_remove_multiple_slashes(one, ccptn_asciiz(P), ccptn_len(P));
  {
    char	two[1 + one_len];

    normalise_remove_single_dot_segments(two, one, one_len);
    if (0) { fprintf(stderr, "%s: out=%s\n", __func__, two); }
    {
      ccptn_init_dup_asciiz(L, R, two);
      R->normalised = 1;
      return R;
    }
  }
}


/** --------------------------------------------------------------------
 ** Composition.
 ** ----------------------------------------------------------------- */

ccptn_t *
ccptn_new_concat (cce_destination_t L, ccptn_t const * prefix, ccptn_t const * suffix)
{
  bool		prefix_is_absolute = ccptn_is_absolute(prefix);
  bool		suffix_is_absolute = ccptn_is_absolute(suffix);
  size_t	R_len;

  /* The resulting  length is the sum  of the original length,  plus one
     for the slash separator. */
  R_len	= ccptn_len(prefix) + ccptn_len(suffix);
  /* If  the  suffix   is  absolute:  its  first  octet   is  the  ASCII
     representation of the  slash separator; otherwise we  must insert a
     separator. */
  if (! suffix_is_absolute) {
    ++R_len;
  }

  if (CCPTN_PATH_MAX < R_len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else {
    /* This array must hold the whole pathname plus the terminating zero
       octet. */
    char	R_pathname[R_len + 1];
    char *	ptr = R_pathname;

    /* Copy the prefix. */
    {
      size_t	len = ccptn_len(prefix);

      strncpy(ptr, ccptn_asciiz(prefix), len);
      ptr += len;
    }

    /* Insert a separator if needed. */
    if (! suffix_is_absolute) {
      *ptr = '/';
      ++ptr;
    }

    /* Copy the suffix and add the terminating zero. */
    {
      size_t	len = ccptn_len(suffix);

      strncpy(ptr, ccptn_asciiz(suffix), len);
      ptr += len;
      *ptr = '\0';
    }

    /* Build and return the resulting pathname. */
    {
      ccptn_t *		R;
      R              = ccptn_new_dup_asciiz(L, R_pathname);
      R->absolute    = prefix_is_absolute;
      R->normalised  = (ccptn_is_normalised(prefix) && ccptn_is_normalised(suffix))? 1 : 0;
      R->realpath    = 0;
      return R;
    }
  }
}

ccptn_t *
ccptn_init_concat (cce_destination_t L, ccptn_t * R, ccptn_t const * prefix, ccptn_t const * suffix)
{
  bool		prefix_is_absolute = ccptn_is_absolute(prefix);
  bool		suffix_is_absolute = ccptn_is_absolute(suffix);
  size_t	R_len;

  /* The resulting  length is the sum  of the original length,  plus one
     for the slash separator. */
  R_len	= ccptn_len(prefix) + ccptn_len(suffix);
  /* If  the  suffix   is  absolute:  its  first  octet   is  the  ASCII
     representation of the  slash separator; otherwise we  must insert a
     separator. */
  if (! suffix_is_absolute) {
    ++R_len;
  }

  if (CCPTN_PATH_MAX < R_len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else {
    /* This array must hold the whole pathname plus the terminating zero
       octet. */
    char	R_pathname[R_len + 1];
    char *	ptr = R_pathname;

    /* Copy the prefix. */
    {
      size_t	len = ccptn_len(prefix);

      strncpy(ptr, ccptn_asciiz(prefix), len);
      ptr += len;
    }

    /* Insert a separator if needed. */
    if (! suffix_is_absolute) {
      *ptr = '/';
      ++ptr;
    }

    /* Copy the suffix and add the terminating zero. */
    {
      size_t	len = ccptn_len(suffix);

      strncpy(ptr, ccptn_asciiz(suffix), len);
      ptr += len;
      *ptr = '\0';
    }

    /* Build and return the resulting pathname. */
    {
      ccptn_init_dup_asciiz(L, R, R_pathname);
      R->absolute    = prefix_is_absolute;
      R->normalised  = (ccptn_is_normalised(prefix) && ccptn_is_normalised(suffix))? 1 : 0;
      R->realpath    = 0;
      return R;
    }
  }
}

/* end of file */
