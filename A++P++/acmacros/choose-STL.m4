dnl Choose STL macro.
dnl Author: Brian Gunney
dnl
dnl This macro defines STL_DIR (the directory for STL header files) and
dnl STL_INCLUDES (the include flag for STL header files).
dnl AC_SUBST is not called for those variables--that is left to configure.in.
dnl Any previous definition of STL_DIR and STL_INCLUDES will be lost.


AC_DEFUN(BTNG_TRY_STL_PATH, [
# Start macro BTNG_TRY_CPP_STL
dnl This macro tries to run cpp on a sample STL program to see if the
dnl header files can be found.  It uses the extra CPPFLAGS specified in the
dnl first argument.  It executes the second argument if the cpp works.
dnl Otherwise, it executes the third argument.  It permanently changes
dnl CPPFLAGS if the cpp run works.  To find the right STL, it tries header
dnl files with .h and without.  Unfortunately, we require a priori knowlege
dnl here, assuming that the only variations in header file names are with and
dnl without .h.  If there are other naming conventions to consider, they
dnl can be added in this macro.
  AC_MSG_CHECKING(whether STL header files can be found using $1)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  padre_save_CPPFLAGS=$CPPFLAGS # Save CPPFLAGS to recover it if changes do not work.
  unset temp_pass
  if test -n "$1"; then CPPFLAGS="$1 $CPPFLAGS"; fi
  AC_TRY_CPP(
    [#include <vector.h>
    #include <list.h>],
    AC_MSG_RESULT([yes[,] found headers with .h])
    $2
    temp_pass=1
    ,
    $3
  )
  dnl End call to macro AC_TRY_CPP

  # All alternative checks proceed only if temp_pas is unset.

  if test -z "$temp_pass"; then
    AC_TRY_CPP(
      [#include <vector>
      #include <list>],
      AC_MSG_RESULT([yes[,] found headers without .h])
      $2
      temp_pass=1
      ,
      $3
    )
    dnl End call to macro AC_TRY_CPP
  fi

  # Restore CPPFLAGS if the new additions did not work.
  if test -z "$temp_pass"; then
    CPPFLAGS=$padre_save_CPPFLAGS
    AC_MSG_RESULT([no[,] cannot find STL headers with suffix .h or blank])
  fi
  AC_LANG_RESTORE
# Ends macro BTNG_TRY_STL_PATH
])	# End of BTNG_TRY_CPP_STL macro








AC_DEFUN(BTNG_CHOOSE_STL,
[

# Start macro BTNG_CHOOSE_STL

if test -z "$STL_INCLUDES"; then
# To prevent problems when this macro is used redundantly, this macro
# is not executed if STL_INCLUDES is already set.

dnl Four cases may occur in the excercise of the --with-STL option:
dnl 0: a path was specified
dnl    Test the path to see if it works.  Error if it does not work.
dnl 1: a "no" was specified (either by --with-STL=no or --without-STL
dnl    Test no -I to see if it works.  Error if it does not work.
dnl 2: a blank was specified by --with-STL
dnl    Test no -I to see if it works.  Try provided STL if it does not work.
dnl 3: unexcercised option (neither --with-STL nor --with-STL) specified.
AC_MSG_CHECKING(location of STL)
AC_ARG_WITH(STL,
  [  --with-STL=ARG ................... set STL include directory to ARG],
  if test "$with_STL" = "no"; then
    # User explicitly specified no special STL directory.
    AC_MSG_RESULT(never specify special STL location)
    STL_DIR=
    padre_stl_case=1
  else
    if test -n "$with_STL"; then
      # User has specified what STL to use.
      STL_DIR="$with_STL";
      AC_MSG_RESULT($STL_DIR)
      padre_stl_case=0
    else
      # Use the package-provided STL.
      STL_DIR="`cd $srcdir && pwd`/STL"
      AC_MSG_RESULT(provided $STL_DIR)
      padre_stl_case=2
    fi
  fi
  ,
  AC_MSG_RESULT(find automatically)
  padre_stl_case=3
)	dnl End call to AC_ARGV_WITH


echo "PADRE stl case: $padre_stl_case" 1>&5
# Test to see if we can preprocess STL.

if test "$padre_stl_case" = 0; then	# Check specific user-specified STL.
  STL_DIR=$with_STL	# STL_DIR is the path we are specifying.
  # Check to make sure we can preprocess a simple STL program.
  BTNG_TRY_STL_PATH(-I$STL_DIR, STL_INCLUDES="-I$STL_DIR")
  # It is an error if a user-specified STL does not work.
  if test -z "$STL_INCLUDES"; then
    AC_MSG_ERROR(Cannot compile simple STL program with the specified STL location $STL_DIR)
  fi
elif test "$padre_stl_case" = 1 || test "$padre_stl_case" = 3; then	# Check no special path.
  STL_DIR=	# STL_DIR is the path we are specifying.
  # Check to make sure we can preprocess a simple STL program.
  BTNG_TRY_STL_PATH(, STL_INCLUDES=" ")
  # It is an error if a user-specified no special STL path and it does not work.
  if test "$padre_stl_case" = 1; then
    AC_MSG_ERROR(Cannot compile simple STL program with the specified STL location $STL_DIR)
  fi
elif test "$padre_stl_case" = 2; then
  : Nothing is done for case 2.  We save it for below when we try the provided STL.
fi




# Try a last ditch STL implementation using an old version of STL.
# Note the very specific circumstance under which this is used.
# The old version of STL was not meant for in any other situation.
AC_REQUIRE([BTNG_INFO_CXX_ID])
if test -z "$STL_INCLUDES" \
  && echo "$host_os" | grep '^solaris' > /dev/null \
  && echo "$CXX_ID" | grep '^sunpro' > /dev/null \
  && echo "$CXX_VERSION" | grep '^0x420' > /dev/null; then
  BTNG_AC_LOG("checking if old version of STL works")
  # echo "PADRE stl trying the old package-provided"
  # Try the old package-provided STL.
  STL_DIR="`cd $srcdir && pwd`/STL-link"	# STL_DIR is the path we are specifying.
  # Check to make sure we can preprocess a simple STL program.
  BTNG_TRY_STL_PATH(-I$STL_DIR, STL_INCLUDES="-I$STL_DIR")
else
  BTNG_AC_LOG(["not checking old version of STL because system is $host_os, $CXX_ID, $CXX_VERSION"])
fi	# End block checking if we should try the STL included in the package.


# Once STL is found, put specify it in the include paths.
if test -n "$STL_INCLUDES"; then
  : INCLUDES="$STL_INCLUDES $INCLUDES"
else
  AC_MSG_ERROR(Cannot find working STL)
fi



fi	# End block checking STL_INCLUDES


# Ends macro BTNG_CHOOSE_STL
]) dnl End of BTNG_CHOOSE_STL macro.






