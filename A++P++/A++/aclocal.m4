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

# Like AC_CONFIG_HEADER, but automatically create stamp file.

# serial 3

# When config.status generates a header, we must update the stamp-h file.
# This file resides in the same directory as the config header
# that is generated.  We must strip everything past the first ":",
# and everything past the last "/".

AC_PREREQ([2.12])

AC_DEFUN([AM_CONFIG_HEADER],
[ifdef([AC_FOREACH],dnl
	 [dnl init our file count if it isn't already
	 m4_ifndef([_AM_Config_Header_Index], m4_define([_AM_Config_Header_Index], [0]))
	 dnl prepare to store our destination file list for use in config.status
	 AC_FOREACH([_AM_File], [$1],
		    [m4_pushdef([_AM_Dest], m4_patsubst(_AM_File, [:.*]))
		    m4_define([_AM_Config_Header_Index], m4_incr(_AM_Config_Header_Index))
		    dnl and add it to the list of files AC keeps track of, along
		    dnl with our hook
		    AC_CONFIG_HEADERS(_AM_File,
dnl COMMANDS, [, INIT-CMDS]
[# update the timestamp
echo timestamp >"AS_ESCAPE(_AM_DIRNAME(]_AM_Dest[))/stamp-h]_AM_Config_Header_Index["
][$2]m4_ifval([$3], [, [$3]]))dnl AC_CONFIG_HEADERS
		    m4_popdef([_AM_Dest])])],dnl
[AC_CONFIG_HEADER([$1])
  AC_OUTPUT_COMMANDS(
   ifelse(patsubst([$1], [[^ ]], []),
	  [],
	  [test -z "$CONFIG_HEADERS" || echo timestamp >dnl
	   patsubst([$1], [^\([^:]*/\)?.*], [\1])stamp-h]),dnl
[am_indx=1
for am_file in $1; do
  case " \$CONFIG_HEADERS " in
  *" \$am_file "*)
    am_dir=\`echo \$am_file |sed 's%:.*%%;s%[^/]*\$%%'\`
    if test -n "\$am_dir"; then
      am_tmpdir=\`echo \$am_dir |sed 's%^\(/*\).*\$%\1%'\`
      for am_subdir in \`echo \$am_dir |sed 's%/% %'\`; do
        am_tmpdir=\$am_tmpdir\$am_subdir/
        if test ! -d \$am_tmpdir; then
          mkdir \$am_tmpdir
        fi
      done
    fi
    echo timestamp > "\$am_dir"stamp-h\$am_indx
    ;;
  esac
  am_indx=\`expr \$am_indx + 1\`
done])
])]) # AM_CONFIG_HEADER

# _AM_DIRNAME(PATH)
# -----------------
# Like AS_DIRNAME, only do it during macro expansion
AC_DEFUN([_AM_DIRNAME],
       [m4_if(m4_regexp([$1], [^.*[^/]//*[^/][^/]*/*$]), -1,
	      m4_if(m4_regexp([$1], [^//\([^/]\|$\)]), -1,
		    m4_if(m4_regexp([$1], [^/.*]), -1,
			  [.],
			  m4_patsubst([$1], [^\(/\).*], [\1])),
		    m4_patsubst([$1], [^\(//\)\([^/].*\|$\)], [\1])),
	      m4_patsubst([$1], [^\(.*[^/]\)//*[^/][^/]*/*$], [\1]))[]dnl
]) # _AM_DIRNAME

dnl * APP_AUTOCONFIG_MACRO()
dnl *
dnl * The intent of this macro is to put common parts of the 
dnl * configure.in file into a single place within the directory tree
dnl * Many of the macros here are taken and/or modified from those
dnl * that appear in the SAMRAI distribution.  We want to thank them
dnl * for their contribution to this work.
dnl *********************************************************************

AC_DEFUN(APP_PPP_COMMON_AUTOCONFIG_MACRO,
[
dnl
dnl Guess the machine architecture and set the variable ARCH
dnl

AC_CANONICAL_HOST
dnl AC_CANONICAL_HOST defines host_cpu, host_vendor and host_os.
dnl These variable should be used instead of those provided
dnl by the very old macro.

echo "In A++P+++_common_autoconfig macro: ARCH = $ARCH"

echo "In A++P+++_common_autoconfig macro: srcdir = ${srcdir}"

AC_MSG_CHECKING([if we should use original source for generation of A++/P++ source code])
enable_DEVELOPMENT_SPECIFIC_DEPENDENCIES="no";
if test -d ${srcdir}/../Common_Code; then
   AC_MSG_RESULT([found \"${srcdir}/../Common_Code\" directory (including dependence information for A++/P++ development).])
   enable_DEVELOPMENT_SPECIFIC_DEPENDENCIES="yes";
else
   AC_MSG_RESULT([can't find \"${srcdir}/../Common_Code\" directory (this must be a distribution of A++/P++ so source code development not practical).])
fi
export enable_DEVELOPMENT_SPECIFIC_DEPENDENCIES;

# Setup Automake conditional to allow inclusion of additional dependences 
# of A++/P++ source code upon macros located in ../Common_Code directory
AM_CONDITIONAL(DEVELOPMENT_SPECIFIC_DEPENDENCIES,test "$enable_DEVELOPMENT_SPECIFIC_DEPENDENCIES" = yes)

echo "In A++P+++_common_autoconfig macro: enable_DEVELOPMENT_SPECIFIC_DEPENDENCIES = $enable_DEVELOPMENT_SPECIFIC_DEPENDENCIES"

# DQ (4/14/2001) Added Maintainer Mode so that I could support the 
# development environment separately from the distribuion environment.
# The development environment has additional dependences placed on 
# the source files so that they are correctly generated from the files
# in A++P++/Common_Code.
# AM_MAINTAINER_MODE
# use conditional mechanism to introduce
# "include $(top_srcdir)/../Common_Code/make_dependence_info.inc"
# into Makefiles
# AM_CONDITIONAL(MAINTAINER_MODE, test "$enable_maintainer_mode" = yes)

# We designed the A++P++ library and header files to be installed
# under a modified prefix (with the package name and lib appended).
# Set prefix earlier than autoconf normally would.
test "x$prefix" = xNONE && prefix=$ac_default_prefix
prefix_appendage=${PACKAGE}/install;
prefix=${prefix}/${prefix_appendage}
AC_MSG_NOTICE(The normal prefix has been changed to '$prefix' in order to prevent name clashes with the P++ package.)

# Build the directory since the semantics of install-sh is that it will assume the 
# directory name parameter is a file name if it is not already a valid directory.
# echo "mkdir -p ${prefix}/include"
AC_MSG_NOTICE([Building ${prefix}/include directory ...])
mkdir -p ${prefix}/include
# echo "mkdir -p ${prefix}/lib"
AC_MSG_NOTICE([Building ${prefix}/lib directory ...])
mkdir -p ${prefix}/lib

# Optional use of Brian Miller's Performance Test Suit
AC_ARG_WITH(PERFORMANCE_TESTS, [  --with-PERFORMANCE_TESTS .......................... compile and run performance tests within both A++ and P++],, with_PERFORMANCE_TESTS=no )
# with_PERFORMANCE_TESTS variable is exported so that other packages
# (e.g. A++ and P++) can set themselves up dependent upon the 
# use/non-use of PERFORMANCE_TESTS
export with_PERFORMANCE_TESTS;

# Setup Automake conditional to allow compilation of Performance Tests
AM_CONDITIONAL(COMPILE_PERFORMANCE_TESTS,test ! "$with_PERFORMANCE_TESTS" = no)

# This is required for using older make's on non-flat packages.
AC_PROG_MAKE_SET

# Choose between gm4 and m4.
AXXPXX_SET_M4

# I'm not sure why we need this
AC_CHECK_PROG(LD, ld,,ld)

dnl *********************************************************************
dnl * Try to find m4, preferably the GNU version.
dnl *********************************************************************

dnl AC_ARG_WITH(M4,
dnl    [  --with-M4=ARG ............................ manually set M4 to ARG],
dnl    M4=$withval,
dnl    [AC_CHECK_PROGS(M4, gm4, m4,)
dnl    BTNG_AC_LOG_VAR(M4)
dnl    if test -z "$M4"; then
dnl      AC_MSG_ERROR([m4 preprocessor not found - specify with --with-M4=ARG])
dnl    fi
dnl   ])
dnl AC_SUBST(M4)


dnl *********************************************************************
dnl Set up the C compiler.  This macro must come first and be followed
dnl AC_ISC_POSIX before other compiler tests are run.  See the autoconfig
dnl manual for details.
dnl *********************************************************************

AC_MSG_CHECKING(C compiler)
AC_ARG_WITH(CC,
   [  --with-CC=ARG ............................ manually set C compiler to ARG],
   CC=$withval,
   [
  app_C_compiler_name_cache_used=yes
  AC_CACHE_VAL(app_cv_compiler_name,app_C_compiler_name_cache_used=no)
  AC_MSG_CHECKING("what C compiler to use")

  dnl check if the name has been cached already
  if test "$app_C_compiler_name_cache_used" = yes; then
    AC_MSG_RESULT(found name of C compiler in the cache file.)
    CC=$app_cv_compiler_name
  else
    AC_MSG_RESULT(C compiler name not found in the cache file.)
    case $host_os in
      hpux*)		CC=cc ;;
      sun*| solaris*)	CC=cc ;;
      osf*)		CC=cc ;;
      aix*)		CC=mpcc ;;
      irix*)		CC=cc ;;
      linux*)		CC=gcc ;;
      *)
      # Try to find a C compiler to use (gcc is the default here)
        CC=gcc
      # AC_PROG_CC
      # Initialize CFLAGS to null string
         CFLAGS=""
        ;;
   esac
  AC_CACHE_VAL(app_cv_compiler_name,app_C_compiler_name=$CC)

  # Set the value used in autoconf (seems risky but autoconf does not set it)
  AC_CACHE_VAL(ac_cv_env_CC_set,ac_cv_env_CC_value=$CC)
  fi
]  )
AC_MSG_RESULT($CC)

AC_PROG_CC($CC)

# echo "Exiting after setting the C compiler!"
# exit 1

AC_DEFINE_UNQUOTED([APP_C_Compiler],"$CC",[Make the name of the C compiler available at compile time.])
export CC # Make variable available to sublibrary configuration.


dnl Do NOT use AC_PROG_CPP on IBM.  Our IBM mpcc compiler barfs up
dnl an UNWARRANTED warning on file handle 2 which the macro mistakes
dnl as an error.  BTNG.
if (echo "$host_os" | grep '^aix' >/dev/null) && (echo "$CC" | grep -i 'mpcc' >/dev/null) ; then
  CPP='${CC} -E'
else
  AC_PROG_CPP
fi
dnl case "$host_os" in
dnl   aix*) CPP='${CC} -E' ;;
dnl   *)
dnl      AC_PROG_CPP
dnl   ;;
dnl esac


dnl
dnl Set up the C++ compiler.
dnl

AC_MSG_CHECKING(C++ compiler)
AC_ARG_WITH(CXX,
   [  --with-CXX=ARG ........................... manually set C++ compiler to ARG],
   CXX=$withval,
   [case $host_os in
      hpux*)		CXX=aCC ;;
      sun* | solaris*)	CXX=CC ;;
      osf*)		CXX=cxx ;;
      irix*)		CXX=CC ;;
      aix*)		CXX=mpCC ;;
      linux*)		CXX=g++ ;;
      *)
dnl DQ(2/7/2000) reordered the different defaults to give 
dnl vendor compilers a higher priority than KCC C++ compiler
dnl but a higher priority than the GNU g++ compiler.
        CXX=g++
      # CCC="CC cxx xlC mpKCC KCC g++"
dnl Try to find a compiler to use
      # AC_PROG_CXX
dnl Initialize CXXFLAGS to null string
        CXXFLAGS=""
        ;;
   esac]  )
AC_MSG_RESULT($CXX)

AC_PROG_CXX($CXX)

AC_PROG_CXXCPP


# Determine what C and C++ compiler are being used.
dnl AC_MSG_CHECKING(what the compilers ${CC} and ${CXX} really are)
dnl BTNG_INFO_CC_CXX_ID
dnl AC_MSG_RESULT($CC_ID-$CC_VERSION and $CXX_ID-$CXX_VERSION)
AC_MSG_CHECKING(what the C compiler ${CC} really is)
BTNG_INFO_CC_ID
AC_MSG_RESULT($CC_ID-$CC_VERSION)
AC_MSG_CHECKING(what the C++ compiler ${CXX} really is)
BTNG_INFO_CXX_ID
AC_MSG_RESULT($CXX_ID-$CXX_VERSION)



AC_DEFINE_UNQUOTED([APP_CXX_Compiler],"$CXX",[Make the name of the C++ compiler available at compile time.])
export CXX # Make variable available to sublibrary configuration.




dnl *********************************************************************
dnl * Set the C++ compiler optimization flags in CXX_OPT
dnl *********************************************************************
dnl This should use the AC_ARG_ENABLE not AC_ARC_WITH!

AC_ARG_ENABLE(CXX_OPT,
   [  --enable-CXX_OPT=ARG ...................... manually set CXX_OPT to ARG],
   CXX_OPT=$enableval,
   [
case "$CXX_ID" in
  gnu)
           CXX_OPT='-O' ;;
  sunpro)
           # DQ (12/28/2001): Turn off optimization while I test the configuration
           CXX_OPT='-fast -xO4' ;;
           # CXX_OPT= ;;
  kai)
    case "$host_os" in
      sun*|solaris*)
           CXX_OPT='+K3 -fast --abstract_float --abstract_pointer' ;;
    esac ;;
  ibm)
           CXX_OPT='-O3 -qstrict -qarch=auto -qtune=auto -qcache=auto' ;;
esac
]  )
export CXX_OPT # Make variable available to sublibrary configuration.


dnl *********************************************************************
dnl * Set the C compiler optimization flags in C_OPT
dnl *********************************************************************
dnl This should use the AC_ARG_ENABLE not AC_ARC_WITH!

AC_ARG_ENABLE(C_OPT,
   [  --enable-C_OPT=ARG ........................ manually set C_OPT (optimization flags) to ARG],
   C_OPT=$enableval,
   [
case "$CC_ID" in
  gnu)
         C_OPT='-O' ;;
  sunpro)
         # DQ (12/28/2001): Turn off optimization while I test the configuration
         C_OPT='-fast -xO4' ;;
         # C_OPT= ;;
  kai)
         C_OPT='-fast -xO5'  ;;
  dec)
         C_OPT='-fast -O4 -tune host' ;;
  ibm)
         C_OPT='-O3 -qstrict -qarch=auto -qtune=auto -qcache=auto' ;;
  sgi)
         C_OPT='-O2 -G 0 -multigot -Wl,-nltgot,312 -OPT:Olimit=3000' ;;
esac
]  )
export C_OPT # Make variable available to sublibrary configuration.


dnl MDI_C_OPT defaults to C_OPT.
dnl This optionally sets MDI_C_OPT differently.
dnl NOTE: Use $C_OPT instead of '$(C_OPT)' for the value of MDI_C_OPT since this simplifies
dnl       the Makefile and avoids the posibility of introducing C_OPT = $(C_OPT) in the default
dnl       case (which is a makefile infinite loop).
dnl MDI_C_OPT='$(C_OPT)'
MDI_C_OPT=$C_OPT
AC_ARG_ENABLE(MDI_C_OPT,
   [  --enable-MDI_C_OPT=ARG .................... manually set MDI_C_OPT (optimization flags) to ARG],
   MDI_C_OPT=$enableval)
export MDI_C_OPT # Make variable available to sublibrary configuration.
AC_SUBST(MDI_C_OPT)


dnl *********************************************************************
dnl * Set up for setting -DNDEBUG
dnl *********************************************************************
AC_ARG_ENABLE(NDEBUG,
  [  --enable-NDEBUG ............................ turn off ALL use of assert macro everywhere],
  [AC_DEFINE([NDEBUG],1,[Turn off use of assert everywhere.])] )
export NDEBUG # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Set the CXX debug options
dnl *********************************************************************
AC_ARG_ENABLE(CXX_DEBUG,
   [  --enable-CXX_DEBUG=ARG .................... manually set CXX_DEBUG (debug flags (typicaly: -g)) to ARG],
   [CXX_DEBUG=$enableval] )
export CXX_DEBUG # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Set the CC debug options
dnl *********************************************************************
AC_ARG_ENABLE(C_DEBUG,
   [  --enable-C_DEBUG=ARG ...................... manually set C_DEBUG (debug flags (typicaly: -g)) to ARG],
   [C_DEBUG=$enableval])
export C_DEBUG # Make variable available to sublibrary configuration.

dnl MDI_C_DEBUG defaults to C_DEBUG.
dnl This optionally sets MDI_C_DEBUG differently.
dnl NOTE DQ: Use $C_DEBUG instead of '$(C_DEBUG)' for the value of MDI_C_DEBUG since this simplifies
dnl       the Makefile and avoids the posibility of introducing C_DEBUG = $(C_DEBUG) in the default
dnl       case (which is a makefile infinite loop).
dnl MDI_C_DEBUG='$(C_DEBUG)'
MDI_C_DEBUG=$C_DEBUG
AC_ARG_ENABLE(MDI_C_DEBUG,
   [  --enable-MDI_C_DEBUG=ARG .................. manually set MDI_C_DEBUG (debug flags (typicaly: -g)) to ARG],
   MDI_C_DEBUG=$enableval)
export MDI_C_DEBUG # Make variable available to sublibrary configuration.
AC_SUBST(MDI_C_DEBUG)

dnl *********************************************************************
dnl * Set the CXX  options since autoconf is braindead in this regard
dnl *********************************************************************
AC_ARG_ENABLE(CXX_OPTIONS,
   [  --enable-CXX_OPTIONS=ARG .................. manually set CXX_OPTIONS to ARG],
   [ CXX_OPTIONS=$enableval],
   [
case "$CXX_ID" in
  kai)
    case "$host_os" in
      irix*)		CXX_OPTIONS='-ptall -64 -woff all' ;;
      sun*|solaris*)	CXX_OPTIONS='--one_instantiation_per_object --display_error_number --diag_suppress 177,550' ;;
      *)		CXX_OPTIONS='--one_instantiation_per_object' ;;
    esac ;;
  sgi)			CXX_OPTIONS='-64' ;;
esac
echo CXX_OPTIONS is set to $CXX_OPTIONS
   ] )
export CXX_OPTIONS # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Set the CC  options since autoconf is braindead in this regard
dnl *********************************************************************
AC_ARG_ENABLE(C_OPTIONS,
   [  --enable-C_OPTIONS=ARG .................... manually set C_OPTIONS (options to C compiler) to ARG],
   [ C_OPTIONS=$enableval],
   [
case "$CC_ID" in
  sgi)	C_OPTIONS='-64 -mips4 -woff 1047,1116,1171,1174,1188,1552' ;;
esac
   ] )
export C_OPTIONS # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Set the RANLIB macro since some operating systems require ranlib
dnl *********************************************************************

AC_PROG_RANLIB

dnl *********************************************************************
dnl * Set the C++ compiler flags in CXX_WARNINGS
dnl *********************************************************************
dnl This should use the AC_ARG_ENABLE not AC_ARC_WITH!

AC_ARG_ENABLE(CXX_WARNINGS,
   [  --enable-CXX_WARNINGS=ARG ................. manually set CXX_WARNINGS to ARG],
   CXX_WARNINGS=$enableval,
   [
case "$CXX_ID" in
  gnu)		CXX_WARNINGS='' ;;
  kai)		CXX_WARNINGS='--for_init_diff_warning --new_for_init' ;;
esac
dnl AC_MSG_RESULT(["In APP_PPP_common.m4: CXX_WARNINGS = $CXX_WARNINGS"])
   ]  )
export CXX_WARNINGS # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Set the C compiler flags in C_WARNINGS
dnl *********************************************************************
dnl This should use the AC_ARG_ENABLE not AC_ARC_WITH!

AC_ARG_ENABLE(C_WARNINGS,
   [  --enable-C_WARNINGS=ARG ................... manually set C_WARNINGS to ARG],
   C_WARNINGS=$enableval,
   [
case "$CC_ID" in
dnl *wdh* 100924   gnu)		C_WARNINGS='-Wstrict-prototypes' ;;
  gnu)		C_WARNINGS='' ;;
esac
]  )
export C_WARNINGS # Make variable available to sublibrary configuration.


dnl *********************************************************************
dnl * Set the C++ compiler flags in CXX_TEMPLATES (Options for templates)
dnl *********************************************************************

AC_ARG_WITH(CXX_TEMPLATES,
   [  --with-CXX_TEMPLATES=ARG ................. manually set CXX_TEMPLATES (repository path) to ARG],
   CXX_TEMPLATES=$withval,
   [
dnl  Added by DQ since CXX_TEMPLATES depends upon CXX_TEMPLATE_REPOSITORY_PATH and CXX_TEMPLATE_REPOSITORY
dnl It should be safe to set CXX_TEMPLATE_REPOSITORY_PATH uniformly to '$(top_builddir)/src',
dnl even if it is not used.  I want to make things uniform where possible.  BTNG
     CXX_TEMPLATE_REPOSITORY_PATH='$(top_builddir)/src'
dnl      CXX_TEMPLATE_REPOSITORY=`$srcdir/../config/optionParser.pl $srcdir/../config/config.options $host_os $CC $CXX CXX_TEMPLATE_REPOSITORY`
dnl      CXX_TEMPLATES=`$srcdir/../config/optionParser.pl $srcdir/../config/config.options $host_os $CC $CXX CXX_TEMPLATES`
case "$CXX_ID" in
  gnu)
    CXX_TEMPLATE_REPOSITORY=
    CXX_TEMPLATES=
    CXX_EMPLATE_OBJECT_FILES=
  ;;
  sunpro)
    CXX_TEMPLATE_REPOSITORY='$(CXX_TEMPLATE_REPOSITORY_PATH)/Templates.DB'
    CXX_TEMPLATES='-ptv -ptr$(CXX_TEMPLATE_REPOSITORY_PATH)'
    CXX_TEMPLATE_OBJECT_FILES='${CXX_TEMPLATE_REPOSITORY}/*.o'
  ;;
  kai)
    CXX_TEMPLATE_REPOSITORY=
    CXX_TEMPLATES='--no_implicit_include'
    CXX_TEMPLATE_OBJECT_FILES=
  ;;
  dec)
    CXX_TEMPLATE_REPOSITORY='$(CXX_TEMPLATE_REPOSITORY_PATH)/cxx_repository'
    CXX_TEMPLATES='-ptv -ptr $(CXX_TEMPLATE_REPOSITORY)'
    CXX_TEMPLATE_OBJECT_FILES='${CXX_TEMPLATE_REPOSITORY}/*.o'
  ;;
  ibm)
    CXX_TEMPLATE_REPOSITORY=
    CXX_TEMPLATES=
    CXX_TEMPLATE_OBJECT_FILES=
  ;;
  sgi)
    CXX_TEMPLATE_REPOSITORY=
    CXX_TEMPLATES=
    CXX_TEMPLATE_OBJECT_FILES=
  ;;
esac
   ]  )
AC_SUBST(CXX_TEMPLATE_OBJECT_FILES)
# Make variable available to sublibrary configuration.
export CXX_TEMPLATE_REPOSITORY
export CXX_TEMPLATE_REPOSITORY_PATH
export CXX_TEMPLATES

dnl *********************************************************************
dnl * Specify the name of the pthreads library
dnl *********************************************************************
AC_ARG_WITH(PTHREADS_LIB,
   [  --with-PTHREADS_LIB=ARG .................. manually set PTHREADS_LIB to ARG],
   PTHREADS_LIB=$withval,
   [
dnl  Added by DQ to support PThreads (DQ (7/4/2001) this is a problem when used with Insure++ for C++ programs)
     PTHREADS_LIB=-lpthread
   ]  )
export PTHREADS_LIB # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Set up for setting -DPTHREADS
dnl *********************************************************************
AC_ARG_ENABLE(USE_PTHREADS,
  [  --enable-USE_PTHREADS .................... turn on internal use of Pthreads],
  [AC_DEFINE([USE_PTHREADS],[],[Turn on use of internal Pthreads.])
dnl AC_MSG_RESULT(["In APP_PPP_common.m4: USE_PTHREADS = $USE_PTHREADS"])
dnl LIBS="$LIBS $PTHREADS_LIB"
  ],
  [
dnl clear the PTHREADS_LIB variable if we are not enabling pthreads
    PTHREADS_LIB=""
  ]
)dnl
export USE_PTHREADS # Make variable available to sublibrary configuration.

AC_SUBST(INCLUDES)

CXXLD="$CXX"	# CXXLD and CXX are equivalent here,
		# but we DO need them to be separate variables.
AC_SUBST(CXXLD)
export CXXLD # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Check for various compiler options:
dnl *             COMPILER_NEEDS_BOOLEAN,
dnl *             COMPILER_EXPLICIT_TEMPLATE_INSTANTIATION,
dnl *             COMPILER_SUPPORTS_NAMESPACE
dnl *********************************************************************

AH_TEMPLATE([HAVE_BOOL],[Not all compilers define bool as a type.])
AH_TEMPLATE([HAVE_NAMESPACE],[Not all compilers define name spaces.])
# AH_TEMPLATE([HAVE_EXPLICIT_TEMPLATE_INSTANTIATION],[Not all compilers can use explicit template instantiation. (required for PADRE)])

# This should be the last of the CASC macros (which we want to phase out)
# CASC_CXX_BOOL
# CASC_CXX_NAMESPACE
# CASC_CXX_EXPLICIT_TEMPLATE_INSTANTIATION

AC_DEFINE([HAVE_EXPLICIT_TEMPLATE_INSTANTIATION],[],[We assume all compilers can use explicit template instantiation. (required for PADRE)])

BTNG_TYPE_BOOL
BTNG_TYPE_NAMESPACE

# The macro associated with BOOL_IS_BROKEN_XXX should follow BOOL_IS_BROKEN (which is what we want)
AH_TEMPLATE([BOOL_IS_BROKEN_XXX],[Build true and false values])
AH_VERBATIM([BOOL_IS_BROKEN_XXX],
[/* Build true and false values for C++ compilers that don't have bool */
#ifdef BOOL_IS_BROKEN
#define true 1
#define false 0
#endif])

# Make sure that the config.h is considered up to date
# (this causes an automake warning even though it is recomended in the autoconf manual)
# AC_CONFIG_FILES([stamp-h],[echo timestamp > stamp-h])
# AC_CONFIG_COMMANDS_POST ([echo timestamp > stamp-h])
# AC_CONFIG_COMMANDS_PRE ([echo timestamp > stamp-h])

dnl *********************************************************************
dnl * Set up the ARCH_LIBS
dnl *********************************************************************

AC_ARG_WITH(ARCH_LIBS,
   [  --with-ARCH_LIBS=ARG ..................... manually set ARCH_LIBS to ARG],
   ARCH_LIBS=$withval,
   [case $host_os in
     solaris* | sun4*)
       LIB_PATH="-L/opt/SUNWspro/SC4.2/lib $LIB_PATH"
dnl    LIBS="$LIBS -L/optSUNWspro/SC4.2/lib -lSUNWPro_lic -lpthread"
dnl    LIBS="$LIBS -L/optSUNWspro/SC4.2/lib -lSUNWPro_lic"
       ;;
#     CYGWIN32)
#       LIB_PATH="-L/usr/local/lib"
#       LIBS="-L/usr/local/lib -liberty -lxdr"
#       ;;
    esac]  )


dnl *********************************************************************
dnl * Set up for setting -DINTERNAL_DEBUG
dnl *********************************************************************
AC_ARG_ENABLE(INTERNALDEBUG,
  [  --enable-INTERNALDEBUG ................... turn on internal A++/P++ debugging ],
  [AC_DEFINE([INTERNALDEBUG],[],[Turn on use of A++/P++ debuging.])]
)dnl

dnl *********************************************************************
dnl * Set up the USE_TAU_PERFORMANCE_MONITOR
dnl * This set needs to be setup properly (not done yet!)
dnl *********************************************************************

AC_ARG_ENABLE(USE_TAU_PERFORMANCE_MONITOR,
   [  --with-USE_TAU_PERFORMANCE_MONITOR=ARG ... manually set USE_TAU_PERFORMANCE_MONITOR to YES or NO ARG],
   USE_TAU_PERFORMANCE_MONITOR=$withval)
   if test "$USE_TAU_PERFORMANCE_MONITOR" = yes; then
     case $host_os in
       solaris* | sun*)
         LIB_PATH="-L./TAU_LOCATION $LIB_PATH"
         APP_LIBS="-lTAU $APP_LIBS"
         LIBS="$LIBS -L./TAU_LOCATION -lTAU"
         ;;
#       CYGWIN32)
#         LIB_PATH="-L./TAU_LOCATION"
#         APP_LIBS="-lTAU $APP_LIBS"
#         LIBS="$LIBS -L./TAU_LOCATION -lTAU"
#         ;;
      esac
    fi


dnl test for GNU compilers
APP_COMPILER_MACRO


dnl **********************************************************************
dnl * 1 December 1999.
dnl * I'm re writing the shared library stuff to enable users to override
dnl * any of the settings at configure time.  This will make adding new
dnl * platforms way more difficult becuase new case statements will have
dnl * to be added to 10 macros instead of one.  Sorry.
dnl *
dnl * Here is the list of options that can be set at configure time and
dnl * that have default values for every platform-compiler combination:
dnl * SHARED_LIBS
dnl * STATIC_LINKER
dnl * STATIC_LINKER_FLAGS
dnl * SHARED_LIB_EXTENSION
dnl * C_DYNAMIC_LINKER
dnl * CXX_DYNAMIC_LINKER
dnl * C_DL_COMPILE_FLAGS
dnl * CXX_DL_COMPILE_FLAGS
dnl * C_DL_LINK_FLAGS
dnl * CXX_DL_LINK_FLAGS
dnl * RUNTIME_LOADER_FLAGS
dnl **********************************************************************
AC_ARG_ENABLE(SHARED_LIBS,
   [  --enable-SHARED_LIBS, .................... manually enable building of shared libraries, off by default],
   SHARED_LIBS=$enableval,
   SHARED_LIBS="no")

dnl AC_ARG_ENABLE(STATIC_LINKER,
dnl    [  --enable-STATIC_LINKER=ARG ............... manually set linker for linking static libraries to ARG],
dnl    STATIC_LINKER=$enableval,
dnl    [STATIC_LINKER=`$srcdir/../config/optionParser.pl $srcdir/../config/config.options $host_os $CC $CXX STATIC_LINKER`]  )

dnl AC_ARG_ENABLE(STATIC_LINKER_FLAGS,
dnl    [  --enable-STATIC_LINKER_FLAGS =ARG ........ manually set static linker flags to ARG],
dnl    STATIC_LINKER_FLAGS=$enableval,
dnl    [STATIC_LINKER_FLAGS=`$srcdir/../config/optionParser.pl $srcdir/../config/config.options $host_os $CC $CXX STATIC_LINKER_FLAGS`]  )

AC_ARG_ENABLE(SHARED_LIB_EXTENSION,
   [  --enable-SHARED_LIB_EXTENSION=ARG ........ manually set file extension for shared libraries to ARG (e.g. "so")],
   SHARED_LIB_EXTENSION=$enableval,
   [
case "$host_os" in
  hpux*)	SHARED_LIB_EXTENSION=sl ;;
  *)		SHARED_LIB_EXTENSION=so ;;
esac
]  )

dnl AC_ARG_ENABLE(C_DYNAMIC_LINKER,
dnl    [  --enable-C_DYNAMIC_LINKER=ARG ............ manually set linker for linking shared library from C object files to ARG],
dnl    C_DYNAMIC_LINKER=$enableval,
dnl    [C_DYNAMIC_LINKER=`$srcdir/../config/optionParser.pl $srcdir/../config/config.options $host_os $CC $CXX C_DYNAMIC_LINKER`]  )

dnl AC_ARG_ENABLE(CXX_DYNAMIC_LINKER,
dnl    [  --enable-CXX_DYNAMIC_LINKER=ARG .......... manually set linker for linking shared library from C++ object files to ARG],
dnl    CXX_DYNAMIC_LINKER=$enableval,
dnl    [CXX_DYNAMIC_LINKER=`$srcdir/../config/optionParser.pl $srcdir/../config/config.options $host_os $CC $CXX CXX_DYNAMIC_LINKER`]  )

AC_ARG_ENABLE(C_DL_COMPILE_FLAGS,
   [  --enable-C_DL_COMPILE_FLAGS=ARG .......... manually set C compiler flags to make objects suitable for building into shared libraries],
   C_DL_COMPILE_FLAGS=$enableval,
   [
case "$CC_ID" in
  gnu)		C_DL_COMPILE_FLAGS='-fPIC' ;;
  sunpro)	C_DL_COMPILE_FLAGS='-KPIC' ;;
  kai)		case "$host_os" in
		  sun*|solaris*) C_DL_COMPILE_FLAGS='-KPIC' ;;
		  *) C_DL_COMPILE_FLAGS= ;;
		esac ;;
  dec)		C_DL_COMPILE_FLAGS= ;;
  ibm)		C_DL_COMPILE_FLAGS= ;;
  sgi)		C_DL_COMPILE_FLAGS= ;;
esac
BTNG_AC_LOG(CC_ID is $CC_ID so C_DL_COMPILE_FLAGS is $C_DL_COMPILE_FLAGS)
]  )
export C_DL_COMPILE_FLAGS
AC_SUBST(C_DL_COMPILE_FLAGS)

AC_ARG_ENABLE(CXX_DL_COMPILE_FLAGS,
   [  --enable-CXX_DL_COMPILE_FLAGS=ARG ........ manually set C++ compiler flags for creating object files suitatble for putting into a shared library ],
   CXX_DL_COMPILE_FLAGS=$enableval,
   [
case "$CXX_ID" in
  gnu)		CXX_DL_COMPILE_FLAGS='-fPIC' ;;
  sunpro)	CXX_DL_COMPILE_FLAGS='-PIC' ;;
  kai)		case "$host_os" in
		  sun*|solaris*) C_DL_COMPILE_FLAGS='-KPIC' ;;
		  *) C_DL_COMPILE_FLAGS= ;;
		esac ;;
  dec)		CXX_DL_COMPILE_FLAGS= ;;
  ibm)		CXX_DL_COMPILE_FLAGS= ;;
  sgi)		CXX_DL_COMPILE_FLAGS= ;;
esac
BTNG_AC_LOG(CXX_ID is $CXX_ID so CXX_DL_COMPILE_FLAGS is $CXX_DL_COMPILE_FLAGS)
]  )
export CXX_DL_COMPILE_FLAGS
AC_SUBST(CXX_DL_COMPILE_FLAGS)

dnl AC_ARG_ENABLE(C_DL_LINK_FLAGS,
dnl    [  --enable-C_DL_LINK_FLAGS=ARG ............. manually set flags for linking C object files into a shared library],
dnl    C_DL_LINK_FLAGS=$enableval,
dnl    [C_DL_LINK_FLAGS=`$srcdir/../config/optionParser.pl $srcdir/../config/config.options $host_os $CC $CXX C_DL_LINK_FLAGS`]  )
dnl export C_DL_LINK_FLAGS

dnl AC_ARG_ENABLE(CXX_DL_LINK_FLAGS,
dnl    [  --enable-CXX_DL_LINK_FLAGS=ARG ........... manually set linker flags for linking C++ object files into a shared library],
dnl    CXX_DL_LINK_FLAGS=$enableval,
dnl    [CXX_DL_LINK_FLAGS=`$srcdir/../config/optionParser.pl $srcdir/../config/config.options $host_os $CC $CXX CXX_DL_LINK_FLAGS`]  )
dnl export CXX_DL_LINK_FLAGS

AC_ARG_ENABLE(RUNTIME_LOADER_FLAGS,
   [  --enable-RUNTIME_LOADER_FLAGS=ARG ........ manually set runtime loader flags to ARG],
   RUNTIME_LOADER_FLAGS=$enableval,
   [
case $CC_ID in
  ibm) RUNTIME_LOADER_FLAGS=-brtl ;;
esac
]  )

dnl since string specific code is not very portable allow it to be optional
dnl while we debug it's use on different architectures
AC_ARG_ENABLE(STRING_SPECIFIC_CODE,
   [  --enable-STRING_SPECIFIC_CODE=ARG ........ manually set use of code requiring string.h (non-portable code)],
   [
dnl CXXOPTIONS="$CXXOPTIONS -DUSE_STRING_SPECIFIC_CODE"
    STRING_SPECIFIC_CODE=$enableval
    AC_DEFINE([USE_STRING_SPECIFIC_CODE],[],[trigger use of code that requires string.h (non-portable code)])],
   [STRING_SPECIFIC_CODE="no"]  )


dnl How to use ar.
BTNG_PROG_AR

AC_MSG_CHECKING([what the compilers ${CC} and ${CXX} really are])
BTNG_INFO_CC_CXX_ID
AC_MSG_RESULT([$CC_ID-$CC_VERSION and $CXX_ID-$CXX_VERSION])

# Determine how to build a C++ library.
AC_MSG_CHECKING([how to build C++ libraries])
BTNG_CXX_AR
if test "$CXX_ID" = ibm; then
  # IBM does not have a method for supporting shared libraries
  # Here is a kludge.
  CXX_SHARED_LIB_UPDATE="`cd ${srcdir}/../config && pwd`/mklib.aix -o"
  BTNG_AC_LOG(CXX_SHARED_LIB_UPDATE changed to $CXX_SHARED_LIB_UPDATE especially for the IBM)
fi
AC_MSG_RESULT([$CXX_STATIC_LIB_UPDATE and $CXX_SHARED_LIB_UPDATE])
AC_SUBST(CXX_STATIC_LIB_UPDATE)
AC_SUBST(CXX_SHARED_LIB_UPDATE)

# Set up for Dan Quinlan's development tests.
AC_ARG_ENABLE(dq-developer-tests,
[--enable-dq-developer-tests   Development option for Dan Quinlan (disregard).])
AM_CONDITIONAL(DQ_DEVELOPER_TESTS,test "$enable_dq_developer_tests" = yes)

# Support for Purify
AXXPXX_SUPPORT_PURIFY

dnl
dnl Make all of the macro substitutions for the generated output files
dnl

AC_SUBST(CPPFLAGS)

AC_SUBST(CXX_OPT)
AC_SUBST(CXX_WARNINGS)
AC_SUBST(CXX_TEMPLATES)
AC_SUBST(CXX_TEMPLATE_REPOSITORY_PATH)
AC_SUBST(CXX_TEMPLATE_REPOSITORY)
AC_SUBST(CXX_OPTIONS)
AC_SUBST(CXX_DEBUG)

AC_SUBST(PTHREADS_LIB)

AC_SUBST(C_OPT)
AC_SUBST(C_WARNINGS)
AC_SUBST(C_OPTIONS)
AC_SUBST(C_DEBUG)

AC_SUBST(SHARED_LIBS)
AC_SUBST(STATIC_LINKER)
AC_SUBST(STATIC_LINKER_FLAGS)
AC_SUBST(SHARED_LIB_EXTENSION)
AC_SUBST(C_DYNAMIC_LINKER)
AC_SUBST(CXX_DYNAMIC_LINKER)
AC_SUBST(C_DL_LINK_FLAGS)
AC_SUBST(CXX_DL_LINK_FLAGS)
AC_SUBST(RUNTIME_LOADER_FLAGS)

dnl AC_SUBST(STRING_SPECIFIC_CODE)

])dnl




dnl Define a wrapper for BTNG_CHOOSE_STL to deal with the A++P++
dnl configure-time variable CXX_OPTIONS, which should be appended
dnl to CPPFLAGS to get A++P++'s compiler working correctly..
dnl A++P++ should use AXXPXX_CHOOSE_STL instead of BTNG_CHOOSE_STL.
AC_DEFUN(AXXPXX_CHOOSE_STL,[
axxpxx_save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $CXX_OPTIONS"
BTNG_CHOOSE_STL
CPPFLAGS="$axxpxx_save_CPPFLAGS"
])




dnl Because the PURIFY library should go after other libraries
dnl on the link command, I removed the Purify-enabling macros
dnl from the A++P++ common autoconfig macros.  It may now be called
dnl directly from configure.in after most of the libraries have been
dnl appended to LIBS.  BTNG.

AC_DEFUN(AXXPXX_SUPPORT_PURIFY,
[
# Begin macro AXXPXX_SUPPORT_PURIFY.

dnl
dnl *********************************************************************
dnl * Use PURIFY
dnl *********************************************************************
AC_ARG_ENABLE(USE_PURIFY_WINDOWS,
  [  --enable-USE_PURIFY_WINDOWS ............... turn on use of PURIFY -windows=yes option],
  [
  PURIFY_WINDOWS_OPTION=yes
  ],
  [
  case "$LOGNAME" in
  gunney)
  	PURIFY_WINDOWS_OPTION=yes ;;
  *)
  	PURIFY_WINDOWS_OPTION=no ;;
  esac
  ]
  )dnl

AC_SUBST(PURIFY_WINDOWS_OPTION)
export PURIFY_WINDOWS_OPTION # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Specify options for PURIFY
dnl *********************************************************************
AC_ARG_WITH(PURIFY_OPTIONS,
   [  --with-PURIFY_OPTIONS=ARG ................ manually set location of PURIFY to ARG],
   PURIFY_OPTIONS=$withval,
   [
dnl  Added by DQ to support PURIFY (using my favorite options and paths which might be platform specific)
dnl note that we have to use the ${} instead of $() for this to work on LINUX and IBM (other platforms do not seem to case)
dnl  PURIFY_RUN_AT_EXIT="-run-at-exit=\"if %z; then echo \\\"%v: %e errors, %l+%L bytes leaked.\\\"; fi\" "
     case "$LOGNAME" in
	gunney)
     PURIFY_OPTIONS="-windows=${PURIFY_WINDOWS_OPTION} -recursion-depth-limit=40000 -chain-length=24 -first-only=yes -leaks-at-exit=yes -inuse-at-exit=yes -always-use-cache-dir=yes -cache-dir=${HOME}/tmp -best-effort ${PURIFY_RUN_AT_EXIT}" ;;
	*)
     PURIFY_OPTIONS="-windows=${PURIFY_WINDOWS_OPTION} -recursion-depth-limit=40000 -chain-length=24 -max_threads=40 -first-only=yes -leaks-at-exit=yes -inuse-at-exit=yes -always-use-cache-dir=yes -cache-dir=${HOME}/tmp ${PURIFY_RUN_AT_EXIT}"
	esac
   ]  )

AC_SUBST(PURIFY_RUN_AT_EXIT)
AC_SUBST(PURIFY_OPTIONS)
export PURIFY_OPTIONS # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Specify the location of PURIFY
dnl *********************************************************************
AC_ARG_WITH(PURIFY_HOME,
   [  --with-PURIFY_HOME=ARG ................... manually set location of PURIFY to ARG],
   PURIFY_HOME=$withval,
   [
dnl  Added by DQ to support PURIFY
dnl note that we have to use the ${} instead of $() for this to work on LINUX and IBM (other platforms do not seem to case)
     PURIFY_HOME="/usr/local/pure/purify-5.1-solaris2"
   ] )


# Form the purify executable command.
PURIFY_EXECUTABLE=purify
test "$PURIFY_HOME" && PURIFY_EXECUTABLE="${PURIFY_HOME}/purify"
test "$PURIFY_OPTIONS" && PURIFY_EXECUTABLE="${PURIFY_EXECUTABLE} ${PURIFY_OPTIONS}"

AC_SUBST(PURIFY_HOME)
AC_SUBST(PURIFY_EXECUTABLE)
export PURIFY_EXECUTABLE # Make variable available to sublibrary configuration.

dnl *********************************************************************
dnl * Use PURIFY
dnl *********************************************************************
AC_ARG_ENABLE(USE_PURIFY,
  [  --enable-USE_PURIFY ....................... turn on use of PURIFY],
  [AC_DEFINE([USE_PURIFY],[],[Turn on use of PURIFY])
  dnl note that we have to use the ${} instead of $() for this to work on LINUX and IBM (other platforms do not seem to case)
  CXXLD="${PURIFY_EXECUTABLE} ${CXXLD}"
  CPPFLAGS="-I${PURIFY_HOME} $CPPFLAGS"
  CXXFLAGS="-I${PURIFY_HOME} $CXXFLAGS"
  INCLUDES="$INCLUDES -I${PURIFY_HOME}"
dnl Purify cannot find the purify_stubs.a without the explicit path
dnl LDFLAGS="-L${PURIFY_HOME} purify_stubs.a $LDFLAGS"
dnl  LDFLAGS="${PURIFY_HOME}/purify_stubs.a $LDFLAGS"
  LIBS="$LIBS ${PURIFY_HOME}/purify_stubs.a"
  AC_MSG_RESULT(["In APP_PPP_common_autoconfig.m4: Use Purify - CXXLD = $CXXLD and  CPPFLAGS = $CPPFLAGS"])
  AC_MSG_RESULT(["                                              CPPFLAGS = $CPPFLAGS"])
  AC_MSG_RESULT(["                                              CXXFLAGS = $CXXFLAGS"])
  ],
  [
  dnl note that we have to use the ${} instead of $() for this to work on LINUX and IBM (other platforms do not seem to case)
  dnl CXXLD="${CXX}"
  AC_MSG_RESULT(["In APP_PPP_common_autoconfig.m4: Do Not Use Purify - CXXLD = $CXXLD"])
  ]
  )dnl
export USE_PURIFY # Make variable available to sublibrary configuration.

# Begin macro AXXPXX_SUPPORT_PURIFY.
]
)


# DQ (8/16/2001): uncommented from brian's version 
AC_DEFUN(AXXPXX_SET_M4,[
   # Set M4 to the m4 command.
   # Choose gm4 if it is available.  Otherwise, choose m4.
   M4=gm4
   $M4 --version >/dev/null 2>&1 || M4=m4
   AC_SUBST(M4)
   BTNG_AC_LOG_VAR(M4)
 ])

dnl $Id: compiler-id.m4,v 1.8 2001/10/22 18:39:23 gunney Exp $

dnl Determines which compiler is being used.
dnl This check uses the compiler behavior when possible.
dnl For some compiler, we resort to a best guess,
dnl because we do not know a foolproof way to get the info.


dnl Simple wrappers to allow using BTNG_INFO_CXX_ID_NAMES and
dnl BTNG_INFO_CC_ID_NAMES without arguments.
dnl The names CC_ID and CC_VERSION are used for the C compiler id and version.
dnl The names CXX_ID and CXX_VERSION are used for the C++ compiler id and version.
AC_DEFUN(BTNG_INFO_CXX_ID,[
  BTNG_INFO_CXX_ID_NAMES(CXX_ID,CXX_VERSION)
])
AC_DEFUN(BTNG_INFO_CC_ID,[
  BTNG_INFO_CC_ID_NAMES(CC_ID,CC_VERSION)
])
AC_DEFUN(BTNG_INFO_CC_CXX_ID,[
  AC_REQUIRE([BTNG_INFO_CC_ID])
  AC_REQUIRE([BTNG_INFO_CXX_ID])
])


dnl BTNG_INFO_CXX_ID and BTNG_INFO_C_ID determine which C or C++ compiler
dnl is being used.
# Set the variables CXX_ID or C_ID as follows:
# Gnu		-> gnu
# SUNWspro	-> sunpro
# Dec		-> dec
# KCC		-> kai
# SGI		-> sgi
# IBM		-> ibm


AC_DEFUN(BTNG_INFO_CXX_ID_NAMES,
dnl Arguments are:
dnl 1. Name of variable to set to the ID string.
dnl 2. Name of variable to set to the version number.
[
# Start macro BTNG_INFO_CXX_ID_NAMES
  AC_REQUIRE([AC_PROG_CXXCPP])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  BTNG_AC_LOG(CXXP is $CXX)
  BTNG_AC_LOG(CXXCPP is $CXXCPP)

  $1=unknown
  $2=unknown

dnl Do not change the following chain of if blocks into a case statement.
dnl We may eventually have a compiler that must be tested in a different
dnl method


  # Check if it is a Sun compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is sunpro)
changequote(BEG,END)
    AC_EGREP_CPP(^0x[0-9]+,__SUNPRO_CC,
changequote([,])
      $1=sunpro
      # SUN compiler defines __SUNPRO_CC to the version number.
      echo __SUNPRO_CC > conftest.C
      $2=`${CXXCPP} conftest.C | sed -n 2p`
      rm -f conftest.C
    )
  fi


  # Check if it is a GNU compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is gnu)
    AC_EGREP_CPP(^yes,
#ifdef __GNUC__
yes;
#endif
,
    $1=gnu
changequote(BEG,END)
    $2=`$CXX --version | sed 's/^[[^0-9]]*//'`
changequote([,])
    )
  fi


  # Check if it is a DEC compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is dec)
    AC_EGREP_CPP(^1,__DECCXX,
      $1=dec
      # DEC compiler defines __DECCXX_VER to the version number.
      echo __DECCXX_VER > conftest.C
      $2=`${CXXCPP} conftest.C | sed -n 2p`
      rm -f conftest.C
    )
  fi


  # Check if it is a KAI compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is kai)
    AC_EGREP_CPP(^1,__KCC,
      $1=kai
      # KCC compiler defines __KCC_VERSION to the version number.
      echo __KCC_VERSION > conftest.C
      $2=`${CXXCPP} conftest.C | sed -n 2p`
      rm -f conftest.C
    )
  fi


  # Check if it is a SGI compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is sgi)
    AC_EGREP_CPP(^1,__sgi,
      $1=sgi
      # SGI compiler defines _COMPILER_VERSION to the version number.
      echo _COMPILER_VERSION > conftest.C
      $2=`${CXXCPP} conftest.C | sed /^\\#/d`
      rm -f conftest.C
    )
  fi


  # Check if it is a IBM compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is ibm)
    AC_EGREP_CPP(^1,_AIX,
      $1=ibm
      # I do not know how to determine version for this compiler.
    )
  fi


  AC_LANG_RESTORE
  BTNG_AC_LOG_VAR(CXX_ID CXX_VERSION)
# End macro BTNG_INFO_CXX_ID_NAMES
])





AC_DEFUN(BTNG_INFO_CC_ID_NAMES,
dnl Arguments are:
dnl 1. Name of variable to set to the ID string.
dnl 2. Name of variable to set to the version number.
[
# Start macro BTNG_INFO_CC_ID_NAMES
  AC_REQUIRE([AC_PROG_CPP])
  AC_LANG_SAVE
  AC_LANG_C
  BTNG_AC_LOG(CC is $CC)
  BTNG_AC_LOG(CPP is $CPP)

  $1=unknown
  $2=unknown

dnl Do not change the following chain of if blocks into a case statement.
dnl We may eventually have a compiler that must be tested in a different
dnl method


  # Check if it is a Sun compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is sunpro)
changequote(BEG,END)
    AC_EGREP_CPP(^ 0x[0-9]+,__SUNPRO_C,
changequote([,])
      $1=sunpro
      # SUN compiler defines __SUNPRO_C to the version number.
      echo __SUNPRO_C > conftest.c
      $2=`${CPP} ${CPPFLAGS} conftest.c | sed -n -e 's/^ //' -e 2p`
      rm -f conftest.c
    )
  fi


  # Check if it is a GNU compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is gnu)
    AC_EGREP_CPP(^yes,
#ifdef __GNUC__
yes;
#endif
,
    $1=gnu
changequote(BEG,END)
    $2=`${CC} --version | sed 's/^[[^0-9]]*//'`
changequote([,])
    )
  fi


  # Check if it is a DEC compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is dec)
    AC_EGREP_CPP(^ 1,__DECC,
      $1=dec
      # DEC compiler defines __DECC_VER to the version number.
      echo __DECC_VER > conftest.c
      $2=`${CPP} ${CPPFLAGS} conftest.c | sed -n -e 's/^ //' -e 2p`
      rm -f conftest.c
    )
  fi


  # Check if it is a KAI compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is kai)
    AC_EGREP_CPP(^1,__KCC,
      $1=kai
      # KCC compiler defines __KCC_VERSION to the version number.
      echo __KCC_VERSION > conftest.c
      $2=`${CPP} ${CPPFLAGS} conftest.c | sed -n 2p`
      rm -f conftest.c
    )
  fi


  # Check if it is a SGI compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is sgi)
    AC_EGREP_CPP(^1,__sgi,
      $1=sgi
      # SGI compiler defines _COMPILER_VERSION to the version number.
      echo _COMPILER_VERSION > conftest.c
      $2=`${CPP} ${CPPFLAGS} conftest.c | sed /^\\#/d`
      rm -f conftest.c
    )
  fi


  # Check if it is a IBM compiler.
  if test $$1 = unknown; then
    BTNG_AC_LOG(checking if $CXX is ibm)
    if echo "$host_os" | grep "aix" >/dev/null ; then
      # The wretched IBM shell does not eval correctly,
      # so we have to help it with a pre-eval eval statement.
      ac_cpp=`eval "echo $ac_cpp"`
      save_ac_cpp=$ac_cpp
      BTNG_AC_LOG(ac_cpp is temporarily set to $ac_cpp)
    else
      save_ac_cpp=
    fi
    BTNG_AC_LOG(ac_cpp is $ac_cpp)
    AC_EGREP_CPP(^1,_AIX,
      $1=ibm
      # I do not know how to determine version for this compiler.
    )
    test "$save_ac_cpp" && ac_cpp=$save_ac_cpp
    BTNG_AC_LOG(ac_cpp is restored to $ac_cpp)
  fi


  AC_LANG_RESTORE
  BTNG_AC_LOG_VAR(CC_ID CC_VERSION)
# End macro BTNG_INFO_CC_ID_NAMES
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

dnl $Id: compiling-boolean.m4,v 1.5 2001/10/22 18:39:23 gunney Exp $


AC_DEFUN(BTNG_TYPE_BOOL,[

# Start macro BTNG_TYPE_BOOL

AC_MSG_CHECKING(checking whether bool type is broken)

AC_CACHE_VAL(btng_cv_type_bool_broken, [

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS

  AC_TRY_COMPILE(, bool b = true; ,
    # bool is not broken.
    btng_cv_type_bool_broken=no
    ,
    # bool is broken.
    btng_cv_type_bool_broken=yes
  )	dnl End AC_TRY_COMPILE call

  AC_LANG_RESTORE

])	dnl End AC_CACHE_VAL call

AC_MSG_RESULT($btng_cv_type_bool_broken)

if test "$btng_cv_type_bool_broken" = yes; then
  AC_DEFINE(BOOL_IS_BROKEN,1,Define if bool type is not properly supported)
fi


# End macro BTNG_TYPE_BOOL

])	dnl End of COMPILE_BOOLEAN_MACRO definition.

dnl $Id: compiling-namespace.m4,v 1.6 2001/10/22 18:39:23 gunney Exp $



AC_DEFUN(BTNG_TYPE_NAMESPACE,[

# Start macro BTNG_TYPE_NAMESPACE

AC_MSG_CHECKING(checking whether namespace is broken)

AC_CACHE_VAL(btng_cv_type_namespace_broken, [

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE(namespace test{ int i; }
		, using namespace test;,
    # namespace is not broken.
    btng_cv_type_namespace_broken=no
    ,
    # namespace is broken.
    btng_cv_type_namespace_broken=yes
  )	dnl End AC_TRY_COMPILE call

  AC_LANG_RESTORE

])	dnl End AC_CACHE_VAL call

AC_MSG_RESULT($btng_cv_type_namespace_broken)

if test "$btng_cv_type_namespace_broken" = yes; then
  AC_DEFINE(NAMESPACE_IS_BROKEN,1,Define if namespace is not properly supported)
fi


# End macro BTNG_TYPE_NAMESPACE

])	dnl End of BTNG_TYPE_NAMESPACE definition.

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

dnl $Id: find-all-progs.m4,v 1.2 2001/10/22 18:39:23 gunney Exp $

dnl BTNG_PATH_FINDALLPROGS documentation.
dnl BTNG_PATH_FINDALLPROGS( program-to-check-for[, path] )
dnl Finds all instances of program-to-check-for in the path.
dnl Sets the variable program-to-check-for_paths (with all '-'
dnl changed to '_') to a space-separated list of paths to the
dnl instances found.
AC_DEFUN(BTNG_PATH_FINDALLPROGS,[
  space_separated_paths=`echo $2 | sed 's/:/ /g'`
  prog_name=`echo $1 | sed 's/-/_/g'`
  for dir in $space_separated_paths
  do
    if test -x ${dir}/$prog_name; then
      eval "${prog_name}_paths=\"\$${prog_name}_paths ${dir}/$1\""
    fi
  done
])	dnl End definition of macro BTNG_PATH_FINDALLPROGS.

dnl $Id: compiler-ar.m4,v 1.6 2001/10/22 18:39:23 gunney Exp $

dnl Define variables that generalizes the command to generate a library.
dnl This is usually done with something like "ar ruv".  But ar does not
dnl work well with C++ because compiling may be done at link time, to
dnl instantiate templates.  In general, a command using the C++ compiler
dnl is preferred over ar.  The variables defined are aimed at literally
dnl replacing the "ar ruv" in the command "ar ruv libxyz.a *.o".
dnl BTNG.

AC_DEFUN(BTNG_CXX_AR,[
dnl Set the variables CXX_ID and CXX_VERSION.

# Determine what compiler we ar using.
# The library-building command depends on the compiler characteristics.
AC_REQUIRE([BTNG_INFO_CXX_ID])


# Create the command syntax such that they can substitute
# the generic case of using ar, i.e. ar ruv.
# This means that the libraries will be generated using
# "$CXX_STATIC_LIB_UPDATE libxyz.a *.o".
case "$CXX_ID" in
  # Each block here defines CXX_STATIC_LIB_UPDATE and CXX_SHARED_LIB_UPDATE
  # for a specific compiler.
  gnu)
    CXX_STATIC_LIB_UPDATE='${AR} ruv'
    # I tried 'gcc -o' for static libraries, but got unresolved symbols
    # and no library.  BTNG.
    case "$host_os" in
      sun*|solaris*)	CXX_SHARED_LIB_UPDATE='${CC} -shared -o' ;;
      # Note that CC is used instead of CXX if CXX is GNU compiler.
      # Assume that if CXX is g++, then CC had better be gcc.
      # For some reason, running "g++ -shared" on the Sun writes
      # a perfectly good file then due to an error on ld, removes
      # that file.  Maybe a future version will correct this.  BTNG
      *)		CXX_SHARED_LIB_UPDATE='${CXX} -shared -o' ;;
      # Manual says I should also include the compile flags such as
      # -fpic and -fPIC but this seems to be working right now and I
      # fear breaking it.  BTNG
    esac
  ;;
  sunpro)
    CXX_STATIC_LIB_UPDATE='${CXX} -xar -o'
    CXX_SHARED_LIB_UPDATE='${CXX} -G -o'
  ;;
  dec)
    CXX_STATIC_LIB_UPDATE='${AR} ruv'
    # I tried 'cxx -o' for static libraries, but got unresolved symbols
    # and no library.  BTNG.
    CXX_SHARED_LIB_UPDATE='${CXX} -shared -o'
  ;;
  kai)
    CXX_STATIC_LIB_UPDATE='${CXX} -o'
    CXX_SHARED_LIB_UPDATE='${CXX} -o'
    # The KAI compiler generates shared or static based on name of output file.
  ;;
  sgi)
    CXX_STATIC_LIB_UPDATE='${AR} ruv'
    CXX_SHARED_LIB_UPDATE='${CXX} -64 -shared -o'
  ;;
  ibm)
    CXX_STATIC_LIB_UPDATE='${AR} -r -u -v'
    # IBM does not provide a method for creating shared libraries.
  ;;
  *)
    # Set the default values.
    # (These generally do not work well when templates are involved.)
    CXX_STATIC_LIB_UPDATE='${AR} ruv'
    CXX_SHARED_LIB_UPDATE='${CXX} -o'
  ;;
