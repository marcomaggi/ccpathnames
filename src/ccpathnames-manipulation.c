/*
  Part of: CCPathnames
  Contents: manipulation functions
  Date: Feb 26, 2018

  Abstract



  Copyright (C) 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
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


/** --------------------------------------------------------------------
 ** Helpers.
 ** ----------------------------------------------------------------- */

__attribute__((__always_inline__,__nonnull__(1,2),__returns_nonnull__))
static inline char const *
find_next_slash_or_end (char const * in, char const * const end)
/* Given an  ASCII string referenced by  IN and terminating at  pointer END excluded:
   find  the  next slash  octet  in  the string.   If  successful:  return a  pointer
   referencing the slash.  Otherwise return END. */
{
  while ((in < end) && ('/' != *in)) {
    ++in;
  }
  return in;
}

__attribute__((__always_inline__,__nonnull__(1,2),__returns_nonnull__))
static inline char const *
find_prev_slash_or_begin (char const * const output_ptr, char const * ou)
/* Given  an ASCII  string  referenced  by OU  and  beginning  at pointer  OUTPUT_PTR
   included: find  the previous slash octet  in the string.  If  successful: return a
   pointer referencing the slash.  Otherwise return OUTPUT_PTR. */
{
  while ((output_ptr < ou) && ('/' != *ou)) {
    --ou;
  }
  return ou;
}

__attribute__((__always_inline__,__nonnull__(1,2),__returns_nonnull__))
static inline char const *
skip_repeated_slashes_or_end (char const * in, char const * const end)
/* Given an  ASCII string referenced by  IN and terminating at  pointer END excluded:
   skip all slash octets starting from  IN onwards.  Return a pointer referencing the
   first octet that is not a slash; this pointer may be END. */
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
__attribute__((__nonnull__(1,2,4),__returns_nonnull__))
static ccptn_t const *
ptn_realpath (cce_destination_t upper_L, ccmem_allocator_t const * const A, ccptn_t * volatile R, ccptn_t const * const P)
{
  cce_location_t	L[1];
  cce_clean_handler_t	rv_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const *	rv;

    errno = 0;
    rv = realpath(P->ptr, NULL);
    if (NULL == rv) {
      cce_raise(L, cce_condition_new_errno_clear());
    } else {
      cce_init_and_register_handler_malloc(L, rv_H, (void *)rv);
      {
	/* Using an  ASCII representation  will force  the "ccptn_t"  constructors to
	   duplicate  the  internal  pathname   representation  using  a  dynamically
	   allocated memory block. */
	ccmem_ascii_t	R_block = {
	  .len	= strlen(rv),
	  .ptr	= (char *)rv
	};
	if (CCPTN_PATH_MAX < R_block.len) {
	  cce_raise(L, ccptn_condition_new_exceeded_length());
	} else {
	  if (R) {
	    ccname_init(ccptn_t, ascii)(L, A, R, R_block);
	  } else {
	    R = (ccptn_t *)ccname_new(ccptn_t, ascii)(L, A, R_block);
	  }
	  R->realpath	= 1;
	  R->normalised	= 1;
	  cce_run_body_handlers(L);
	  return R;
	}
      }
    }
  }
}

void
ccname_init(ccptn_t, realpath) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * R, ccptn_t const * const P)
{
  ptn_realpath(L, A, R, P);
}

ccptn_t const *
ccname_new(ccptn_t, realpath) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t const * const P)
{
  return ptn_realpath(L, A, NULL, P);
}
#endif


/** --------------------------------------------------------------------
 ** Low-level normalisation functions: useless slashses removal.
 ** ----------------------------------------------------------------- */

