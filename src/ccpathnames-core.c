/*
  Part of: CCPathnames
  Contents: core functions for pathnames manipulation
  Date: Sun Feb 25, 2018

  Abstract

	This source file implements core functions for pathnames manipulation.

  Copyright (C) 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#undef CCPTN_DEBUGGING
#define CCPTN_DEBUGGING		0


/** --------------------------------------------------------------------
 ** Data struct "ccptn_t": methods table.
 ** ----------------------------------------------------------------- */

/* Type of trait  constructor functions.  Functions of this  type build instances
   of "ccstructs_dtor_T" as implemented by "ccptn_t". */
typedef ccstructs_dtor_T ccname_trait_new_type(ccstructs_dtor_T, ccptn_t) (ccptn_t const * self);

/* Function  prototype:   constructor  for   "ccstructs_dtor_T"  as   implemented  by
   "ccptn_t".  This variant destroys embedded instances. */
static ccname_trait_new_type(ccstructs_dtor_T, ccptn_t) ccname_trait_new(ccstructs_dtor_T, ccptn_t, embedded);

/* Function  prototype:   constructor  for   "ccstructs_dtor_T"  as   implemented  by
   "ccptn_t".  This variant destroys standalone instances. */
static ccname_trait_new_type(ccstructs_dtor_T, ccptn_t) ccname_trait_new(ccstructs_dtor_T, ccptn_t, standalone);

/* Table of methods for "ccptn_t". */
struct ccname_table_type(ccptn_t) {
  ccname_trait_new_type(ccstructs_dtor_T, ccptn_t) *	new_dtor;
};

/* Methods table for "ccptn_t": this variant is for embedded instances. */
static ccname_table_type(ccptn_t) const ccname_table(ccptn_t, embedded) = {
  .new_dtor	= ccname_trait_new(ccstructs_dtor_T, ccptn_t, embedded)
};

/* Methods table for "ccptn_t": this variant is for standalone instances. */
static ccname_table_type(ccptn_t) const ccname_table(ccptn_t, standalone) = {
  .new_dtor	= ccname_trait_new(ccstructs_dtor_T, ccptn_t, standalone)
};


/** --------------------------------------------------------------------
 ** Data struct "ccptn_t": well known API functions.
 ** ----------------------------------------------------------------- */

static void
ccname_final(ccptn_t) (ccptn_t const * P)
{
  if (P->dynamically_allocated_buffer) {
    ccmem_free(P->allocator, (void *)P->ptr);
  }
  if (CCPTN_DEBUGGING) { fprintf(stderr, "%-35s: finalised %p\n", __func__, (void *)P); }
}

static void
ccname_release(ccptn_t) (ccptn_t const * P)
{
  if (P->dynamically_allocated_base) {
    ccmem_free(P->allocator, (void *)P);
  }
  if (CCPTN_DEBUGGING) { fprintf(stderr, "%-35s: released %p\n", __func__, (void *)P); }
}

static void
ccname_delete(ccptn_t) (ccptn_t const * P)
{
  ccname_final(ccptn_t)(P);
  ccname_release(ccptn_t)(P);
  if (CCPTN_DEBUGGING) { fprintf(stderr, "%-35s: deleted %p\n", __func__, (void *)P); }
}


/** --------------------------------------------------------------------
 ** Helpers.
 ** ----------------------------------------------------------------- */

static void
scan_for_non_terminating_zeros (cce_destination_t L, char const * const ptr, size_t const len)
/* Scan for zero octets inside the pathname: raise an exception if one is found. */
{
  for (size_t i=0; i<len; ++i) {
    if ('\0' == ptr[i]) {
      cce_raise(L, ccptn_condition_new_invalid_length());
    }
  }
}


/** --------------------------------------------------------------------
 ** Constructor functions for embedded instances.
 ** ----------------------------------------------------------------- */

