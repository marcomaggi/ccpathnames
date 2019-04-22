/*
  Part of: CCPathnames
  Contents: utilities for endianness conversion
  Date: Mon Apr 22, 2019

  Abstract



  Copyright (C) 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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
 ** Storage of "uint32_t" vlues.
 ** ----------------------------------------------------------------- */

void
store_uint32_in_network_byte_order (uint8_t * const network_byte_order_store, uint32_t const the_val)
{
  if (MMUX_NETWORK_BYTE_ORDER) {
    /* The underlying  platform uses  "network byte order"  endianness, it  uses "big
       endianness".  The  most significant  byte from "the_val"  goes in  the lowest
       address in the memory referenced by "network_byte_order_store".*/
    network_byte_order_store[0] = (the_val >> (8 * 3)) & 0xFF;
    network_byte_order_store[1] = (the_val >> (8 * 2)) & 0xFF;
    network_byte_order_store[2] = (the_val >> (8 * 1)) & 0xFF;
    network_byte_order_store[3] = (the_val >> (8 * 0)) & 0xFF;
  } else {
    /* The underlying platform does not use  "network byte order" endianness, it uses
       "little endianness".  The  least significant byte from "the_val"  goes in the
       lowest address in the memory referenced by "network_byte_order_store". */
    network_byte_order_store[0] = (the_val >> (8 * 0)) & 0xFF;
    network_byte_order_store[1] = (the_val >> (8 * 1)) & 0xFF;
    network_byte_order_store[2] = (the_val >> (8 * 2)) & 0xFF;
    network_byte_order_store[3] = (the_val >> (8 * 3)) & 0xFF;
  }
  if (0) {
    fprintf(stderr, "%s: stored %lu in network_byte_order_store=[", __func__, (unsigned long) the_val);
    for (unsigned i=0; i<sizeof(uint32_t); ++i) {
      fprintf(stderr, " %d", network_byte_order_store[i]);
    }
    fprintf(stderr, " ]\n");
  }
}

void
retrieve_uint32_in_network_byte_order (uint32_t * const _the_val_ptr, uint8_t const * const network_byte_order_store)
{
  CCPTN_PC(uint8_t, the_val_ptr, _the_val_ptr);

  if (MMUX_NETWORK_BYTE_ORDER) {
    /* The underlying  platform uses  "network byte order"  endianness, it  uses "big
       endianness".  The  byte from the  lowest address  in the memory  referenced by
       "network_byte_order_store"   goes   into   the  most   significant   byte   of
       "the_val".*/
    the_val_ptr[3] = network_byte_order_store[0];
    the_val_ptr[2] = network_byte_order_store[1];
    the_val_ptr[1] = network_byte_order_store[2];
    the_val_ptr[0] = network_byte_order_store[3];
  } else {
    /* The underlying platform does not use  "network byte order" endianness, it uses
       "little  endianness".   The  byte  from  the  lowest  address  in  the  memory
       referenced by  "network_byte_order_store" goes into the  lowest memory address
       in the memory referenced by "the_val_ptr". */
    the_val_ptr[0] = network_byte_order_store[0];
    the_val_ptr[1] = network_byte_order_store[1];
    the_val_ptr[2] = network_byte_order_store[2];
    the_val_ptr[3] = network_byte_order_store[3];
  }
  if (0) {
    fprintf(stderr, "%s: retrieved %lu from network_byte_order_store=[", __func__, (unsigned long) *_the_val_ptr);
    for (unsigned i=0; i<sizeof(uint32_t); ++i) {
      fprintf(stderr, " %d", network_byte_order_store[i]);
    }
    fprintf(stderr, " ]\n");
  }
}

/* end of file */