esac


# Let user override.
AC_ARG_WITH(cxx_static_lib_update,
[  --with-cxx_static_lib_update=COMMAND
			Use COMMAND (not 'ar ruv') to make static C++ library.],
CXX_STATIC_LIB_UPDATE=$with_cxx_static_lib_update)
AC_ARG_WITH(cxx_shared_lib_update,
[  --with-cxx_shared_lib_update=COMMAND
			Use COMMAND (not 'ar ruv') to make shared C++ library.],
CXX_SHARED_LIB_UPDATE=$with_cxx_shared_lib_update)

])

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







dnl **********************************************************************
dnl * APP_BUILD_SHARED_LIBRARY_CHECK
dnl * 
dnl * this macro will build a couple of files, compile some using C, others
dnl * using C++, link some to static library, others to shared library
dnl * and test building an executable that links to both.
dnl **********************************************************************

AC_DEFUN(APP_BUILD_SHARED_LIBRARY_CHECK,
[
# start macro APP_BUILD_SHARED_LIBRARY_CHECK

  # Make sure AR is defined, because it may be used in CXX_SHARED_LIB_UPDATE.
  if test ! "$AR"; then
    AC_CHECK_PROG(AR, ar,, ar)
  fi

  # CXX_*_LIB_UPDATE variables are defined with shell variables in them.
  # Resolve those variables.
  cxx_shared_lib_update=`eval "echo $CXX_SHARED_LIB_UPDATE"`
  cxx_static_lib_update=`eval "echo $CXX_STATIC_LIB_UPDATE"`

  # Remove old temporary shared lib source directory.
  rm -rf SrcForSharedLibTest
  # Create temporary shared lib source directory in the compile tree.
  mkdir SrcForSharedLibTest
  cat <<__EOM__> SrcForSharedLibTest/func1.h
#ifdef __cplusplus
extern "C" 
{
#endif
  
int func1();

#ifdef __cplusplus
}
#endif
__EOM__
  cat <<__EOM__> SrcForSharedLibTest/func1.c
#include "func1.h"
int func1()
{return 1;}
__EOM__
  cat <<__EOM__> SrcForSharedLibTest/func2.h
int func2();
__EOM__
  cat <<__EOM__> SrcForSharedLibTest/func2.C
#include "func2.h"
int func2()
{return 2;}
__EOM__
  cat <<__EOM__> SrcForSharedLibTest/main.C
#include "func1.h"
#include "func2.h"
int main(){
int a1=func1(),a2=func2();
return 0;}
__EOM__
 
  app_build_shared_library_cache=yes
  AC_CACHE_VAL(app_cv_build_shared_libs,app_build_shared_library_cache=no)
  AC_CACHE_VAL(app_cv_build_shared_lib_target,app_build_shared_library_cache=no)
  

  dnl check if they've all be cached already
  if test "$app_build_shared_library_cache" = yes; then
    AC_MSG_RESULT(found in cache file.)

    AC_MSG_CHECKING("whether we build shared libs or not")
    BUILD_SHARED_LIBS=$app_cv_build_shared_libs
    AC_MSG_RESULT("\(cached\) $app_cv_build_shared_libs")

    AC_MSG_CHECKING("for name of A++ shared lib target")
    APP_BUILD_SHARED_LIB_TARGET=$app_cv_build_shared_lib_target
    AC_MSG_RESULT("\(cached\) $app_cv_build_shared_lib_target")

  else

    dnl set the initial value for BUILD_SHARED_LIBS
    BUILD_SHARED_LIBS="yes"

    AC_MSG_RESULT(Not Found in cache, checking.)
    
    AC_MSG_CHECKING(for A++ shared library target)

    echo "$CC $C_DL_COMPILE_FLAGS -I./SrcForSharedLibTest -c ./SrcForSharedLibTest/func1.c"
    if $CC $C_DL_COMPILE_FLAGS -I./SrcForSharedLibTest -c ./SrcForSharedLibTest/func1.c > /dev/null; then
      mv func1.o ./SrcForSharedLibTest/func1.o
    else
      AC_MSG_RESULT("Compilation of C file using $C_DL_COMPILE_FLAGS PIC flags failed.")
      dnl exit 1
      BUILD_SHARED_LIBS="no"
    fi

    echo "$CXX $CXX_DL_COMPILE_FLAGS -I./SrcForSharedLibTest -c ./SrcForSharedLibTest/func2.C"
    if $CXX $CXX_DL_COMPILE_FLAGS -I./SrcForSharedLibTest -c ./SrcForSharedLibTest/func2.C > /dev/null; then
      mv func2.o ./SrcForSharedLibTest/func2.o
    else
      AC_MSG_RESULT("Compilation of C++ file using $CXX_DL_COMPILE_FLAGS PIC flags failed.")
      dnl exit 1
      BUILD_SHARED_LIBS="no"
    fi

dnl *
dnl * Build a static library from func1.o, build a shared library from func2.o
dnl *
    echo "$cxx_static_lib_update libfunc1_static.a ./SrcForSharedLibTest/func1.o"
    if $cxx_static_lib_update libfunc1_static.a ./SrcForSharedLibTest/func1.o > /dev/null; then
      mv ./libfunc1_static.a ./SrcForSharedLibTest
      echo ""
    else
      AC_MSG_RESULT("static link: $cxx_static_lib_update libfunc1_static.a func1.o ...FAILED")
      dnl exit 1
      BUILD_SHARED_LIBS="no"
      exit 0
    fi

dnl *
dnl * NOTE:  here we differentiate between rs6000 and everything else becuase of the funny
dnl *  way that shared libs are built on those machines
dnl *
    echo "$cxx_static_lib_update libfunc2.a ./SrcForSharedLibTest/func2.o"
    if $cxx_static_lib_update libfunc2.a ./SrcForSharedLibTest/func2.o > /dev/null; then
      mv ./libfunc2.a ./SrcForSharedLibTest
      echo ""
    else
      AC_MSG_RESULT("static link: $cxx_static_lib_update libfunc2.a func2.o ...FAILED")
      dnl exit 1
      BUILD_SHARED_LIBS="no"
    fi

    echo "$cxx_shared_lib_update libfunc2.$SHARED_LIB_EXTENSION ./SrcForSharedLibTest/func2.o"
    if $cxx_shared_lib_update libfunc2.$SHARED_LIB_EXTENSION ./SrcForSharedLibTest/func2.o > /dev/null; then
      mv ./libfunc2.$SHARED_LIB_EXTENSION ./SrcForSharedLibTest/
      echo ""
    else
      AC_MSG_RESULT("dynamic link: $cxx_shared_lib_update libfunc2.$SHARED_LIB_EXTENSION func2.o ...FAILED")
      dnl exit 1
      BUILD_SHARED_LIBS="no"
    fi

dnl *
dnl * compiling the main program with the shared library and static library
dnl *
    echo "$CXX $CXX_OPTIONS $RUNTIME_LOADER_FLAGS -o ./SrcForSharedLibTest/test ./SrcForSharedLibTest/main.C -L./SrcForSharedLibTest -lfunc1_static -lfunc2 -lc -lm"
    if $CXX $CXX_OPTIONS $RUNTIME_LOADER_FLAGS -o ./SrcForSharedLibTest/test ./SrcForSharedLibTest/main.C -L./SrcForSharedLibTest -lfunc1_static -lfunc2 -lc -lm >&5 ; then
      echo ""
    else
      AC_MSG_RESULT("build executable: $CXX $CXX_OPTIONS $RUNTIME_LOADER_FLAGS -o test main.C -L . -lfunc1_static -lfunc2  -lc -lm ...FAILED")
      dnl exit 1
      BUILD_SHARED_LIBS="no"
    fi

dnl *
dnl * building execute script to run the test
dnl *
    if test "$ARCH" != rs6000; then
    cat>runTest<<EOF
#!/bin/sh
LD_LIBRARY_PATH=./SrcForSharedLibTest:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
./SrcForSharedLibTest/test
EOF
    else
    cat>runTest<<EOF
#!/bin/sh
export MP_RESD="YES"
export MP_HOSTFILE=""
export MP_EUILIB=us
export MP_EUIDEVICE=css0
poe ./SrcForSharedLibTest/test -rmpool 0 -nodes 1 -procs 1
EOF
    fi

    chmod 770 runTest

    APP_BUILD_SHARED_LIB_TARGET=

    if ./runTest > /dev/null; then
      APP_BUILD_SHARED_LIB_TARGET=libApp.shared
      rm -f runTest ./SrcForSharedLibTest/*.o ./SrcForSharedLibTest/*.a ./SrcForSharedLibTest/*.so ./SrcForSharedLibTest/test
      AC_MSG_RESULT("$APP_BUILD_SHARED_LIB_TARGET")
    else
      AC_MSG_RESULT(runTest script Failed.)
      dnl exit 1
      BUILD_SHARED_LIBS="no"
    fi

    AC_CACHE_VAL(app_cv_build_shared_libs,app_cv_build_shared_libs=$BUILD_SHARED_LIBS)
    AC_CACHE_VAL(app_cv_build_shared_lib_target,app_cv_build_shared_lib_target=$APP_BUILD_SHARED_LIB_TARGET)
    AC_CACHE_SAVE()
  fi
  
  AC_SUBST(APP_BUILD_SHARED_LIB_TARGET)
  AC_SUBST(BUILD_SHARED_LIBS)

  # rm -rf SrcForSharedLibTest
# end macro APP_BUILD_SHARED_LIBRARY_CHECK
])dnl

