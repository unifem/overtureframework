# aclocal.m4 generated automatically by aclocal 1.5

# Copyright 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# **********************************************************************
# This is the ONE AND ONLY PLACE to change the version number for A++P++.
# All it does is set AXXPXX_VERSION.
AC_DEFUN(AXXPXX_VAR_VERSION,[
# Begin macro AXXPXX_VAR_VERSION
# Set the version number for A++P++.
AXXPXX_VERSION=0.7.9d
# If you change the version number,
# please remember to document it in ChangeLog.
# End macro AXXPXX_VAR_VERSION
])


# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 5

# There are a few dirty hacks below to avoid letting `AC_PROG_CC' be
# written in clear, in which case automake, when reading aclocal.m4,
# will think it sees a *use*, and therefore will trigger all it's
# C support machinery.  Also note that it means that autoscan, seeing
# CC etc. in the Makefile, will ask for an AC_PROG_CC use...


# We require 2.13 because we rely on SHELL being computed by configure.
AC_PREREQ([2.13])

# AC_PROVIDE_IFELSE(MACRO-NAME, IF-PROVIDED, IF-NOT-PROVIDED)
# -----------------------------------------------------------
# If MACRO-NAME is provided do IF-PROVIDED, else IF-NOT-PROVIDED.
# The purpose of this macro is to provide the user with a means to
# check macros which are provided without letting her know how the
# information is coded.
# If this macro is not defined by Autoconf, define it here.
ifdef([AC_PROVIDE_IFELSE],
      [],
      [define([AC_PROVIDE_IFELSE],
              [ifdef([AC_PROVIDE_$1],
                     [$2], [$3])])])


# AM_INIT_AUTOMAKE(PACKAGE,VERSION, [NO-DEFINE])
# ----------------------------------------------
AC_DEFUN([AM_INIT_AUTOMAKE],
[AC_REQUIRE([AC_PROG_INSTALL])dnl
# test to see if srcdir already configured
if test "`CDPATH=:; cd $srcdir && pwd`" != "`pwd`" &&
   test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run \"make distclean\" there first])
fi

# Define the identity of the package.
PACKAGE=$1
AC_SUBST(PACKAGE)dnl
VERSION=$2
AC_SUBST(VERSION)dnl
ifelse([$3],,
[AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package])])

# Autoconf 2.50 wants to disallow AM_ names.  We explicitly allow
# the ones we care about.
ifdef([m4_pattern_allow],
      [m4_pattern_allow([^AM_[A-Z]+FLAGS])])dnl

# Autoconf 2.50 always computes EXEEXT.  However we need to be
# compatible with 2.13, for now.  So we always define EXEEXT, but we
# don't compute it.
AC_SUBST(EXEEXT)
# Similar for OBJEXT -- only we only use OBJEXT if the user actually
# requests that it be used.  This is a bit dumb.
: ${OBJEXT=o}
AC_SUBST(OBJEXT)

# Some tools Automake needs.
AC_REQUIRE([AM_SANITY_CHECK])dnl
AC_REQUIRE([AC_ARG_PROGRAM])dnl
AM_MISSING_PROG(ACLOCAL, aclocal)
AM_MISSING_PROG(AUTOCONF, autoconf)
AM_MISSING_PROG(AUTOMAKE, automake)
AM_MISSING_PROG(AUTOHEADER, autoheader)
AM_MISSING_PROG(MAKEINFO, makeinfo)
AM_MISSING_PROG(AMTAR, tar)
AM_PROG_INSTALL_SH
AM_PROG_INSTALL_STRIP
# We need awk for the "check" target.  The system "awk" is bad on
# some platforms.
AC_REQUIRE([AC_PROG_AWK])dnl
AC_REQUIRE([AC_PROG_MAKE_SET])dnl
AC_REQUIRE([AM_DEP_TRACK])dnl
AC_REQUIRE([AM_SET_DEPDIR])dnl
AC_PROVIDE_IFELSE([AC_PROG_][CC],
                  [_AM_DEPENDENCIES(CC)],
                  [define([AC_PROG_][CC],
                          defn([AC_PROG_][CC])[_AM_DEPENDENCIES(CC)])])dnl
