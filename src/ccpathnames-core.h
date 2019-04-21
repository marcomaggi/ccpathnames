/*
  Part of: CCPathnames
  Contents: subordinate public header file, core pathnames API
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

#ifndef CCPATHNAMES_CORE_H
#define CCPATHNAMES_CORE_H 1


/** --------------------------------------------------------------------
 ** Pathname type definitions.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_t				ccptn_t;

struct ccptn_t {
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

ccptn_decl void ccname_init(ccptn_t, pointer)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, pointer, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, asciiz)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3)));

ccptn_decl void ccname_init(ccptn_t, asciiz, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, ascii)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, ccmem_ascii_t input_rep)
  __attribute__((__nonnull__(1,2,3)));

__attribute__((__nonnull__(1,2,3),__always_inline__))
static inline void
ccname_init(ccptn_t, ascii, dup) (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P, ccmem_ascii_t input_rep)
{
  ccname_init(ccptn_t, ascii)(L, A, P, input_rep);
}

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, deserialisable)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * P)
  __attribute__((__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, clone)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4)));


/** --------------------------------------------------------------------
 ** Pathname constructors for standalone instances.
 ** ----------------------------------------------------------------- */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer)
  (cce_destination_t L, ccmem_allocator_t const * A, char const * input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, char const * input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz)
  (cce_destination_t L, ccmem_allocator_t const * A, ccmem_asciiz_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, ccmem_asciiz_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, ascii)
  (cce_destination_t L, ccmem_allocator_t const * A, ccmem_ascii_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, ascii, dup)
  (cce_destination_t L, ccmem_allocator_t const * A, ccmem_ascii_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t * ccname_new(ccptn_t, deserialisable)
  (cce_destination_t L, ccmem_allocator_t const * A)
  __attribute__((__returns_nonnull__,__nonnull__(1,2)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, clone)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Pathnames: predicates.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1),__always_inline__,__pure__))
static inline bool
ccptn_is_absolute (ccptn_t const * const P)
{
  return (P->absolute)? true : false;
}

__attribute__((__nonnull__(1),__always_inline__,__pure__))
static inline bool
ccptn_is_relative (ccptn_t const * const P)
{
  return (P->absolute)? false : true;
}

__attribute__((__nonnull__(1),__always_inline__,__pure__))
static inline bool
ccptn_is_normalised (ccptn_t const * const P)
{
  return (P->normalised)? true : false;
}

__attribute__((__nonnull__(1),__always_inline__,__pure__))
static inline bool
ccptn_is_realpath (ccptn_t const * const P)
{
  return (P->realpath)? true : false;
}


/** --------------------------------------------------------------------
 ** Pathnames: accessors.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1),__always_inline__,__pure__))
static inline ccmem_ascii_t
ccptn_ascii (ccptn_t const * const P)
{
  ccmem_ascii_t		A = {
    .ptr	= (char *)P->ptr,
    .len	= P->len
  };
  return A;
}

__attribute__((__nonnull__(1),__always_inline__,__pure__))
static inline ccmem_asciiz_t
ccptn_asciiz (ccptn_t const * const P)
{
  ccmem_asciiz_t	A = {
    .ptr	= (char *)P->ptr,
    .len	= P->len
  };
  return A;
}

__attribute__((__nonnull__(1),__always_inline__,__pure__))
static inline ccmem_block_t
ccptn_block (ccptn_t const * const P)
{
  ccmem_block_t		A = {
    .ptr	= (uint8_t *)P->ptr,
    .len	= 1 + P->len
  };
  return A;
}

__attribute__((__nonnull__(1),__always_inline__,__pure__))
static inline size_t
ccptn_len (ccptn_t const * const P)
{
  return (P->len);
}

__attribute__((__nonnull__(1),__always_inline__,__pure__,__returns_nonnull__))
static inline char const *
ccptn_ptr (ccptn_t const * const P)
{
  return (P->ptr);
}

__attribute__((__nonnull__(1),__always_inline__,__pure__,__returns_nonnull__))
static inline ccmem_allocator_t const *
ccptn_allocator (ccptn_t const * const P)
{
  return (P->allocator);
}


/** --------------------------------------------------------------------
 ** Pathnames manipulation.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccname_init(ccptn_t, realpath)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, normalise)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, concat)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * result, ccptn_t const * prefix, ccptn_t const * suffix)
  __attribute__((__nonnull__(1,2,3,4,5)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, realpath)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, normalise)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, concat)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * prefix, ccptn_t const * suffix)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));


/** --------------------------------------------------------------------
 ** Pathname components.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccname_init(ccptn_t, rootname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, dirname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, tailname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, filename)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, rootname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, dirname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, tailname)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, filename)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));


/** --------------------------------------------------------------------
 ** Pathnames: input/output.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccptn_fwrite (cce_destination_t L, FILE * stream, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Constructors for traits implemented by pathname.
 ** ----------------------------------------------------------------- */

ccptn_decl ccstructs_dtor_T ccname_trait_new(ccstructs_dtor_T, ccptn_t) (ccptn_t const * self)
  __attribute__((__nonnull__(1)));

ccptn_decl ccstructs_dtor_T ccname_trait_new(ccstructs_dtor_T, ccptn_t, embedded) (ccptn_t const * self)
  __attribute__((__nonnull__(1)));

ccptn_decl ccstructs_dtor_T ccname_trait_new(ccstructs_dtor_T, ccptn_t, standalone) (ccptn_t const * self)
  __attribute__((__nonnull__(1)));

ccptn_decl ccstructs_dumpable_T ccname_trait_new(ccstructs_dumpable_T, ccptn_t) (ccptn_t const * self)
  __attribute__((__nonnull__(1)));

ccptn_decl ccstructs_serialiser_T ccname_trait_new(ccstructs_serialiser_T, ccptn_t) (ccptn_t const * self)
  __attribute__((__nonnull__(1)));

ccptn_decl ccstructs_deserialiser_T ccname_trait_new(ccstructs_deserialiser_T, ccptn_t) (ccptn_t * self)
  __attribute__((__nonnull__(1)));

ccptn_decl ccstructs_pathname_T ccname_trait_new(ccstructs_pathname_T, ccptn_t) (ccptn_t const * self)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#endif /* CCPATHNAMES_CORE_H */

/* end of file */