void
ccname_init(ccptn_t, ascii) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * const P, ccmem_ascii_t const input_rep)
/* Initialise  an  already  allocated  instance  with  data  from  the  ASCII  struct
   "input_rep".  We  cannot assume that  the input  string is zero-terminated,  so we
   duplicate the data to add a terminating zero. */
{
  if (CCPTN_PATH_MAX < input_rep.len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < input_rep.len) {
    scan_for_non_terminating_zeros(L, input_rep.ptr, input_rep.len);
    P->methods				= &ccname_table(ccptn_t, embedded);
    P->allocator			= A;
    P->len				= input_rep.len;
    P->dynamically_allocated_base	= 0;
    P->dynamically_allocated_buffer	= 1;
    P->absolute				= ('/' == input_rep.ptr[0])? 1 : 0;
    P->normalised			= 0;
    P->realpath				= 0;
    P->ptr				= ccmem_malloc(L, A, 1+input_rep.len);
    memcpy((void *)(P->ptr), input_rep.ptr, input_rep.len);
    ((char *)(P->ptr))[input_rep.len] = '\0';
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, asciiz) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * const P, ccmem_asciiz_t const input_rep)
/* Initialise  an  already  allocated  instance  with data  from  the  ASCIIZ  struct
   "input_rep".  The  data is *not*  duplicated: the instance  references "input_rep"
   itself. */
{
  if (CCPTN_PATH_MAX < input_rep.len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < input_rep.len) {
    P->methods				= &ccname_table(ccptn_t, embedded);
    P->allocator			= A;
    P->len				= input_rep.len;
    P->dynamically_allocated_base	= 0;
    P->dynamically_allocated_buffer	= 0;
    P->absolute				= ('/' == input_rep.ptr[0])? 1 : 0;
    P->normalised			= 0;
    P->realpath				= 0;
    P->ptr				= input_rep.ptr;
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}

void
ccname_init(ccptn_t, asciiz, dup) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * const P, ccmem_asciiz_t input_rep)
/* Initialise  an  already  allocated  instance  with data  from  the  ASCIIZ  struct
   "input_rep".   The  data  is   duplicated:  the  instance  references  "input_rep"
   itself. */
{
  /* The constructor for the ASCII block duplicates the input. */
  ccname_init(ccptn_t, ascii)(L, A, P, ccmem_new_ascii_from_asciiz(input_rep));
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, pointer) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * const P, char const * const input_rep)
/* Initialise  an  already  allocated  instance  with data  from  the  ASCIIZ  string
   "input_rep".  The  data is *not*  duplicated: the instance  references "input_rep"
   itself. */
{
  /* This constructor for the ASCIIZ block does not duplicate the input. */
  ccname_init(ccptn_t, asciiz)(L, A, P, ccmem_new_asciiz_from_str((char *)input_rep));
}

void
ccname_init(ccptn_t, pointer, dup) (cce_destination_t L, ccmem_allocator_t const * const A, ccptn_t * const P, char const * const input_rep)
/* Initialise  an  already  allocated  instance  with data  from  the  ASCIIZ  string
   "input_rep".  The  data is *not*  duplicated: the instance  references "input_rep"
   itself. */
{
  /* The constructor for the ASCII block duplicates the input. */
  ccname_init(ccptn_t, ascii)(L, A, P, ccmem_new_ascii_from_str((char *)input_rep));
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, deserialisable) (cce_destination_t L CCPTN_UNUSED, ccmem_allocator_t const * A, ccptn_t * P)
/* Initialise an already allocated  instance which is meant to be  used as target for
   deserialisation from an instance of "ccstructs_deserialiser_T". */
{
  P->methods				= &ccname_table(ccptn_t, embedded);
  P->allocator				= A;
  P->dynamically_allocated_base		= 0;
  P->dynamically_allocated_buffer	= 0;
  P->absolute				= 0;
  P->normalised				= 0;
  P->realpath				= 0;
  P->len				= 0;
  P->ptr				= NULL;
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, clone) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
{
  dst->methods				= &ccname_table(ccptn_t, embedded);
  dst->allocator			= A;
  dst->dynamically_allocated_base	= 0;
  dst->dynamically_allocated_buffer	= 1;
  dst->absolute				= src->absolute;
  dst->normalised			= src->normalised;
  dst->realpath				= src->realpath;
  dst->len				= src->len;
  dst->ptr = ccmem_malloc(L, A, (1 + src->len));
  memcpy((void *)(dst->ptr), src->ptr, (1 + src->len));
}


/** --------------------------------------------------------------------
 ** Constructor functions for standalone instances.
 ** ----------------------------------------------------------------- */