AC_PROVIDE_IFELSE([AC_PROG_][CXX],
                  [_AM_DEPENDENCIES(CXX)],
                  [define([AC_PROG_][CXX],
                          defn([AC_PROG_][CXX])[_AM_DEPENDENCIES(CXX)])])dnl
])

#
# Check to make sure that the build environment is sane.
#

# serial 3

# AM_SANITY_CHECK
# ---------------
AC_DEFUN([AM_SANITY_CHECK],
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftest.file
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftest.file 2> /dev/null`
   if test "$[*]" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftest.file`
   fi
   rm -f conftest.file
   if test "$[*]" != "X $srcdir/configure conftest.file" \
      && test "$[*]" != "X conftest.file $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "$[2]" = conftest.file
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
AC_MSG_RESULT(yes)])


# serial 2

# AM_MISSING_PROG(NAME, PROGRAM)
# ------------------------------
AC_DEFUN([AM_MISSING_PROG],
[AC_REQUIRE([AM_MISSING_HAS_RUN])
$1=${$1-"${am_missing_run}$2"}
AC_SUBST($1)])


# AM_MISSING_HAS_RUN
# ------------------
# Define MISSING if not defined so far and test if it supports --run.
# If it does, set am_missing_run to use it, otherwise, to nothing.
AC_DEFUN([AM_MISSING_HAS_RUN],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
test x"${MISSING+set}" = xset || MISSING="\${SHELL} $am_aux_dir/missing"
# Use eval to expand $SHELL
if eval "$MISSING --run true"; then
  am_missing_run="$MISSING --run "
else
  am_missing_run=
  am_backtick='`'
  AC_MSG_WARN([${am_backtick}missing' script is too old or missing])
fi
])

# AM_AUX_DIR_EXPAND

# For projects using AC_CONFIG_AUX_DIR([foo]), Autoconf sets
# $ac_aux_dir to `$srcdir/foo'.  In other projects, it is set to
# `$srcdir', `$srcdir/..', or `$srcdir/../..'.
#
# Of course, Automake must honor this variable whenever it calls a
# tool from the auxiliary directory.  The problem is that $srcdir (and
# therefore $ac_aux_dir as well) can be either absolute or relative,
# depending on how configure is run.  This is pretty annoying, since
# it makes $ac_aux_dir quite unusable in subdirectories: in the top
# source directory, any form will work fine, but in subdirectories a
# relative path needs to be adjusted first.
#
# $ac_aux_dir/missing
#    fails when called from a subdirectory if $ac_aux_dir is relative
# $top_srcdir/$ac_aux_dir/missing
#    fails if $ac_aux_dir is absolute,
#    fails when called from a subdirectory in a VPATH build with
#          a relative $ac_aux_dir
#
# The reason of the latter failure is that $top_srcdir and $ac_aux_dir
# are both prefixed by $srcdir.  In an in-source build this is usually
# harmless because $srcdir is `.', but things will broke when you
# start a VPATH build or use an absolute $srcdir.
#
# So we could use something similar to $top_srcdir/$ac_aux_dir/missing,
# iff we strip the leading $srcdir from $ac_aux_dir.  That would be:
#   am_aux_dir='\$(top_srcdir)/'`expr "$ac_aux_dir" : "$srcdir//*\(.*\)"`
# and then we would define $MISSING as
#   MISSING="\${SHELL} $am_aux_dir/missing"
# This will work as long as MISSING is not called from configure, because
# unfortunately $(top_srcdir) has no meaning in configure.
# However there are other variables, like CC, which are often used in
# configure, and could therefore not use this "fixed" $ac_aux_dir.
#
# Another solution, used here, is to always expand $ac_aux_dir to an
# absolute PATH.  The drawback is that using absolute paths prevent a
# configured tree to be moved without reconfiguration.

AC_DEFUN([AM_AUX_DIR_EXPAND], [
# expand $ac_aux_dir to an absolute path
am_aux_dir=`CDPATH=:; cd $ac_aux_dir && pwd`
])

# AM_PROG_INSTALL_SH
# ------------------
# Define $install_sh.
AC_DEFUN([AM_PROG_INSTALL_SH],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
install_sh=${install_sh-"$am_aux_dir/install-sh"}
AC_SUBST(install_sh)])

