/*
  Part of: CCPathnames
  Contents: pathname components extraction
  Date: Tue Feb 27, 2018

  Abstract



  Copyright (C) 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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
/* Given a pointer "in" to an ASCII string of at least "len" octets: return a segment
   struct referencing the octects up to, but not including, the first slash character
   or the end of  the string.  The returned segment does  *not* include a terminating
   zero octet.*/
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
/* Given a  pointer "in"  to an  ASCII string of  at least  "len" octets:  return the
   number of octets between the one referenced by "in" (included) and the first slash
   octet (excluded); return "len" itself if no slash is found. */
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
ccptn_extension_fwrite (cce_destination_t L, FILE * stream, ccptn_extension_t E)
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
/* Given an ASCII  string representing a pathname, referenced by  BEG and holding LEN
 * octets  (not including  a terminating  zero,  if any):  find and  return the  last
 * segment  in the  pathname.  The  returned segment  does not  contain a  leading or
 * ending slash.  The returned segment can be empty.  Examples:
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

  /* If the  last octet  in the  pathname is  the ASCII  representation of  the slash
   * separator: step back.  We want the following segment extraction:
   *
   *	"/path/to/dir.ext/"	=> "dir.ext"
   */
  if ('/' == *(end-1)) {
    --end;
  }

  /* Find the first slash separator starting from the end. */
  for (ptr = end-1; beg <= ptr; --ptr) {
    if ('/' == *ptr) {
      /* "ptr" is at the beginning of the last component, slash included. */
      ++ptr;
      ccptn_segment_t	S = {
	.ptr	= ptr,
	.len	= end - ptr
      };
      return S;
    }
  }

  /* If we are  here: no slash was found  in the pathname, starting from  the end; it
     means the whole pathname is the last segment. */
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
    ccptn_segment_t	S = ccptn_asciiz_find_last_segment(P->ptr, ccptn_len(P));

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
	.ptr	= P->ptr + ccptn_len(P)
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
	    /* The dot is the  first octet in the last segment:  this pathname has no
	       extension.  It is a dotfile like: ".fvwmrc". */
	    break;
	  } else {
	    ccptn_extension_t	E = {
	      .ptr = ptr,
	      /* If the input pathname ends with a  slash: do *not* include it in the
		 extension. */
	      .len = (('/' == *(end-1))? (end-1) : end) - ptr
	    };
	    return E;
	  }
	}
      }

      /* Extension not found in the last segment.  Return an empty extension. */
      {
	char const *	p = P->ptr + ccptn_len(P);

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

__attribute__((__returns_nonnull__,__nonnull__(1,2,4)))
static ccptn_t *
ptn_rootname (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * R, ccptn_t const * const P)
{
  if (ccptn_is_normalised(P)) {
    ccptn_extension_t	E = ccptn_extension(L, P);

    if (ccptn_extension_is_empty(E)) {
      ccptn_segment_t	S = ccptn_asciiz_find_last_segment(P->ptr, ccptn_len(P));

      if (0) {
	fprintf(stderr, "%s: input pathname=%s\n", __func__, P->ptr);
	fprintf(stderr, "%s: last segment=", __func__);
	ccptn_segment_print(L, stderr, S);
	fprintf(stderr, "\n");
      }

      if (IS_STANDALONE_SLASH(P->ptr, P->ptr + ccptn_len(P)) ||
	  ccptn_segment_is_dot(S) || ccptn_segment_is_double_dot(S)) {
	cce_raise(L, ccptn_condition_new_no_rootname());
      } else {
	char const *	beg = P->ptr;
	size_t		len = ccptn_len(P);

	if ('/' == beg[len - 1]) {
	  --len;
	}
	{
	  /* Using an ASCII  representation will force the  "ccptn_t" constructors to
	     duplicate  the  internal  pathname representation  using  a  dynamically
	     allocated memory block. */
	  ccmem_ascii_t	R_block = { .ptr = (char *)beg, .len = len };

	  if (R) {
	    ccname_init(ccptn_t, ascii)(L, A, R, R_block);
	  } else {
	    R = (ccptn_t *)ccname_new(ccptn_t, ascii)(L, A, R_block);
	  }
	  R->normalised	= 1;
	  return R;
	}
      }
    } else {
      /* Using  an ASCII  representation  will force  the  "ccptn_t" constructors  to
	 duplicate the internal pathname representation using a dynamically allocated
	 memory block. */
      ccmem_ascii_t	R_block = { .ptr = (char *)P->ptr, .len = E.ptr - P->ptr };

      if (R) {
	ccname_init(ccptn_t, ascii)(L, A, R, R_block);
      } else {
	R = (ccptn_t *)ccname_new(ccptn_t, ascii)(L, A, R_block);
      }
      R->normalised = 1;
      return R;
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}

void
ccname_init(ccptn_t, rootname) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * R, ccptn_t const * P)
/* Initialise an already  allocated instance of "ccptn_t", referenced by  R, with the
 * rootname  of the  input pathname  referenced  by P.   The input  pathname must  be
 * normalised.  Return a R itself.
 *
 * The rootname is the pathname representing the input pathname with the extension of
 * the last segment stripped.  Examples:
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
  ptn_rootname(L, A, R, P);
}

ccptn_t const *
ccname_new(ccptn_t, rootname) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * P)
/* Build and  return a  new instance  of "ccptn_t" representing  the rootname  of the
 * input pathname referenced by P.  The input pathname must be normalised.
 *
 * The rootname is the pathname representing the input pathname with the extension of
 * the last segment stripped.  Examples:
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
  return ptn_rootname(L, A, NULL, P);
}


/** --------------------------------------------------------------------
 ** Pathname components: dirname.
 ** ----------------------------------------------------------------- */

__attribute__((__returns_nonnull__,__nonnull__(1,2,4)))
static ccptn_t *
ptn_dirname (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * R, ccptn_t const * const P)
{
  if (ccptn_is_normalised(P)) {
    size_t const	len = ccptn_len(P);
    char const * const	beg = P->ptr;
    char const * const	end = beg + len;

    /* If the pathname is ".": the directory part is "..". */
    if (IS_STANDALONE_SINGLE_DOT(beg, end)) {
      if (R) {
	ccname_init(ccptn_t, pointer)(L, A, R, "..");
      } else {
	R = (ccptn_t *)ccname_new(ccptn_t, pointer)(L, A, "..");
      }
      R->normalised	= 1;
      return R;
    }
    /* If the pathname is "..", it has no dirname. */
    else if (IS_STANDALONE_DOUBLE_DOT(beg, end)) {
      cce_raise(L, ccptn_condition_new_no_dirname());
    }
    /* If the pathname is "/": the directory part is "/". */
    else if (IS_STANDALONE_SLASH(beg, end)) {
      if (R) {
	ccname_init(ccptn_t, pointer)(L, A, R, "/");
      } else {
	R = (ccptn_t *)ccname_new(ccptn_t, pointer)(L, A, "/");
      }
      R->normalised	= 1;
      return R;
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
	/* If we have found a slash: include it and build the return value. */
	++ptr;
	/* Using an  ASCII representation  will force  the "ccptn_t"  constructors to
	   duplicate  the  internal  pathname   representation  using  a  dynamically
	   allocated memory block. */
	ccmem_ascii_t	R_block = { .ptr = (char *)beg, .len = ptr - beg };
	if (R) {
	  ccname_init(ccptn_t, ascii)(L, A, R, R_block);
	} else {
	  R = (ccptn_t *)ccname_new(ccptn_t, ascii)(L, A, R_block);
	}
	R->normalised	= 1;
	return R;
      } else {
	/* There is no slash: the input pathname has no dirname. */
	cce_raise(L, ccptn_condition_new_no_dirname());
      }
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}

void
ccname_init(ccptn_t, dirname) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * R, ccptn_t const * P)
/* Initialise an already  allocated instance of "ccptn_t", referenced by  R, with the
 * dirname  of the  input  pathname referenced  by  P.  The  input  pathname must  be
 * normalised.  Return a R itself.
 *
 * The dirname is the pathname representing  the input pathname with the last segment
 * stripped.  Examples:
 *
 *	"/path/to/file.ext"	=> "/path/to"
 *	"/path/to/dir/"		=> "/path/to"
 *	"file.ext"		--> error
 *	"."			=> ".."
 *	".."			--> error
 */
{
  ptn_dirname(L, A, R, P);
}

ccptn_t const *
ccname_new(ccptn_t, dirname) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * P)
/* Build and return a new instance of "ccptn_t" representing the dirname of the input
 * pathname referenced by P.  The input pathname must be normalised.
 *
 * The dirname is the pathname representing  the input pathname with the last segment
 * stripped.  Examples:
 *
 *	"/path/to/file.ext"	=> "/path/to"
 *	"/path/to/dir/"		=> "/path/to"
 *	"file.ext"		--> error
 *	"."			=> ".."
 *	".."			--> error
 */
{
  return ptn_dirname(L, A, NULL, P);
}


/** --------------------------------------------------------------------
 ** Pathname components: tailname.
 ** ----------------------------------------------------------------- */

__attribute__((__returns_nonnull__,__nonnull__(1,2,4)))
static ccptn_t *
ptn_tailname (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * R, ccptn_t const * const P)
{
  if (ccptn_is_normalised(P)) {
    size_t const	len = ccptn_len(P);
    char const * const	beg = P->ptr;

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

      {
	/* Using an  ASCII representation  will force  the "ccptn_t"  constructors to
	   duplicate  the  internal  pathname   representation  using  a  dynamically
	   allocated memory block. */
	ccmem_ascii_t	R_block = { .ptr = (char *)ptr, .len = strlen(ptr) };

	if (R) {
	  ccname_init(ccptn_t, ascii)(L, A, R, R_block);
	} else {
	  R = (ccptn_t *)ccname_new(ccptn_t, ascii)(L, A, R_block);
	}
	R->normalised	= 1;
	return R;
      }
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}

void
ccname_init(ccptn_t, tailname) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * R, ccptn_t const * P)
/* Initialise an already  allocated instance of "ccptn_t", referenced by  R, with the
 * tailname  of the  input pathname  referenced  by P.   The input  pathname must  be
 * normalised.  Return a R itself.
 *
 * The tailname is the relative pathname representing the last segment.  Examples:
 *
 *	"/path/to/file.ext"	=> "file.ext"
 *	"/path/to/dir/"		=> "dir/"
 *	"file.ext"		=> "file.ext"
 *	"."			=> "."
 *	".."			=> ".."
 */
{
  ptn_tailname(L, A, R, P);
}

ccptn_t const *
ccname_new(ccptn_t, tailname) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * P)
/* Build and  return a  new instance  of "ccptn_t" representing  the tailname  of the
 * input pathname referenced by P.  The input pathname must be normalised.
 *
 * The tailname is the relative pathname representing the last segment.  Examples:
 *
 *	"/path/to/file.ext"	=> "file.ext"
 *	"/path/to/dir/"		=> "dir/"
 *	"file.ext"		=> "file.ext"
 *	"."			=> "."
 *	".."			=> ".."
 */
{
  return ptn_tailname(L, A, NULL, P);
}


