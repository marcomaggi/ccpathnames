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

size_t
ccptn_normal_pass_remove_useless_slashes (char * output_ptr, char const * const input_ptr, size_t const input_len)
/* Copy  a  pathname  from  "input_ptr"  to  "output_ptr"  performing  a
   normalisation pass  in the process:  removal of multiple  slashes and
   terminating slashes.

   The array referenced  by "input_ptr" must represent  an ASCIIZ string
   with  at least  "input_len" octets,  terminating zero  excluded.  The
   array referenced  by "output_ptr"  must be at  least "1  + input_len"
   octets wide.

   Return  the  number of  octets  stored  in  the array  referenced  by
   "output_ptr", terminating zero excluded. */
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

size_t
ccptn_normal_pass_remove_single_dot_segments (char * output_ptr, char const * const input_ptr, size_t const input_len)
/* Copy  a  pathname  from  "input_ptr"  to  "output_ptr"  performing  a
   normalisation pass  in the  process: removal of  single-dot segments.
   This     pass      is     meant      to     be      applied     after
   "ccptn_normal_pass_remove_useless_slashes()".

   The array referenced  by "input_ptr" must represent  an ASCIIZ string
   with  at least  "input_len" octets,  terminating zero  excluded.  The
   array referenced  by "output_ptr"  must be at  least "1  + input_len"
   octets wide.

   Return  the  number of  octets  stored  in  the array  referenced  by
   "output_ptr", terminating zero excluded. */
{
  if (IS_SINGLE_DOT(input_ptr, input_len)) {
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
	size_t	in_len = ccptn_segment_size_of_next (in, end - in);

	ccptn_segment_t	S = ccptn_segment_next(in, end - in);

	if (IS_SINGLE_DOT(in, in_len)) {
	  /* Skip this segment.  If after the dot there is a slash: skip
	     the slash too. */
	  ++in;
	  if ((in < end) && ('/' == *in)) {
	    ++in;
	  }
	} else {
	  /* Copy the segment to the output. */
	  for (size_t i=0; i<S.len; ++i) {
	    *ou++ = *in++;
	  }
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

size_t
ccptn_normal_pass_remove_double_dot_segments (cce_destination_t L, char * output_ptr, char const * const input_ptr, size_t const input_len)
/* Copy  a  pathname  from  "input_ptr"  to  "output_ptr"  performing  a
   normalisation pass in the process: removal of double-dot segments and
   the segments  before them.  This  pass is  meant to be  applied after
   "ccptn_normal_pass_remove_single_dot_segments()".

   The array referenced  by "input_ptr" must represent  an ASCIIZ string
   with  at least  "input_len" octets,  terminating zero  excluded.  The
   array referenced  by "output_ptr"  must be at  least "1  + input_len"
   octets wide.

   Return  the  number of  octets  stored  in  the array  referenced  by
   "output_ptr", terminating zero excluded. */
{
  char const * const	end = input_ptr + input_len;
  char const *		in  = input_ptr;
  char *		ou  = output_ptr;

  while (in < end) {
    /* Set "next" to point to the slash after this segment or the end of
     * input.  For example:
     *
     *    /path/to/file.ext
     *    ^    ^
     *    in   next
     */
    char const *	next = in;
    if ('/' == *next) {
      ++next;
    }
    while (('/' != *next) && (next < end)) {
      ++next;
    }

    if (0) {
      fprintf(stderr, "%s: in=", __func__);
      fwrite(in, 1, (end - in), stderr);
      fprintf(stderr, ", output_ptr=");
      fwrite(output_ptr, 1, (ou - output_ptr), stderr);
      fprintf(stderr, ", next=");
      fwrite(in, 1, (next - in), stderr);
      fprintf(stderr, "\n");
    }

    /* Now NEXT  points to  a slash octet  or to the  end of  input.  Is
       there a slash+double-dot between IN and NEXT? */
    if ((3 == (next - in)) && ('/' == in[0]) && ('.' == in[1]) && ('.' == in[2])) {
      /* Yes, the next segment is a slash+double-dot: "/..". */
      if (output_ptr < ou) {
	/* The  pointer OU  does *not*  reference the  beginning of  the
	   output: there is a previous segment we can remove by stepping
	   back the pointer OU. */
	if ('.' == ou[-1]) {
	  /* The previous segment is a double-dot one that was left in a
	     previous   iteration:   do   not  remove   it,   copy   the
	     slash+double-dot segment to the output. */
	  in    = next;
	  *ou++ = '/';
	  *ou++ = '.';
	  *ou++ = '.';
	} else {
	  /* The  previous segment  is  *not* a  double-dot one:  remove
	     it. */
	  do {
	    --ou;
	  } while ((ou > output_ptr) && ('/' != *ou));
	  in = next;
	}
      } else {
	/* The pointer OU references the  beginning of the output: there
	   is no previous segment to remove. */
	if ('/' != *input_ptr) {
	  /* The  input  pathname  is   relative:  copy  the  double-dot
	     (without the  leading slash) to  the output and move  on to
	     the next segment.  */
	  in    = next;
	  *ou++ = '.';
	  *ou++ = '.';
	} else {
	  /* The input  pathname is absolute: raise  an exception, there
	     is no way to normalise the input. */
	  cce_raise(L, ccptn_condition_new_invalid_pathname());
	}
      }
    } else {
      /* No, the  next segment  is *not*  a double-dot:  copy it  to the
	 output. */
      while (in < next) {
	*ou++ = *in++;
      }
    }

  }

  if (0) {
    fprintf(stderr, "%s: in=", __func__);
    fwrite(in, 1, (end - in), stderr);
    fprintf(stderr, ", output_ptr=");
    fwrite(output_ptr, 1, (ou - output_ptr), stderr);
    fprintf(stderr, "\n");
  }

  /* Have we removed all the segments in the pathname? */
  if (output_ptr == ou) {
    /* Yes,  we have.   If the  input pathname  is absolute:  the output
       pathname is a  single slash.  If the input  pathname is relative:
       the output pathname is a single dot. */
    *ou++ = ('/' == *input_ptr)? '/' : '.';
  }
  *ou = '\0';
  if (0) { fprintf(stderr, "%s: output_ptr=%s, len=%lu\n", __func__, output_ptr, strlen(output_ptr)); }
  return (ou - output_ptr);
}


/** --------------------------------------------------------------------
 ** Normalisation: pathname normalisation.
 ** ----------------------------------------------------------------- */

ccptn_t *
ccptn_new_normalise (cce_destination_t L, ccptn_t const * const P)
{
  char		one[1 + ccptn_len(P)];
  size_t	one_len;
  one_len = ccptn_normal_pass_remove_useless_slashes(one, ccptn_asciiz(P), ccptn_len(P));
  {
    char	two[1 + one_len];
    size_t	two_len;
    two_len = ccptn_normal_pass_remove_single_dot_segments(two, one, one_len);
    one_len = ccptn_normal_pass_remove_double_dot_segments(L, one, two, two_len);
    if (0) { fprintf(stderr, "%s: out=%s\n", __func__, one); }
    {
      ccptn_t *	R = ccptn_new_dup_asciiz(L, one);
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
  one_len = ccptn_normal_pass_remove_useless_slashes(one, ccptn_asciiz(P), ccptn_len(P));
  {
    char	two[1 + one_len];
    size_t	two_len;
    two_len = ccptn_normal_pass_remove_single_dot_segments(two, one, one_len);
    one_len = ccptn_normal_pass_remove_double_dot_segments(L, one, two, two_len);
    if (0) { fprintf(stderr, "%s: out=%s\n", __func__, one); }
    {
      ccptn_init_dup_asciiz(L, R, one);
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
