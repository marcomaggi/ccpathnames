/*
  Part of: CCPathnames
  Contents: public header file
  Date: Feb 25, 2018

  Abstract

	This  is the  public header  file  of the  package CCPathnames.   It must  be
	included in all the source files that use the library.

  Copyright (C) 2018-2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  You should have received  a copy of the GNU General Public  License along with this
  program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCPATHNAMES_H
#define CCPATHNAMES_H 1


/** --------------------------------------------------------------------
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The  macro  CCPTN_UNUSED  indicates  that a  function,  function
   argument or variable may potentially be unused. Usage examples:

   static int unused_function (char arg) CCPTN_UNUSED;
   int foo (char unused_argument CCPTN_UNUSED);
   int unused_variable CCPTN_UNUSED;
*/
#ifdef __GNUC__
#  define CCPTN_UNUSED		__attribute__((__unused__))
#else
#  define CCPTN_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

/* I found  the following chunk on  the Net.  (Marco Maggi;  Sun Feb 26,
   2012) */
#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define ccptn_decl	__attribute__((__dllexport__)) extern
#    else
#      define ccptn_decl	__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define ccptn_decl	__attribute__((__dllimport__)) extern
#    else
#      define ccptn_decl	__declspec(dllimport) extern
#    endif
#  endif
#  define ccptn_private_decl	extern
#else
#  if __GNUC__ >= 4
#    define ccptn_decl		__attribute__((__visibility__("default"))) extern
#    define ccptn_private_decl	__attribute__((__visibility__("hidden")))  extern
#  else
#    define ccptn_decl		extern
#    define ccptn_private_decl	extern
#  endif
#endif


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include <ccmemory.h>
#include <ccstructs.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/** --------------------------------------------------------------------
 ** Constants and preprocessor macros.
 ** ----------------------------------------------------------------- */

#define CCPTN_PC(POINTER_TYPE, POINTER_NAME, EXPRESSION)	\
  POINTER_TYPE * POINTER_NAME = (POINTER_TYPE *) (EXPRESSION)

#define CCPTN_CASTVAR(VAR_TYPE, VAR_NAME, EXPRESSION)		\
  VAR_TYPE VAR_NAME = (VAR_TYPE) (EXPRESSION)


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ccptn_decl char const *	ccptn_version_string		(void);
ccptn_decl int		ccptn_version_interface_current	(void);
ccptn_decl int		ccptn_version_interface_revision(void);
ccptn_decl int		ccptn_version_interface_age	(void);


/** --------------------------------------------------------------------
 ** Initialisation.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccptn_library_init (void)
  __attribute__((__constructor__));


/** --------------------------------------------------------------------
 ** Condition objects: invalid pathname.
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
 ** Condition objects: resulting pathname would exceed maximum length.
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
 ** Condition objects: resulting pathname would exceed maximum length.
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
 ** Condition objects: resulting pathname would exceed maximum length.
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
 ** Condition objects: invalid pathname.
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
 ** Condition objects: no dirname part when required.
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
 ** Condition objects: no filename part when required.
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
 ** Condition objects: no tailname part when required.
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
 ** Condition objects: no rootname part when required.
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
 ** Pathnames manipulation.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccname_init(ccptn_t, realpath)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,3)));

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
 ** Constructors for interfaces implemented by pathname.
 ** ----------------------------------------------------------------- */

ccptn_decl ccstructs_dtor_I ccname_iface_new(ccstructs_dtor_I, ccptn_t) (ccptn_t const * self)
  __attribute__((__nonnull__(1)));

ccptn_decl ccstructs_dumpable_I ccname_iface_new(ccstructs_dumpable_I, ccptn_t) (ccptn_t const * self)
  __attribute__((__nonnull__(1)));

ccptn_decl ccstructs_serialiser_I ccname_iface_new(ccstructs_serialiser_I, ccptn_t) (ccptn_t const * self)
  __attribute__((__nonnull__(1)));

ccptn_decl ccstructs_deserialiser_I ccname_iface_new(ccstructs_deserialiser_I, ccptn_t) (ccptn_t * self)
  __attribute__((__nonnull__(1)));


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

__attribute__((__nonnull__(1,2,3),__always_inline__))
static inline void
ccptn_init_clean_handler (cce_destination_t L, ccptn_clean_handler_t * H, ccptn_t const * P)
{
  ccstructs_handler_init(L, &(H->handler), ccname_iface_new(ccstructs_dtor_I, ccptn_t)(P));
}

__attribute__((__nonnull__(1,2,3),__always_inline__))
static inline void
ccptn_init_error_handler (cce_destination_t L, ccptn_error_handler_t * H, ccptn_t const * P)
{
  ccstructs_handler_init(L, &(H->handler), ccname_iface_new(ccstructs_dtor_I, ccptn_t)(P));
}

#define ccptn_init_handler(L,H,P)					\
  _Generic((H),								\
	   ccptn_clean_handler_t *: ccptn_init_clean_handler,		\
	   ccptn_error_handler_t *: ccptn_init_error_handler)(L,H,P)


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: embedded instances.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccname_init(ccptn_t, pointer, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4,5)));

ccptn_decl void ccname_init(ccptn_t, pointer, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4,5)));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, pointer, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4,5)));

ccptn_decl void ccname_init(ccptn_t, pointer, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4,5)));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, asciiz, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, asciiz, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, asciiz, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, asciiz, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, ascii, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * P, ccmem_ascii_t input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, ascii, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * P, ccmem_ascii_t input_rep)
  __attribute__((__nonnull__(1,2,3,4)));