/** --------------------------------------------------------------------
 ** Pathname components: filename.
 ** ----------------------------------------------------------------- */

__attribute__((__returns_nonnull__,__nonnull__(1,2,4)))
static ccptn_t *
ptn_filename (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * R, ccptn_t const * const P)
{
  if (ccptn_is_normalised(P)) {
    size_t const	len = ccptn_len(P);
    char const * const	beg = P->ptr;
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

      {
	/* Using an  ASCII representation  will force  the "ccptn_t"  constructors to
	   duplicate  the  internal  pathname   representation  using  a  dynamically
	   allocated memory block. */
	ccmem_ascii_t	R_block = { .ptr = (char *)ptr, .len = strlen(ptr) };

	if (R) {
	  ccname_init(ccptn_t, ascii)(L, A, R, R_block);
	} else {
	  R = (ccptn_t *)ccname_new(ccptn_t, ascii)(L, A, R_block);
	}
	R->normalised	= 1;
	return R;
      }
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}

void
ccname_init(ccptn_t, filename) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * R, ccptn_t const * P)
/* Initialise an already  allocated instance of "ccptn_t", referenced by  R, with the
 * filename  of the  input pathname  referenced  by P.   The input  pathname must  be
 * normalised.  Return a R itself.
 *
 * The filename is the relative pathname representing the last segment if it does not
 * represent a directory.  Examples:
 *
 *	"/path/to/file.ext"	=> "file.ext"
 *	"/path/to/dir/"		--> error
 *	"file.ext"		=> "file.ext"
 *	"."			--> error
 *	".."			--> error
 */
{
  ptn_filename(L, A, R, P);
}

