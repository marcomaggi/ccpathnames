/*
  Part of: CCPathnames
  Contents: private header file
  Date: Feb 26, 2018

  Abstract

	This  header  file is  for  internal  definitions.  It  must  be
	included by all the source files in this package.

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

#ifndef CCPATHNAMES_INTERNALS_H
#define CCPATHNAMES_INTERNALS_H 1


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE		500

#include "ccpathnames.h"
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <string.h>


/** --------------------------------------------------------------------
 ** Preprocessor macros.
 ** ----------------------------------------------------------------- */

/* Evaluate to true  if PTR references an array of  ASCII characters, of
   LEN octets, representing a single-dot segment. */
#define IS_SINGLE_DOT(PTR,LEN)		\
  ((1 == (LEN)) && ('.' == (PTR)[0]))

/* Evaluate to true  if PTR references an array of  ASCII characters, of
   LEN octets, representing a double-dot segment. */
#define IS_DOUBLE_DOT(PTR,LEN)		\
  ((2 == (LEN)) && ('.' == (PTR)[0]) && ('.' == (PTR)[1]))

/* Evaluate to true  if PTR references an array of  ASCII characters, of
   LEN octets, representing a slash followed by a double-dot segment. */
#define IS_SLASH_DOUBLE_DOT(PTR,LEN)	\
  ((3 == (LEN)) && ('/' == (PTR)[0]) && ('.' == (PTR)[1]) && ('.' == (PTR)[2]))


/** --------------------------------------------------------------------
 ** Constants.
 ** ----------------------------------------------------------------- */

/* We can customise this value at package configuration time using, for example:
 *
 *   $ ./configure CPPFLAGS='-DCCPTN_PATH_MAX=8192'
 *
 */
#ifndef CCPTN_PATH_MAX
#  define CCPTN_PATH_MAX	4096
#endif


/** --------------------------------------------------------------------
 ** Inline functions and function prototypes.
 ** ----------------------------------------------------------------- */

#define INLINE1	__attribute__((__always_inline__,__nonnull__(1))) static inline
#define INLINE2	__attribute__((__always_inline__,__nonnull__(1,2))) static inline

INLINE1 bool
INPUT_IS_RELATIVE (char const * const input_ptr)
{
  return ('/' != *input_ptr);
}

INLINE1 bool
INPUT_IS_ABSOLUTE (char const * const input_ptr)
{
  return ('/' == *input_ptr);
}

/* ------------------------------------------------------------------ */

INLINE2 bool
IS_STANDALONE_SLASH (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate to true if the string has 1 octet representing the
   pathname "/"; otherwise evaluate to false. */
{
  return ((end == (1+in)) && ('/' == in[0]));
}

/* ------------------------------------------------------------------ */

INLINE2 bool
IS_STANDALONE_SINGLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate to true if the string has 1 octet representing the
   pathname "."; otherwise evaluate to false. */
{
  return ((end == (1+in)) && ('.' == in[0]));
}

INLINE2 bool
IS_STANDALONE_SINGLE_DOT_SLASH (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 2  octets representing
   the pathname "./"; otherwise evaluate to false. */
{
  return ((end == (2+in)) && ('.' == in[0]) && ('/' == in[1]));
}

INLINE2 bool
IS_STANDALONE_SLASH_SINGLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 2  octets representing
   the pathname "/."; otherwise evaluate to false. */
{
  return ((end == (2+in)) && ('/' == in[0]) && ('.' == in[1]));
}

/* ------------------------------------------------------------------ */

INLINE2 bool
IS_STANDALONE_DOUBLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 2  octets representing
   the pathname ".."; otherwise evaluate to false. */
{
  return ((end == (2+in)) && ('.' == in[0]) && ('.' == in[1]));
}

INLINE2 bool
IS_STANDALONE_DOUBLE_DOT_SLASH (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 3  octets representing
   the pathname "../"; otherwise evaluate to false. */
{
  return ((end == (3+in)) && ('.' == in[0]) && ('.' == in[1]) && ('/' == in[2]));
}

INLINE2 bool
IS_STANDALONE_SLASH_DOUBLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true if  the string has 3  octets representing
   the pathname "/.."; otherwise evaluate to false. */
{
  return ((end == (3+in)) && ('/' == in[0]) && ('.' == in[1]) && ('.' == in[2]));
}

/* ------------------------------------------------------------------ */

INLINE2 bool
BEGINS_WITH_SINGLE_DOT_SLASH (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true  if the string  begins with  the pathname
   "./"; otherwise evaluate to false. */
{
  return ((end > (1+in)) && ('.' == in[0]) && ('/' == in[1]));
}

INLINE2 bool
BEGINS_WITH_SLASH_SINGLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true  if the string  begins with  the pathname
   "/.", but not "/.."; otherwise evaluate to false. */
{
  return ((end > (1 + in)) && ('/' == in[0]) && ('.' == in[1]) && ('.' != in[2]));
}

/* ------------------------------------------------------------------ */

INLINE2 bool
BEGINS_WITH_DOUBLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true  if the string  begins with  the pathname
   ".."; otherwise evaluate to false. */
{
  return ((end > (1+in)) && ('.' == in[0]) && ('.' == in[1]));
}

INLINE2 bool
BEGINS_WITH_SLASH_DOUBLE_DOT (char const * const in, char const * const end)
/* Given an ASCII string referenced by IN and terminating at pointer END
   excluded: evaluate  to true  if the string  begins with  the pathname
   "/.."; otherwise evaluate to false. */
{
  return ((end > (2+in)) && ('/' == in[0]) && ('.' == in[1]) && ('.' == in[2]));
}


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCPATHNAMES_INTERNALS_H */

/* end of file */
