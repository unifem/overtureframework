dnl $Id: support-nsl.m4,v 1.4 2002/01/30 16:33:49 gunney Exp $

AC_DEFUN(BTNG_VAR_SET_NSL,[
dnl Provides support for the nsl library.
dnl
dnl Arguments are:
dnl 1. Name of variable to set to path where nsl are installed.
dnl    Nothig is done if this variable is unset.
dnl 2. Name of the INCLUDES variable similar to the automake INCLUDES variable.
dnl    This variable is modified ONLY if it is NOT set.
dnl 3. Name of the LIBS variable similar to the automake LIBS variable.
dnl    This variable is modified ONLY if it is NOT set.
dnl
dnl If arg1 is defined, assume that the user wants nsl
dnl support.  Do so by assigning arg2 and arg3 if they are not defined.
dnl
if test "${$1+set}" = set ; then
  if test ! "${$2+set}" = set ; then
    test -n "${$1}" && $2="-I${$1}/include"
  fi
  if test ! "${$3+set}" = set ; then
    btng_save_LIBS="$LIBS";	# Save for later recovery.
    test -n "${$1}" &&	\
      LIBS="-L${$1}/lib $LIBS"	# Add path flag for library search.
    # Look for library.
    AC_SEARCH_LIBS([getnetname],nsl,[
      $3=`echo " $LIBS" | sed "s! $btng_save_LIBS!!"`; # Action if found
      test -n "${$1}" &&	\
        $3="-L${$1}/lib ${$3}"	# Add path flag to output variable.
      BTNG_AC_LOG_VAR($3, Found nsl library flag)
      ],[
      BTNG_AC_LOG_VAR($3, Did not find nsl library flag)
      ])
    LIBS="$btng_save_LIBS";	# Restore global-use variable.
  fi
fi
])dnl



AC_DEFUN(BTNG_SUPPORT_NSL,[
dnl Support nsl library by setting the variables
dnl nsl_PREFIX, nsl_INCLUDES, and nsl_LIBS.
dnl Arg1: non-empty if you want the default to be on.
dnl
# Begin macro BTNG_SUPPORT_NSL

BTNG_ARG_WITH_ENV_WRAPPER(nsl, nsl_PREFIX,
ifelse($1,,
[  --with-nsl[=PATH]
			Use nsl and optionally specify where
			it is installed.],
[  --without-nsl	Do not use the nsl library.]),
ifelse($1,,unset nsl_PREFIX; test "${with_nsl+set}" = set && nsl_PREFIX=,nsl_PREFIX=))

BTNG_ARG_WITH_PREFIX(nsl-includes,nsl_INCLUDES,
[  --with-nsl-includes=STRING
			Specify the INCLUDES flags for nsl.
			If not specified, and --with-nsl=PATH is,
			this defaults to "-IPATH/include".])dnl

BTNG_ARG_WITH_PREFIX(nsl-libs,nsl_LIBS,
[  --with-nsl-libs=STRING
			Specify LIBS flags for nsl.
			If not specified, and --with-nsl=PATH is,
			this defaults to "-LPATH/lib -llapack -lblas".])dnl

BTNG_VAR_SET_NSL(nsl_PREFIX,nsl_INCLUDES,nsl_LIBS)
# End macro BTNG_SUPPORT_NSL
])