ccptn_t const *
ccname_new(ccptn_t, ascii) (cce_destination_t L, ccmem_allocator_t const * const A, ccmem_ascii_t const input_rep)
/* Build a newly allocated instance with  data from the ASCII struct "input_rep".  We
   cannot assume that  the input string is zero-terminated, so  we duplicate the data
   to add a terminating zero. */
{
  if (CCPTN_PATH_MAX < input_rep.len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < input_rep.len) {
    /* We store both the base struct and the buffer in the same dynamically allocated
       memory block. */
    ccptn_t *	P = ccmem_malloc(L, A, sizeof(ccptn_t) + input_rep.len + 1);

    scan_for_non_terminating_zeros(L, input_rep.ptr, input_rep.len);
    P->methods				= &ccname_table(ccptn_t, standalone);
    P->allocator			= A;
    P->len				= input_rep.len;
    P->dynamically_allocated_base	= 1;
    P->dynamically_allocated_buffer	= 0;
    P->absolute				= ('/' == input_rep.ptr[0])? 1 : 0;
    P->normalised			= 0;
    P->realpath				= 0;
    P->ptr				= (char *)(((uint8_t *)P) + sizeof(ccptn_t));
    memcpy((void *)(P->ptr), input_rep.ptr, input_rep.len);
    ((char *)(P->ptr))[input_rep.len] = '\0';
    return P;
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, asciiz) (cce_destination_t L, ccmem_allocator_t const * const A, ccmem_asciiz_t const input_rep)
/* Build a  newly allocated instance  with data  from the ASCIIZ  struct "input_rep".
   The data is *not* duplicated: the instance references "input_rep" itself. */
{
  if (CCPTN_PATH_MAX < input_rep.len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < input_rep.len) {
    ccptn_t *	P = ccmem_malloc(L, A, sizeof(ccptn_t));

    P->methods				= &ccname_table(ccptn_t, standalone);
    P->allocator			= A;
    P->len				= input_rep.len;
    P->dynamically_allocated_base	= 1;
    P->dynamically_allocated_buffer	= 0;
    P->absolute				= ('/' == input_rep.ptr[0])? 1 : 0;
    P->normalised			= 0;
    P->realpath				= 0;
    P->ptr				= input_rep.ptr;
    return P;
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
}

ccptn_t const *
ccname_new(ccptn_t, asciiz, dup) (cce_destination_t L, ccmem_allocator_t const * const A, ccmem_asciiz_t input_rep)
/* Build a  newly allocated instance  with data  from the ASCIIZ  struct "input_rep".
   The data is duplicated: the instance references "input_rep" itself. */
{
  /* The constructor for the ASCII block duplicates the input. */
  return ccname_new(ccptn_t, ascii)(L, A, ccmem_new_ascii_from_asciiz(input_rep));
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, pointer) (cce_destination_t L, ccmem_allocator_t const * const A, char const * const input_rep)
/* Build a  newly allocated instance  with data  from the ASCIIZ  string "input_rep".
   The data is *not* duplicated: the instance references "input_rep" itself. */
{
  /* This constructor for the ASCIIZ block does not duplicate the input. */
  return ccname_new(ccptn_t, asciiz)(L, A, ccmem_new_asciiz_from_str((char *)input_rep));
}

ccptn_t const *
ccname_new(ccptn_t, pointer, dup) (cce_destination_t L, ccmem_allocator_t const * const A, char const * const input_rep)
/* Build a  newly allocated instance  with data  from the ASCIIZ  string "input_rep".
   The data is *not* duplicated: the instance references "input_rep" itself. */
{
  /* The constructor for the ASCII block duplicates the input. */
  return ccname_new(ccptn_t, ascii)(L, A, ccmem_new_ascii_from_str((char *)input_rep));
}

/* ------------------------------------------------------------------ */

ccptn_t *
ccname_new(ccptn_t, deserialisable) (cce_destination_t L, ccmem_allocator_t const * A)
/* Build  a  newly allocated  instance  which  is meant  to  be  used as  target  for
   deserialisation from an instance of "ccstructs_deserialiser_T". */
{
  ccptn_t *	P = ccmem_malloc(L, A, sizeof(ccptn_t));

  P->methods				= &ccname_table(ccptn_t, standalone);
  P->allocator				= A;
  P->dynamically_allocated_base		= 1;
  P->dynamically_allocated_buffer	= 0;
  P->absolute				= 0;
  P->normalised				= 0;
  P->realpath				= 0;
  P->len				= 0;
  P->ptr				= NULL;
  return P;
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, clone) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
{
  /* We store both the  base struct and the buffer in  the same dynamically allocated
     memory block. */
  ccptn_t *	dst = ccmem_malloc(L, A, sizeof(ccptn_t) + ccptn_len(src) + 1);

  dst->methods				= &ccname_table(ccptn_t, standalone);
  dst->allocator			= A;
  dst->dynamically_allocated_base	= 1;
  dst->dynamically_allocated_buffer	= 0;
  dst->absolute				= src->absolute;
  dst->normalised			= src->normalised;
  dst->realpath				= src->realpath;
  dst->len				= src->len;
  dst->ptr				= (char *)(((uint8_t *)dst) + sizeof(ccptn_t));
  memcpy((void *)(dst->ptr), src->ptr, (1 + src->len));
  return dst;
}


/** --------------------------------------------------------------------
 ** Trait "ccstructs_dtor_T": implementation for embedded "ccptn_t".
 ** ----------------------------------------------------------------- */

static ccstructs_core_destructor_fun_t	ccptn_embedded_destructor;

ccstructs_dtor_T
ccname_trait_new(ccstructs_dtor_T, ccptn_t, embedded) (ccptn_t const * const self)
{
  return ccname_new(ccstructs_dtor_T)(ccstructs_core(self), ccptn_embedded_destructor);
}

/* ------------------------------------------------------------------ */

void
ccptn_embedded_destructor (ccstructs_core_t * S)
{
  CCSTRUCTS_PC(ccptn_t, self, S);

  ccname_final(ccptn_t)(self);
  if (CCPTN_DEBUGGING) { fprintf(stderr, "%-35s: finalised by dtor\n", __func__); }
}


/** --------------------------------------------------------------------
 ** Trait "ccstructs_dtor_T": implementation for standalone "ccptn_t".
 ** ----------------------------------------------------------------- */

static ccstructs_core_destructor_fun_t	ccptn_standalone_destructor;

ccstructs_dtor_T
ccname_trait_new(ccstructs_dtor_T, ccptn_t, standalone) (ccptn_t const * const self)
{
  return ccname_new(ccstructs_dtor_T)(ccstructs_core(self), ccptn_standalone_destructor);
}

/* ------------------------------------------------------------------ */

void
ccptn_standalone_destructor (ccstructs_core_t * S)
{
  CCSTRUCTS_PC(ccptn_t, self, S);

  ccname_delete(ccptn_t)(self);
  if (CCPTN_DEBUGGING) { fprintf(stderr, "%-35s: deleted by dtor\n", __func__); }
}


/** --------------------------------------------------------------------
 ** Trait "ccstructs_dtor_T": public constructor for "ccptn_t".
 ** ----------------------------------------------------------------- */

ccstructs_dtor_T
ccname_trait_new(ccstructs_dtor_T, ccptn_t) (ccptn_t const * const self)
/* Trait constructor for "ccstructs_dtor_T" implemented by "ccptn_t". */
{
  return self->methods->new_dtor(self);
}


/** --------------------------------------------------------------------
 ** Trait "ccstructs_dumpable_T": implementation for "ccptn_t".
 ** ----------------------------------------------------------------- */

static ccname_trait_method_type(ccstructs_dumpable_T, dump)  ccname_trait_method(ccstructs_dumpable_T, ccptn_t, dump);

static ccname_trait_table_type(ccstructs_dumpable_T) const ccname_trait_table(ccstructs_dumpable_T, ccptn_t) = {
  .dump	= ccname_trait_method(ccstructs_dumpable_T, ccptn_t, dump)
};

ccstructs_dumpable_T
ccname_trait_new(ccstructs_dumpable_T, ccptn_t) (ccptn_t const * S)
{
  return ccname_new(ccstructs_dumpable_T)(ccstructs_core(S), &ccname_trait_table(ccstructs_dumpable_T, ccptn_t));
}

void
ccname_trait_method(ccstructs_dumpable_T, ccptn_t, dump) (cce_destination_t L, ccstructs_dumpable_T I)
{
  CCSTRUCTS_PC(ccptn_t, P, ccstructs_dumpable_self(I));
  int	rv;

  errno = 0;
  rv = fwrite(P->ptr, sizeof(char), ccptn_len(P), stderr);
  if (-1 == rv) {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}


/** --------------------------------------------------------------------
 ** Trait "ccstructs_serialiser_T": implementation for "ccptn_t".
 ** ----------------------------------------------------------------- */

static ccname_trait_method_type(ccstructs_serialiser_T, required_size)
  ccname_trait_method(ccstructs_serialiser_T, ccptn_t, required_size);
static ccname_trait_method_type(ccstructs_serialiser_T, write)
  ccname_trait_method(ccstructs_serialiser_T, ccptn_t, write);

/* Trait  table  of  methods.   Implementation  of  "ccstructs_serialiser_T"  for
   "ccptn_t". */
static ccname_trait_table_type(ccstructs_serialiser_T) const ccname_trait_table(ccstructs_serialiser_T, ccptn_t) = {
  .required_size = ccname_trait_method(ccstructs_serialiser_T, ccptn_t, required_size),
  .write         = ccname_trait_method(ccstructs_serialiser_T, ccptn_t, write),
};

ccstructs_serialiser_T
ccname_trait_new(ccstructs_serialiser_T, ccptn_t) (ccptn_t const * S)
/* Trait  constructor.    Build  an   instance  of   "ccstructs_serialiser_T"  as
   implemented by "ccptn_t". */
{
  return ccname_new(ccstructs_serialiser_T)(ccstructs_core(S), &ccname_trait_table(ccstructs_serialiser_T, ccptn_t));
}

/* ------------------------------------------------------------------ */

size_t
ccname_trait_method(ccstructs_serialiser_T, ccptn_t, required_size) (ccstructs_serialiser_T I)
/* Return the minimum number of bytes  required to hold the serialised representation
   of "ccptn_t".*/
{
  CCSTRUCTS_PC(ccptn_t const, P, ccstructs_serialiser_self(I));

  return (1 + ccptn_len(P));
}

ccmem_block_t
ccname_trait_method(ccstructs_serialiser_T, ccptn_t,
		    write) (cce_destination_t L CCPTN_UNUSED, ccstructs_serialiser_T I, ccmem_block_t B)
/* Trait method implementation.  Serialise an instance of "ccptn_t" in the memory
   block "B". */
{
  CCSTRUCTS_PC(ccptn_t const, P, ccstructs_serialiser_self(I));
  ccmem_block_t	N = {
    .ptr	= B.ptr + (1 + ccptn_len(P)),
    .len	= B.len - (1 + ccptn_len(P))
  };

  if (0) { fprintf(stderr, "%s: writing %lu bytes\n", __func__, (1 + ccptn_len(P))); }
  memcpy(B.ptr, ccptn_ptr(P), (1 + ccptn_len(P)));
  return N;
}


/** --------------------------------------------------------------------
 ** Trait "ccstructs_deserialiser_T": implementation for "ccptn_t".
 ** ----------------------------------------------------------------- */

static ccname_trait_method_type(ccstructs_deserialiser_T, required_size)
  ccname_trait_method(ccstructs_deserialiser_T, ccptn_t, required_size);
static ccname_trait_method_type(ccstructs_deserialiser_T, read)
  ccname_trait_method(ccstructs_deserialiser_T, ccptn_t, read);

/* Trait  table of  methods.   Implementation  of "ccstructs_deserialiser_T"  for
   "ccptn_t". */
static ccname_trait_table_type(ccstructs_deserialiser_T) const ccname_trait_table(ccstructs_deserialiser_T, ccptn_t) = {
  .required_size = ccname_trait_method(ccstructs_deserialiser_T, ccptn_t, required_size),
  .read          = ccname_trait_method(ccstructs_deserialiser_T, ccptn_t, read)
};

ccstructs_deserialiser_T
ccname_trait_new(ccstructs_deserialiser_T, ccptn_t) (ccptn_t * S)
/* Trait  constructor.   Build  an   instance  of  "ccstructs_deserialiser_T"  as
   implemented by "ccptn_t". */
{
  return ccname_new(ccstructs_deserialiser_T)(ccstructs_core(S), &ccname_trait_table(ccstructs_deserialiser_T, ccptn_t));
}

/* ------------------------------------------------------------------ */

size_t
ccname_trait_method(ccstructs_deserialiser_T, ccptn_t, required_size) (ccstructs_deserialiser_T I CCPTN_UNUSED)
/* Return the minimum number of bytes  required to hold the serialised representation
   of "ccptn_t".*/
{
  return (1 + CCPTN_PATH_MAX);
}

ccmem_block_t
ccname_trait_method(ccstructs_deserialiser_T, ccptn_t,
		    read) (cce_destination_t L CCSTRUCTS_UNUSED, ccstructs_deserialiser_T I, ccmem_block_t B)
/* Trait method implementation.  Dedeserialise an  instance of "ccptn_t" from the
   memory block "B". */
{
  CCSTRUCTS_PC(ccptn_t, P, ccstructs_deserialiser_self(I));
  size_t	len = ((uint8_t *)strchr((char *)B.ptr, 0)) - B.ptr;
  ccmem_block_t	N = {
    .ptr	= B.ptr + (1 + len),
    .len	= B.len - (1 + len)
  };
  ccmem_ascii_t	P_block = {
    .ptr	= (char *)B.ptr,
    .len	= len
  };

  if (0) { fprintf(stderr, "%s: len=%lu\n", __func__, len); }

  if (CCPTN_PATH_MAX < P_block.len) {
    cce_raise(L, ccptn_condition_new_exceeded_length());
  } else if (0 < P_block.len) {
    scan_for_non_terminating_zeros(L, P_block.ptr, P_block.len);
    P->len				= P_block.len;
    P->dynamically_allocated_buffer	= 1;
    P->absolute				= ('/' == P_block.ptr[0])? 1 : 0;
    P->ptr				= ccmem_malloc(L, P->allocator, 1+P_block.len);
    memcpy((void *)(P->ptr), P_block.ptr, P_block.len);
    ((char *)(P->ptr))[P_block.len] = '\0';
  } else {
    cce_raise(L, ccptn_condition_new_zero_length());
  }
  return N;
}


/** --------------------------------------------------------------------
 ** Trait "ccstructs_pathname_T": implementation for "ccptn_t".
 ** ----------------------------------------------------------------- */

static ccname_trait_method_type(ccstructs_pathname_T, asciiz)        ccname_trait_method(ccstructs_pathname_T, ccptn_t, asciiz);
static ccname_trait_method_type(ccstructs_pathname_T, is_static)     ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_static);
static ccname_trait_method_type(ccstructs_pathname_T, is_absolute)   ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_absolute);
static ccname_trait_method_type(ccstructs_pathname_T, is_relative)   ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_relative);
static ccname_trait_method_type(ccstructs_pathname_T, is_normalised) ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_normalised);
static ccname_trait_method_type(ccstructs_pathname_T, is_realpath)   ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_realpath);