ccptn_t const *
ccname_new(ccptn_t, filename) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * P)
/* Build and  return a  new instance  of "ccptn_t" representing  the filename  of the
 * input pathname referenced by P.  The input pathname must be normalised.
 *
 * The filename is the relative pathname representing the last segment if it does not
 * represent a directory.  Examples:
 *
 *	"/path/to/file.ext"	=> "file.ext"
 *	"/path/to/dir/"		--> error
 *	"file.ext"		=> "file.ext"
 *	"."			--> error
 *	".."			--> error
 */
{
  return ptn_filename(L, A, NULL, P);
}


/** --------------------------------------------------------------------
 ** Pathname components: guarded constructors.
 ** ----------------------------------------------------------------- */

void
ccname_init(ccptn_t, rootname, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				       ccptn_t * R, ccptn_t const * P)
{
  ccname_init(ccptn_t, rootname)(L, A, R, P);
  ccptn_init_and_register_handler(L, H, R);
}

void
ccname_init(ccptn_t, rootname, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				       ccptn_t * R, ccptn_t const * P)
{
  ccname_init(ccptn_t, rootname)(L, A, R, P);
  ccptn_init_and_register_handler(L, H, R);
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, rootname, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				      ccptn_t const * P)
{
  ccptn_t const *	R = ccname_new(ccptn_t, rootname)(L, A, P);
  ccptn_init_and_register_handler(L, H, R);
  return R;
}

