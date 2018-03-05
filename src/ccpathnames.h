/*
  Part of: CCPathnames
  Contents: public header file
  Date: Feb 25, 2018

  Abstract

	This is the  public header file of the  package CCPathnames.  It
	must be included in all the source files that use the library.

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
#      define ccptn_decl		__attribute__((__dllexport__)) extern
#    else
#      define ccptn_decl		__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define ccptn_decl		__attribute__((__dllimport__)) extern
#    else
#      define ccptn_decl		__declspec(dllimport) extern
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
#include <stdlib.h>
#include <stdbool.h>


/** --------------------------------------------------------------------
 ** Constants and preprocessor macros.
 ** ----------------------------------------------------------------- */

#define CCPTN_PC(POINTER_TYPE, POINTER_NAME, EXPRESSION)	\
  POINTER_TYPE * POINTER_NAME = (POINTER_TYPE *) (EXPRESSION)


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

ccptn_decl void ccptn_init (void)
  __attribute__((constructor));


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
 ** Condition objects: invalid pathname.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_descriptor_normalised_pathname_t	ccptn_descriptor_normalised_pathname_t;
typedef struct ccptn_condition_normalised_pathname_t	ccptn_condition_normalised_pathname_t;

struct ccptn_descriptor_normalised_pathname_t {
  cce_descriptor_t	descriptor;
};

struct ccptn_condition_normalised_pathname_t {
  cce_condition_runtime_error_t	runtime_error;
};

ccptn_decl void ccptn_condition_init_normalised_pathname (ccptn_condition_normalised_pathname_t * C)
  __attribute__((__nonnull__(1)));

ccptn_decl cce_condition_t const * ccptn_condition_new_normalised_pathname (void)
  __attribute__((__returns_nonnull__));

