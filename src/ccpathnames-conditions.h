/*
  Part of: CCPathnames
  Contents: subordinate public header file, exceptional-condition objects
  Date: Apr 21, 2019

  Abstract

	This is a subordinate public header file of the package CCPathnames.  It must
	be included by "ccpathnames.h".

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

#ifndef CCPATHNAMES_CONDITIONS_H
#define CCPATHNAMES_CONDITIONS_H 1


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: invalid pathname.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_invalid_pathname_t	ccptn_descriptor_invalid_pathname_t;
typedef struct ccptn_condition_invalid_pathname_t	ccptn_condition_invalid_pathname_t;

struct ccptn_descriptor_invalid_pathname_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_invalid_pathname_t {
  cce_condition_runtime_error_t	runtime_error;
};

ccptn_decl void ccptn_condition_init_invalid_pathname (ccptn_condition_invalid_pathname_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_invalid_pathname (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_invalid_pathname (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: resulting pathname would exceed maximum length.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_exceeded_length_t	ccptn_descriptor_exceeded_length_t;
typedef struct ccptn_condition_exceeded_length_t	ccptn_condition_exceeded_length_t;

struct ccptn_descriptor_exceeded_length_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_exceeded_length_t {
  cce_condition_runtime_error_t	runtime_error;
};

ccptn_decl void ccptn_condition_init_exceeded_length (ccptn_condition_exceeded_length_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_exceeded_length (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_exceeded_length (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: resulting pathname would exceed maximum length.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_zero_length_t	ccptn_descriptor_zero_length_t;
typedef struct ccptn_condition_zero_length_t	ccptn_condition_zero_length_t;

struct ccptn_descriptor_zero_length_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_zero_length_t {
  cce_condition_runtime_error_t	runtime_error;
};

ccptn_decl void ccptn_condition_init_zero_length (ccptn_condition_zero_length_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_zero_length (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_zero_length (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: resulting pathname would exceed maximum length.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_invalid_length_t	ccptn_descriptor_invalid_length_t;
typedef struct ccptn_condition_invalid_length_t		ccptn_condition_invalid_length_t;

struct ccptn_descriptor_invalid_length_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_invalid_length_t {
  cce_condition_runtime_error_t	runtime_error;
};

ccptn_decl void ccptn_condition_init_invalid_length (ccptn_condition_invalid_length_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_invalid_length (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_invalid_length (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: invalid pathname.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_normalised_pathname_t	ccptn_descriptor_normalised_pathname_t;
typedef struct ccptn_condition_normalised_pathname_t	ccptn_condition_normalised_pathname_t;

struct ccptn_descriptor_normalised_pathname_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_normalised_pathname_t {
  ccptn_condition_invalid_pathname_t	invalid_pathname;
};

ccptn_decl void ccptn_condition_init_normalised_pathname (ccptn_condition_normalised_pathname_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_normalised_pathname (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_normalised_pathname (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: no dirname part when required.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_no_dirname_t	ccptn_descriptor_no_dirname_t;
typedef struct ccptn_condition_no_dirname_t	ccptn_condition_no_dirname_t;

struct ccptn_descriptor_no_dirname_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_no_dirname_t {
  ccptn_condition_invalid_pathname_t	invalid_pathname;
};

ccptn_decl void ccptn_condition_init_no_dirname (ccptn_condition_no_dirname_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_no_dirname (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_no_dirname (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: no filename part when required.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_no_filename_t	ccptn_descriptor_no_filename_t;
typedef struct ccptn_condition_no_filename_t	ccptn_condition_no_filename_t;

struct ccptn_descriptor_no_filename_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_no_filename_t {
  ccptn_condition_invalid_pathname_t	invalid_pathname;
};

ccptn_decl void ccptn_condition_init_no_filename (ccptn_condition_no_filename_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_no_filename (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_no_filename (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: no tailname part when required.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_no_tailname_t	ccptn_descriptor_no_tailname_t;
typedef struct ccptn_condition_no_tailname_t	ccptn_condition_no_tailname_t;

struct ccptn_descriptor_no_tailname_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_no_tailname_t {
  ccptn_condition_invalid_pathname_t	invalid_pathname;
};

ccptn_decl void ccptn_condition_init_no_tailname (ccptn_condition_no_tailname_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_no_tailname (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_no_tailname (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: no rootname part when required.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_no_rootname_t	ccptn_descriptor_no_rootname_t;
typedef struct ccptn_condition_no_rootname_t	ccptn_condition_no_rootname_t;

struct ccptn_descriptor_no_rootname_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_no_rootname_t {
  ccptn_condition_invalid_pathname_t	invalid_pathname;
};

ccptn_decl void ccptn_condition_init_no_rootname (ccptn_condition_no_rootname_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_no_rootname (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_no_rootname (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#endif /* CCPATHNAMES_CONDITIONS_H */

/* end of file */
