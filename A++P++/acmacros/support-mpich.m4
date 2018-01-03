dnl Define a macro for supporting MPICH.


AC_DEFUN(BTNG_SUPPORT_MPICH,[
dnl Support mpich libraries by setting the variables
dnl mpich_PREFIX, mpich_INCLUDES, and mpich_LIBS.
dnl Arg1: empty if you want the default to be on.
dnl
# Begin macro BTNG_SUPPORT_MPICH
BTNG_ARG_WITH_ENV_WRAPPER(mpich, mpich_PREFIX,
ifelse($1,,
[  --with-mpich=PATH	Use MPICH and optionally specify where it is installed.],
[  --without-mpich	Do not use the MPICH library.]),
ifelse($1,,mpich_PREFIX=,unset mpich_PREFIX))
BTNG_VAR_SET_MPICH(mpich_PREFIX,mpich_INCLUDES,mpich_LIBS)
BTNG_AC_LOG_VAR(mpich_PREFIX mpich_INCLUDES mpich_LIBS)
# End macro BTNG_SUPPORT_MPICH
])


AC_DEFUN(BTNG_VAR_SET_MPICH,[
dnl Provides support for the blas and lapack libraries.
dnl
dnl Arguments are:
dnl 1. Name of variable to set to path where mpich is installed.
dnl    Nothig is done if this variable is unset.
dnl 2. Name of the INCLUDES variable similar to the automake INCLUDES variable.
dnl    This variable is modified ONLY if it is NOT set.
dnl 3. Name of the LIBS variable similar to the automake LIBS variable.
dnl    This variable is modified ONLY if it is NOT set.
dnl
dnl If arg1 is defined, assume that the user wants blas and lapack
dnl support.  Do so by assigning arg2 and arg3 if they are not defined.
dnl
# Begin macro BTNG_VAR_SET_MPICH
if test "${$1+set}" = set ; then
  if test ! "${$2+set}" = set ; then
    if test -n "${$1}" ; then
      $2="-I${$1}/include"
      if test -d ${$1}/include/mpi2c++; then
        $2="${$2} -I${$1}/include/mpi2c++"
	BTNG_AC_LOG(Found mpi2c++ include directory ${$1}/include/mpi2c++)
      fi
    fi
  fi
  if test ! "${$3+set}" = set ; then
    $3="-lmpich"
    if test -n "${$1}" ; then
      for i in ${$3} ; do
	tmp_name=`echo $i | sed 's/^-l//'`
        if test ! -f "${$1}/lib/lib${tmp_name}.a" && \
          test ! -f "${$1}/lib/lib${tmp_name}.so"; then
          AC_MSG_WARN(Library file ${tmp_name} seems to be missing from ${$1}.)
        fi
      done
      $3="-L${$1}/lib ${$3}"
    fi
  fi
fi
# End macro BTNG_VAR_SET_MPICH
])dnl


AC_DEFUN(BTNG_SUPPORT_MPICH_OBSOLETE,[

# Begin BTNG_SUPPORT_MPICH
# Defines mpich_PREFIX mpich_INCLUDES and mpich_LIBS if with-mpich is specified.

BTNG_ARG_WITH_ENV_WRAPPER(mpich, mpich_PREFIX,
[  --with-mpich=PATH  Use MPICH and optionally specify where MPICH is installed.],
with_mpich=no)

BTNG_AC_LOG_VAR(with_mpich)
case "$with_mpich" in
  no)
    BTNG_AC_LOG(Not setting up for MPICH)
    unset mpich_PREFIX mpich_INCLUDES mpich_LIBS
  ;;
  yes)
    # with-mpich was given, but install path was not specified.
    # If there is an mpi2c++ directory in one of the standard include places,
    # MPICH header files are there.
    BTNG_AC_LOG(Looking for MPICH installation)
    for dir in /usr /usr/local; do
      if test -d ${dir}/include/mpi2c++; then
        mpich_INCLUDES="-I${dir}/include/mpi2c++"
	BTNG_AC_LOG(Found mpi2c++ include directory ${dir}/include/mpi2c++)
        break
      fi
    done
  ;;
  *)
    # MPICH install path was specified.
    BTNG_AC_LOG(Expect MPICH installation in $with_mpich)
    mpich_PREFIX=$with_mpich
    mpich_INCLUDES="-I${mpich_PREFIX}/include -I${mpich_PREFIX}/include/mpi2c++"
    mpich_LIBS='-L'"${mpich_PREFIX}"'/lib'" ${mpich_LIBS}"
    BTNG_AC_LOG(Set mpich_INCLUDES to $mpich_INCLUDES)
  ;;
esac
mpich_LIBS="$mpich_LIBS -lmpich"
BTNG_AC_LOG_VAR(mpich_PREFIX mpich_INCLUDES mpich_LIBS)

BTNG_PATH_PROG(mpirun, mpich_MPIRUN,
[
# Prefer the mpirun under mpich_PREFIX.
test -n "$mpich_PREFIX" &&	\
test -x "$mpich_PREFIX/bin/mpirun" &&	\
mpich_MPIRUN="$mpich_PREFIX/bin/mpirun"
])
if test -n "$mpich_MPIRUN"; then
AC_PATH_PROG(mpirun,mpich_MPIRUN)
fi

# END BTNG_SUPPORT_MPICH

])dnl End definition of BTNG_SUPPORT_MPICH
