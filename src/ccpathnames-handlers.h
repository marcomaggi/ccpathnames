/*
  Part of: CCPathnames
  Contents: subordinate public header file, exception handlers API
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

#ifndef CCPATHNAMES_HANDLERS_H
#define CCPATHNAMES_HANDLERS_H 1


/** --------------------------------------------------------------------
 ** Pathnames: exception handlers.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_clean_handler_t	ccptn_clean_handler_t;
typedef struct ccptn_error_handler_t	ccptn_error_handler_t;

struct ccptn_clean_handler_t {
  ccstructs_clean_handler_t	handler;
};

struct ccptn_error_handler_t {
  ccstructs_error_handler_t	handler;
};

/* ------------------------------------------------------------------ */

static inline ccstructs_clean_handler_t *
ccptn_handler_clean_handler (ccptn_clean_handler_t * H)
{
  return &(H->handler);
}

static inline ccstructs_error_handler_t *
ccptn_handler_error_handler (ccptn_error_handler_t * H)
{
  return &(H->handler);
}

#define ccptn_handler_handler(H)					\
  _Generic((H),								\
	   ccptn_clean_handler_t *: ccptn_handler_clean_handler,	\
	   ccptn_error_handler_t *: ccptn_handler_error_handler,	\
	   ccptn_clean_handler_t const *: ccptn_handler_clean_handler,	\
	   ccptn_error_handler_t const *: ccptn_handler_error_handler)(H)

/* ------------------------------------------------------------------ */

ccptn_decl void ccptn_init_and_register_clean_handler (cce_destination_t L, ccptn_clean_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

ccptn_decl void ccptn_init_and_register_error_handler (cce_destination_t L, ccptn_error_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

#define ccptn_init_and_register_handler(L,H,P)				\
  _Generic((H),								\
	   ccptn_clean_handler_t *: ccptn_init_and_register_clean_handler, \
	   ccptn_error_handler_t *: ccptn_init_and_register_error_handler)(L,H,P)


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: embedded instances.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccname_init(ccptn_t, pointer, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, char const * input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

ccptn_decl void ccname_init(ccptn_t, pointer, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, char const * input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, pointer, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, char const * input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

ccptn_decl void ccname_init(ccptn_t, pointer, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, char const * input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, asciiz, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, ccmem_asciiz_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

ccptn_decl void ccname_init(ccptn_t, asciiz, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, ccmem_asciiz_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, asciiz, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, ccmem_asciiz_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

ccptn_decl void ccname_init(ccptn_t, asciiz, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, ccmem_asciiz_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, ascii, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, ccmem_ascii_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

ccptn_decl void ccname_init(ccptn_t, ascii, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, ccmem_ascii_t input_rep)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

__attribute__((__nonnull__(1,2,3,4),__always_inline__))
static inline void
ccname_init(ccptn_t, ascii, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, ccmem_ascii_t input_rep)
{
  ccname_init(ccptn_t, ascii, clean)(L, A, H, P, input_rep);
}

__attribute__((__nonnull__(1,2,3,4),__always_inline__))
static inline void
ccname_init(ccptn_t, ascii, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, ccmem_ascii_t input_rep)
{
  ccname_init(ccptn_t, ascii, error)(L, A, H, P, input_rep);
}

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, deserialisable, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

ccptn_decl void ccname_init(ccptn_t, deserialisable, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, clone, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

ccptn_decl void ccname_init(ccptn_t, clone, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: standalone instances.
 ** ----------------------------------------------------------------- */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, char const * input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3,4)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, char const * input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, char const * input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3,4)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, char const * input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccmem_asciiz_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccmem_asciiz_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccmem_asciiz_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccmem_asciiz_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, ascii, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccmem_ascii_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, ascii, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccmem_ascii_t input_rep)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

__attribute__((__returns_nonnull__,__nonnull__(1,2,3),__always_inline__))
static inline ccptn_t const *
ccname_new(ccptn_t, ascii, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccmem_ascii_t input_rep)
{
  return ccname_new(ccptn_t, ascii, clean)(L, A, H, input_rep);
}

__attribute__((__returns_nonnull__,__nonnull__(1,2,3),__always_inline__))
static inline ccptn_t const *
ccname_new(ccptn_t, ascii, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccmem_ascii_t input_rep)
{
  return ccname_new(ccptn_t, ascii, error)(L, A, H, input_rep);
}

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t * ccname_new(ccptn_t, deserialisable, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

ccptn_decl ccptn_t * ccname_new(ccptn_t, deserialisable, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, clone, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * src)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3,4)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, clone, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * src)
  __attribute__((__returns_nonnull__,__nonnull__(1,2,3,4)));


/** --------------------------------------------------------------------
 ** Pathnames manipulation: guarded constructors.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccname_init(ccptn_t, realpath, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

ccptn_decl void ccname_init(ccptn_t, realpath, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, realpath, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

ccptn_decl ccptn_t const * ccname_new(ccptn_t, realpath, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, normalise, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

ccptn_decl void ccname_init(ccptn_t, normalise, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5);

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, normalise, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

ccptn_decl ccptn_t const * ccname_new(ccptn_t, normalise, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * src)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, concat, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * result, ccptn_t const * prefix, ccptn_t const * suffix)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5,6);

ccptn_decl void ccname_init(ccptn_t, concat, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * result, ccptn_t const * prefix, ccptn_t const * suffix)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5,6);

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, concat, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * prefix, ccptn_t const * suffix)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

ccptn_decl ccptn_t const * ccname_new(ccptn_t, concat, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * prefix, ccptn_t const * suffix)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4,5)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;


/** --------------------------------------------------------------------
 ** Pathname components: guarded constructors.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccname_init(ccptn_t, rootname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * R, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

ccptn_decl void ccname_init(ccptn_t, rootname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * R, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, rootname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

ccptn_decl ccptn_t const * ccname_new(ccptn_t, rootname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, dirname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * R, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

ccptn_decl void ccname_init(ccptn_t, dirname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * R, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, dirname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

ccptn_decl ccptn_t const * ccname_new(ccptn_t, dirname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, tailname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * R, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

ccptn_decl void ccname_init(ccptn_t, tailname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * R, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, tailname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

ccptn_decl ccptn_t const * ccname_new(ccptn_t, tailname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, filename, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * R, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

ccptn_decl void ccname_init(ccptn_t, filename, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * R, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4);

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, filename, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

ccptn_decl ccptn_t const * ccname_new(ccptn_t, filename, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * P)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#endif /* CCPATHNAMES_CORE_H */

/* end of file */
