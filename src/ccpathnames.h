/*
  Part of: CCPathnames
  Contents: public header file
  Date: Feb 25, 2018

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

#ifndef CCPATHNAMES_H
#define CCPATHNAMES_H 1


/** --------------------------------------------------------------------
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The  macro  CCPTN_UNUSED  indicates  that a  function,  function
   argument or variable may potentially be unused. Usage examples:

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

/* I found  the following chunk on  the Net.  (Marco Maggi;  Sun Feb 26,
   2012) */
#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define ccptn_decl		__attribute__((__dllexport__)) extern
#    else
#      define ccptn_decl		__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define ccptn_decl		__attribute__((__dllimport__)) extern
#    else
#      define ccptn_decl		__declspec(dllimport) extern
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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/** --------------------------------------------------------------------
 ** Constants and preprocessor macros.
 ** ----------------------------------------------------------------- */

#define CCPTN_PC(POINTER_TYPE, POINTER_NAME, EXPRESSION)	\
  POINTER_TYPE * POINTER_NAME = (POINTER_TYPE *) (EXPRESSION)


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ccptn_decl char const *	ccptn_version_string		(void);
ccptn_decl int		ccptn_version_interface_current	(void);
ccptn_decl int		ccptn_version_interface_revision(void);
ccptn_decl int		ccptn_version_interface_age	(void);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCPATHNAMES_H */

/* end of file */