# One issue with vendor `install' (even GNU) is that you can't
# specify the program used to strip binaries.  This is especially
# annoying in cross-compiling environments, where the build's strip
# is unlikely to handle the host's binaries.
# Fortunately install-sh will honor a STRIPPROG variable, so we
# always use install-sh in `make install-strip', and initialize
# STRIPPROG with the value of the STRIP variable (set by the user).
AC_DEFUN([AM_PROG_INSTALL_STRIP],
[AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
INSTALL_STRIP_PROGRAM="\${SHELL} \$(install_sh) -c -s"
AC_SUBST([INSTALL_STRIP_PROGRAM])])

# serial 4						-*- Autoconf -*-



# There are a few dirty hacks below to avoid letting `AC_PROG_CC' be
# written in clear, in which case automake, when reading aclocal.m4,
# will think it sees a *use*, and therefore will trigger all it's
# C support machinery.  Also note that it means that autoscan, seeing
# CC etc. in the Makefile, will ask for an AC_PROG_CC use...



# _AM_DEPENDENCIES(NAME)
# ---------------------
# See how the compiler implements dependency checking.
# NAME is "CC", "CXX" or "OBJC".
# We try a few techniques and use that to set a single cache variable.
#
# We don't AC_REQUIRE the corresponding AC_PROG_CC since the latter was
# modified to invoke _AM_DEPENDENCIES(CC); we would have a circular
# dependency, and given that the user is not expected to run this macro,
# just rely on AC_PROG_CC.
AC_DEFUN([_AM_DEPENDENCIES],
[AC_REQUIRE([AM_SET_DEPDIR])dnl
AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
AC_REQUIRE([AM_MAKE_INCLUDE])dnl
AC_REQUIRE([AM_DEP_TRACK])dnl

ifelse([$1], CC,   [depcc="$CC"   am_compiler_list=],
       [$1], CXX,  [depcc="$CXX"  am_compiler_list=],
       [$1], OBJC, [depcc="$OBJC" am_compiler_list='gcc3 gcc']
       [$1], GCJ,  [depcc="$GCJ"  am_compiler_list='gcc3 gcc'],
                   [depcc="$$1"   am_compiler_list=])

AC_CACHE_CHECK([dependency style of $depcc],
               [am_cv_$1_dependencies_compiler_type],
[if test -z "$AMDEP_TRUE" && test -f "$am_depcomp"; then
  # We make a subdir and do the tests there.  Otherwise we can end up
  # making bogus files that we don't know about and never remove.  For
  # instance it was reported that on HP-UX the gcc test will end up
  # making a dummy file named `D' -- because `-MD' means `put the output
  # in D'.
  mkdir conftest.dir
  # Copy depcomp to subdir because otherwise we won't find it if we're
  # using a relative directory.
  cp "$am_depcomp" conftest.dir
  cd conftest.dir

  am_cv_$1_dependencies_compiler_type=none
  if test "$am_compiler_list" = ""; then
     am_compiler_list=`sed -n ['s/^#*\([a-zA-Z0-9]*\))$/\1/p'] < ./depcomp`
  fi
  for depmode in $am_compiler_list; do
    # We need to recreate these files for each test, as the compiler may
    # overwrite some of them when testing with obscure command lines.
    # This happens at least with the AIX C compiler.
    echo '#include "conftest.h"' > conftest.c
    echo 'int i;' > conftest.h
    echo "${am__include} ${am__quote}conftest.Po${am__quote}" > confmf

    case $depmode in
    nosideeffect)
      # after this tag, mechanisms are not by side-effect, so they'll
      # only be used when explicitly requested
      if test "x$enable_dependency_tracking" = xyes; then
	continue
      else
	break
      fi
      ;;
    none) break ;;
    esac
    # We check with `-c' and `-o' for the sake of the "dashmstdout"
    # mode.  It turns out that the SunPro C++ compiler does not properly
    # handle `-M -o', and we need to detect this.
    if depmode=$depmode \
       source=conftest.c object=conftest.o \
       depfile=conftest.Po tmpdepfile=conftest.TPo \
       $SHELL ./depcomp $depcc -c conftest.c -o conftest.o >/dev/null 2>&1 &&
       grep conftest.h conftest.Po > /dev/null 2>&1 &&
       ${MAKE-make} -s -f confmf > /dev/null 2>&1; then
      am_cv_$1_dependencies_compiler_type=$depmode
      break
    fi
  done

  cd ..
  rm -rf conftest.dir
else
  am_cv_$1_dependencies_compiler_type=none
fi
])
$1DEPMODE="depmode=$am_cv_$1_dependencies_compiler_type"
AC_SUBST([$1DEPMODE])
])