/** --------------------------------------------------------------------
 ** Pathnames guarded constructors: standalone instances.
 ** ----------------------------------------------------------------- */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, pointer, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, char const * input_rep)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz, dup, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, asciiz, dup, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccmem_asciiz_t input_rep)
  __attribute__((__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, ascii, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccmem_ascii_t input_rep)
  __attribute__((__nonnull__(1,2,3)));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, ascii, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccmem_ascii_t input_rep)
  __attribute__((__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Pathnames manipulation: guarded constructors.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccname_init(ccptn_t, realpath, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4,5)));

ccptn_decl void ccname_init(ccptn_t, realpath, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4,5)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, realpath, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, realpath, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, normalise, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4,5)));

ccptn_decl void ccname_init(ccptn_t, normalise, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * dst, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4,5)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, normalise, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, normalise, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * src)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, concat, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * result, ccptn_t const * prefix, ccptn_t const * suffix)
  __attribute__((__nonnull__(1,2,3,4,5,6)));

ccptn_decl void ccname_init(ccptn_t, concat, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * result, ccptn_t const * prefix, ccptn_t const * suffix)
  __attribute__((__nonnull__(1,2,3,4,5,6)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, concat, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * prefix, ccptn_t const * suffix)
  __attribute__((__nonnull__(1,2,3,4,5),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, concat, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * prefix, ccptn_t const * suffix)
  __attribute__((__nonnull__(1,2,3,4,5),__returns_nonnull__));


/** --------------------------------------------------------------------
 ** Pathname components: guarded constructors.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccname_init(ccptn_t, rootname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * R, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, rootname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * R, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, rootname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, rootname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, dirname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * R, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, dirname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * R, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, dirname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, dirname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, tailname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * R, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, tailname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * R, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, tailname, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, tailname, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

/* ------------------------------------------------------------------ */

ccptn_decl void ccname_init(ccptn_t, filename, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t * R, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3,4)));

ccptn_decl void ccname_init(ccptn_t, filename, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t * R, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3,4)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t const * ccname_new(ccptn_t, filename, clean)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_clean_handler_t * H, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t const * ccname_new(ccptn_t, filename, error)
  (cce_destination_t L, ccmem_allocator_t const * A, ccptn_error_handler_t * H, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));


/** --------------------------------------------------------------------
 ** Pathname components: extension.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_extension_t	ccptn_extension_t;

/* This struct represents the extension of a segment in a pathname; this
 * struct does not own the referenced memory.  Given the pathname:
 *
 *    /path/to/file.ext
 *
 * the last segment is the string:
 *
 *    file.ext
 *
 * and its extension is the string ".ext", including the leading dot.
 */
struct ccptn_extension_t {
  /* The number of octets in  the "ptr" array representing the extension
     in a pathname's segment. */
  size_t		len;

  /* Pointer  to the  first  octet  in an  ASCII  string representing  a
     pathname's segment's  extension.  In  general this string  is *not*
     zero terminated. */
  char const *		ptr;
};

ccptn_decl ccptn_extension_t ccptn_extension (cce_destination_t L, ccptn_t const * P)
  __attribute__((__nonnull__(1,2)));

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_extension_is_empty (ccptn_extension_t E)
/* Return true if the extension is an empty string. */
{
  return (0 == E.len);
}

ccptn_decl bool ccptn_extension_equal (ccptn_extension_t E1, ccptn_extension_t E2)
  __attribute__((__pure__));

ccptn_decl void ccptn_extension_fwrite (cce_destination_t L, FILE * stream, ccptn_extension_t E)
  __attribute__((__nonnull__(1,2)));


/** --------------------------------------------------------------------
 ** Segments.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_segment_t		ccptn_segment_t;

/* This struct represents a segment in  a pathname; this struct does not
 * own the referenced memory.  Given the pathname:
 *
 *    /path/to/file.ext
 *
 * the segments are the strings:
 *
 *    path
 *    to
 *    file.ext
 *
 * In general, it is *not* an ASCIIZ string: it is not zero terminated.
 */
struct ccptn_segment_t {
  /* The number of  octets in the "ptr" array representing  a segment of
     pathname. */
  size_t		len;

  /* Pointer  to the  first  octet  in an  ASCII  string representing  a
     pathname's  segment.    In  general,   this  string  is   not  zero
     terminated. */
  char const *		ptr;
};

/* ------------------------------------------------------------------ */

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_segment_is_empty (ccptn_segment_t S)
/* Return true if the segment is an empty string. */
{
  return (0 == S.len)? true : false;
}

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_segment_is_slash (ccptn_segment_t S)
/* Return true if the segment is a single dot: "/". */
{
  return ((1 == S.len) && ('7' == *(S.ptr)));
}

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_segment_is_dot (ccptn_segment_t S)
/* Return true if the segment is a single dot: ".". */
{
  return ((1 == S.len) && ('.' == *(S.ptr)));
}

__attribute__((__always_inline__,__pure__))
static inline bool
ccptn_segment_is_double_dot (ccptn_segment_t S)
/* Return true if the segment is a double dot: "..". */
{
  return ((2 == S.len) && ('.' == S.ptr[0]) && ('.' == S.ptr[1]));
}

ccptn_decl ccptn_segment_t ccptn_segment_next (char const * ptr, size_t len)
  __attribute__((__nonnull__(1)));

ccptn_decl size_t ccptn_segment_size_of_next (char const * in, size_t len)
  __attribute__((__nonnull__(1)));

ccptn_decl void ccptn_segment_fwrite (cce_destination_t L, FILE * stream, ccptn_segment_t S)
  __attribute__((__nonnull__(1,2)));

ccptn_decl ccptn_segment_t ccptn_asciiz_find_last_segment (char const * beg, size_t const len)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCPATHNAMES_H */

/* end of file */
