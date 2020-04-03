/*
  Part of: CCPathnames
  Contents: subordinate public header file, core pathnames API
  Date: Apr 21, 2019

  Abstract

	This is a subordinate public header file of the package CCPathnames.  It must
	be included by "ccpathnames.h".

  Copyright (C) 2018-2020 Marco Maggi <mrc.mgg@gmail.com>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCPATHNAMES_CORE_H
#define CCPATHNAMES_CORE_H 1


/** --------------------------------------------------------------------
 ** Pathname type definitions.
 ** ----------------------------------------------------------------- */

typedef struct ccname_table_type(ccptn_t)	ccname_table_type(ccptn_t);
typedef struct ccptn_t				ccptn_t;

struct ccptn_t {
  ccname_table_type(ccptn_t) const *	methods;

  /* Memory allocator used for this struct and its fields. */
  ccmem_allocator_t const *	allocator;

  struct {
    /* True if this struct has been dynamically allocated using the allocator. */
    unsigned int	dynamically_allocated_base: 1;

    /* True if the char buffer has been dynamically allocated using the allocator. */
    unsigned int	dynamically_allocated_buffer: 1;

    /* True if  this pathname is  absolute; false if it  is relative.  A  pathname is
       absolute  if  its  first  octet  is the  ASCII  representation  of  the  slash
       character. */
    unsigned int	absolute: 1;

    /* True if this pathname has been  normalised (to remove multiple slashes and the
       like). */
    unsigned int	normalised: 1;

    /* True if this pathname has been normalised with a call to "realpath()". */
    unsigned int	realpath: 1;
  };

  /* The number of octets in the "buf" array, *not* counting the terminating zero. */
  size_t		len;

  /* Pointer to the ASCIIZ string representing the pathname. */
  char const *		ptr;
};


/** --------------------------------------------------------------------
 ** Pathname constructors for embedded instances.
 ** ----------------------------------------------------------------- */

cclib_decl void ccname_init(ccptn_t, pointer)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, char const * input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

cclib_decl void ccname_init(ccptn_t, pointer, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, char const * input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

cclib_decl void ccname_init(ccptn_t, asciiz)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, ccmem_asciiz_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

cclib_decl void ccname_init(ccptn_t, asciiz, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, ccmem_asciiz_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

/* ------------------------------------------------------------------ */

cclib_decl void ccname_init(ccptn_t, ascii)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, ccmem_ascii_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void
ccname_init(ccptn_t, ascii, dup) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, ccmem_ascii_t input_rep)
{
  ccname_init(ccptn_t, ascii)(L, A, P, input_rep);
}

/* ------------------------------------------------------------------ */

cclib_decl void ccname_init(ccptn_t, deserialisable)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

/* ------------------------------------------------------------------ */

cclib_decl void ccname_init(ccptn_t, clone)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);


/** --------------------------------------------------------------------
 ** Pathname constructors for standalone instances.
 ** ----------------------------------------------------------------- */