ccptn_t const *
ccname_new(ccptn_t, rootname, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				      ccptn_t const * P)
{
  ccptn_t const *	R = ccname_new(ccptn_t, rootname)(L, A, P);
  ccptn_init_and_register_handler(L, H, R);
  return R;
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, dirname, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				      ccptn_t * R, ccptn_t const * P)
{
  ccname_init(ccptn_t, dirname)(L, A, R, P);
  ccptn_init_and_register_handler(L, H, R);
}

void
ccname_init(ccptn_t, dirname, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				      ccptn_t * R, ccptn_t const * P)
{
  ccname_init(ccptn_t, dirname)(L, A, R, P);
  ccptn_init_and_register_handler(L, H, R);
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, dirname, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				     ccptn_t const * P)
{
  ccptn_t const *	R = ccname_new(ccptn_t, dirname)(L, A, P);
  ccptn_init_and_register_handler(L, H, R);
  return R;
}

ccptn_t const *
ccname_new(ccptn_t, dirname, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				     ccptn_t const * P)
{
  ccptn_t const *	R = ccname_new(ccptn_t, dirname)(L, A, P);
  ccptn_init_and_register_handler(L, H, R);
  return R;
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, tailname, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				       ccptn_t * R, ccptn_t const * P)
{
  ccname_init(ccptn_t, tailname)(L, A, R, P);
  ccptn_init_and_register_handler(L, H, R);
}

void
ccname_init(ccptn_t, tailname, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				       ccptn_t * R, ccptn_t const * P)
{
  ccname_init(ccptn_t, tailname)(L, A, R, P);
  ccptn_init_and_register_handler(L, H, R);
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, tailname, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				      ccptn_t const * P)
{
  ccptn_t const *	R = ccname_new(ccptn_t, tailname)(L, A, P);
  ccptn_init_and_register_handler(L, H, R);
  return R;
}

ccptn_t const *
ccname_new(ccptn_t, tailname, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				      ccptn_t const * P)
{
  ccptn_t const *	R = ccname_new(ccptn_t, tailname)(L, A, P);
  ccptn_init_and_register_handler(L, H, R);
  return R;
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, filename, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				       ccptn_t * R, ccptn_t const * P)
{
  ccname_init(ccptn_t, filename)(L, A, R, P);
  ccptn_init_and_register_handler(L, H, R);
}

void
ccname_init(ccptn_t, filename, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				       ccptn_t * R, ccptn_t const * P)
{
  ccname_init(ccptn_t, filename)(L, A, R, P);
  ccptn_init_and_register_handler(L, H, R);
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, filename, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				      ccptn_t const * P)
{
  ccptn_t const *	R = ccname_new(ccptn_t, filename)(L, A, P);
  ccptn_init_and_register_handler(L, H, R);
  return R;
}

ccptn_t const *
ccname_new(ccptn_t, filename, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				      ccptn_t const * P)
{
  ccptn_t const *	R = ccname_new(ccptn_t, filename)(L, A, P);
  ccptn_init_and_register_handler(L, H, R);
  return R;
}

/* end of file */