# AM_SET_DEPDIR
# -------------
# Choose a directory name for dependency files.
# This macro is AC_REQUIREd in _AM_DEPENDENCIES
AC_DEFUN([AM_SET_DEPDIR],
[rm -f .deps 2>/dev/null
mkdir .deps 2>/dev/null
if test -d .deps; then
  DEPDIR=.deps
else
  # MS-DOS does not allow filenames that begin with a dot.
  DEPDIR=_deps
fi
rmdir .deps 2>/dev/null
AC_SUBST(DEPDIR)
])


# AM_DEP_TRACK
# ------------
AC_DEFUN([AM_DEP_TRACK],
[AC_ARG_ENABLE(dependency-tracking,
[  --disable-dependency-tracking Speeds up one-time builds
  --enable-dependency-tracking  Do not reject slow dependency extractors])
if test "x$enable_dependency_tracking" != xno; then
  am_depcomp="$ac_aux_dir/depcomp"
  AMDEPBACKSLASH='\'
fi
AM_CONDITIONAL([AMDEP], [test "x$enable_dependency_tracking" != xno])
pushdef([subst], defn([AC_SUBST]))
subst(AMDEPBACKSLASH)
popdef([subst])
])

# Generate code to set up dependency tracking.
# This macro should only be invoked once -- use via AC_REQUIRE.
# Usage:
# AM_OUTPUT_DEPENDENCY_COMMANDS

#
# This code is only required when automatic dependency tracking
# is enabled.  FIXME.  This creates each `.P' file that we will
# need in order to bootstrap the dependency handling code.
AC_DEFUN([AM_OUTPUT_DEPENDENCY_COMMANDS],[
AC_OUTPUT_COMMANDS([
test x"$AMDEP_TRUE" != x"" ||
for mf in $CONFIG_FILES; do
  case "$mf" in
  Makefile) dirpart=.;;
  */Makefile) dirpart=`echo "$mf" | sed -e 's|/[^/]*$||'`;;
  *) continue;;
  esac
  grep '^DEP_FILES *= *[^ #]' < "$mf" > /dev/null || continue
  # Extract the definition of DEP_FILES from the Makefile without
  # running `make'.
  DEPDIR=`sed -n -e '/^DEPDIR = / s///p' < "$mf"`
  test -z "$DEPDIR" && continue
  # When using ansi2knr, U may be empty or an underscore; expand it
  U=`sed -n -e '/^U = / s///p' < "$mf"`
  test -d "$dirpart/$DEPDIR" || mkdir "$dirpart/$DEPDIR"
  # We invoke sed twice because it is the simplest approach to
  # changing $(DEPDIR) to its actual value in the expansion.
  for file in `sed -n -e '
    /^DEP_FILES = .*\\\\$/ {
      s/^DEP_FILES = //
      :loop
	s/\\\\$//
	p
	n
	/\\\\$/ b loop
      p
    }
    /^DEP_FILES = / s/^DEP_FILES = //p' < "$mf" | \
       sed -e 's/\$(DEPDIR)/'"$DEPDIR"'/g' -e 's/\$U/'"$U"'/g'`; do
    # Make sure the directory exists.
    test -f "$dirpart/$file" && continue
    fdir=`echo "$file" | sed -e 's|/[^/]*$||'`
    $ac_aux_dir/mkinstalldirs "$dirpart/$fdir" > /dev/null 2>&1
    # echo "creating $dirpart/$file"
    echo '# dummy' > "$dirpart/$file"
  done
done
], [AMDEP_TRUE="$AMDEP_TRUE"
ac_aux_dir="$ac_aux_dir"])])