cclib_decl ccptn_t const * ccname_new(ccptn_t, pointer)
  (cce_destination_t L, ccmem_allocator_t const * A, char const * input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccptn_t const * ccname_new(ccptn_t, pointer, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, char const * input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

cclib_decl ccptn_t const * ccname_new(ccptn_t, asciiz)
  (cce_destination_t L, ccmem_allocator_t const * A, ccmem_asciiz_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccptn_t const * ccname_new(ccptn_t, asciiz, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, ccmem_asciiz_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

cclib_decl ccptn_t const * ccname_new(ccptn_t, ascii)
  (cce_destination_t L, ccmem_allocator_t const * A, ccmem_ascii_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccptn_t const * ccname_new(ccptn_t, ascii, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, ccmem_ascii_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

cclib_decl ccptn_t * ccname_new(ccptn_t, deserialisable)
  (cce_destination_t L, ccmem_allocator_t const * A)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

cclib_decl ccptn_t const * ccname_new(ccptn_t, clone)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;


/** --------------------------------------------------------------------
 ** Pathnames: predicates.
 ** ----------------------------------------------------------------- */

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline bool
ccptn_is_absolute (ccptn_t const * const P)
{
  return (P->absolute)? true : false;
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline bool
ccptn_is_relative (ccptn_t const * const P)
{
  return (P->absolute)? false : true;
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline bool
ccptn_is_normalised (ccptn_t const * const P)
{
  return (P->normalised)? true : false;
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline bool
ccptn_is_realpath (ccptn_t const * const P)
{
  return (P->realpath)? true : false;
}


/** --------------------------------------------------------------------
 ** Pathnames: accessors.
 ** ----------------------------------------------------------------- */

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline ccmem_ascii_t
ccptn_ascii (ccptn_t const * const P)
{
  ccmem_ascii_t		A = {
    .ptr	= (char *)P->ptr,
    .len	= P->len
  };
  return A;
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline ccmem_asciiz_t
ccptn_asciiz (ccptn_t const * const P)
{
  ccmem_asciiz_t	A = {
    .ptr	= (char *)P->ptr,
    .len	= P->len
  };
  return A;
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline ccmem_block_t
ccptn_block (ccptn_t const * const P)
{
  ccmem_block_t		A = {
    .ptr	= (uint8_t *)P->ptr,
    .len	= 1 + P->len
  };
  return A;
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
static inline size_t
ccptn_len (ccptn_t const * const P)
{
  return (P->len);
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline char const *
ccptn_ptr (ccptn_t const * const P)
{
  return (P->ptr);
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_PURE
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline ccmem_allocator_t const *
ccptn_allocator (ccptn_t const * const P)
{
  return (P->allocator);
}


/** --------------------------------------------------------------------
 ** Pathnames manipulation.
 ** ----------------------------------------------------------------- */

cclib_decl void ccname_init(ccptn_t, realpath)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

cclib_decl void ccname_init(ccptn_t, normalise)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

cclib_decl void ccname_init(ccptn_t, concat)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * result, ccptn_t const * prefix, ccptn_t const * suffix)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

/* ------------------------------------------------------------------ */

cclib_decl ccptn_t const * ccname_new(ccptn_t, realpath)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccptn_t const * ccname_new(ccptn_t, normalise)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccptn_t const * ccname_new(ccptn_t, concat)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * prefix, ccptn_t const * suffix)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;


/** --------------------------------------------------------------------
 ** Pathname components.
 ** ----------------------------------------------------------------- */

cclib_decl void ccname_init(ccptn_t, rootname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

cclib_decl void ccname_init(ccptn_t, dirname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

cclib_decl void ccname_init(ccptn_t, tailname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

cclib_decl void ccname_init(ccptn_t, filename)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

cclib_decl ccptn_t const * ccname_new(ccptn_t, rootname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccptn_t const * ccname_new(ccptn_t, dirname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccptn_t const * ccname_new(ccptn_t, tailname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccptn_t const * ccname_new(ccptn_t, filename)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;


/** --------------------------------------------------------------------
 ** Pathnames: input/output.
 ** ----------------------------------------------------------------- */

cclib_decl void ccptn_fwrite (cce_destination_t L, FILE * stream, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);


/** --------------------------------------------------------------------
 ** Constructors for traits implemented by pathname.
 ** ----------------------------------------------------------------- */

cclib_decl ccstructs_dtor_T ccname_trait_new(ccstructs_dtor_T, ccptn_t) (ccptn_t const * self)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl ccstructs_dumpable_T ccname_trait_new(ccstructs_dumpable_T, ccptn_t) (ccptn_t const * self)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl ccstructs_serialiser_T ccname_trait_new(ccstructs_serialiser_T, ccptn_t) (ccptn_t const * self)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl ccstructs_deserialiser_T ccname_trait_new(ccstructs_deserialiser_T, ccptn_t) (ccptn_t * self)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl ccstructs_pathname_T ccname_trait_new(ccstructs_pathname_T, ccptn_t) (ccptn_t const * self)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#endif /* CCPATHNAMES_CORE_H */

/* end of file */
