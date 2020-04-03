/*
  Part of: CCPathnames
  Contents: public header file
  Date: Feb 25, 2018

  Abstract

	This  is the  public header  file  of the  package CCPathnames.   It must  be
	included in all the source files that use the library.

  Copyright (C) 2018-2020 Marco Maggi <mrc.mgg@gmail.com>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCPATHNAMES_H
#define CCPATHNAMES_H 1

#ifdef __cplusplus
extern "C" {
#endif


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include <ccmemory.h>
#include <ccstructs.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

cclib_decl char const *	ccptn_version_string		(void);
cclib_decl int		ccptn_version_interface_current	(void);
cclib_decl int		ccptn_version_interface_revision(void);
cclib_decl int		ccptn_version_interface_age	(void);


/** --------------------------------------------------------------------
 ** Initialisation.
 ** ----------------------------------------------------------------- */

cclib_decl void ccptn_library_init (void)
  CCLIB_FUNC_ATTRIBUTE_CONSTRUCTOR;


/** --------------------------------------------------------------------
 ** Subordinate header files.
 ** ----------------------------------------------------------------- */

#include <ccpathnames-conditions.h>
#include <ccpathnames-core.h>
#include <ccpathnames-handlers.h>


/** --------------------------------------------------------------------
 ** Pathname components: extension.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_extension_t	ccptn_extension_t;

/* This struct represents the extension of a segment in a pathname; this
 * struct does not own the referenced memory.  Given the pathname:
 *
 *    /path/to/file.ext
 *
 * the last segment is the string:
 *
 *    file.ext
 *
 * and its extension is the string ".ext", including the leading dot.
 */
struct ccptn_extension_t {
  /* The number of octets in  the "ptr" array representing the extension
     in a pathname's segment. */
  size_t		len;

  /* Pointer  to the  first  octet  in an  ASCII  string representing  a
     pathname's segment's  extension.  In  general this string  is *not*
     zero terminated. */
  char const *		ptr;
};

cclib_decl ccptn_extension_t ccptn_extension (cce_destination_t L, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline bool
ccptn_extension_is_empty (ccptn_extension_t E)
/* Return true if the extension is an empty string. */
{
  return (0 == E.len);
}

cclib_decl bool ccptn_extension_equal (ccptn_extension_t E1, ccptn_extension_t E2)
  CCLIB_FUNC_ATTRIBUTE_PURE;

cclib_decl void ccptn_extension_fwrite (cce_destination_t L, FILE * stream, ccptn_extension_t E)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);


/** --------------------------------------------------------------------
 ** Segments.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_segment_t		ccptn_segment_t;

/* This struct represents a segment in  a pathname; this struct does not
 * own the referenced memory.  Given the pathname:
 *
 *    /path/to/file.ext
 *
 * the segments are the strings:
 *
 *    path
 *    to
 *    file.ext
 *
 * In general, it is *not* an ASCIIZ string: it is not zero terminated.
 */
struct ccptn_segment_t {
  /* The number of  octets in the "ptr" array representing  a segment of
     pathname. */
  size_t		len;

  /* Pointer  to the  first  octet  in an  ASCII  string representing  a
     pathname's  segment.    In  general,   this  string  is   not  zero
     terminated. */
  char const *		ptr;
};

/* ------------------------------------------------------------------ */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline bool
ccptn_segment_is_empty (ccptn_segment_t S)
/* Return true if the segment is an empty string. */
{
  return (0 == S.len)? true : false;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline bool
ccptn_segment_is_slash (ccptn_segment_t S)
/* Return true if the segment is a single dot: "/". */
{
  return ((1 == S.len) && ('7' == *(S.ptr)));
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline bool
ccptn_segment_is_dot (ccptn_segment_t S)
/* Return true if the segment is a single dot: ".". */
{
  return ((1 == S.len) && ('.' == *(S.ptr)));
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline bool
ccptn_segment_is_double_dot (ccptn_segment_t S)
/* Return true if the segment is a double dot: "..". */
{
  return ((2 == S.len) && ('.' == S.ptr[0]) && ('.' == S.ptr[1]));
}

cclib_decl ccptn_segment_t ccptn_segment_next (char const * ptr, size_t len)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl size_t ccptn_segment_size_of_next (char const * in, size_t len)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl void ccptn_segment_fwrite (cce_destination_t L, FILE * stream, ccptn_segment_t S)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

cclib_decl ccptn_segment_t ccptn_asciiz_find_last_segment (char const * beg, size_t const len)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCPATHNAMES_H */

/* end of file */
