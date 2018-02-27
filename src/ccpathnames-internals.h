/*
  Part of: CCPathnames
  Contents: private header file
  Date: Feb 26, 2018

  Abstract

	This  header  file is  for  internal  definitions.  It  must  be
	included by all the source files in this package.

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

/* We can customise this value at package configuration time using, for example:
 *
 *   $ ./configure CPPFLAGS='-DCCPTN_PATH_MAX=8192'
 *
 */
#ifndef CCPTN_PATH_MAX
#  define CCPTN_PATH_MAX	4096
#endif


/** --------------------------------------------------------------------
 ** Constants.
 ** ----------------------------------------------------------------- */




/** --------------------------------------------------------------------
 ** Function prototypes.
 ** ----------------------------------------------------------------- */



/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCPATHNAMES_INTERNALS_H */

/* end of file */
