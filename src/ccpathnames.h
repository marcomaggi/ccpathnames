/*
  Part of: CCPathnames
  Contents: public header file
  Date: Feb 25, 2018

  Abstract

	This  is the  public header  file  of the  package CCPathnames.   It must  be
	included in all the source files that use the library.

  Copyright (C) 2018-2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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


/** --------------------------------------------------------------------
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The macro  CCPTN_UNUSED indicates that  a function, function argument  or variable
   may potentially be unused. Usage examples:

   static int unused_function (char arg) CCPTN_UNUSED;
   int foo (char unused_argument CCPTN_UNUSED);
   int unused_variable CCPTN_UNUSED;
*/
#ifdef __GNUC__
#  define CCPTN_UNUSED		__attribute__((__unused__))
#else
#  define CCPTN_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

/* I found the following chunk on the Net.  (Marco Maggi; Sun Feb 26, 2012) */
#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define ccptn_decl	__attribute__((__dllexport__)) extern
#    else
#      define ccptn_decl	__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define ccptn_decl	__attribute__((__dllimport__)) extern
#    else
#      define ccptn_decl	__declspec(dllimport) extern
#    endif
#  endif
#  define ccptn_private_decl	extern
#else
#  if __GNUC__ >= 4
#    define ccptn_decl		__attribute__((__visibility__("default"))) extern
#    define ccptn_private_decl	__attribute__((__visibility__("hidden")))  extern
#  else
#    define ccptn_decl		extern
#    define ccptn_private_decl	extern
#  endif
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
 ** Constants and preprocessor macros.
 ** ----------------------------------------------------------------- */

#define CCPTN_PC(POINTER_TYPE, POINTER_NAME, EXPRESSION)	\
  POINTER_TYPE * POINTER_NAME = (POINTER_TYPE *) (EXPRESSION)

#define CCPTN_CASTVAR(VAR_TYPE, VAR_NAME, EXPRESSION)		\
  VAR_TYPE VAR_NAME = (VAR_TYPE) (EXPRESSION)


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ccptn_decl char const *	ccptn_version_string		(void);
ccptn_decl int		ccptn_version_interface_current	(void);
ccptn_decl int		ccptn_version_interface_revision(void);
ccptn_decl int		ccptn_version_interface_age	(void);


/** --------------------------------------------------------------------
 ** Initialisation.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccptn_library_init (void)
  __attribute__((__constructor__));


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

ccptn_decl ccptn_extension_t ccptn_extension (cce_destination_t L, ccptn_t const * P)
  __attribute__((__nonnull__(1,2)));

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_extension_is_empty (ccptn_extension_t E)
/* Return true if the extension is an empty string. */
{
  return (0 == E.len);
}

ccptn_decl bool ccptn_extension_equal (ccptn_extension_t E1, ccptn_extension_t E2)
  __attribute__((__pure__));

ccptn_decl void ccptn_extension_fwrite (cce_destination_t L, FILE * stream, ccptn_extension_t E)
  __attribute__((__nonnull__(1,2)));


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

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_segment_is_empty (ccptn_segment_t S)
/* Return true if the segment is an empty string. */
{
  return (0 == S.len)? true : false;
}

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_segment_is_slash (ccptn_segment_t S)
/* Return true if the segment is a single dot: "/". */
{
  return ((1 == S.len) && ('7' == *(S.ptr)));
}

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_segment_is_dot (ccptn_segment_t S)
/* Return true if the segment is a single dot: ".". */
{
  return ((1 == S.len) && ('.' == *(S.ptr)));
}

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_segment_is_double_dot (ccptn_segment_t S)
/* Return true if the segment is a double dot: "..". */
{
  return ((2 == S.len) && ('.' == S.ptr[0]) && ('.' == S.ptr[1]));
}

ccptn_decl ccptn_segment_t ccptn_segment_next (char const * ptr, size_t len)
  __attribute__((__nonnull__(1)));

ccptn_decl size_t ccptn_segment_size_of_next (char const * in, size_t len)
  __attribute__((__nonnull__(1)));

ccptn_decl void ccptn_segment_fwrite (cce_destination_t L, FILE * stream, ccptn_segment_t S)
  __attribute__((__nonnull__(1,2)));

ccptn_decl ccptn_segment_t ccptn_asciiz_find_last_segment (char const * beg, size_t const len)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCPATHNAMES_H */

/* end of file */