static ccname_trait_method_type(ccstructs_pathname_T, dtor)         ccname_trait_method(ccstructs_pathname_T, ccptn_t, dtor);
static ccname_trait_method_type(ccstructs_pathname_T, dumpable)     ccname_trait_method(ccstructs_pathname_T, ccptn_t, dumpable);
static ccname_trait_method_type(ccstructs_pathname_T, serialiser)   ccname_trait_method(ccstructs_pathname_T, ccptn_t, serialiser);
static ccname_trait_method_type(ccstructs_pathname_T, deserialiser) ccname_trait_method(ccstructs_pathname_T, ccptn_t, deserialiser);

/* Table  of methods  for  trait.  Implementation  of "ccstructs_pathname_T"  for
   "ccptn_t". */
static ccname_trait_table_type(ccstructs_pathname_T) const ccname_trait_table(ccstructs_pathname_T, ccptn_t) = {
  .asciiz	= ccname_trait_method(ccstructs_pathname_T, ccptn_t, asciiz),
  .is_static	= ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_static),
  .is_absolute	= ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_absolute),
  .is_relative	= ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_relative),
  .is_normalised= ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_normalised),
  .is_realpath	= ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_realpath),
  .dtor		= ccname_trait_method(ccstructs_pathname_T, ccptn_t, dtor),
  .dumpable	= ccname_trait_method(ccstructs_pathname_T, ccptn_t, dumpable),
  .serialiser	= ccname_trait_method(ccstructs_pathname_T, ccptn_t, serialiser),
  .deserialiser	= ccname_trait_method(ccstructs_pathname_T, ccptn_t, deserialiser)
};

