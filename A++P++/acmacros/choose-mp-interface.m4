dnl $Id: choose-mp-interface.m4,v 1.4 2001/10/22 18:39:23 gunney Exp $

dnl File choose-mp-interface.m4

dnl Define macro to choose a message passing interface.
dnl Currently recognizing mpi and pvm.
dnl If mpi is chosen, AC_DEFINE macro ENABLE_MP_INTERFACE_MPI.
dnl If pvm is chosen, AC_DEFINE macro ENABLE_MP_INTERFACE_PVM.
dnl Defaults to mpi if none is given.
dnl If an argument is given, it is prepended to the C macro
dnl defined as to not contaminate the C macro namespace.
dnl Set enable_mp_interface to the name of the C macro used.

AC_DEFUN(BTNG_CHOOSE_MP_INTERFACE,[
# Begin macro BTNG_CHOOSE_MP_INTERFACE.

AC_ARG_ENABLE(mp-interface,[
--enable-mp-interface=...	Specify message-passing interface to enable.
				Defaults to MPI.  Argument is required if
				specified.],,enable_mp_interface=mpi)
BTNG_AC_LOG_VAR(enable_mp_interface)
# Map enable_mp_interface to the name of a C macro.
dnl Note that the first argument is prefixed to the C macro name.
enable_mp_interface=`echo "$enable_mp_interface" | tr '[A-Z]' '[a-z]'`
case "$enable_mp_interface" in
  mpi) cmacro_mp_interface=$1ENABLE_MP_INTERFACE_MPI ;;
  pvm) cmacro_mp_interface=$1ENABLE_MP_INTERFACE_PVM ;;
  *)
    AC_MSG_ERROR([
Message passing interface $enable_mp_interface not recognized.
Please use mpi or pvm.])
esac
BTNG_AC_LOG_VAR(cmacro_mp_interface)
AC_DEFINE_UNQUOTED(${cmacro_mp_interface})

# End macro BTNG_CHOOSE_MP_INTERFACE.
])
