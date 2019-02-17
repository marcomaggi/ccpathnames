/*
  Part of: CCPathnames
  Contents: version functions
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


char const *
ccptn_version_string (void)
{
  return ccpathnames_VERSION_INTERFACE_STRING;
}
int
ccptn_version_interface_current (void)
{
  return ccpathnames_VERSION_INTERFACE_CURRENT;
}
int
ccptn_version_interface_revision (void)
{
  return ccpathnames_VERSION_INTERFACE_REVISION;
}
int
ccptn_version_interface_age (void)
{
  return ccpathnames_VERSION_INTERFACE_AGE;
}

/* end of file */
