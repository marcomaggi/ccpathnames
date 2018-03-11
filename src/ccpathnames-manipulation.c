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
#include <assert.h>
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

#define INLINE	__attribute__((__always_inline__,__nonnull__(1))) static inline


/** --------------------------------------------------------------------
 ** Helpers.
 ** ----------------------------------------------------------------- */

INLINE bool
INPUT_IS_RELATIVE (char const * const input_ptr)
{
  return ('/' != *input_ptr);
}

INLINE bool
INPUT_IS_ABSOLUTE (char const * const input_ptr)
{
  return ('/' == *input_ptr);
}

/* ------------------------------------------------------------------ */

INLINE bool
IS_STANDALONE_SLASH (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate to true if the string has 1 octet representing the
   pathname "/"; otherwise evaluate to false. */
{
  return ((end == (1+in)) && ('/' == in[0]));
}

/* ------------------------------------------------------------------ */

INLINE bool
IS_STANDALONE_SINGLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate to true if the string has 1 octet representing the
   pathname "."; otherwise evaluate to false. */
{
  return ((end == (1+in)) && ('.' == in[0]));
}

INLINE bool
IS_STANDALONE_SINGLE_DOT_SLASH (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 2  octets representing
   the pathname "./"; otherwise evaluate to false. */
{
  return ((end == (2+in)) && ('.' == in[0]) && ('/' == in[1]));
}

INLINE bool
IS_STANDALONE_SLASH_SINGLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 2  octets representing
   the pathname "/."; otherwise evaluate to false. */
{
  return ((end == (2+in)) && ('/' == in[0]) && ('.' == in[1]));
}

/* ------------------------------------------------------------------ */

INLINE bool
IS_STANDALONE_DOUBLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 2  octets representing
   the pathname ".."; otherwise evaluate to false. */
{
  return ((end == (2+in)) && ('.' == in[0]) && ('.' == in[1]));
}

INLINE bool
IS_STANDALONE_DOUBLE_DOT_SLASH (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 3  octets representing
   the pathname "../"; otherwise evaluate to false. */
{
  return ((end == (3+in)) && ('.' == in[0]) && ('.' == in[1]) && ('/' == in[2]));
}

INLINE bool
IS_STANDALONE_SLASH_DOUBLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 3  octets representing
   the pathname "/.."; otherwise evaluate to false. */
{
  return ((end == (3+in)) && ('/' == in[0]) && ('.' == in[1]) && ('.' == in[2]));
}

/* ------------------------------------------------------------------ */

INLINE bool
BEGINS_WITH_SINGLE_DOT_SLASH (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true  if the string  begins with  the pathname
   "./"; otherwise evaluate to false. */
{
  return ((end > (1+in)) && ('.' == in[0]) && ('/' == in[1]));
}

INLINE bool
BEGINS_WITH_SLASH_SINGLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true  if the string  begins with  the pathname
   "/.", but not "/.."; otherwise evaluate to false. */
{
  return ((end > (1 + in)) && ('/' == in[0]) && ('.' == in[1]) && ('.' != in[2]));
}

/* ------------------------------------------------------------------ */

INLINE bool
BEGINS_WITH_DOUBLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true  if the string  begins with  the pathname
   ".."; otherwise evaluate to false. */
{
  return ((end > (1+in)) && ('.' == in[0]) && ('.' == in[1]));
}

INLINE bool
BEGINS_WITH_SLASH_DOUBLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true  if the string  begins with  the pathname
   "/.."; otherwise evaluate to false. */
{
  return ((end > (2+in)) && ('/' == in[0]) && ('.' == in[1]) && ('.' == in[2]));
}

/* ------------------------------------------------------------------ */

__attribute__((__always_inline__,__nonnull__(1,2),__returns_nonnull__))
static inline char const *
find_next_slash_or_end (char const * in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: find  the next slash  octet in the string.   If successful:
   return a pointer referencing the slash.  Otherwise return END. */
{
  while ((in < end) && ('/' != *in)) {
    ++in;
  }
  return in;
}

__attribute__((__always_inline__,__nonnull__(1,2),__returns_nonnull__))
static inline char const *
find_prev_slash_or_begin (char const * const output_ptr, char const * ou)
/* Given  an ASCII  string referenced  by  OU and  beginning at  pointer
   OUTPUT_PTR included: find the previous slash octet in the string.  If
   successful: return a pointer referencing the slash.  Otherwise return
   OUTPUT_PTR. */
{
  while ((output_ptr < ou) && ('/' != *ou)) {
    --ou;
  }
  return ou;
}

__attribute__((__always_inline__,__nonnull__(1,2),__returns_nonnull__))
static inline char const *
skip_repeated_slashes_or_end (char const * in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: skip all  slash octets starting from IN  onwards.  Return a
   pointer referencing the first octet that is not a slash; this pointer
   may be END. */
{
  while ((in < end) && ('/' == *in)) {
    ++in;
  }
  return in;
}

/* ------------------------------------------------------------------ */

#define COPY_INPUT_TO_OUTPUT(OU, IN, IN_END)	\
  while ((IN) < (IN_END)) { *(OU)++ = *(IN)++; }


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
 ** Low-level normalisation functions: useless slashses removal.
 ** ----------------------------------------------------------------- */

size_t
ccptn_normal_pass_remove_useless_slashes (char * output_ptr, char const * const input_ptr, size_t const input_len)
/* Copy  a  pathname  from  "input_ptr"  to  "output_ptr"  performing  a
   normalisation pass in the process: removal of multiple slashes.

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
    *ou++ = *in++;
    /* If there are repeated slahses: skip them. */
    if ('/' == in[-1]) {
      in = skip_repeated_slashes_or_end(in, end);
    }
  }
  *ou = '\0';
  return (ou - output_ptr);
}


/** --------------------------------------------------------------------
 ** Low-level normalisation functions: useless single-dot removal.
 ** ----------------------------------------------------------------- */

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
#undef VERBOSE
#define VERBOSE		0
  char const * const	end = input_ptr + input_len;
  char const *		in  = input_ptr;
  char *		ou  = output_ptr;

  if (INPUT_IS_ABSOLUTE(input_ptr)) {
    /* The input pathname is absolute, we want these normalisations:
     *
     *		"/"		=> "/"
     *		"/."		=> "/"
     *		"/././."	=> "/"
     *		"/./path"	=> "/path"
     *		"/./././path"	=> "/path"
     */
    if (IS_STANDALONE_SLASH(in, end)) {
      *ou++ = '/';
      goto done;
    } else {
      /* Remove all the leading "/." chunks. */
      while (BEGINS_WITH_SLASH_SINGLE_DOT(in,end)) {
	in += 2;
      }
      /* If  we have  removed  the whole  input: the  output  is just  a
	 slash. */
      if (end == in) {
	*ou++ = '/';
	goto done;
      }
    }
    /* If  we  are  still  here:  IN references  the  slash  starting  a
       chunk. */
  } else {
    /* The input pathname is relative, we want these normalisations:
     *
     *		"."			=> "."
     *		"./"			=> "."
     *		"./././"		=> "."
     *		"./path"		=> "./"
     *		"./././path"		=> "./path"
     *		"file.ext"		=> "file.ext"
     *		"./file.ext"		=> "./file.ext"
     *		"./path/to/file.ext"	=> "path/to/file.ext"
     */
    if (IS_STANDALONE_SINGLE_DOT(in, end)) {
      /* This relative input pathname is a standalone dot ".". */
      *ou++ = '.';
      goto done;
    } else if (! BEGINS_WITH_SINGLE_DOT_SLASH(in,end)) {
      /* This relative input pathname starts  with a segment that is not
	 a standalone dot. */
      COPY_INPUT_TO_OUTPUT(ou, in, find_next_slash_or_end(in, end));
    } else {
      /* Skip all the leading "./" chunks. */
      while (BEGINS_WITH_SINGLE_DOT_SLASH(in,end)) {
	in += 2;
      }
      /* If  we have  skipped  the whole  input: the  output  is just  a
	 single-dot. */
      if (end == in) {
	*ou++ = '.';
	goto done;
      }
      {
	char const *	next = find_next_slash_or_end(in, end);
	if (VERBOSE) {
	  fprintf(stderr, "%s: in=", __func__);
	  fwrite(in, 1, (end - in), stderr);
	  fprintf(stderr, ", output_ptr=");
	  fwrite(output_ptr, 1, (ou - output_ptr), stderr);
	  fprintf(stderr, ", next=");
	  fwrite(in, 1, (next - in), stderr);
	  fprintf(stderr, "\n");
	}
	/* We  do not  want to  wholly skip  a directory  part.  If  the
	   remaining input  has no  slash in  it: insert  a "./"  in the
	   output. */
	if ((end == next) &&
	    (! IS_STANDALONE_SINGLE_DOT(in, next)) &&
	    (! IS_STANDALONE_DOUBLE_DOT(in, next))) {
	  /* Here the string starting at IN is one among:
	   *
	   *	"file.ext"
	   *	"."
	   *	".."
	   */
	  *ou++	= '.';
	  *ou++	= '/';
	}
	/* Now copy the segment to the output. */
	COPY_INPUT_TO_OUTPUT(ou, in, next);
      }
    }
  }

  /* Now either IN references the end  of input or it references a slash
     octet. */

  /* In the  following loop  we copy normal  segments from  INPUT_PTR to
   * OUTPUT_PTR  in chunks  including the  leading slash.   For example,
   * given the input:
   *
   *    /path/to/file.ext
   *
   * we copy it to the output in the three chunks:
   *
   *    /path
   *    /to
   *    /file.ext
   *
   * only when a chunk is "/." we do something different.
   */
  while (in < end) {
    /* Set NEXT to point  to the slash after this segment  or the end of
     * input.  For example:
     *
     *    /path/to/file.ext
     *    ^    ^
     *    in   next
     */
    char const * next = find_next_slash_or_end(1+in, end);

    if (VERBOSE) {
      fprintf(stderr, "%s: in=", __func__);
      fwrite(in, 1, (end - in), stderr);
      fprintf(stderr, ", output_ptr=");
      fwrite(output_ptr, 1, (ou - output_ptr), stderr);
      fprintf(stderr, ", next=");
      fwrite(in, 1, (next - in), stderr);
      fprintf(stderr, "\n");
    }

    if (BEGINS_WITH_SLASH_SINGLE_DOT(in,end)) {
      /* Skip the next chunk because it is a "/.", but if it is the last
       * append a slash to the output.
       *
       *	"/path/./to"	=> "path/to"
       *	      ^in
       *	"dir/."		=> "dir/"
       *	    ^in
       */
      in = next;
      if (end == in) {
	if ((output_ptr == ou) && INPUT_IS_RELATIVE(input_ptr)) {
	  *ou++ = '.';
	} else {
	  *ou++ = '/';
	}
      }
    } else {
      /* Copy the next chunk because it is normal. */
      COPY_INPUT_TO_OUTPUT(ou, in, next);
    }
  }

  /* Done. */
 done:
  *ou = '\0';
  if (VERBOSE) {
    fprintf(stderr, "%s: final in=", __func__);
    fwrite(in, 1, (end - in), stderr);
    fprintf(stderr, ", output_ptr=%s, len=%lu\n", output_ptr, strlen(output_ptr));
  }
  return (ou - output_ptr);
}


/** --------------------------------------------------------------------
 ** Low-level normalisation functions: double-dot processing.
 ** ----------------------------------------------------------------- */

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
#undef VERBOSE
#define VERBOSE		0
  char const * const	end = input_ptr + input_len;
  char const *		in  = input_ptr;
  char *		ou  = output_ptr;

  if (VERBOSE) {
    fprintf(stderr, "%s: begin in=%s\n", __func__, input_ptr);
  }

  if (BEGINS_WITH_DOUBLE_DOT(in, end)) {
    in    += 2;
    *ou++ = '.';
    *ou++ = '.';
    if ((end == in) || IS_STANDALONE_SLASH(in, end)) {
      /* The input is a standalone double-dot ".." or "../". */
      goto done;
    }
  } else if (INPUT_IS_RELATIVE(input_ptr)) {
    /* This relative input pathname starts with  a segment that is not a
       double dot. */
    COPY_INPUT_TO_OUTPUT(ou, in, find_next_slash_or_end(in, end));
  }

  /* Now either IN references the end  of input or it references a slash
     octet. */

  /* In this loop  we copy normal segments from  INPUT_PTR to OUTPUT_PTR
   * in  chunks including  the leading  slash.  For  example, given  the
   * input:
   *
   *    /path/to/file.ext
   *
   * we copy it to the output in the three chunks:
   *
   *    /path
   *    /to
   *    /file.ext
   *
   * only when a chunk is "/.." we do something different.
   */
  while (in < end) {
    /* Set NEXT to point  to the slash after this segment  or the end of
     * input.  For example:
     *
     *    /path/to/file.ext
     *    ^    ^
     *    in   next
     *
     * another example:
     *
     *    path/to/file.ext
     *    ^   ^
     *    in  next
     */
    char const *	next = find_next_slash_or_end(1+in, end);

    if (VERBOSE) {
      fprintf(stderr, "%s: in=", __func__);
      fwrite(in, 1, (end - in), stderr);
      fprintf(stderr, ", output_ptr=");
      fwrite(output_ptr, 1, (ou - output_ptr), stderr);
      fprintf(stderr, ", next=");
      fwrite(in, 1, (next - in), stderr);
      fprintf(stderr, "\n");
    }

    /* Now NEXT points to a slash octet or to the end of input. */
    if (! BEGINS_WITH_SLASH_DOUBLE_DOT(in, next)) {
      /* No, the next chunk is *not* a "/..": copy it to the output. */
      COPY_INPUT_TO_OUTPUT(ou, in, next);
    } else {
      char const *	prev = find_prev_slash_or_begin(output_ptr, ou-1);

      if (VERBOSE) {
	fprintf(stderr, "%s: prev=", __func__);
	fwrite(prev, 1, (ou - prev), stderr);
	fprintf(stderr, "\n");
      }

      if (output_ptr < ou) {
	/* The  pointer OU  does *not*  reference the  beginning of  the
	   output: there is a previous segment we can remove by stepping
	   back the pointer OU itself. */
	if (BEGINS_WITH_DOUBLE_DOT(prev, ou) || BEGINS_WITH_SLASH_DOUBLE_DOT(prev, ou)) {
	  /* The previous segment is a ".."   or "/..", this is the case
	   * of the following normalisations:
	   *
	   *	"/../../path"	=> "/../../path"
	   *	    ^in
	   *
	   *	"../../path"	=> "../../path"
	   *	   ^in
	   */
	  in    = next;
	  *ou++ = '/';
	  *ou++ = '.';
	  *ou++ = '.';
	} else {
	  /* The  previous segment  is neither  a double-dot:  remove it
	   * from  the  output.   This  is the  case  of  the  following
	   * normalisation:
	   *
	   *	"path/to/../file.ext"	=> "path/file.ext"
	   */
	  in = next;
	  ou = (char *)prev;
	  /* We  must handle  the case  of  first segment  removal in  a
	   * pathname.
	   *
	   * When  the  pathname  is  absolute, we  want  the  following
	   * normalisations:
	   *
	   *	"/path/../to/file.ext"	=> "/to/file.ext"
	   *	"/path/.."		=> "/"
	   *
	   * When  the  pathname  is  relative, we  want  the  following
	   * normalisations:
	   *
	   *	"path/../to/file.ext"	=> "to/file.ext"
	   *	"path/../file.ext"	=> "./file.ext"
	   *
	   * where the output is still a relative pathname and still has
	   * a directory part; we do *not* want the following results:
	   *
	   *	"path/../to/file.ext"	=> "/to/file.ext"
	   *	"path/../file.ext"	=> "file.ext"
	   *
	   * where the output has become  an absolute pathname or has no
	   * directory part anymore.
	   */
	  if (output_ptr == ou) {
	    if (INPUT_IS_ABSOLUTE(input_ptr)) {
	      if (end == in) {
		/* The  pathname is  absolute  and we  have removed  the
		 * whole input.  This is the case of:
		 *
		 *	"/path/.."	=> "/"
		 */
		*ou++ = '/';
	      }
	    } else {
	      if (end == in) {
		/* We are at the end  of the relative input pathname and
		 * we have  removed the whole output  pathname.  This is
		 * the case of:
		 *
		 *	"path/.."	=> "."
		 */
		*ou++ = '.';
	      } else if (IS_STANDALONE_SLASH(in, end)) {
		/* In the  relative input pathname: the  next segment is
		 * an ending slash and we  have removed the whole output
		 * pathname.  This is the case of:
		 *
		 *	"path/../"	=> "."
		 */
		++in;
		*ou++ = '.';
	      } else if (IS_STANDALONE_SLASH_SINGLE_DOT(in, end)) {
		/* In the  relative input pathname: the  next segment is
		 * an ending "/."  and we  have removed the whole output
		 * pathname.  This is the case of:
		 *
		 *	"path/../."	=> "."
		 */
		in    = end;
		*ou++ = '.';
	      } else if (IS_STANDALONE_SLASH_DOUBLE_DOT(in, end)) {
		/* In the  relative input pathname: the  next segment is
		 * an ending "/.."  and we have removed the whole output
		 * pathname.  This is the case of:
		 *
		 *	"path/../.."	=> ".."
		 */
		in    = end;
		*ou++ = '.';
		*ou++ = '.';
	      } else if (end == find_next_slash_or_end(1+in, end)) {
		/* The rest of the relative input pathname has no slash in
		 * it and we have removed the whole output pathname.  This
		 * is the case of:
		 *
		 *	"path/../file.ext"	=> "./file.ext"
		 */
		*ou++ = '.';
	      }
	    }
	  }
	}
      } else {
	/* The pointer OU references the  beginning of the output: there
	   is no previous segment to remove. */
	if (INPUT_IS_RELATIVE(input_ptr)) {
	  /* The input  pathname is  relative, this is  the case  of the
	   * following normalisation:
	   *
	   *	"path/../../to"	=> "../to"
	   *	        ^in
	   */
	  in    = next;
	  *ou++ = '.';
	  *ou++ = '.';
	} else {
	  /* The input  pathname is  absolute, this is  the case  of the
	   * following input:
	   *
	   *	"/path/../.."
	   *
	   * raise an exception, there is no way to normalise the input.
	   */
	  cce_raise(L, ccptn_condition_new_invalid_pathname());
	}
      }
    }
  }

done:
  *ou = '\0';
  if (VERBOSE) {
    fprintf(stderr, "%s: final in=", __func__);
    fwrite(in, 1, (end - in), stderr);
    fprintf(stderr, ", output_ptr=%s, len=%lu\n", output_ptr, strlen(output_ptr));
  }
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