# AM_MAKE_INCLUDE()
# -----------------
# Check to see how make treats includes.
AC_DEFUN([AM_MAKE_INCLUDE],
[am_make=${MAKE-make}
cat > confinc << 'END'
doit:
	@echo done
END
# If we don't find an include directive, just comment out the code.
AC_MSG_CHECKING([for style of include used by $am_make])
am__include='#'
am__quote=
_am_result=none
# First try GNU make style include.
echo "include confinc" > confmf
# We grep out `Entering directory' and `Leaving directory'
# messages which can occur if `w' ends up in MAKEFLAGS.
# In particular we don't look at `^make:' because GNU make might
# be invoked under some other name (usually "gmake"), in which
# case it prints its new name instead of `make'.
if test "`$am_make -s -f confmf 2> /dev/null | fgrep -v 'ing directory'`" = "done"; then
   am__include=include
   am__quote=
   _am_result=GNU
fi
# Now try BSD make style include.
if test "$am__include" = "#"; then
   echo '.include "confinc"' > confmf
   if test "`$am_make -s -f confmf 2> /dev/null`" = "done"; then
      am__include=.include
      am__quote='"'
      _am_result=BSD
   fi
fi
AC_SUBST(am__include)
AC_SUBST(am__quote)
AC_MSG_RESULT($_am_result)
rm -f confinc confmf
])

# serial 3

# AM_CONDITIONAL(NAME, SHELL-CONDITION)
# -------------------------------------
# Define a conditional.
#
# FIXME: Once using 2.50, use this:
# m4_match([$1], [^TRUE\|FALSE$], [AC_FATAL([$0: invalid condition: $1])])dnl
AC_DEFUN([AM_CONDITIONAL],
[ifelse([$1], [TRUE],
        [errprint(__file__:__line__: [$0: invalid condition: $1
])dnl
m4exit(1)])dnl
ifelse([$1], [FALSE],
       [errprint(__file__:__line__: [$0: invalid condition: $1
])dnl
m4exit(1)])dnl
AC_SUBST([$1_TRUE])
AC_SUBST([$1_FALSE])
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi])

dnl File arg-with-environment.m4
dnl Written by Brian T.N. Gunney
dnl gunneyb@llnl.gov
dnl $Id: arg-with-environment.m4,v 1.17 2001/10/22 21:05:45 gunney Exp $



