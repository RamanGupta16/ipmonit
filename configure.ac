AC_INIT(binary/ipmonit.cpp)
AM_INIT_AUTOMAKE(ipmonot, 0.1)

# Checks for programs.
AC_PROG_CXX
AC_LANG(C++)

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdint.h stdlib.h string.h sys/socket.h unistd.h iostream])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([memset socket])

AC_OUTPUT(
 Makefile
)