ccstructs_pathname_T
ccname_trait_new(ccstructs_pathname_T, ccptn_t) (ccptn_t const * self)
{
  return ccname_new(ccstructs_pathname_T)(ccstructs_core(self), &ccname_trait_table(ccstructs_pathname_T, ccptn_t));
}

/* ------------------------------------------------------------------ */

static ccmem_asciiz_t
ccname_trait_method(ccstructs_pathname_T, ccptn_t, asciiz) (cce_destination_t L CCPTN_UNUSED, ccstructs_pathname_T I)
{
  CCPTN_PC(ccptn_t const, P, ccstructs_pathname_self(I));
  return ccptn_asciiz(P);
}

static bool
ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_static) (ccstructs_pathname_T I CCPTN_UNUSED)
{
  return false;
}

static bool
ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_absolute)   (cce_destination_t L CCPTN_UNUSED, ccstructs_pathname_T I)
{
  CCPTN_PC(ccptn_t const, P, ccstructs_pathname_self(I));
  return ccptn_is_absolute(P);
}

static bool
ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_relative) (cce_destination_t L CCPTN_UNUSED, ccstructs_pathname_T I)
{
  CCPTN_PC(ccptn_t const, P, ccstructs_pathname_self(I));
  return ccptn_is_relative(P);
}

static bool
ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_normalised) (cce_destination_t L CCPTN_UNUSED, ccstructs_pathname_T I)
{
  CCPTN_PC(ccptn_t const, P, ccstructs_pathname_self(I));
  return ccptn_is_normalised(P);
}

