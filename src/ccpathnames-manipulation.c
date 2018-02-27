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
 ** Normalisation.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_REALPATH
ccptn_t *
ccptn_realpath (cce_destination_t L, ccptn_t const * const P)
{
  char		RP[PATH_MAX + 1];
  char const *	rv;

  errno = 0;
  rv = realpath(ccptn_asciiz(P), RP);
  if (NULL != rv) {
    ccptn_t *	Q = ccptn_new_dup_asciiz(L, RP);

    Q->realpath		= (int)1;
    Q->normalised	= (int)1;
    return Q;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}
#endif


/** --------------------------------------------------------------------
 ** Composition.
 ** ----------------------------------------------------------------- */

ccptn_t *
ccptn_append (cce_destination_t L, ccptn_t const * prefix, ccptn_t const * suffix)
{
  ccptn_t *	result;
  size_t	result_len;

  /* The resulting  length is the sum  of the original length,  plus one
     for the slash separator. */
  result_len	= ccptn_len(prefix) + ccptn_len(suffix) + 1;

  if (PATH_MAX < result_len) {
    cce_raise(L, ccptn_condition_new_invalid_pathname());
  } else {
    /* This array must hold the whole pathname plus the terminating zero
       octet. */
    char	result_pathname[result_len + 1];

    strncpy(result_pathname, ccptn_asciiz(prefix), ccptn_len(prefix));
    result_pathname[ccptn_len(prefix)] = '/';
    strncpy(result_pathname + ccptn_len(prefix) + 1, ccptn_asciiz(suffix), ccptn_len(suffix));
    result_pathname[result_len] = '\0';
    result	= ccptn_new_dup_asciiz(L, result_pathname);
  }
  return result;
}

/* end of file */