AC_DEFUN(BTNG_ARG_WITH_ENV_WRAPPER,[
dnl This is a high-level macro similar to AC_ARG_WITH but it does
dnl   a few extra things.
dnl
dnl It is meant for setting a shell variable using either the
dnl   --with-feature configure flag or by setting a shell variable
dnl   in the environment.  But its primary goal it to set or unset
dnl   the shell variable (arg2).
dnl
dnl One of several things can happen to the shell variable
dnl   when you use this macro, depending first on the configure
dnl   option issued:
dnl   |
dnl   `-- no option given
dnl   |   `-- leave shell variable alone, regardless of whether
dnl   |       it is set (This is how you avoid
dnl   |       having to use the configure option, such as in
dnl   |       running the check rule by automake.)
dnl   `-- with-feature=no or without-feature
dnl   |   `-- unset shell variable, regardless of whether it is set
dnl   `-- with-feature=string
dnl   |   `-- set shell variable to the string
dnl   `-- with-feature or with-feature=yes
dnl       `-- if shell variable already set
dnl       |   `-- leave it a lone
dnl       `-- else if developer gave optional arg4
dnl       |   `-- execute optional arg4 to set shell variable
dnl       `-- else
dnl           `-- set shell variable to blank
dnl
dnl One of two things can happen to the with_feature variable,
dnl   assuming the developer does not change it using arg4.
dnl   `-- no option given
dnl       `-- with_feature is unset
dnl   `-- one of the options referring to "feature" is given
dnl       `-- with_feature is set
dnl
dnl In addition to running AC_ARG_WITH and caching the result, it:
dnl   Allows the variable to be set by the environment.  This is
dnl     for avoiding having to manually issue configure options
dnl     or when manual configure options are not permissible, as
dnl     in running "make check".  The environment variable is
dnl     checked if the --with-something=something_else option
dnl     is not given or given without the equal sign.
dnl   Lets you specify command to run if --with-blah is issued
dnl     without the equal sign or not issued at all.  In this
dnl     case, the environment variable is consulted.  An unset
dnl     environment variable is the same as --without-bla.  A set
dnl     variable is the same as --with-blah=$value.  If $value is an
dnl     empty string, runs optional command (arg5) to set it.
dnl   Lets you specify command (arg4) to check the value chosen
dnl     to make it is good, before caching it.
dnl The arguments to this macro are:
dnl   1: Name of what is being sought (the first argument in
dnl     AC_ARG_WITH).
dnl   2: Name of variable to set (also name of environment variable
dnl      to look for if configure option is not issued).
dnl   3(optional): Help message.  A generic message is provided if
dnl     this argument is empty.
dnl   4(optional): Commands to run if configure flag is not specific
dnl     and environment variable is not set.  These commands are
dnl     run if with_blah is "yes" or "".  They should set or unset
dnl     the variable named in arg2, depending on what you want
dnl     the default behavior to be in these cases.
dnl   5(optional): Quality checking commands, to check if arg2 is good.
dnl     This is run before caching result.  Generally, this would issue
dnl     a warning or error as appropriate.  For example, if this macro
dnl     is used to set the path to a program, you may want to check
dnl     if that program exist and is executable.
# Start macro BTNG_ARG_WITH_ENV_WRAPPER with args $1 and $2
AC_CACHE_CHECK(for $1,btng_cv_prog_[]translit($1,-,_),[
AC_ARG_WITH($1,
ifelse($3,,[  --with-$1	Specify $1 (same as setting $2 in environment)],[$3]))
# Set $2, using environment setting if available
#   and if command line is ambiguous.
case "$with_[]translit($1,-,_)" in
  no[)]
    # User explictly turned off $1.
    # Ignore value of $2, even if set in the environment.
    unset $2
    ;;
  yes|''[)]
    # Flag unissued or ambiguously issued using --with-$1.
    # Because the user did not explicitly turn if off,
    # try to set $2.
    # If environment variable $2 is available, use it.
    # If not, try the user-supplied commands to set it.
    if test -n "${$2}" ;  then
      : Nothing to do here actually, because $2 is already in the environment.
    else
      ifelse($4,,:,$4)
    fi
    ;;
  *)
    # User issued a specific string using --with-$1=non-null-string.
    # so that is used to set $2.
    $2=$with_[]translit($1,-,_)
    ;;
esac
dnl if test ! "${$2+set}" = set ; then
dnl   # $2 is still unset, after processing --with-$1 flag,
dnl   # and possibly using optional command to set it.
dnl   # At this point, check to make sure it is not required.
dnl   # if it is, then we have an error.
dnl   case "$with_[]translit($1,-,_)" in
dnl     no|'')
dnl       : $2 is not set but it is ok because user did not
dnl       : explicitly ask for it by issuing --with-$1=something.
dnl       ;;
dnl     *)
dnl       # If user explicitly asked for $1 and we cannot find it[,]
dnl       # that is an error
dnl       AC_MSG_ERROR(cannot find appropriate value for $2)
dnl       ;;
dnl   esac
dnl fi
if test "${$2+set}" = set ; then
  # This block executes the quality check commands, if any, for $2.
  ifelse($5,,:,$5)
fi
# Cache the value if it was found.
if test "${$2+set}" = set ; then
  btng_cv_prog_[]translit($1,-,_)=${$2}
fi
])
dnl This part runs if $2 should be set from cache.
# Set $2 from cache.
# $2 is not yet set if we grabbed it from cache.
if test "${btng_cv_prog_[]translit($1,-,_)+set}" = set ; then
  $2=$btng_cv_prog_[]translit($1,-,_)
else
  unset $2
fi
# End macro BTNG_ARG_WITH_ENV_WRAPPER with args $2 and $1
])




AC_DEFUN(BTNG_PATH_PROG,[
dnl This is a high-level macro to find paths to certain programs.
dnl In addition to (possibly) running AC_ARG_WITH and AC_PATH_PROG it:
dnl   Allows the path to be set in an environment variable ($1),
dnl     useful for setting configuration during "make check" and
dnl     for avoiding manual configure options setting.
dnl   Makes sure that the program is executable, if the user explicitly
dnl     specified it.
dnl The arguments are (similar to AC_PATH_PROG):
dnl   1: Variable name to set to the path (used in BTNG_PATH_PROG).
dnl   2: Name of program being sought (used in BTNG_PATH_PROG).
dnl   3(optional): Commands to set $1 in case neither environment
dnl      nor command line options are given.  Defaults to a call to
dnl      AC_PATH_PROG($1,$2).
dnl   4(optional): Quality check commands to make sure that a
dnl      sufficiently good program is found.  Defaults to a simple
dnl      check that the program is executable.
BTNG_ARG_WITH_ENV_WRAPPER($2,$1,
[[  --with-$2=PATH	Specify path to $2 program
			(equivalent to setting $1 in environment)]]dnl
,
[
dnl Commands to run if user was not specific.
# Just set the variable to blank and check later.
$1=
],
dnl Quality check commands.
ifelse($4,,[
  # if $1 is an absolute path, make sure it is executable.
  if echo "${$1}" | grep '^/' > /dev/null && test ! -x "${$1}"; then
    AC_MSG_WARN($2 program ${$1} is not executable.)
  fi],$4)
)dnl
if test "${$1+set}" = set; then
  ifelse($3,,[AC_PATH_PROG($1,$2)],$3)
fi
])




AC_DEFUN(BTNG_ARG_WITH_PREFIX,[
dnl This is a high-level macro to set the prefix for a
dnl previously installed package.
dnl The macro arguments are:
dnl 1. package name
dnl 2. variable to contain installation prefix.
dnl 3(optional): Help message.  A generic message is provided if
dnl   this argument is empty.
dnl 4(optional): Commands to run if configure flag is not specific
dnl   and environment variable is not set.  These commands are
dnl   run if with_blah is "yes" or "".  They should set or unset
dnl   the variable named in arg2, depending on what you want
dnl   the defaul behavior to be in these cases.  The default is
dnl   to exit with an error.
# Start macro BTNG_ARG_WITH_PREFIX
BTNG_ARG_WITH_ENV_WRAPPER($1,$2,
ifelse([$3],,
[[  --with-$1=PATH	Specify prefix where $1 is installed
			(equivalent to setting $2 in the environment)]]
,[[[$3]]]),
ifelse([$4],,
[[if test "${with_[]translit($1,-,_)}" = yes ; then
  AC_MSG_ERROR([[If you specify --with-$1, you must give it the path as in --with-$1=/installation/path]])
fi
BTNG_AC_LOG(environment $2 not defined)
]],[[[$4]]])
)dnl
# End macro BTNG_ARG_WITH_PREFIX
])




AC_DEFUN(BTNG_AC_LOG,[echo "configure:__oline__:" $1 >&AC_FD_CC])

AC_DEFUN(BTNG_AC_LOG_VAR,[
dnl arg1 is list of variables to log.
dnl arg2 (optional) is a label.
dnl ifelse($2,,define(btng_log_label),define(btng_log_label,$2: ))
define([btng_log_label],ifelse($2,,,[$2: ]))
btng_log_vars="$1"
for btng_log_vars_index in $btng_log_vars ; do
  BTNG_AC_LOG("btng_log_label$btng_log_vars_index is '`eval echo \\\"\$\{$btng_log_vars_index\}\\\"`'")
done
undefine([btng_log_label])
])