static bool
ccname_trait_method(ccstructs_pathname_T, ccptn_t, is_realpath) (cce_destination_t L CCPTN_UNUSED, ccstructs_pathname_T I)
{
  CCPTN_PC(ccptn_t const, P, ccstructs_pathname_self(I));
  return ccptn_is_realpath(P);
}

static ccstructs_dtor_T
ccname_trait_method(ccstructs_pathname_T, ccptn_t, dtor) (ccstructs_pathname_T I)
{
  CCPTN_PC(ccptn_t const, P, ccstructs_pathname_self(I));
  return ccname_trait_new(ccstructs_dtor_T, ccptn_t)(P);
}

static ccstructs_dumpable_T
ccname_trait_method(ccstructs_pathname_T, ccptn_t, dumpable) (cce_destination_t L CCPTN_UNUSED, ccstructs_pathname_T I)
{
  CCPTN_PC(ccptn_t const, P, ccstructs_pathname_self(I));
  return ccname_trait_new(ccstructs_dumpable_T, ccptn_t)(P);
}

static ccstructs_serialiser_T
ccname_trait_method(ccstructs_pathname_T, ccptn_t, serialiser) (cce_destination_t L CCPTN_UNUSED, ccstructs_pathname_T I)
{
  CCPTN_PC(ccptn_t const, P, ccstructs_pathname_self(I));
  return ccname_trait_new(ccstructs_serialiser_T, ccptn_t)(P);
}

