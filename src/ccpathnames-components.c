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

static ccptn_extension_t const	empty_extension = {
  .ptr = NULL,
  .len = 0
};

static ccptn_segment_t const	empty_segment = {
  .ptr = NULL,
  .len = 0
};


/** --------------------------------------------------------------------
 ** Segments.
 ** ----------------------------------------------------------------- */

bool
ccptn_segment_is_empty (ccptn_segment_t S)
/* Return true if the segment is an empty string. */
{
  return (0 == S.len)? true : false;
}

bool
ccptn_segment_is_dot (ccptn_segment_t S)
/* Return true if the segment is a single dot: ".". */
{
  return ((1 == S.len) && ('.' == *(S.ptr)))? true : false;
}

bool
ccptn_segment_is_double_dot (ccptn_segment_t S)
/* Return true if the segment is a double dot: "..". */
{
  return ((2 == S.len) && ('.' == S.ptr[0]) && ('.' == S.ptr[1]))? true : false;
}

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
ccptn_extension_is_empty (ccptn_extension_t E)
/* Return true if the extension is an empty string. */
{
  return (0 == E.len)? true : false;
}

bool
ccptn_extension_equal (ccptn_extension_t E1, ccptn_extension_t E2)
{
  return ((E1.len == E2.len) && (0 == strncmp(E1.ptr, E2.ptr, E1.len))) ? true : false;
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

static ccptn_segment_t
ccptn_asciiz_find_last_segment (char const * beg, size_t const len)
{
  char const *	end = beg + len;

  if ('/' == *end) {
    /* The last octet in the pathname is the ASCII representation of the
       slash separator; the last segment is empty. */
    return empty_segment;
  } else {
    /* Here we  know that the  last octet in  the pathname is  *not* the
       ASCII representation of the slash separator. */

    /* Find the first slash separator starting from the end. */
    for (char const * ptr = --end; beg <= ptr; --ptr) {
      if ('/' == *ptr) {
	/* "ptr"  is  at  the  beginning  of  the  last  component,  slash
	   included. */
	ccptn_segment_t	S = {
	  .ptr	= 1+ptr,
	  .len	= end - ptr
	};
	return S;
      } else {

      }
    }

    /* If we are  here: no slash was found in  the pathname, starting from
       the end; it means the whole pathname is the last segment. */
    {
      ccptn_segment_t	S = {
	.ptr	= beg,
	.len	= len
      };
      return S;
    }
  }
}


/** --------------------------------------------------------------------
 ** Extension and rootname.
 ** ----------------------------------------------------------------- */

ccptn_extension_t
ccptn_extension (cce_destination_t L, ccptn_t const * P)
{
  if (ccptn_is_normalised(P)) {
    ccptn_segment_t	S = ccptn_asciiz_find_last_segment(ccptn_asciiz(P), ccptn_len(P));

    if (ccptn_segment_is_dot(S) ||
	ccptn_segment_is_double_dot(S) ||
	ccptn_segment_is_empty(S)) {
      return empty_extension;
    } else {
      char const *	beg = S.ptr;
      char const *	end = beg + S.len;
      char const *	ptr = end - 1;

      for (; beg <= ptr; --ptr) {
	if ('.' == *ptr) {
	  /* Found the last dot in the segment. */
	  if (ptr == beg) {
	    /* The  dot  is  the  first  octet:  this  pathname  has  no
	       extension.  It is a dotfile like: ".fvwmrc". */
	    return empty_extension;
	  } else if ('/' == *(ptr - 1)) {
	    /* The dot is the first octet right after a slash separator:
	       this pathname  has no extension.   It is a  dotfile like:
	       "/home/marco/.fvwmrc". */
	    return empty_extension;
	  } else {
	    /* Found an actual extension. */
	    ccptn_extension_t	E = {
	      .ptr = ptr,
	      .len = end - ptr
	    };
	    return E;
	  }
	}
      }
      return empty_extension;
    }
  } else {
    cce_raise(L, ccptn_condition_new_normalised_pathname());
  }
}

ccptn_t *
ccptn_rootname (cce_destination_t L CCPTN_UNUSED, ccptn_t const * P CCPTN_UNUSED)
/* Build and return a new  pathname representing the input pathname with
   the exclusion  of the last  segment's extension; the dot  leading the
   extension is excluded from the returned pathname. */
{
  return NULL;
}

/* end of file */