ccptn_decl bool ccptn_condition_is_normalised_pathname (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Type definitions.
 ** ----------------------------------------------------------------- */

typedef struct ccptn_t			ccptn_t;
typedef struct ccptn_segment_t		ccptn_segment_t;
typedef struct ccptn_extension_t	ccptn_extension_t;

typedef void ccptn_final_fun_t (ccptn_t * P);

/* This struct represents a full pathname. */
struct ccptn_t {
  /* Finalisation function  for this  struct instance.  It  releases all
     the dynamic resources associated to this instance. */
  ccptn_final_fun_t *	final;

  struct {
    /* True if  this pathname is absolute;  false if it is  relative.  A
       pathname  is   absolute  if   its  first   octet  is   the  ASCII
       representation of the slash character. */
    unsigned int	absolute: 1;

    /* True if  this pathname  has been  normalised (to  remove multiple
       slashes and the like). */
    unsigned int	normalised: 1;

    /* True  if  this  pathname  has  been normalised  with  a  call  to
       "realpath()". */
    unsigned int	realpath: 1;
  };

  /* The  number  of octets  in  the  "buf"  array, *not*  counting  the
     terminating zero. */
  size_t		len;

  /* Pointer to the ASCIIZ string representing the pathname. */
  char *		buf;
};

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


/** --------------------------------------------------------------------
 ** Pathnames: constructors and destructors.
 ** ----------------------------------------------------------------- */

ccptn_decl ccptn_t * ccptn_init_nodup_asciiz (cce_destination_t L, ccptn_t * P, char const * pathname)
  __attribute__((__returns_nonnull__,__nonnull__(1,2)));

ccptn_decl ccptn_t * ccptn_init_dup_asciiz (cce_destination_t L, ccptn_t * P, char const * pathname)
  __attribute__((__returns_nonnull__,__nonnull__(1,2)));

ccptn_decl ccptn_t * ccptn_new_nodup_asciiz (cce_destination_t L, char const * pathname)
  __attribute__((__returns_nonnull__,__nonnull__(1,2)));

ccptn_decl ccptn_t * ccptn_new_dup_asciiz (cce_destination_t L, char const * pathname)
  __attribute__((__returns_nonnull__,__nonnull__(1,2)));

/* ------------------------------------------------------------------ */

__attribute__((__nonnull__(1),__always_inline__))
static inline void
ccptn_final (ccptn_t * P)
{
  P->final(P);
}

/* ------------------------------------------------------------------ */

ccptn_decl void ccptn_cleanup_handler_ptn_init (cce_destination_t L, cce_handler_t * H, ccptn_t * P)
  __attribute__((__nonnull__(1,2,3)));

ccptn_decl void ccptn_error_handler_ptn_init (cce_destination_t L, cce_handler_t * H, ccptn_t * P)
  __attribute__((__nonnull__(1,2,3)));

#define ccptn_handler_ptn_init(L,H,P)					\
  _Generic((H),								\
	   cce_cleanup_handler_t	*: ccptn_cleanup_handler_ptn_init, \
	   cce_error_handler_t		*: ccptn_error_handler_ptn_init)(L,&(H->handler),P)


/** --------------------------------------------------------------------
 ** Pathnames: accessors.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1),__always_inline__,__pure__,__returns_nonnull__))
static inline char const *
ccptn_asciiz (ccptn_t const * const P)
{
  return (P->buf);
}

__attribute__((__nonnull__(1),__always_inline__,__pure__))
static inline size_t
ccptn_len (ccptn_t const * const P)
{
  return (P->len);
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
 ** Pathnames: manipulation.
 ** ----------------------------------------------------------------- */

ccptn_decl ccptn_t * ccptn_new_realpath (cce_destination_t L, ccptn_t const * P)
  __attribute__((__nonnull__(1,2),__returns_nonnull__));

ccptn_decl ccptn_t * ccptn_init_realpath (cce_destination_t L, ccptn_t * R, ccptn_t const * const P)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t * ccptn_new_normalise (cce_destination_t L, ccptn_t const * const P)
  __attribute__((__nonnull__(1,2)));

ccptn_decl ccptn_t * ccptn_init_normalise (cce_destination_t L, ccptn_t * R, ccptn_t const * const P)
  __attribute__((__nonnull__(1,2,3)));

ccptn_decl size_t ccptn_normal_pass_remove_useless_slashes (char * output_ptr, char const * const input_ptr, size_t const input_len)
  __attribute__((__nonnull__(1,2)));

ccptn_decl size_t ccptn_normal_pass_remove_single_dot_segments (char * output_ptr, char const * const input_ptr, size_t const input_len)
  __attribute__((__nonnull__(1,2)));

ccptn_decl size_t ccptn_normal_pass_remove_double_dot_segments (cce_destination_t L, char * output_ptr,
								char const * const input_ptr, size_t const input_len)
  __attribute__((__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccptn_decl ccptn_t * ccptn_new_concat (cce_destination_t L, ccptn_t const * prefix, ccptn_t const * suffix)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccptn_decl ccptn_t * ccptn_init_concat (cce_destination_t L, ccptn_t * result, ccptn_t const * prefix, ccptn_t const * suffix)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));


/** --------------------------------------------------------------------
 ** Pathnames: components.
 ** ----------------------------------------------------------------- */

ccptn_decl ccptn_extension_t ccptn_extension (cce_destination_t L, ccptn_t const * P)
  __attribute__((__nonnull__(1,2)));

ccptn_decl ccptn_t * ccptn_rootname (cce_destination_t L, ccptn_t const * P)
  __attribute__((__nonnull__(1,2)));

ccptn_decl ccptn_t * ccptn_dirname (cce_destination_t L, ccptn_t const * P)
  __attribute__((__nonnull__(1,2)));

ccptn_decl ccptn_t * ccptn_tailname (cce_destination_t L, ccptn_t const * P)
  __attribute__((__nonnull__(1,2)));


/** --------------------------------------------------------------------
 ** Pathnames: input/output.
 ** ----------------------------------------------------------------- */

ccptn_decl void ccptn_print (cce_destination_t L, FILE * stream, ccptn_t const * P)
  __attribute__((__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Segments.
 ** ----------------------------------------------------------------- */

ccptn_decl bool ccptn_segment_is_empty (ccptn_segment_t S)
  __attribute__((__pure__));

ccptn_decl bool ccptn_segment_is_dot (ccptn_segment_t S)
  __attribute__((__pure__));

ccptn_decl bool ccptn_segment_is_double_dot (ccptn_segment_t S)
  __attribute__((__pure__));

ccptn_decl ccptn_segment_t ccptn_segment_next (char const * ptr, size_t len)
  __attribute__((__nonnull__(1)));

ccptn_decl size_t ccptn_segment_size_of_next (char const * in, size_t len)
  __attribute__((__nonnull__(1)));

ccptn_decl void ccptn_segment_print (cce_destination_t L, FILE * stream, ccptn_segment_t S)
  __attribute__((__nonnull__(1,2)));


/** --------------------------------------------------------------------
 ** Extensions.
 ** ----------------------------------------------------------------- */

ccptn_decl bool ccptn_extension_is_empty (ccptn_extension_t E)
  __attribute__((__pure__));

ccptn_decl bool ccptn_extension_equal (ccptn_extension_t E1, ccptn_extension_t E2)
  __attribute__((__pure__));

ccptn_decl void ccptn_extension_print (cce_destination_t L, FILE * stream, ccptn_extension_t E)
  __attribute__((__nonnull__(1,2)));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCPATHNAMES_H */

/* end of file */