size_t
ccptn_normal_pass_remove_useless_slashes (char * output_ptr, char const * const input_ptr, size_t const input_len)
/* Copy a pathname  from "input_ptr" to "output_ptr" performing  a normalisation pass
   in the process: removal of multiple slashes.

   The array referenced by "input_ptr" must  represent an ASCIIZ string with at least
   "input_len"  octets,   terminating  zero   excluded.   The  array   referenced  by
   "output_ptr" must be at least "1 + input_len" octets wide.

   Return  the number  of  octets stored  in the  array  referenced by  "output_ptr",
   terminating zero excluded. */
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
/* Copy a pathname  from "input_ptr" to "output_ptr" performing  a normalisation pass
   in the process: removal of single-dot segments.   This pass is meant to be applied
   after "ccptn_normal_pass_remove_useless_slashes()".

   The array referenced by "input_ptr" must  represent an ASCIIZ string with at least
   "input_len"  octets,   terminating  zero   excluded.   The  array   referenced  by
   "output_ptr" must be at least "1 + input_len" octets wide.

   Return  the number  of  octets stored  in the  array  referenced by  "output_ptr",
   terminating zero excluded. */
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

    if (IS_STANDALONE_SLASH_SINGLE_DOT(in,next)) {
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
/* Copy a pathname  from "input_ptr" to "output_ptr" performing  a normalisation pass
   in the process: removal of double-dot segments and the segments before them.  This
   pass         is         meant          to         be         applied         after
   "ccptn_normal_pass_remove_single_dot_segments()".

   The array referenced by "input_ptr" must  represent an ASCIIZ string with at least
   "input_len"  octets,   terminating  zero   excluded.   The  array   referenced  by
   "output_ptr" must be at least "1 + input_len" octets wide.

   Return  the number  of  octets stored  in the  array  referenced by  "output_ptr",
   terminating zero excluded. */
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

__attribute__((__nonnull__(1,2,4),__returns_nonnull__))
static ccptn_t *
ptn_normalise (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * R, ccptn_t const * const P)
{
  char		one[1 + ccptn_len(P)];
  size_t	one_len;
  one_len = ccptn_normal_pass_remove_useless_slashes(one, P->ptr, ccptn_len(P));
  {
    char	two[1 + one_len];
    size_t	two_len;
    two_len = ccptn_normal_pass_remove_single_dot_segments(two, one, one_len);
    one_len = ccptn_normal_pass_remove_double_dot_segments(L, one, two, two_len);
    {
      /* Using  an ASCII  representation  will force  the  "ccptn_t" constructors  to
	 duplicate the internal pathname representation using a dynamically allocated
	 memory block. */
      ccmem_ascii_t	R_block = {
	.len	= one_len,
	.ptr	= one
      };
      if (R) {
	ccname_init(ccptn_t, ascii)(L, A, R, R_block);
      } else {
	R = (ccptn_t *)ccname_new(ccptn_t, ascii)(L, A, R_block);
      }
      R->normalised = 1;
      return R;
    }
  }
}

void
ccname_init(ccptn_t, normalise) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * const R, ccptn_t const * const P)
{
  ptn_normalise(L, A, R, P);
}

ccptn_t const *
ccname_new(ccptn_t, normalise) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t const * const P)
{
  return ptn_normalise(L, A, NULL, P);
}


/** --------------------------------------------------------------------
 ** Composition.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1,2,4,5),__returns_nonnull__))
static ccptn_t *
ptn_concat (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * R,
	    ccptn_t const * const prefix, ccptn_t const * const suffix)
{
  /* The resulting length is the sum of  the original lengths, plus one for the slash
   * separator.
   *
   * If the suffix  is absolute: its first  octet is the ASCII  representation of the
   * slash separator; we will copy it into the output.
   *
   * If the suffix is relative: its first  octet is *not* the ASCII representation of
   * the slash separator; we will explicitly insert a separator.
   */
  size_t	R_len = ccptn_len(prefix) + ccptn_len(suffix);
  if (! ccptn_is_absolute(suffix)) {
    ++R_len;
  }

  if (CCPTN_PATH_MAX < R_len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else {
    /* This array must hold the whole pathname plus the terminating zero
       octet. */
    char	R_ptr[R_len + 1];
    char *	ptr = R_ptr;

    /* Copy the prefix. */
    {
      size_t	len = ccptn_len(prefix);

      strncpy(ptr, prefix->ptr, len);
      ptr += len;
    }

    /* Insert a separator if needed. */
    if (! ccptn_is_absolute(suffix)) {
      *ptr++ = '/';
    }

    /* Copy the suffix and add the terminating zero. */
    {
      size_t	len = ccptn_len(suffix);

      strncpy(ptr, suffix->ptr, len);
      ptr += len;
      *ptr = '\0';
    }

    /* Build and return the resulting pathname. */
    {
      /* Using  an ASCII  representation  will force  the  "ccptn_t" constructors  to
	 duplicate the internal pathname representation using a dynamically allocated
	 memory block. */
      ccmem_ascii_t	R_block = {
	.len	= R_len,
	.ptr	= R_ptr
      };

      if (NULL != R) {
	ccname_init(ccptn_t, ascii)(L, A, R, R_block);
      } else {
	R = (ccptn_t *)ccname_new(ccptn_t, ascii)(L, A, R_block);
      }
      R->absolute = ccptn_is_absolute(prefix);
      return R;
    }
  }
}

