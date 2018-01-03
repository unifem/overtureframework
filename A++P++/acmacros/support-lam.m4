dnl $Id: support-lam.m4,v 1.3 2001/10/22 18:39:23 gunney Exp $

dnl Define a macro for supporting LAM/MPI.


AC_DEFUN(BTNG_SUPPORT_LAM,[

# Begin BTNG_SUPPORT_LAM
# Defines LAM_PREFIX LAM_INCLUDES and LAM_LIBS if with-lam is specified.

AC_ARG_WITH(lam,
[ --with-lam=PATH  Use LAM/MPI and optionally specify where LAM is installed.],
, with_lam=no)

BTNG_AC_LOG(with_lam is $with_lam)
test ! "$with_lam" = no && lam_LIBS='-lmpi -llam'
case "$with_lam" in
  no)
    BTNG_AC_LOG(Not setting up for LAM)
    : Do nothing
  ;;
  yes)
    # LAM install path was not specified.
    # If there is an mpi2c++ directory in one of the standard include places,
    # LAM header files are there.
    BTNG_AC_LOG(Looking for LAM installation)
    for dir in /usr /usr/local; do
      if test -d ${dir}/include/mpi2c++; then
        lam_INCLUDES="-I${dir}/include/mpi2c++"
	BTNG_AC_LOG(Found mpi2c++ include directory ${dir}/include/mpi2c++)
        break
      fi
    done
  ;;
  *)
    # LAM install path was specified.
    BTNG_AC_LOG(Expect LAM installation in $with_lam)
    lam_PREFIX=$with_lam
    lam_INCLUDES='-I${lam_PREFIX}/include -I${lam_PREFIX}/include/mpi2c++'
    lam_LIBS='-L${lam_PREFIX}/lib'" ${lam_LIBS}"
    BTNG_AC_LOG(Set lam_INCLUDES to $lam_INCLUDES)
  ;;
esac
BTNG_AC_LOG(lam_INCLUDES is $lam_INCLUDES)
BTNG_AC_LOG(lam_LIBS is $lam_LIBS)
#  if test ! "$with_lam" = yes; then
#    # LAM install path was specified.
#      lam_PREFIX=$with_lam
#      lam_INCLUDES='-I${lam_PREFIX}/include -I${lam_PREFIX}/include/mpi2c++'
#      lam_LIBS='-L${lam_PREFIX}/lib'" ${lam_LIBS}"
#      BTNG_AC_LOG(Set lam_INCLUDES to $lam_INCLUDES)
#  else
#    # LAM install path was not specified.
#    # If there is an mpi2c++ directory in one of the standard include places,
#    # LAM header files are there.
#    for dir in /usr /usr/local; do
#      if test -d ${dir}/include/mpi2c++; then
#        lam_INCLUDES="-I${dir}/include/mpi2c++"
#	BTNG_AC_LOG(Set lam_INCLUDES to $lam_INCLUDES)
#        break
#      fi
#    done
#  fi

# END BTNG_SUPPORT_LAM

])dnl End definition of BTNG_SUPPORT_LAM
