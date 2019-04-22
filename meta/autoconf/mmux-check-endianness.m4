### mmux-check-endianness.m4 --
#
# MMUX_CHECK_ENDIANNESS
#
# Define the preprocessor symbols:
#
#   MMUX_BIG_ENDIAN
#   MMUX_LITTLE_ENDIAN
#   MMUX_NETWORK_BYTE_ORDER
#
# to 1 or 0 according to the endiannes of the platform.
#
AC_DEFUN([MMUX_CHECK_ENDIANNESS],[
  AC_C_BIGENDIAN([AS_VAR_SET([mmux_big_endian],1)
                  AS_VAR_SET([mmux_little_endian],0)],
                 [AS_VAR_SET([mmux_big_endian],0)
                  AS_VAR_SET([mmux_little_endian],1)])
  AS_VAR_SET([mmux_network_byte_order],$mmux_big_endian)
  AC_DEFINE_UNQUOTED([MMUX_BIG_ENDIAN],$mmux_big_endian,[Set to 1 if the platform has big endianness.])
  AC_DEFINE_UNQUOTED([MMUX_LITTLE_ENDIAN],$mmux_little_endian,[Set to 1 if the platform has little endianness.])
  AC_DEFINE_UNQUOTED([MMUX_NETWORK_BYTE_ORDER],$mmux_network_byte_order,[Set to 1 if the platform has network byte order endianness.])
  ])

### end of file
# Local Variables:
# mode: autoconf
# End:
