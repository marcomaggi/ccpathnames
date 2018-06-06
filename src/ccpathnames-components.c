/*
  Part of: CCPathnames
  Contents: pathname components extraction
  Date: Tue Feb 27, 2018

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
#include <errno.h>


/** --------------------------------------------------------------------
 ** Segments.
 ** ----------------------------------------------------------------- */

void
ccptn_segment_print (cce_destination_t L, FILE * stream, ccptn_segment_t S)
{
  int	rv;

  errno = 0;
  rv = fwrite(S.ptr, sizeof(char), S.len, stream);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}

ccptn_segment_t
ccptn_segment_next (char const * const in, size_t const len)
/* Given a  pointer "in" to  an ASCII string  of at least  "len" octets:
   return  a segment  struct  referencing  the octects  up  to, but  not
   including, the first  slash character or the end of  the string.  The
   returned segment does *not* include a terminating zero octet.*/
{
  char const *	ptr = in;
  char const *	end = in + len;

  while (ptr < end) {
    if ('/' == *ptr) {
      break;
    } else {
      ++ptr;
    }
  }
  {
    ccptn_segment_t	S = {
      .ptr	= in,
      .len	= ptr - in
    };
    return S;
  }
}

size_t
ccptn_segment_size_of_next (char const * const in, size_t const len)
/* Given a  pointer "in" to  an ASCII string  of at least  "len" octets:
   return  the number  of  octets  between the  one  referenced by  "in"
   (included) and the first slash  octet (excluded); return "len" itself
   if no slash is found. */
{
  char const *	ptr = in;

  for (size_t i=0; i<len; ++i) {
    if ('/' == ptr[i]) {
      return i;
    }
  }
  return len;
}


/** --------------------------------------------------------------------
 ** Extensions.
 ** ----------------------------------------------------------------- */

bool
ccptn_extension_equal (ccptn_extension_t E1, ccptn_extension_t E2)
{
  return ((E1.len == E2.len) && (0 == strncmp(E1.ptr, E2.ptr, E1.len)));
}

void
ccptn_extension_print (cce_destination_t L, FILE * stream, ccptn_extension_t E)
{
  int	rv;

  errno = 0;
  rv = fwrite(E.ptr, sizeof(char), E.len, stream);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}


/** --------------------------------------------------------------------
 ** Helpers.
 ** ----------------------------------------------------------------- */

ccptn_segment_t
ccptn_asciiz_find_last_segment (char const * beg, size_t const len)
/* Given an ASCII string representing  a pathname, referenced by BEG and
 * holding LEN octets  (not including a terminating zero,  if any): find
 * and return  the last segment  in the pathname.  The  returned segment
 * does not contain a leading or ending slash.  The returned segment can
 * be empty.  Examples:
 *
 *	"/path/to/file.ext"	=> "file.ext"
 *	"/path/to/dir/"		=> "dir"
 *	"/"			=> ""
 *	"."			=> "."
 *	".."			=> ".."
 */
{
  char const *	end = beg + len;
  char const *	ptr;

  /* If the  last octet in the  pathname is the ASCII  representation of
   * the  slash separator:  step back.   We want  the following  segment
   * extraction:
   *
   *	"/path/to/dir.ext/"	=> "dir.ext"
   */
  if ('/' == *(end-1)) {
    --end;
  }

  /* Find the first slash separator starting from the end. */
  for (ptr = end-1; beg <= ptr; --ptr) {
    if ('/' == *ptr) {
      /* "ptr"  is  at  the  beginning  of  the  last  component,  slash
	 included. */
      ++ptr;
      ccptn_segment_t	S = {
	.ptr	= ptr,
	.len	= end - ptr
      };
      return S;
    }
  }

  /* If we are  here: no slash was found in  the pathname, starting from
     the end; it means the whole pathname is the last segment. */
  {
    ccptn_segment_t	S = {
      .ptr	= beg,
      .len	= end - beg
    };
    return S;
  }
}


/** --------------------------------------------------------------------
 ** Pathname components: extension.
 ** ----------------------------------------------------------------- */

