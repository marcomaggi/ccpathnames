/*
  Part of: CCPathnames
  Contents: condition objects definitions
  Date: Feb 27, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you can  redistribute it and/or modify it under
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


/** --------------------------------------------------------------------
 ** Condition objects: invalid pathname.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_invalid_pathname;

static ccptn_descriptor_invalid_pathname_t ccptn_descriptor_invalid_pathname_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_invalid_pathname
};

ccptn_descriptor_invalid_pathname_t const * const ccptn_descriptor_invalid_pathname_ptr = &ccptn_descriptor_invalid_pathname_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_invalid_pathname_t const ccptn_condition_invalid_pathname_stru = {
  .runtime_error.error.root.condition.descriptor = &(ccptn_descriptor_invalid_pathname_stru.descriptor)
};

ccptn_condition_invalid_pathname_t const * const ccptn_condition_invalid_pathname_ptr = &ccptn_condition_invalid_pathname_stru;

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_invalid_pathname (cce_condition_t const * C CCPTN_UNUSED)
{
  return "invalid file system pathname";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_invalid_pathname (ccptn_condition_invalid_pathname_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccptn_condition_new_invalid_pathname (void)
{
  return (cce_condition_t const *) ccptn_condition_invalid_pathname_ptr;
}

bool
ccptn_condition_is_invalid_pathname (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccptn_descriptor_invalid_pathname_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: maximum pathname length exceeded.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_exceeded_length;

static ccptn_descriptor_exceeded_length_t ccptn_descriptor_exceeded_length_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_exceeded_length
};

ccptn_descriptor_exceeded_length_t const * const ccptn_descriptor_exceeded_length_ptr = &ccptn_descriptor_exceeded_length_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_exceeded_length_t const ccptn_condition_exceeded_length_stru = {
  .runtime_error.error.root.condition.descriptor = &(ccptn_descriptor_exceeded_length_stru.descriptor)
};

ccptn_condition_exceeded_length_t const * const ccptn_condition_exceeded_length_ptr = &ccptn_condition_exceeded_length_stru;

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_exceeded_length (cce_condition_t const * C CCPTN_UNUSED)
{
  return "the operation would exceed maximum pathname length";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_exceeded_length (ccptn_condition_exceeded_length_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccptn_condition_new_exceeded_length (void)
{
  return (cce_condition_t const *) ccptn_condition_exceeded_length_ptr;
}

bool
ccptn_condition_is_exceeded_length (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccptn_descriptor_exceeded_length_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: zero length.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_zero_length;

static ccptn_descriptor_zero_length_t ccptn_descriptor_zero_length_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_zero_length
};

ccptn_descriptor_zero_length_t const * const ccptn_descriptor_zero_length_ptr = &ccptn_descriptor_zero_length_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_zero_length_t const ccptn_condition_zero_length_stru = {
  .runtime_error.error.root.condition.descriptor = &(ccptn_descriptor_zero_length_stru.descriptor)
};

ccptn_condition_zero_length_t const * const ccptn_condition_zero_length_ptr = &ccptn_condition_zero_length_stru;

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_zero_length (cce_condition_t const * C CCPTN_UNUSED)
{
  return "the operation would generate a zero length pathname";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_zero_length (ccptn_condition_zero_length_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccptn_condition_new_zero_length (void)
{
  return (cce_condition_t const *) ccptn_condition_zero_length_ptr;
}

bool
ccptn_condition_is_zero_length (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccptn_descriptor_zero_length_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: invalid pathname.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccptn_condition_static_message_normalised_pathname;

static ccptn_descriptor_normalised_pathname_t ccptn_descriptor_normalised_pathname_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccptn_descriptor_invalid_pathname_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccptn_condition_static_message_normalised_pathname
};

ccptn_descriptor_normalised_pathname_t const * const ccptn_descriptor_normalised_pathname_ptr = &ccptn_descriptor_normalised_pathname_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccptn_condition_normalised_pathname_t const ccptn_condition_normalised_pathname_stru = {
  .runtime_error.error.root.condition.descriptor = &(ccptn_descriptor_normalised_pathname_stru.descriptor)
};

ccptn_condition_normalised_pathname_t const * const ccptn_condition_normalised_pathname_ptr = &ccptn_condition_normalised_pathname_stru;

/* ------------------------------------------------------------------ */

char const *
ccptn_condition_static_message_normalised_pathname (cce_condition_t const * C CCPTN_UNUSED)
{
  return "normalised file system pathname is required";
}

/* ------------------------------------------------------------------ */

void
ccptn_condition_init_normalised_pathname (ccptn_condition_normalised_pathname_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccptn_condition_new_normalised_pathname (void)
{
  return (cce_condition_t const *) ccptn_condition_normalised_pathname_ptr;
}

bool
ccptn_condition_is_normalised_pathname (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccptn_descriptor_normalised_pathname_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Module initialisation.
 ** ----------------------------------------------------------------- */

void
ccptn_init (void)
{
  ccptn_descriptor_invalid_pathname_stru.descriptor.parent	= &(cce_descriptor_runtime_error_ptr->descriptor);
  ccptn_descriptor_exceeded_length_stru.descriptor.parent	= &(cce_descriptor_runtime_error_ptr->descriptor);
}

/* end of file */