void
ccname_init(ccptn_t, concat) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * R,
			      ccptn_t const * const prefix, ccptn_t const * const suffix)
{
  ptn_concat(L, A, R, prefix, suffix);
}

ccptn_t const *
ccname_new(ccptn_t, concat) (cce_destination_t L, ccmem_allocator_t const * const A,
			     ccptn_t const * const prefix, ccptn_t const * const suffix)
{
  return ptn_concat(L, A, NULL, prefix, suffix);
}


/** --------------------------------------------------------------------
 ** Pathnames manipulation: guarded constructors.
 ** ----------------------------------------------------------------- */

void
ccname_init(ccptn_t, realpath, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				       ccptn_t * dst, ccptn_t const * src)
{
  ccname_init(ccptn_t, realpath)(L, A, dst, src);
  ccptn_init_handler(L, H, dst);
}

void
ccname_init(ccptn_t, realpath, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				       ccptn_t * dst, ccptn_t const * src)
{
  ccname_init(ccptn_t, realpath)(L, A, dst, src);
  ccptn_init_handler(L, H, dst);
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, realpath, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				      ccptn_t const * src)
{
  ccptn_t const *	dst = ccname_new(ccptn_t, realpath)(L, A, src);
  ccptn_init_handler(L, H, dst);
  return dst;
}

ccptn_t const *
ccname_new(ccptn_t, realpath, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				      ccptn_t const * src)
{
  ccptn_t const *	dst = ccname_new(ccptn_t, realpath)(L, A, src);
  ccptn_init_handler(L, H, dst);
  return dst;
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, normalise, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
					ccptn_t * dst, ccptn_t const * src)
{
  ccname_init(ccptn_t, normalise)(L, A, dst, src);
  ccptn_init_handler(L, H, dst);
}

void
ccname_init(ccptn_t, normalise, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
					ccptn_t * dst, ccptn_t const * src)
{
  ccname_init(ccptn_t, normalise)(L, A, dst, src);
  ccptn_init_handler(L, H, dst);
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, normalise, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				       ccptn_t const * src)
{
  ccptn_t const *	dst = ccname_new(ccptn_t, normalise)(L, A, src);
  ccptn_init_handler(L, H, dst);
  return dst;
}

ccptn_t const *
ccname_new(ccptn_t, normalise, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				       ccptn_t const * src)
{
  ccptn_t const *	dst = ccname_new(ccptn_t, normalise)(L, A, src);
  ccptn_init_handler(L, H, dst);
  return dst;
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, concat, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				     ccptn_t * dst, ccptn_t const * prefix, ccptn_t const * suffix)
{
  ccname_init(ccptn_t, concat)(L, A, dst, prefix, suffix);
  ccptn_init_handler(L, H, dst);
}

void
ccname_init(ccptn_t, concat, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				     ccptn_t * dst, ccptn_t const * prefix, ccptn_t const * suffix)
{
  ccname_init(ccptn_t, concat)(L, A, dst, prefix, suffix);
  ccptn_init_handler(L, H, dst);
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, concat, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				    ccptn_t const * prefix, ccptn_t const * suffix)
{
  ccptn_t const *	dst = ccname_new(ccptn_t, concat)(L, A, prefix, suffix);
  ccptn_init_handler(L, H, dst);
  return dst;
}

ccptn_t const *
ccname_new(ccptn_t, concat, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				    ccptn_t const * prefix, ccptn_t const * suffix)
{
  ccptn_t const *	dst = ccname_new(ccptn_t, concat)(L, A, prefix, suffix);
  ccptn_init_handler(L, H, dst);
  return dst;
}

/* end of file */