ccptn_extension_t
ccptn_extension (cce_destination_t L, ccptn_t const * const P)
{
  if (ccptn_is_normalised(P)) {
    ccptn_segment_t	S = ccptn_asciiz_find_last_segment(ccptn_asciiz(P), ccptn_len(P));

    if (0) {
      fprintf(stderr, "%s: ", __func__);
      ccptn_segment_print(L, stderr, S);
      fprintf(stderr, "\n");
    }

    if (ccptn_segment_is_slash(S) ||
	ccptn_segment_is_dot(S) ||
	ccptn_segment_is_double_dot(S) ||
	ccptn_segment_is_empty(S)) {
      ccptn_extension_t	E = {
	.len	= 0,
	.ptr	= ccptn_asciiz(P) + ccptn_len(P)
      };
      return E;
    } else {
      char const *	beg = S.ptr;
      char const *	end = beg + S.len;
      char const *	ptr = end - 1;

      for (; beg <= ptr; --ptr) {
	if ('.' == *ptr) {
	  /* Found the last dot in the segment. */
	  if (ptr == beg) {
	    /* The  dot is  the first  octet in  the last  segment: this
	       pathname  has  no  extension.   It  is  a  dotfile  like:
	       ".fvwmrc". */
	    break;
	  } else {
	    ccptn_extension_t	E = {
	      .ptr = ptr,
	      /* If  the input  pathname  ends with  a  slash: do  *not*
		 include it in the extension. */
	      .len = (('/' == *(end-1))? (end-1) : end) - ptr
	    };
	    return E;
	  }
	}
      }

      /* Extension  not found  in  the last  segment.   Return an  empty
	 extension. */
      {
	char const *	p = ccptn_asciiz(P) + ccptn_len(P);

	ccptn_extension_t	E = {
	  .len	= 0,
	  .ptr = (('/' == *(p-1))? (p-1) : p)
	};
	return E;
      }
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}


/** --------------------------------------------------------------------
 ** Pathname components: rootname.
 ** ----------------------------------------------------------------- */

__attribute__((__returns_nonnull__,__nonnull__(1,3)))
static ccptn_t *
ptn_rootname (cce_destination_t L, ccptn_t * const R, ccptn_t const * const P)
{
  if (ccptn_is_normalised(P)) {
    ccptn_extension_t	E = ccptn_extension(L, P);

    if (ccptn_extension_is_empty(E)) {
      ccptn_segment_t	S = ccptn_asciiz_find_last_segment(ccptn_asciiz(P), ccptn_len(P));

      if (0) {
	fprintf(stderr, "%s: input pathname=%s\n", __func__, ccptn_asciiz(P));
	fprintf(stderr, "%s: last segment=", __func__);
	ccptn_segment_print(L, stderr, S);
	fprintf(stderr, "\n");
      }

      if (IS_STANDALONE_SLASH(ccptn_asciiz(P), ccptn_asciiz(P) + ccptn_len(P)) ||
	  ccptn_segment_is_dot(S) || ccptn_segment_is_double_dot(S)) {
	cce_raise(L, ccptn_condition_new_no_rootname());
      } else {
	char const *	beg = ccptn_asciiz(P);
	size_t		len = ccptn_len(P);

	if ('/' == beg[len - 1]) {
	  --len;
	}
	if (R) {
	  ccptn_init_dup_ascii(L, R, beg, len);
	  R->normalised	= 1;
	  return R;
	} else {
	  ccptn_t *	Q = ccptn_new_dup_ascii(L, beg, len);
	  Q->normalised	= 1;
	  return Q;
	}
      }
    } else {
      char const * const	beg = ccptn_asciiz(P);

      if (R) {
	ccptn_init_dup_ascii(L, R, beg, E.ptr - beg);
	R->normalised	= 1;
	return R;
      } else {
	ccptn_t *	Q = ccptn_new_dup_ascii(L, beg, E.ptr - beg);
	Q->normalised	= 1;
	return Q;
      }
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}

ccptn_t *
ccptn_new_rootname (cce_destination_t L, ccptn_t const * P)
/* Build  and  return  a  new instance  of  "ccptn_t"  representing  the
 * rootname of the  input pathname referenced by P.   The input pathname
 * must be normalised.
 *
 * The rootname is the pathname representing the input pathname with the
 * extension of the last segment stripped.  Examples:
 *
 *	"/path/to/file.ext"	=> "/path/to/file"
 *	"/path/to/file"		=> "/path/to/file"
 *	"/path/to/dir/"		=> "/path/to/dir"
 *	"file.ext"		=> "file.ext"
 *	".fvwmrc"		=> ".fvwmrc"
 *	"."			--> error
 *	".."			--> error
 */
{
  return ptn_rootname(L, NULL, P);
}

ccptn_t *
ccptn_init_rootname (cce_destination_t L, ccptn_t * const R, ccptn_t const * const P)
/* Initialise an already allocated  instance of "ccptn_t", referenced by
 * R, with  the rootname  of the  input pathname  referenced by  P.  The
 * input pathname must be normalised.  Return a R itself.
 *
 * The rootname is the pathname representing the input pathname with the
 * extension of the last segment stripped.  Examples:
 *
 *	"/path/to/file.ext"	=> "/path/to/file"
 *	"/path/to/file"		=> "/path/to/file"
 *	"/path/to/dir/"		=> "/path/to/dir"
 *	"file.ext"		=> "file.ext"
 *	".fvwmrc"		=> ".fvwmrc"
 *	"."			--> error
 *	".."			--> error
 */
{
  return ptn_rootname(L, R, P);
}


/** --------------------------------------------------------------------
 ** Pathname components: dirname.
 ** ----------------------------------------------------------------- */

__attribute__((__returns_nonnull__,__nonnull__(1,3)))
static ccptn_t *
ptn_dirname (cce_destination_t L, ccptn_t * const R, ccptn_t const * const P)
{
  if (ccptn_is_normalised(P)) {
    size_t const	len = ccptn_len(P);
    char const * const	beg = ccptn_asciiz(P);
    char const * const	end = beg + len;

    /* If the pathname is ".": the directory part is "..". */
    if (IS_STANDALONE_SINGLE_DOT(beg, end)) {
      if (R) {
	ccptn_init_dup_asciiz(L, R, "..");
	R->normalised	= 1;
	return R;
      } else {
	ccptn_t *	Q = ccptn_new_dup_asciiz(L, "..");
	Q->normalised	= 1;
	return Q;
      }
    }
    /* If the pathname is "..", it has no dirname. */
    else if (IS_STANDALONE_DOUBLE_DOT(beg, end)) {
      cce_raise(L, ccptn_condition_new_no_dirname());
    }
    /* If the pathname is "/": the directory part is "/". */
    else if (IS_STANDALONE_SLASH(beg, end)) {
      if (R) {
	ccptn_init_dup_asciiz(L, R, "/");
	R->normalised	= 1;
	return R;
      } else {
	ccptn_t *	Q = ccptn_new_dup_asciiz(L, "/");
	Q->normalised	= 1;
	return Q;
      }
    }
    else {
      char const *	ptr = end - 1;

      /* Skip the trailing slash, if any. */
      if ('/' == *ptr) {
	--ptr;
      }

      /* Find the latest slash. */
      while ((beg < ptr) && ('/' != *ptr)) {
	--ptr;
      }

      if (beg < ptr) {
	/* If we  have found a  slash: include  it and build  the return
	   value. */
	++ptr;
	if (R) {
	  ccptn_init_dup_ascii(L, R, beg, ptr - beg);
	  R->normalised	= 1;
	  return R;
	} else {
	  ccptn_t *	Q = ccptn_new_dup_ascii(L, beg, ptr - beg);
	  Q->normalised	= 1;
	  return Q;
	}
      } else {
	/* There is no slash: the input pathname has no dirname. */
	cce_raise(L, ccptn_condition_new_no_dirname());
      }
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}

ccptn_t *
ccptn_new_dirname (cce_destination_t L, ccptn_t const * P)
/* Build and return a new instance of "ccptn_t" representing the dirname
 * of the  input pathname referenced by  P.  The input pathname  must be
 * normalised.
 *
 * The dirname is the pathname  representing the input pathname with the
 * last segment stripped.  Examples:
 *
 *	"/path/to/file.ext"	=> "/path/to"
 *	"/path/to/dir/"		=> "/path/to"
 *	"file.ext"		--> error
 *	"."			=> ".."
 *	".."			--> error
 */
{
  return ptn_dirname(L, NULL, P);
}

ccptn_t *
ccptn_init_dirname (cce_destination_t L, ccptn_t * const R, ccptn_t const * const P)
/* Initialise an already allocated  instance of "ccptn_t", referenced by
 * R, with the dirname of the input pathname referenced by P.  The input
 * pathname must be normalised.  Return a R itself.
 *
 * The dirname is the pathname  representing the input pathname with the
 * last segment stripped.  Examples:
 *
 *	"/path/to/file.ext"	=> "/path/to"
 *	"/path/to/dir/"		=> "/path/to"
 *	"file.ext"		--> error
 *	"."			=> ".."
 *	".."			--> error
 */
{
  return ptn_dirname(L, R, P);
}


/** --------------------------------------------------------------------
 ** Pathname components: tailname.
 ** ----------------------------------------------------------------- */

__attribute__((__returns_nonnull__,__nonnull__(1,3)))
static ccptn_t *
ptn_tailname (cce_destination_t L, ccptn_t * const R, ccptn_t const * const P)
{
  if (ccptn_is_normalised(P)) {
    size_t const	len = ccptn_len(P);
    char const * const	beg = ccptn_asciiz(P);

    if (IS_STANDALONE_SLASH(beg, beg + len)) {
      cce_raise(L, ccptn_condition_new_no_tailname());
    } else {
      char const *	ptr = beg + len - 1;

      /* Skip the trailing slash, if any. */
      if ('/' == *ptr) {
	--ptr;
      }

      /* Find the latest slash. */
      while ((beg < ptr) && ('/' != *ptr)) {
	--ptr;
      }

      /* If we have found a slash: skip it. */
      if (beg < ptr) {
	++ptr;
      }

      if (R) {
	ccptn_init_dup_asciiz(L, R, ptr);
	R->normalised	= 1;
	return R;
      } else {
	ccptn_t *	Q = ccptn_new_dup_asciiz(L, ptr);
	Q->normalised	= 1;
	return Q;
      }
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}

ccptn_t *
ccptn_new_tailname (cce_destination_t L, ccptn_t const * P)
/* Build  and  return  a  new instance  of  "ccptn_t"  representing  the
 * tailname of the  input pathname referenced by P.   The input pathname
 * must be normalised.
 *
 * The tailname is the relative  pathname representing the last segment.
 * Examples:
 *
 *	"/path/to/file.ext"	=> "file.ext"
 *	"/path/to/dir/"		=> "dir/"
 *	"file.ext"		=> "file.ext"
 *	"."			=> "."
 *	".."			=> ".."
 */
{
  return ptn_tailname(L, NULL, P);
}

ccptn_t *
ccptn_init_tailname (cce_destination_t L, ccptn_t * const R, ccptn_t const * const P)
/* Initialise an already allocated  instance of "ccptn_t", referenced by
 * R, with  the tailname  of the  input pathname  referenced by  P.  The
 * input pathname must be normalised.  Return a R itself.
 *
 * The tailname is the relative  pathname representing the last segment.
 * Examples:
 *
 *	"/path/to/file.ext"	=> "file.ext"
 *	"/path/to/dir/"		=> "dir/"
 *	"file.ext"		=> "file.ext"
 *	"."			=> "."
 *	".."			=> ".."
 */
{
  return ptn_tailname(L, R, P);
}


/** --------------------------------------------------------------------
 ** Pathname components: filename.
 ** ----------------------------------------------------------------- */

__attribute__((__returns_nonnull__,__nonnull__(1,3)))
static ccptn_t *
ptn_filename (cce_destination_t L, ccptn_t * const R, ccptn_t const * const P)
{
  if (ccptn_is_normalised(P)) {
    size_t const	len = ccptn_len(P);
    char const * const	beg = ccptn_asciiz(P);
    char const * const	end = beg + len;

    if (IS_STANDALONE_SLASH(beg, end) || IS_STANDALONE_SINGLE_DOT(beg, end) || IS_STANDALONE_DOUBLE_DOT(beg, end)) {
      cce_raise(L, ccptn_condition_new_no_filename());
    } else {
      char const *	ptr = end - 1;

      /* There is a trailing slash: it represents a directory. */
      if ('/' == *ptr) {
	cce_raise(L, ccptn_condition_new_no_filename());
      }

      /* Find the latest slash. */
      while ((beg < ptr) && ('/' != *ptr)) {
	--ptr;
      }

      /* If we have found a slash: skip it. */
      if (beg < ptr) {
	++ptr;
      }

      if (R) {
	ccptn_init_dup_asciiz(L, R, ptr);
	R->normalised	= 1;
	return R;
      } else {
	ccptn_t *	Q = ccptn_new_dup_asciiz(L, ptr);
	Q->normalised	= 1;
	return Q;
      }
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}

ccptn_t *
ccptn_new_filename (cce_destination_t L, ccptn_t const * P)
/* Build  and  return  a  new instance  of  "ccptn_t"  representing  the
 * filename of the  input pathname referenced by P.   The input pathname
 * must be normalised.
 *
 * The filename is  the relative pathname representing  the last segment
 * if it does not represent a directory.  Examples:
 *
 *	"/path/to/file.ext"	=> "file.ext"
 *	"/path/to/dir/"		--> error
 *	"file.ext"		=> "file.ext"
 *	"."			--> error
 *	".."			--> error
 */
{
  return ptn_filename(L, NULL, P);
}

ccptn_t *
ccptn_init_filename (cce_destination_t L, ccptn_t * const R, ccptn_t const * const P)
/* Initialise an already allocated  instance of "ccptn_t", referenced by
 * R, with  the filename  of the  input pathname  referenced by  P.  The
 * input pathname must be normalised.  Return a R itself.
 *
 * The filename is  the relative pathname representing  the last segment
 * if it does not represent a directory.  Examples:
 *
 *	"/path/to/file.ext"	=> "file.ext"
 *	"/path/to/dir/"		--> error
 *	"file.ext"		=> "file.ext"
 *	"."			--> error
 *	".."			--> error
 */
{
  return ptn_filename(L, R, P);
}

/* end of file */
