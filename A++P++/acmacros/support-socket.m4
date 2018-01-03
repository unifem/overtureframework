dnl $Id: support-socket.m4,v 1.5 2002/01/30 16:33:49 gunney Exp $

AC_DEFUN(BTNG_VAR_SET_SOCKET,[
dnl Provides support for the socket library.
dnl
dnl Arguments are:
dnl 1. Name of variable to set to path where socket are installed.
dnl    Nothig is done if this variable is unset.
dnl 2. Name of the INCLUDES variable similar to the automake INCLUDES variable.
dnl    This variable is modified ONLY if it is NOT set.
dnl 3. Name of the LIBS variable similar to the automake LIBS variable.
dnl    This variable is modified ONLY if it is NOT set.
dnl
dnl If arg1 is defined, assume that the user wants socket
dnl support.  Do so by assigning arg2 and arg3 if they are not defined.
dnl
if test "${$1+set}" = set ; then
  BTNG_AC_LOG($1,$1 is set)
  if test ! "${$2+set}" = set ; then
    test -n "${$1}" && $2="-I${$1}/include"
  fi
  if test ! "${$3+set}" = set ; then
    BTNG_AC_LOG($3,$3 is not set)
    btng_save_socket_LIBS="$LIBS";	# Save for later recovery.
    test -n "${$1}" &&	\
      LIBS="-L${$1}/lib $LIBS"	# Add path flag for library search.
    # Look for library.
    AC_SEARCH_LIBS([getsockname],socket,[
      BTNG_AC_LOG("removing btng_save_socket_libs <$btng_save_socket_LIBS> from LIBS <$LIBS>")
      $3=`echo " $LIBS" | sed "s! $btng_save_socket_LIBS!!"`; # Action if found
      test -n "${$1}" &&	\
        $3="-L${$1}/lib ${$3}"	# Add path flag to output variable.
      BTNG_AC_LOG_VAR($3, Found socket library flag)
      ],[
      BTNG_AC_LOG_VAR($3, Did not find socket library flag)
      ])
    LIBS="$btng_save_socket_LIBS";	# Restore global-use variable.
  fi
fi
])dnl



AC_DEFUN(BTNG_SUPPORT_SOCKET,[
dnl Support socket library by setting the variables
dnl socket_PREFIX, socket_INCLUDES, and socket_LIBS.
dnl Arg1: non-empty if you want the default to be on.
dnl
# Begin macro BTNG_SUPPORT_SOCKET

BTNG_ARG_WITH_ENV_WRAPPER(socket, socket_PREFIX,
ifelse($1,,
[  --with-socket[=PATH]
			Use socket and optionally specify where
			it is installed.],
[  --without-socket	Do not use the socket library.]),
ifelse($1,,unset socket_PREFIX; test "${with_socket+set}" = set && socket_PREFIX=,socket_PREFIX=))

BTNG_ARG_WITH_PREFIX(socket-includes,socket_INCLUDES,
[  --with-socket-includes=STRING
			Specify the INCLUDES flags for socket.
			If not specified, and --with-socket=PATH is,
			this defaults to "-IPATH/include".])dnl

BTNG_ARG_WITH_PREFIX(socket-libs,socket_LIBS,
[  --with-socket-libs=STRING
			Specify LIBS flags for socket.
			If not specified, and --with-socket=PATH is,
			this defaults to "-LPATH/lib -llapack -lblas".])dnl

BTNG_AC_LOG(Setting socket variables)
BTNG_VAR_SET_SOCKET(socket_PREFIX,socket_INCLUDES,socket_LIBS)
BTNG_AC_LOG_VAR(socket_PREFIX socket_INCLUDES socket_LIBS)
# End macro BTNG_SUPPORT_SOCKET
])
