dnl $Id: support-hypre.m4,v 1.7 2001/10/22 18:39:23 gunney Exp $

dnl Define macros for supporting HYPRE.


AC_DEFUN(BTNG_SUPPORT_HYPRE,[
dnl Support hypre libraries by setting the variables
dnl hypre_PREFIX, hypre_INCLUDES, and hypre_LIBS.
dnl Arg1: empty if you want the default to be off.
dnl
# Begin macro BTNG_SUPPORT_HYPRE
BTNG_ARG_WITH_ENV_WRAPPER(hypre, hypre_PREFIX,
ifelse($1,,
[  --with-hypre[=PATH]	Use HYPRE and optionally specify where it is installed.],
[  --without-hypre	Do not use the HYPRe library.]),
ifelse($1,,unset hypre_PREFIX,hypre_PREFIX=))
BTNG_VAR_SET_HYPRE(hypre_PREFIX,hypre_INCLUDES,hypre_LIBS)
BTNG_AC_LOG_VAR(hypre_PREFIX hypre_INCLUDES hypre_LIBS)
# End macro BTNG_SUPPORT_HYPRE
])


AC_DEFUN(BTNG_VAR_SET_HYPRE,[
dnl Provides support for the blas and lapack libraries.
dnl
dnl Arguments are:
dnl 1. Name of variable to set to path where hypre is installed.
dnl    Nothig is done if this variable is unset.
dnl 2. Name of the INCLUDES variable similar to the automake INCLUDES variable.
dnl    This variable is modified ONLY if it is NOT set.
dnl 3. Name of the LIBS variable similar to the automake LIBS variable.
dnl    This variable is modified ONLY if it is NOT set.
dnl
dnl If arg1 is defined, assume that the user wants blas and lapack
dnl support.  Do so by assigning arg2 and arg3 if they are not defined.
dnl
# Begin macro BTNG_VAR_SET_HYPRE
if test "${$1+set}" = set ; then
  if test ! "${$2+set}" = set ; then
    test -n "${$1}" && $2="-I${$1}/include"
  fi
  if test ! "${$3+set}" = set ; then
    $3="-lHYPRE_struct_ls -lHYPRE_struct_mv -lHYPRE_utilities"
    if test -n "${$1}" ; then
      for i in ${$3} ; do
	tmp_name=`echo $i | sed 's/^-l//'`
        if test ! -f "${$1}/lib/lib${tmp_name}.a" && \
          test ! -f "${$1}/lib/lib${tmp_name}.so"; then
          AC_MSG_WARN(Library file for ${tmp_name} is missing from ${$1}/lib.)
        fi
      done
      $3="-L${$1}/lib ${$3}"
    fi
  fi
fi
# End macro BTNG_VAR_SET_HYPRE
])dnl
