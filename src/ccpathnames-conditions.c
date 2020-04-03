/*
  Part of: CCPathnames
  Contents: exceptional-condition objects definitions
  Date: Feb 27, 2018

  Abstract



  Copyright (C) 2018, 2019, 2020 Marco Maggi <mrc.mgg@gmail.com>

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


/** --------------------------------------------------------------------
 ** Condition objects: invalid pathname.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_invalid_pathname;

static ccptn_descriptor_invalid_pathname_t ccptn_descriptor_invalid_pathname = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_invalid_pathname
};

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_invalid_pathname_t const ccptn_condition_invalid_pathname = {
  .runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccptn_descriptor_invalid_pathname)
};

void
cce_descriptor_set_parent_to(ccptn_descriptor_invalid_pathname_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccptn_descriptor_invalid_pathname);
}

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_invalid_pathname (cce_condition_t const * C CCPTN_UNUSED)
{
  return "invalid file system pathname";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_invalid_pathname (cce_destination_t L CCPTN_UNUSED, ccptn_condition_invalid_pathname_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccptn_condition_new_invalid_pathname (cce_destination_t L CCPTN_UNUSED)
{
  return (cce_condition_t const *) &ccptn_condition_invalid_pathname;
}

bool
ccptn_condition_is_invalid_pathname (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccptn_descriptor_invalid_pathname));
}


/** --------------------------------------------------------------------
 ** Condition objects: maximum pathname length exceeded.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_exceeded_length;

static ccptn_descriptor_exceeded_length_t ccptn_descriptor_exceeded_length = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_exceeded_length
};

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_exceeded_length_t const ccptn_condition_exceeded_length = {
  .runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccptn_descriptor_exceeded_length)
};

void
cce_descriptor_set_parent_to(ccptn_descriptor_exceeded_length_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccptn_descriptor_exceeded_length);
}

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_exceeded_length (cce_condition_t const * C CCPTN_UNUSED)
{
  return "the operation would exceed maximum pathname length";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_exceeded_length (cce_destination_t L CCPTN_UNUSED, ccptn_condition_exceeded_length_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccptn_condition_new_exceeded_length (cce_destination_t L CCPTN_UNUSED)
{
  return (cce_condition_t const *) &ccptn_condition_exceeded_length;
}

bool
ccptn_condition_is_exceeded_length (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccptn_descriptor_exceeded_length));
}


/** --------------------------------------------------------------------
 ** Condition objects: zero length.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_zero_length;

static ccptn_descriptor_zero_length_t ccptn_descriptor_zero_length = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_zero_length
};

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_zero_length_t const ccptn_condition_zero_length = {
  .runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccptn_descriptor_zero_length)
};

void
cce_descriptor_set_parent_to(ccptn_descriptor_zero_length_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccptn_descriptor_zero_length);
}

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_zero_length (cce_condition_t const * C CCPTN_UNUSED)
{
  return "the operation would generate a zero length pathname";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_zero_length (cce_destination_t L CCPTN_UNUSED, ccptn_condition_zero_length_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccptn_condition_new_zero_length (cce_destination_t L CCPTN_UNUSED)
{
  return (cce_condition_t const *) &ccptn_condition_zero_length;
}

bool
ccptn_condition_is_zero_length (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccptn_descriptor_zero_length));
}


/** --------------------------------------------------------------------
 ** Condition objects: invalid length.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_invalid_length;

static ccptn_descriptor_invalid_length_t ccptn_descriptor_invalid_length = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_invalid_length
};

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_invalid_length_t const ccptn_condition_invalid_length = {
  .runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccptn_descriptor_invalid_length)
};

void
cce_descriptor_set_parent_to(ccptn_descriptor_invalid_length_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccptn_descriptor_invalid_length);
}

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_invalid_length (cce_condition_t const * C CCPTN_UNUSED)
{
  return "the operation would generate an invalid length pathname";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_invalid_length (cce_destination_t L CCPTN_UNUSED, ccptn_condition_invalid_length_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccptn_condition_new_invalid_length (cce_destination_t L CCPTN_UNUSED)
{
  return (cce_condition_t const *) &ccptn_condition_invalid_length;
}

bool
ccptn_condition_is_invalid_length (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccptn_descriptor_invalid_length));
}


/** --------------------------------------------------------------------
 ** Condition objects: normalised pathname.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_normalised_pathname;

static ccptn_descriptor_normalised_pathname_t ccptn_descriptor_normalised_pathname = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccptn_descriptor_invalid_pathname),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_normalised_pathname
};

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_normalised_pathname_t const ccptn_condition_normalised_pathname = {
  .invalid_pathname.runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccptn_descriptor_normalised_pathname)
};

void
cce_descriptor_set_parent_to(ccptn_descriptor_normalised_pathname_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccptn_descriptor_normalised_pathname);
}

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_normalised_pathname (cce_condition_t const * C CCPTN_UNUSED)
{
  return "normalised file system pathname is required";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_normalised_pathname (cce_destination_t L CCPTN_UNUSED, ccptn_condition_normalised_pathname_t * C)
{
  ccptn_condition_init_invalid_pathname(L, &(C->invalid_pathname));
}

cce_condition_t const *
ccptn_condition_new_normalised_pathname (cce_destination_t L CCPTN_UNUSED)
{
  return (cce_condition_t const *) &ccptn_condition_normalised_pathname;
}

bool
ccptn_condition_is_normalised_pathname (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccptn_descriptor_normalised_pathname));
}


/** --------------------------------------------------------------------
 ** Condition objects: pathname with no dirname.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_no_dirname;

static ccptn_descriptor_no_dirname_t ccptn_descriptor_no_dirname = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccptn_descriptor_invalid_pathname),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_no_dirname
};

/* This  is  the  single  instance   of  the  "no  dirname"  exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_no_dirname_t const ccptn_condition_no_dirname = {
  .invalid_pathname.runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccptn_descriptor_no_dirname)
};

void
cce_descriptor_set_parent_to(ccptn_descriptor_no_dirname_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccptn_descriptor_no_dirname);
}

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_no_dirname (cce_condition_t const * C CCPTN_UNUSED)
{
  return "input pathname has no directory part";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_no_dirname (cce_destination_t L CCPTN_UNUSED, ccptn_condition_no_dirname_t * C)
{
  ccptn_condition_init_invalid_pathname(L, &(C->invalid_pathname));
}

cce_condition_t const *
ccptn_condition_new_no_dirname (cce_destination_t L CCPTN_UNUSED)
{
  return (cce_condition_t const *) &ccptn_condition_no_dirname;
}

bool
ccptn_condition_is_no_dirname (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccptn_descriptor_no_dirname));
}


/** --------------------------------------------------------------------
 ** Condition objects: pathname with no filename.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_no_filename;

static ccptn_descriptor_no_filename_t ccptn_descriptor_no_filename = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccptn_descriptor_invalid_pathname),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_no_filename
};

/* This  is  the  single  instance  of  the  "no  filename"  exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_no_filename_t const ccptn_condition_no_filename = {
  .invalid_pathname.runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccptn_descriptor_no_filename)
};

void
cce_descriptor_set_parent_to(ccptn_descriptor_no_filename_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccptn_descriptor_no_filename);
}

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_no_filename (cce_condition_t const * C CCPTN_UNUSED)
{
  return "input pathname has no filename part";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_no_filename (cce_destination_t L CCPTN_UNUSED, ccptn_condition_no_filename_t * C)
{
  ccptn_condition_init_invalid_pathname(L, &(C->invalid_pathname));
}

cce_condition_t const *
ccptn_condition_new_no_filename (cce_destination_t L CCPTN_UNUSED)
{
  return (cce_condition_t const *) &ccptn_condition_no_filename;
}

bool
ccptn_condition_is_no_filename (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccptn_descriptor_no_filename));
}


/** --------------------------------------------------------------------
 ** Condition objects: pathname with no tailname.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_no_tailname;

static ccptn_descriptor_no_tailname_t ccptn_descriptor_no_tailname = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccptn_descriptor_invalid_pathname),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_no_tailname
};

/* This  is  the  single  instance  of  the  "no  tailname"  exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_no_tailname_t const ccptn_condition_no_tailname = {
  .invalid_pathname.runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccptn_descriptor_no_tailname)
};

void
cce_descriptor_set_parent_to(ccptn_descriptor_no_tailname_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccptn_descriptor_no_tailname);
}

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_no_tailname (cce_condition_t const * C CCPTN_UNUSED)
{
  return "input pathname has no tailname part";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_no_tailname (cce_destination_t L CCPTN_UNUSED, ccptn_condition_no_tailname_t * C)
{
  ccptn_condition_init_invalid_pathname(L, &(C->invalid_pathname));
}

cce_condition_t const *
ccptn_condition_new_no_tailname (cce_destination_t L CCPTN_UNUSED)
{
  return (cce_condition_t const *) &ccptn_condition_no_tailname;
}

bool
ccptn_condition_is_no_tailname (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccptn_descriptor_no_tailname));
}


/** --------------------------------------------------------------------
 ** Condition objects: pathname with no rootname.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_no_rootname;

static ccptn_descriptor_no_rootname_t ccptn_descriptor_no_rootname = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccptn_descriptor_invalid_pathname),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_no_rootname
};

/* This  is  the  single  instance  of  the  "no  rootname"  exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_no_rootname_t const ccptn_condition_no_rootname = {
  .invalid_pathname.runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccptn_descriptor_no_rootname)
};

void
cce_descriptor_set_parent_to(ccptn_descriptor_no_rootname_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccptn_descriptor_no_rootname);
}

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_no_rootname (cce_condition_t const * C CCPTN_UNUSED)
{
  return "input pathname has no rootname part";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_no_rootname (cce_destination_t L CCPTN_UNUSED, ccptn_condition_no_rootname_t * C)
{
  ccptn_condition_init_invalid_pathname(L, &(C->invalid_pathname));
}

cce_condition_t const *
ccptn_condition_new_no_rootname (cce_destination_t L CCPTN_UNUSED)
{
  return (cce_condition_t const *) &ccptn_condition_no_rootname;
}

bool
ccptn_condition_is_no_rootname (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccptn_descriptor_no_rootname));
}


/** --------------------------------------------------------------------
 ** Module initialisation.
 ** ----------------------------------------------------------------- */

void
ccptn_library_init (void)
{
  cce_descriptor_set_parent_to(cce_descriptor_runtime_error_t)(cce_descriptor_pointer(ccptn_descriptor_invalid_pathname));
  cce_descriptor_set_parent_to(cce_descriptor_runtime_error_t)(cce_descriptor_pointer(ccptn_descriptor_exceeded_length));
  cce_descriptor_set_parent_to(cce_descriptor_runtime_error_t)(cce_descriptor_pointer(ccptn_descriptor_zero_length));
  cce_descriptor_set_parent_to(cce_descriptor_runtime_error_t)(cce_descriptor_pointer(ccptn_descriptor_invalid_length));
}

/* end of file */
