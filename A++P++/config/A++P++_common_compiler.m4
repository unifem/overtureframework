dnl **********************************************************************
dnl * APP_GNU_COMPILE_MACRO()
dnl *
dnl * This macro will simply check whether the C compiler is gcc or the
dnl * C++ compiler is g++.  If so, it defines GNU.
dnl **********************************************************************

AC_DEFUN(APP_COMPILER_MACRO,
[
  AC_REQUIRE([AC_PROG_CC])
  AC_REQUIRE([AC_PROG_CXX])

dnl Initialize CFLAGS and CXXFLAGS to null strings here
   CFLAGS=""
   CXXFLAGS=""

dnl bjm(11 June 1999) This doesn't seem to work in all 
dnl cases, so we put the definintion of INLINE  
dnl below where it should work.
dnl 
dnl   if test "$GCC"=yes || test "$GXX"=yes; then
dnl     AC_DEFINE(INLINE, )
dnl   else
dnl     AC_DEFINE(INLINE,inline)
dnl   fi

  case $CXX in
      g++)
        AC_DEFINE([INLINE],[],[Define INLINE to be empty.])
        ;;
      *)
        AC_DEFINE([INLINE],inline,[Define INLINE to be inline so that inlining will be done at compile-time.])
        ;;
  esac

dnl bjm (14 June 1999) in P++/PADRE/PARTI, bsparti.h has a section where
dnl there is a choice between varargs.h and stdarg.h.  In most cases, we
dnl should use stdarg.h, but there may be some where 

  case $CXX in
      g++)
        ;;
      KCC | mpKCC)
        AC_DEFINE([STD_COMPLIANT_COMPILER],[],[PARTI requires this definition to select between varargs.h and stdarg.h.])
        ;;
      CC)
        ;;
  esac

])