static ccstructs_deserialiser_T
ccname_trait_method(ccstructs_pathname_T, ccptn_t, deserialiser) (cce_destination_t L CCPTN_UNUSED, ccstructs_pathname_T I)
{
  CCPTN_PC(ccptn_t, P, ccstructs_pathname_self(I));
  return ccname_trait_new(ccstructs_deserialiser_T, ccptn_t)(P);
}


/** --------------------------------------------------------------------
 ** Pathnames: exception handlers.
 ** ----------------------------------------------------------------- */

void
ccptn_init_and_register_clean_handler (cce_destination_t L, ccptn_clean_handler_t * H, ccptn_t const * P)
{
  ccstructs_init_and_register_handler(L, ccptn_handler_handler(H), ccname_trait_new(ccstructs_dtor_T, ccptn_t)(P));
}

 void
ccptn_init_and_register_error_handler (cce_destination_t L, ccptn_error_handler_t * H, ccptn_t const * P)
{
  ccstructs_init_and_register_handler(L, ccptn_handler_handler(H), ccname_trait_new(ccstructs_dtor_T, ccptn_t)(P));
}


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: embedded instances, pointer input.
 ** ----------------------------------------------------------------- */

void
ccname_init(ccptn_t, pointer, clean) (cce_destination_t L, ccmem_allocator_t const * A,
				      ccptn_clean_handler_t * H, ccptn_t * P,
				      char const * input_rep)
{
  ccname_init(ccptn_t, pointer)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}

void
ccname_init(ccptn_t, pointer, error) (cce_destination_t L, ccmem_allocator_t const * A,
				      ccptn_error_handler_t * H, ccptn_t * P,
				      char const * input_rep)
{
  ccname_init(ccptn_t, pointer)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, pointer, dup, clean) (cce_destination_t L, ccmem_allocator_t const * A,
					  ccptn_clean_handler_t * H, ccptn_t * P,
					  char const * input_rep)
{
  ccname_init(ccptn_t, pointer)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}

void
ccname_init(ccptn_t, pointer, dup, error) (cce_destination_t L, ccmem_allocator_t const * A,
					  ccptn_error_handler_t * H, ccptn_t * P,
					  char const * input_rep)
{
  ccname_init(ccptn_t, pointer)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: embedded instances, ASCIIZ input.
 ** ----------------------------------------------------------------- */

void
ccname_init(ccptn_t, asciiz, clean) (cce_destination_t L, ccmem_allocator_t const * A,
				     ccptn_clean_handler_t * H, ccptn_t * P,
				     ccmem_asciiz_t input_rep)
{
  ccname_init(ccptn_t, asciiz)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}

void
ccname_init(ccptn_t, asciiz, error) (cce_destination_t L, ccmem_allocator_t const * A,
				     ccptn_error_handler_t * H, ccptn_t * P,
				     ccmem_asciiz_t input_rep)
{
  ccname_init(ccptn_t, asciiz)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}

/* ------------------------------------------------------------------ */

void
ccname_init(ccptn_t, asciiz, dup, clean) (cce_destination_t L, ccmem_allocator_t const * A,
					 ccptn_clean_handler_t * H, ccptn_t * P,
					 ccmem_asciiz_t input_rep)
{
  ccname_init(ccptn_t, asciiz)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}

void
ccname_init(ccptn_t, asciiz, dup, error) (cce_destination_t L, ccmem_allocator_t const * A,
					 ccptn_error_handler_t * H, ccptn_t * P,
					 ccmem_asciiz_t input_rep)
{
  ccname_init(ccptn_t, asciiz)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: embedded instances, ASCII input.
 ** ----------------------------------------------------------------- */

void
ccname_init(ccptn_t, ascii, clean) (cce_destination_t L, ccmem_allocator_t const * A,
				    ccptn_clean_handler_t * H, ccptn_t * P,
				    ccmem_ascii_t input_rep)
{
  ccname_init(ccptn_t, ascii)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}

void
ccname_init(ccptn_t, ascii, error) (cce_destination_t L, ccmem_allocator_t const * A,
				    ccptn_error_handler_t * H, ccptn_t * P,
				    ccmem_ascii_t input_rep)
{
  ccname_init(ccptn_t, ascii)(L, A, P, input_rep);
  ccptn_init_and_register_handler(L, H, P);
}


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: embedded instances, deserialisable.
 ** ----------------------------------------------------------------- */

void
ccname_init(ccptn_t, deserialisable, clean) (cce_destination_t L, ccmem_allocator_t const * A,
					     ccptn_clean_handler_t * H, ccptn_t * P)
{
  ccname_init(ccptn_t, deserialisable)(L, A, P);
  ccptn_init_and_register_handler(L, H, P);
}

void
ccname_init(ccptn_t, deserialisable, error) (cce_destination_t L, ccmem_allocator_t const * A,
					     ccptn_error_handler_t * H, ccptn_t * P)
{
  ccname_init(ccptn_t, deserialisable)(L, A, P);
  ccptn_init_and_register_handler(L, H, P);
}


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: embedded instances, clone.
 ** ----------------------------------------------------------------- */

void
ccname_init(ccptn_t, clone, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H,
				    ccptn_t * dst, ccptn_t const * src)
{
  ccname_init(ccptn_t, clone)(L, A, dst, src);
  ccptn_init_and_register_handler(L, H, dst);
}

void
ccname_init(ccptn_t, clone, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H,
				    ccptn_t * dst, ccptn_t const * src)
{
  ccname_init(ccptn_t, clone)(L, A, dst, src);
  ccptn_init_and_register_handler(L, H, dst);
}


/** --------------------------------------------------------------------
n ** Pathnames guarded constructors: standalone instances, pointer input.
 ** ----------------------------------------------------------------- */

ccptn_t const *
ccname_new(ccptn_t, pointer, clean) (cce_destination_t L, ccmem_allocator_t const * A,
				     ccptn_clean_handler_t * H, char const * input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, pointer)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}

ccptn_t const *
ccname_new(ccptn_t, pointer, error) (cce_destination_t L, ccmem_allocator_t const * A,
				     ccptn_error_handler_t * H, char const * input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, pointer)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, pointer, dup, clean) (cce_destination_t L, ccmem_allocator_t const * A,
					 ccptn_clean_handler_t * H, char const * input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, pointer)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}

