dnl $Id: specify-ar.m4,v 1.3 2001/10/22 18:39:23 gunney Exp $

dnl Define macro BTNG_PROG_AR

dnl This macro finds the ar program and sets flags for using it.
dnl It sets the environment:
dnl AR: name of ar program
dnl AR_UPDATE: command to update a library, usually $AR -r -u -v
dnl AR_EXTRACT: command to extract object files from library, usually $AR -o


AC_DEFUN(BTNG_PROG_AR, [

# Start macro BTNG_PROG_AR

  # Get the location of ar.
  if test -z "$AR"; then
    AC_ARG_WITH(ar,
      [--with-ar=PROGRAM      Specify the library archive program],
      AR=$withval,
      [
      # Automatically find ar program.
      # Current concern is that some ar does not support xo option.
      # Try to find one that does support it.  If cannot find such
      # use "ar".
      # Find any ar to start with.
      AC_CHECK_PROG(AR,ar,ar)
      # Look for a better AR.
      AC_MSG_CHECKING(for ar which supports o option)
      BTNG_PATH_FINDALLPROGS(ar,$PATH)
      # Build a library for extraction test.
      touch conftest.o
      $AR -r conftest.a conftest.o > /dev/null 2>&1
      # Check all ar in ar_paths in turn.
      new_AR=
      for i in $ar_paths; do
        $i xo conftest.a > /dev/null 2>&1
        if test $? = 0; then
	  new_AR=$i
	  break
        fi
      done
      rm -f conftest.o conftest.a
      # If no ar supports o option, use default ar.
      if test -n "$new_AR"; then
        AR=$new_AR
	AC_MSG_RESULT($AR)
      else
	AC_MSG_RESULT([none defaulting to $AR])
      fi
      ]
    )	dnl End call to AC_ARG_WITH
  fi

  # Get the ar update command.

  # Treat special case of KCC compiler which wants
  # to itself as the library archive program.
  AC_REQUIRE([BTNG_INFO_CXX_ID])
  if test "$CXX_ID" = kai; then
    AR_UPDATE="$CXX -o"
  fi

  if test -z "$AR_UPDATE"; then
    AC_ARG_WITH(ar-update,
      [--with-ar-update=COMMAND      Specify command to update (or create) library file],
      AR_UPDATE_FLAGS=$withval, [

        if test -z "$AR_UPDATE_FLAGS"; then
          AC_ARG_WITH(ar-update-flags,
            [--with-ar-update-flags=FLAGS      Specify ar flags to update (or create) library file],
            AR_UPDATE_FLAGS=$withval, [
              AC_MSG_CHECKING(whether $AR accepts ruv option to update library file)
              touch conftest.o
              $AR ruv conftest.a conftest.o > /dev/null 2>&1
              if test $? = 0; then
                AC_MSG_RESULT(yes)
                AR_UPDATE_FLAGS=ruv
              else
                AC_MSG_RESULT([no, using -r -u -v])
                AR_UPDATE_FLAGS="-r -u -v"
              fi
              rm -f conftest.o conftest.a
            ]	dnl End action-if-not-given block
          )	dnl End call to AC_ARG_WITH
        fi
        AR_UPDATE="$AR $AR_UPDATE_FLAGS"

      ]	dnl End action-if-not-given block
    )	dnl End call to AC_ARG_WITH
  fi


  # Get the ar extract command.
  if test -z "$AR_EXTRACT"; then
    AC_ARG_WITH(ar-extract,
      [--with-ar-extract=COMMAND      Specify command to extract object files from library file],
      AR_EXTRACT=$withval, [

        if test -z "$AR_EXTRACT_FLAGS"; then
          AC_ARG_WITH(ar-extract-flags,
            [--with-ar-extract-flags=FLAGS      Specify ar flags to extract object files from library file],
            AR_EXTRACT_FLAGS=$withval, [

              AC_MSG_CHECKING(whether $AR accepts o option to preserve file time)
              touch conftest.o
              $AR_UPDATE conftest.a conftest.o > /dev/null 2>&1
              $AR xo conftest.a > /dev/null 2>&1
              if test $? = 0; then
                AC_MSG_RESULT(yes)
                AR_EXTRACT_FLAGS=xo
              else
                AC_MSG_RESULT([no, using x])
                AR_EXTRACT_FLAGS=x
              fi
              rm -f conftest.o conftest.a

            ]	dnl End action-if-not-given block
          )	dnl End call to AC_ARG_WITH
        fi
        AR_EXTRACT="$AR $AR_EXTRACT_FLAGS"

      ]	dnl End action-if-not-given block
    )	dnl End call to AC_ARG_WITH
  fi


  AC_SUBST(AR)
  AC_SUBST(AR_UPDATE_FLAGS)
  AC_SUBST(AR_UPDATE)
  AC_SUBST(AR_EXTRACT_FLAGS)
  AC_SUBST(AR_EXTRACT)

# End macro BTNG_PROG_AR

])	dnl	End definition of BTNG_PROG_AR
