dnl $Id: support-fortran.m4,v 1.2 2001/10/24 22:06:00 gunney Exp $
dnl
dnl This file contains functions to crudely determine
dnl loader flags for linking C/C++ and Fortran.

AC_DEFUN(BTNG_LIB_C_FORTRAN, [
dnl Use this macro to set fortran_LIBS to the library flgas
dnl needed to link in fortran from C codes.
dnl
dnl The macro AC_F77_LIBRARY_LDFLAGS, which was installed
dnl with autoconf, did not work for me.  BTNG.
# Start macro BTNG_LIB_C_FORTRAN

if test -z "$fortran_LIBS" ; then
  # Guessing the correct flags is just based on past experience.
  # But it seems to work well in this case.  We rely heavily on
  # the name of compiler, so compilers with strange names will
  # not successfily be guessed.
  AC_REQUIRE([BTNG_INFO_CC_CXX_ID])
  case "$CXX_ID" in
    gnu)	# The GNU compiler.
      BTNG_AC_LOG(About to check for RedHat gnu-2.96 compiler)
      BTNG_AC_LOG_VAR(CXX_ID CXX_VERSION)
      case "$host_os" in
	# linux*)		fortran_LIBS=
	linux*)		fortran_LIBS='-lg2c'
        if test "$CXX_VERSION" = "2.96" ; then
	  # Fix problem with RedHat distribution of experimental
	  # gcc version 2.96 not knowing where it put the g2c library.
          [ extra_path=`which $CXX | sed 's:bin/[^/]*$:lib:'` ]
      	  BTNG_AC_LOG_VAR(extra_path)
	  test -d "$extra_path/gcc-lib/i386-redhat-linux/2.96" && extra_path="$extra_path/gcc-lib/i386-redhat-linux/2.96"
      	  BTNG_AC_LOG_VAR(extra_path)
	  if test -d "$extra_path" ; then
	    BTNG_AC_LOG(Adding $extra_path to fortran_LIBS)
	    fortran_LIBS="-L$extra_path $fortran_LIBS"
	  fi
        fi
	;;
	solaris*)	fortran_LIBS='-lg2c' ;;
      esac
    ;;
    kai)	# The KCC compiler runs on many platforms.
      # This compiler usually uses the native loader so use the
      # flags from the native compiler/loader.
      case "$host_os" in
	osf*)		fortran_LIBS='-lfor' ;;
	linux*)		fortran_LIBS='-lg2c' ;;
	sun*|solaris*)	fortran_LIBS='-lF77 -lM77 -lV77 -lsunmath' ;;
	sgi)		fortran_LIBS='-lftn' ;;
      esac
    ;;
    sgi)	# SGI compiler.
        fortran_LIBS='-lftn'
    ;;
    sunpro)		# Sunpro compiler.
	fortran_LIBS='-lF77 -lM77 -lV77 -lsunmath'
    ;;
    dec)	# The DEC compiler.
	fortran_LIBS='-lfor'
    ;;
  esac
fi

# End macro BTNG_LIB_C_FORTRAN
])dnl	End definition of BTNG_LIB_C_FORTRAN




AC_DEFUN(BTNG_SUPPORT_FORTRAN_FROM_C, [
# Start macro BTNG_SUPPORT_FORTRAN_FROM_C
dnl Use this macro if you have to link with Fortran codes.
dnl The macro AC_F77_LIBRARY_LDFLAGS, which was installed
dnl with autoconf, did not work for me.  BTNG.
AC_MSG_CHECKING([how to set library flags for linking to Fortran code])
AC_ARG_WITH(fortran-libs,
[--with-fortran-libs=LIBS
		Specify library flags for linking with Fortran code.],
fortran_LIBS=$with_fortran_libs)
BTNG_LIB_C_FORTRAN
AC_MSG_RESULT([$fortran_LIBS])
AC_SUBST(fortran_LIBS)
# End macro BTNG_SUPPORT_FORTRAN_FROM_C
])dnl	End definition of BTNG_SUPPORT_FORTRAN_FROM_C