ccptn_t const *
ccname_new(ccptn_t, pointer, dup, error) (cce_destination_t L, ccmem_allocator_t const * A,
					 ccptn_error_handler_t * H, char const * input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, pointer)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: standalone instances, ASCIIZ input.
 ** ----------------------------------------------------------------- */

ccptn_t const *
ccname_new(ccptn_t, asciiz, clean) (cce_destination_t L, ccmem_allocator_t const * A,
				    ccptn_clean_handler_t * H, ccmem_asciiz_t input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, asciiz)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}

ccptn_t const *
ccname_new(ccptn_t, asciiz, error) (cce_destination_t L, ccmem_allocator_t const * A,
				    ccptn_error_handler_t * H, ccmem_asciiz_t input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, asciiz)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}

/* ------------------------------------------------------------------ */

ccptn_t const *
ccname_new(ccptn_t, asciiz, dup, clean) (cce_destination_t L, ccmem_allocator_t const * A,
					ccptn_clean_handler_t * H, ccmem_asciiz_t input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, asciiz)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}

ccptn_t const *
ccname_new(ccptn_t, asciiz, dup, error) (cce_destination_t L, ccmem_allocator_t const * A,
					ccptn_error_handler_t * H, ccmem_asciiz_t input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, asciiz)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: standalone instances, ASCII input.
 ** ----------------------------------------------------------------- */

ccptn_t const *
ccname_new(ccptn_t, ascii, clean) (cce_destination_t L, ccmem_allocator_t const * A,
				   ccptn_clean_handler_t * H, ccmem_ascii_t input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, ascii)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}

ccptn_t const *
ccname_new(ccptn_t, ascii, error) (cce_destination_t L, ccmem_allocator_t const * A,
				   ccptn_error_handler_t * H, ccmem_ascii_t input_rep)
{
  ccptn_t const *	P = ccname_new(ccptn_t, ascii)(L, A, input_rep);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: standalone instances, deserialisable.
 ** ----------------------------------------------------------------- */

ccptn_t *
ccname_new(ccptn_t, deserialisable, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H)
{
  ccptn_t *	P = ccname_new(ccptn_t, deserialisable)(L, A);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}

ccptn_t *
ccname_new(ccptn_t, deserialisable, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H)
{
  ccptn_t *	P = ccname_new(ccptn_t, deserialisable)(L, A);
  ccptn_init_and_register_handler(L, H, P);
  return P;
}


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: standalone instances, clone.
 ** ----------------------------------------------------------------- */

ccptn_t const *
ccname_new(ccptn_t, clone, clean) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * src)
{
  ccptn_t const	*dst = ccname_new(ccptn_t, clone)(L, A, src);
  ccptn_init_and_register_handler(L, H, dst);
  return dst;
}

ccptn_t const *
ccname_new(ccptn_t, clone, error) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * src)
{
  ccptn_t const	*dst = ccname_new(ccptn_t, clone)(L, A, src);
  ccptn_init_and_register_handler(L, H, dst);
  return dst;
}


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */


/* end of file */
