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

AC_DEFUN(PPP_SPECIFIC_SETUP,
[
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
# test "x$prefix" = xNONE && prefix=$ac_default_prefix
# prefix_appendage=${PACKAGE}/lib;
# prefix=${prefix}/${prefix_appendage}
# AC_MSG_NOTICE(The normal prefix has been changed to '$prefix' in order to prevent name clashes with the A++ package.)

# Build the install directory since the semantics of install-sh is that it will assume the
# directory name parameter is a file name if it is not already a valid directory.
# mkdir -p ${prefix}/include
# mkdir -p ${prefix}/lib

# export prefix_appendage so PADRE will see it.
# export prefix_appendage

# Optional use of Brian Miller's Performance Test Suit
# AC_ARG_WITH(PERFORMANCE_TESTS, [  --with-PERFORMANCE_TESTS .......................... compile and run performance tests within both A++ and P++],, with_PERFORMANCE_TESTS=no )
# with_PERFORMANCE_TESTS variable is exported so that other packages
# (e.g. A++ and P++) can set themselves up dependent upon the 
# use/non-use of PERFORMANCE_TESTS
# export with_PERFORMANCE_TESTS;

# Setup Automake conditional to allow compilation of Performance Tests
# AM_CONDITIONAL(COMPILE_PERFORMANCE_TESTS,test ! "$with_PERFORMANCE_TESTS" = no)

#This is required for using older make's on non-flat packages.
# AC_PROG_MAKE_SET

# Choose between gm4 and m4.
# DQ (8/16/2001) uncommented from brian's version
# AXXPXX_SET_M4

# DQ: Do we really need this?
# This tells automake where to find the "missing" file 
# (a script with is run to rebuild autoconf/automake files).
# missing_dir=`cd $ac_aux_dir && pwd`
# AM_MISSING_PROG(ACLOCAL, aclocal, $missing_dir)
# AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
# AM_MISSING_PROG(AUTOMAKE, automake, $missing_dir)
# AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)

# I'm not sure why we need this
# AC_CHECK_PROG(LD, ld,,ld)

# This is the start of the inlined PPP_AUTOCONFIG_MACRO, modified here.
# Call the A++ Macro first and then call anything specific to P++

echo "Before APP PPP COMMON AUTOCONFIG MACRO call: INCLUDES = $INCLUDES"

AC_MSG_NOTICE([In P++.m4 before APP PPP COMMON AUTOCONFIG MACRO: CPPFLAGS = $CPPFLAGS])
# Call the macro that is common to both A++ and P++
APP_PPP_COMMON_AUTOCONFIG_MACRO
AC_MSG_NOTICE([In P++.m4 after APP PPP COMMON AUTOCONFIG MACRO: CPPFLAGS = $CPPFLAGS])

# Trigger variable in config.h to allow source code to be written specific to A++ or P++
AC_DEFINE([USE_PPP],1,[Signal to user if P++ is used at comple time.])

# Make sure the templates make there way into the config.hin file (or whatever we will call it in the future)
AH_TEMPLATE([PXX_ENABLE_MP_INTERFACE_MPI],[Allow specification of MPI within P++])
AH_TEMPLATE([PXX_ENABLE_MP_INTERFACE_PVM],[Allow specification of PVM within P++])
BTNG_CHOOSE_MP_INTERFACE(PXX_)
# The message passing must be one that we support.
case "$enable_mp_interface" in
     mpi) :
     ;;
     *) AC_MSG_ERROR([Sorry. $PACKAGE only supports message passing interface MPI at this time.])
     ;;
esac

# Determine how to build a C++ library.
# AC_MSG_CHECKING([how to build C++ libraries])
# BTNG_CXX_AR
# if test "$CXX_ID" = ibm; then
#   # IBM does not have a method for supporting shared libraries
#   # Here is a kludge.
#   CXX_SHARED_LIB_UPDATE="`cd ${srcdir}/../config && pwd`/mklib.aix -o"
#   BTNG_AC_LOG(CXX_SHARED_LIB_UPDATE changed to $CXX_SHARED_LIB_UPDATE especially for the IBM)
# fi
# AC_MSG_RESULT([$CXX_STATIC_LIB_UPDATE and $CXX_SHARED_LIB_UPDATE])
# AC_SUBST(CXX_STATIC_LIB_UPDATE)
# AC_SUBST(CXX_SHARED_LIB_UPDATE)

# Since COMPILE_APP is not defined we don't want to define it to "0"
# since then "if defined(COMPILE_APP)" will be true (unfortunately).
# It is better to never define it than to define it to "0", since then
# it is left at "#undef COMPILE_APP" within the config.h file generated 
# by autoconf (more specifically the configure script generated by autoconf).
# AC_DEFINE(COMPILE_APP,0)

# We have changed how the P++ source specifies the COMPILE_PPP and COMPILE_SERIAL_APP macros.
# Now the source code sets these instead of requiring the compile line to specify it.  This
# simplifies how automake can build the source code. We can have one way instead of two ways, thus
# we can use the default automake mechanism (which is more portable).
AC_DEFINE([COMPILE_PPP],1,[Trigger complete trip through all P++ header files required for P++ compilation])



echo "Before LAM MPI Setup: INCLUDES = $INCLUDES"

AC_MSG_CHECKING(whether to use LAM)
BTNG_SUPPORT_LAM
AC_MSG_RESULT($with_lam)
# lam_PREFIX, lam_INCLUDES, lam_LIBS are defined by BTNG_SUPPORT_LAM.
AC_SUBST(lam_PREFIX)
AC_SUBST(lam_INCLUDES)
AC_SUBST(lam_LIBS)
# if test "$with_lam" && test ! "$with_lam" = no; then
#   # INCLUDES="$lam_INCLUDES $INCLUDES"
#   # LIBS="$lam_LIBS $LIBS"
# fi

echo "Before MPICH MPI Setup: INCLUDES = $INCLUDES"

AC_ARG_WITH(mpich,[ --with-mpich=PATH	Specify prefix where MPICH was installed],,
# If with-mpich is not specified, it is automatically set to yes or no,
# which ever is the oposite of with-lam.
if test "$with_lam" = no ; then with_mpich=yes ; else with_mpich=no ; fi )
BTNG_AC_LOG_VAR(with_mpich)
# mpich_PREFIX, mpich_INCLUDES, mpich_LIBS are defined if --with-mpich is used.
if test ! "$with_mpich" = no ;  then
  AC_MSG_WARN([MPICH was not specified on the command line -- so use default settings])
  if test "$with_mpich" = yes ;  then
    # The user specified mpich but no particular installation, so look for it.
    BTNG_AC_LOG(Checking for MPI using automatic finding)
    # We use the AXXPXX_FIND_MPI macro to look for the mpich installation.
    # AXXPXX_FIND_MPI
    AXXPXX_SET_MPI
    BTNG_AC_LOG_VAR(MPIINCLUDE MPILIBDIRS MPILIBS)
    mpich_INCLUDES="$MPIINCLUDE"
    mpich_LIBS="$MPILIBDIRS $MPILIBS"
    if echo "$host_os" | grep '^solaris' > /dev/null; then
      LIBS="$LIBS -lc"
      # "-lc" is included here because it is required for the Sun Solaris use of
      # MPI with the C++ compiler.  Specificly this fixes the problem of the
      # wrong libC library being used and preventing use of MPI with more than
      # 3 processors! (difficult bug to find!)
    fi
  else
    # The user specified the mpich installation, so use it.
    BTNG_AC_LOG("Using mpich prefix of $with_mpich")
    mpich_PREFIX=$with_mpich
    mpich_INCLUDES="-I$mpich_PREFIX/include"
    mpich_LIBS="-L$mpich_PREFIX/lib -lmpich"
    test -d "$mpich_PREFIX/include/mpi2c++" &&	\
      mpich_INCLUDES="$mpich_INCLUDES -I$mpich_PREFIX/include/mpi2c++"
    AC_SUBST(mpich_PREFIX)
    AC_SUBST(mpich_INCLUDES)
    AC_SUBST(mpich_LIBS)
    BTNG_AC_LOG_VAR(mpich_PREFIX mpich_INCLUDES mpich_LIBS)
    case "$host_os" in
      solaris*) LIBS="$LIBS -lc"
     ;;
    esac
  fi

  # Make available to lower level configurations.
  export mpich_PREFIX mpich_INCLUDES mpich_LIBS
fi # End if-block on with_mpich != no

AC_MSG_RESULT([In configure.in: mpich_INCLUDES = $mpich_INCLUDES])
AC_MSG_RESULT([In configure.in: mpich_LIBS     = $mpich_LIBS])

# echo "Exiting after test for MPI ..."
# exit 1

# Allow the user to choose lam or mpich or nothing.
# Set the variable mpi_... accordingly
BTNG_AC_LOG_VAR(with_mpich with_lam)
if test ! "$with_mpich" = no && test "$with_lam" = no ; then
  # The user chose mpich over lam.
  mpi_PREFIX="$mpich_PREFIX"
  mpi_INCLUDES="$mpich_INCLUDES"
  mpi_LIBS="$mpich_LIBS"
  AC_DEFINE([MPI_IS_MPICH],1,[Define MPICH version of MPI])
elif test ! "$with_lam" = no && test "$with_mpich" = no ; then
  # The user chose lam over mpich.
  mpi_PREFIX="$lam_PREFIX"
  mpi_INCLUDES="$lam_INCLUDES"
  mpi_LIBS="$lam_LIBS"
  AC_DEFINE([MPI_IS_LAM],1,[Define LAM version of MPI])
elif test ! "$with_mpich" = no && ! test "$with_lam" = no ; then 
  # The user chose both mpich and lam.
  AC_MSG_ERROR([You cannot specify both lam and mpich.  Choose one or the other.])
fi

if test -n "$mpi_INCLUDES"; then
  CPPFLAGS="$mpi_INCLUDES $CPPFLAGS"

  echo "After MPI Setup (before adding (mpi_INCLUDES=$mpi_INCLUDES) to INCLUDE: INCLUDES = $INCLUDES"
  INCLUDES="$mpi_INCLUDES $INCLUDES"
  echo "After MPI Setup (after adding (mpi_INCLUDES=$mpi_INCLUDES) to INCLUDE: INCLUDES = $INCLUDES"
fi

if test -n "$mpi_LIBS"; then
  # LIBS="$LIBS $lam_LIBS"
  LIBS="$LIBS $mpi_LIBS"
fi
export mpi_PREFIX mpi_INCLUDES mpi_LIBS
BTNG_AC_LOG_VAR(mpi_PREFIX mpi_INCLUDES mpi_LIBS)

echo "After MPI setup: CPPFLAGS = $CPPFLAGS"
echo "After MPI setup: INCLUDES = $INCLUDES"
echo "After MPI setup: LIBS     = $LIBS"

# echo "Exiting after test for MPI ..."
# exit 1

# ***********************************************************************
# Generic information should be done by this point,
# so we can set INCLUDES, LIBS.
# Below should only be project-specific information.
# ***********************************************************************

# This is already done above so let's not do it twice!
# test -n "$mpi_INCLUDES" && INCLUDES="$mpi_INCLUDES $INCLUDES"
# test -n "$mpi_LIBS" && LIBS="$LIBS $mpi_LIBS"

 # Execute the MPIRUN check
   AC_ARG_ENABLE(mpirun-check,
      [  --disable-mpirun-check ................... disable check on MPI characteristics.]
      ,, enable_mpirun_check=yes )
   if test ! "$enable_mpirun_check" = no; then
      # AC_MSG_RESULT([In P++ configure.in: PPP MPIRUN CHECK commented out!])
      PPP_MPIRUN_CHECK
   fi

AC_ARG_WITH(INDIRECT_ADDRESSING,
   [  --with-INDIRECT_ADDRESSING .......................... use parallel indirect addressing within P++] )

# *********************************************************************
# * Set up PADRE for use in P++
# *********************************************************************

# Supporting Parti in PADRE.
PADRE_SUPPORT_Parti
AM_CONDITIONAL(ENABLE_Parti, test "$enable_Parti" = yes)

# Supporting Kelp in PADRE.
PADRE_SUPPORT_Kelp
AM_CONDITIONAL(ENABLE_Kelp, test "$enable_Kelp" = yes)

AC_ARG_WITH(PADRE,
   [  --without-PADRE .......................... avoid using PADRE Library within P++],, with_PADRE=yes )
BTNG_AC_LOG(with_PADRE is $with_PADRE)
# with_PADRE variable is exported so that other packages
# (e.g. indirect addressing) can set 
# themselves up dependent upon the use/non-use of PADRE
export with_PADRE;

AC_MSG_RESULT([In P++ configure.in: with_PADRE = $with_PADRE])

AM_CONDITIONAL(COMPILE_PADRE_DIRECTORY,test ! "$with_PADRE" = no)

if test "$with_INDIRECT_ADDRESSING" = yes; then
     # Allow source code to be included if indirect addressing is used.
     AC_DEFINE([USE_PARALLEL_INDIRECT_ADDRESSING_SUPPORT], 1,[Indirect addressing support.])
fi

AC_LANG_PUSH(C++)

# Currently, P++ only needs STL when it is using PADRE or the indirect addressing support.
if test ! "$with_PADRE" = no || test "$with_INDIRECT_ADDRESSING" = yes; then
    AC_MSG_NOTICE([Find a suitable STL ... ])
  # Determine location of STL.
  # BTNG_CHOOSE_STL
  # AC_SUBST(STL_DIR)
  # AC_SUBST(STL_INCLUDES)
  # INCLUDES="$INCLUDES $STL_INCLUDES"

  # Determine whether bool type works.  See compile-bool.m4. Needed by CHOOSE_STL macro.
  BTNG_TYPE_BOOL

  # Determine whether namespace works.  See compile-namespace.m4. Needed by CHOOSE_STL macro.
  BTNG_TYPE_NAMESPACE

  AXXPXX_CHOOSE_STL
  AC_SUBST(STL_DIR)
  AC_SUBST(STL_INCLUDES)

  echo "After CHOOSE STL (before adding (STL_INCLUDES=$STL_INCLUDES) to INCLUDE: INCLUDES = $INCLUDES"
  INCLUDES="$INCLUDES $STL_INCLUDES"
  echo "After CHOOSE STL (after adding (STL_INCLUDES=$STL_INCLUDES) to INCLUDE: INCLUDES = $INCLUDES"

  # Must set CPPFLAGS to STL location before calling Brian's macros 
  # for searching STL for correct header file names
  # CPPFLAGS="$CPPFLAGS $STL_INCLUDES"
  # CXXFLAGS="$CXXFLAGS $STL_INCLUDES"

  # AC_SUBST(STL_INCLUDES)
  # AC_SUBST(STL_DIR)

  # echo "After CHOOSE STL: INCLUDES    = $INCLUDES"
  # echo "After CHOOSE STL (options are used in AC CHECK HEADER macro): STL_DIR     = $STL_DIR"
  # echo "After CHOOSE STL (options are used in AC CHECK HEADER macro): CPPFLAGS    = $CPPFLAGS"
  # echo "After CHOOSE STL (options are used in AC CHECK HEADER macro): CXXFLAGS    = $CXXFLAGS"
  # echo "After CHOOSE STL (options are used in AC CHECK HEADER macro): CXX_OPTIONS = $CXX_OPTIONS"

  # Determine pecularities of STL (see compiling-stl.m4).
  # These should go after CHOOSE_STL so they would use the
  # header files defined by that macro.
  # axxpxx_save_CPPFLAGS="$CPPFLAGS $CXX_OPTIONS"
  axxpxx_save_CPPFLAGS="$CPPFLAGS"
# CPPFLAGS="$CPPFLAGS $CXX_OPTIONS"
  CPPFLAGS="$CPPFLAGS $STL_INCLUDES"
  BTNG_STL_STRING_HEADER_FILENAME
  BTNG_STL_LIST_HEADER_FILENAME
# BTNG_STL_STACK_HEADER_FILENAME

  BTNG_STL_VECTOR_HEADER_FILENAME
  BTNG_STL_ITERATOR_HEADER_FILENAME
  BTNG_STL_ALGO_HEADER_FILENAME
  BTNG_STL_MAP_HEADER_FILENAME
  CPPFLAGS="$axxpxx_save_CPPFLAGS"
fi

AC_LANG_POP(C++)

# AC_MSG_ERROR([cannot continue: Exiting after STL test! ...])

AC_DEFINE([COMPILE_PADRE],1,[Trigger complete trip through all PADRE header files required for PADRE compilation])
PADRE_INCLUDES='-I$(top_srcdir)/PADRE/src -I$(top_builddir)/PADRE'
if test "$with_PADRE" = no; then
     echo "NO PADRE SPECIFIED (PARALLEL_PADRE FALSE) (nothing to setup in PADRE.m4)!"
else
     echo "In PADRE.m4: PADRE SPECIFIED (PARALLEL_PADRE TRUE)!"
     AC_DEFINE([PARALLEL_PADRE],1,[Use PADRE in parallel environment.])
fi

if test ! "$with_PADRE" = 'no'; then
      AC_MSG_RESULT([SETUP P++ FOR USE WITH PADRE (setup PADRE variable in PADRE.m4)!])

    # Since we define USE_PADRE using the AC_DEFINE mechanism we don't need -DUSE_PADRE in CPPFLAGS
      AC_DEFINE([USE_PADRE],1,[Use PADRE within P++.])

      PXX_SUBLIBS='$(top_builddir)/PADRE/src/libPADRE.a'
      test "$enable_Parti" = yes &&	\
        PXX_SUBLIBS="$PXX_SUBLIBS "'${top_builddir}/PARTI/libPARTI_Source.a'
      PXX_SUBLIBS="$PXX_SUBLIBS "'$(top_builddir)/PADRE/src/libPADRE.a $(top_builddir)/PADRE/PGSLIB/libPADRE_PGSLIB_Source.a'
  else
      AC_MSG_RESULT([DO NOT SETUP P++ FOR USE WITH PADRE (setup PADRE variable in PADRE.m4)!])
      PXX_SUBLIBS='$(top_builddir)/PARTI/libPARTI_Source.a $(top_builddir)/PADRE/PGSLIB/libPADRE_PGSLIB_Source.a'
fi


# Support for Parallel Indirect Addressing
if test "$with_INDIRECT_ADDRESSING" = yes; then
   OPTIONAL_INDIRECT_ADDRESSING_SUBDIRS="PADRE/IndirectAddressing"
   INDIRECT_ADDRESSING_INCLUDES="-I\$(top_srcdir)/PADRE/IndirectAddressing -I\$(top_builddir)/PADRE/IndirectAddressing"
   INCLUDES="$INCLUDES $INDIRECT_ADDRESSING_INCLUDES"
   PXX_SUBLIBS="$PXX_SUBLIBS \$(top_builddir)/PADRE/IndirectAddressing/libIndirectAddressing.a"
fi


# DQ: (12/24/2000) This code must appear before the PADRE/nonPADRE modification of LIBS variable
# Add in the P++ library
# BTNG: riptide requires -lPpp to follow -lPpp_static.  I still kept the first
# -lPpp because removing it MAY break on other platforms.
LIBS="$LIBS -L\$(top_builddir)/src -lPpp -lPpp_static -lPpp"

# guess_defaults=yes

# Where PADRE's sublibraries are:
PARTI_DIR='$(top_srcdir)/PARTI'
PARTI_INCLUDES="-I\$(top_srcdir)/PARTI -I\$(top_builddir)/PARTI"
PARTI_LIBS="-L\$(top_srcdir)/PARTI -L\$(top_builddir)/PARTI -lPARTI_Source"

# Normally, we need to specify PARTI_LIBS, but not here because we combine the libraries.
PARTI_LIBS=
PGSLIB_DIR='$(top_srcdir)/PADRE/PGSLIB'
PGSLIB_INCLUDES="-I\$(top_srcdir)/PADRE/PGSLIB -I\$(top_builddir)/PADRE/PGSLIB"
PGSLIB_LIBS="-L\$(top_srcdir)/PADRE/PGSLIB -L\$(top_builddir)/PADRE/PGSLIB -lPADRE_PGSLIB_Source"

# Normally, we need to specify PGSLIB_LIBS, but not here because we combine the libraries.
PGSLIB_LIBS=
PADRE_DIR='$(top_srcdir)/PADRE/src'
PADRE_INCLUDES='-I$(top_srcdir)/PADRE/src -I$(top_builddir)/PADRE'
PADRE_LIBS='-L$(top_builddir)/PADRE/src -lPADRE'

# Normally, we need to specify PADRE_LIBS, but not here because we combine the libraries.
PADRE_LIBS=

AC_SUBST(PARTI_DIR)
AC_SUBST(PARTI_INCLUDES)
AC_SUBST(PARTI_LIBS)
AC_SUBST(PGSLIB_DIR)
AC_SUBST(PGSLIB_INCLUDES)
AC_SUBST(PGSLIB_LIBS)
AC_SUBST(PADRE_DIR)
AC_SUBST(PADRE_INCLUDES)
AC_SUBST(PADRE_LIBS)

# Supporting GlobalArrays in PADRE.
# Handle enable-GlobalArrays and with-GlobalArrays flag.
# The PADRE_SUPPORT_GlobalArrays macro is owned by PADRE (see aclocal call).
# We use the PADRE macro to set up support for compiling PADRE source
# code that uses GlobalArrays.
PADRE_SUPPORT_GlobalArrays
if test "$enable_GlobalArrays" = yes ; then
  test "$GlobalArrays_LIBS" && LIBS="$LIBS $GlobalArrays_LIBS"
  test "$GlobalArrays_INCLUDES" && INCLUDES="$GlobalArrays_INCLUDES $INCLUDES"
fi
BTNG_AC_LOG_VAR(LIBS INCLUDES)

# Inclusion of PADRE and/or its sublibraries.
if test "$with_PADRE" = no; then
   # If PADRE is not specified, then bypass PADRE to get to PARTI and PGSLIB sublibraries. (only require PARTI!)
   # OPTIONAL_PADRE_SUBDIRS="PARTI PADRE/PGSLIB"
   # configure will fail creating PADRE/... if PADRE does not exist.
   # so create PADRE (we should be in P++ at this point).
   # mkdir -p PADRE
   # INCLUDES="$INCLUDES $PARTI_INCLUDES $PGSLIB_INCLUDES"
   # LIBS="$LIBS $PARTI_LIBS $PGSLIB_LIBS"
   INCLUDES="$INCLUDES $PARTI_INCLUDES"
   LIBS="$LIBS $PARTI_LIBS"
   # AC_CONFIG_SUBDIRS( PADRE/PARTI )
else
   # If PADRE is specified, then configure in PADRE
   # without regard to its sublibraries.
   # OPTIONAL_PADRE_SUBDIRS="PADRE"
   INCLUDES="$INCLUDES $PADRE_INCLUDES $PARTI_INCLUDES $PGSLIB_INCLUDES"
   LIBS="$LIBS $PADRE_LIBS $PARTI_LIBS $PGSLIB_LIBS"
   # AC_CONFIG_SUBDIRS( PADRE )
fi

# DQ (1/2/2002): Use the P++ configure to generate the Makefiles in PADRE and PADRE's subdirectories
#                This reduces (and simplifies) the number of times configure must be called.
# Always call PADRE's configure, but use automake's conditional
# compilation mechanism to control what is compiled.
# AC_CONFIG_SUBDIRS( PADRE )

# OPTIONAL_PADRE_SUBDIRS is used in Makefile.am to determine the SUBDIRS list.
AC_SUBST(OPTIONAL_PADRE_SUBDIRS)
BTNG_AC_LOG(OPTIONAL_PADRE_SUBDIRS is $OPTIONAL_PADRE_SUBDIRS)
AC_SUBST(OPTIONAL_INDIRECT_ADDRESSING_SUBDIRS)

AC_SUBST(PADRE_INCLUDES)
AC_SUBST(INCLUDES)
AC_SUBST(PXX_SUBLIBS)

# This is the end of the inlined PPP_AUTOCONFIG_MACRO, inlined.

# set up for use of IS_AIX conditional
AM_CONDITIONAL(IS_AIX,echo "$host_os" | grep '^aix')


# Set up for Dan Quinlan's development tests.
# AC_ARG_ENABLE(dq-developer-tests,
# [--enable-dq-developer-tests   Development option for Dan Quinlan (disregard).])
# AM_CONDITIONAL(DQ_DEVELOPER_TESTS,test "$enable_dq_developer_tests" = yes)

# set up conditional for shared library construction
AM_CONDITIONAL(IF_PPP_BUILD_SHARED_LIBS,test "$PPP_BUILD_SHARED_LIBS" = yes)

# AXXPXX_SUPPORT_PURIFY

# Since we appended MPI to LIBS, we have also appended other libraris
# that use MPI, so we have to add MPI again.  BTNG
LIBS="$LIBS $mpi_LIBS -lc"

# If GlobalArrays is enabled, support Fortran, because GlobalArrays needs it.
if test "$enable_GlobalArrays" = yes; then
  SUPPORT_FORTRAN
  LIBS="$LIBS $Fortran_LIBS"
  # LIBS="$LIBS $FLIBS"
fi

# DQ: (12/23/2000) Added -lm to fix problem with KCC compile line using 
#     Makefile.user.defs (due to PADRE fortran libs (I think))
LIBS="$LIBS $PTHREADS_LIB -lm"


# Set PXX_RPATH to point to indicate where P++ libraries are built.
    rpath=$PWD/src
    case $host_os in
    solaris*)
      # The solaris syntax for specifying PXX_RPATH.
      PXX_RPATH="-R $rpath"
    ;;
    *)
      # Unless you know PXX_RPATH is required, set it to null.
      PXX_RPATH=
    ;;
esac
AC_SUBST(PXX_RPATH)


# AM_CONDITIONAL(PERFORMANCE_TESTS,test ! "$with_PERFORMANCE_TESTS" = no)

# Inclusion of PerformanceTests and/or its sublibraries.
# if test "$with_PERFORMANCE_TESTS" = no; then
#    # If PerformanceTests is not specified, then don't use it.
#    echo "Skipping PerformanceTests!"
#    OPTIONAL_PERFORMANCE_TESTS_SUBDIRS=""
# else
#    # If PADRE is specified, then configure in PERFORMANCE_TESTS
#    # without regard to its sublibraries.
#    echo "Setup PerformanceTests!"
#    OPTIONAL_PERFORMANCE_TESTS_SUBDIRS="PerformanceTests"
#    INCLUDES="$INCLUDES $PERFORMANCE_TESTS_INCLUDES -I$(top_srcdir)/../BenchmarkBase/STL-1995"
#    LIBS="$LIBS $PERFORMANCE_TESTS_LIBS"
#    # AC_CONFIG_SUBDIRS( PerformanceTests )
# fi

# OPTIONAL_PERFORMANCE_TESTS_SUBDIRS is used in Makefile.am to determine the SUBDIRS list.
# AC_SUBST(OPTIONAL_PERFORMANCE_TESTS_SUBDIRS)
# BTNG_AC_LOG(OPTIONAL_PERFORMANCE_TESTS_SUBDIRS is $OPTIONAL_PERFORMANCE_TESTS_SUBDIRS)

# AC_CONFIG_SUBDIRS( $OPTIONAL_PERFORMANCE_TESTS_SUBDIRS )
# AC_CONFIG_SUBDIRS( PerformanceTests )

# DQ: Get rid of this since we don't need it in A++/P++ (but do it later)
# Let user specify where to find sla.
# Specify by --with-sla= or setting sla_PREFIX.
AC_MSG_WARN([SLA still setup in P++!])
BTNG_ARG_WITH_PREFIX(sla,sla_PREFIX)
if test "$sla_PREFIX" ; then
  sla_INCLUDES="-I$sla_PREFIX/include"
  sla_LIBS="-L$sla_PREFIX/lib -lsla"
fi
AC_SUBST(sla_PREFIX)
AC_SUBST(sla_INCLUDES)
AC_SUBST(sla_LIBS)
BTNG_AC_LOG_VAR(sla_INCLUDES)
BTNG_AC_LOG_VAR(sla_LIBS)
# Do not append to INCLUDES and LIBS because sla is not needed everywhere.


# Specify the number of processors to test on
# Later, we will write an autoconf macro to specify this on the configure
# line and also implement small, medium and large specifications.  BTNG.
TEST_NPROCS='1,2,3,4,5'
AC_ARG_WITH(TEST_NPROCS,
[ --with-test-nprocs=LIST	Specify list of nprocs to use in tests ],
TEST_NPROCS=$with_test_nprocs)
AC_SUBST(TEST_NPROCS)

])dnl











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


dnl ********************************************************************
dnl * AXXPXX_PROG_MPICC searches the PATH for an available MPI C compiler
dnl * wraparound.  It assigns the name to MPICC.
dnl ********************************************************************

AC_DEFUN(AXXPXX_PROG_MPICC,
[
   AC_CHECK_PROGS(MPICC, mpcc_r mpcc mpicc tmcc hcc)
   test -z "$MPICC" && AC_MSG_ERROR([no acceptable mpicc found in \$PATH])
])dnl


dnl ********************************************************************
dnl * AXXPXX_PROG_MPICXX searches the PATH for an available MPI C++ 
dnl * compiler wraparound.  It assigns the name to MPICXX.
dnl ********************************************************************

AC_DEFUN(AXXPXX_PROG_MPICXX,
[
   AC_CHECK_PROGS(MPICXX, mpKCC_r mpKCC mpCC mpig++ mpiCC hcp)
   test -z "$MPICXX" && AC_MSG_ERROR([no acceptable mpic++ found in \$PATH])
])dnl


dnl **********************************************************************
dnl * AXXPXX_PROG_MPIF77 searches the PATH for an available MPI Fortran 77 
dnl * compiler wraparound.  It assigns the name to MPIF77.
dnl **********************************************************************

AC_DEFUN(AXXPXX_PROG_MPIF77,
[
   AC_CHECK_PROGS(MPIF77, mpf77 mpxlf mpif77 mpixlf tmf77 hf77)
   test -z "$MPIF77" && AC_MSG_ERROR([no acceptable mpif77 found in \$PATH])
])dnl


dnl ***********************************************************************
dnl * AXXPXX_CHECK_MPIF77_PP checks whether the preprocessor needs to
dnl * be called before calling the compiler for Fortran files with
dnl * preprocessor directives and MPI function calls.  If the preprocessor
dnl * is necessary, MPIF77NEEDSPP is set to "yes", otherwise it is set to
dnl * "no"
dnl ***********************************************************************

AC_DEFUN(AXXPXX_CHECK_MPIF77_PP,
[
   AC_REQUIRE([AXXPXX_PROG_MPIF77])

   rm -f testppmp.o

   AC_MSG_CHECKING(whether $FPP needs to be called before $MPIF77)

   # This follows the same procedur as AXXPXX_CHECK_F77_PP, except it tests
   # $MPIF77 using a test program that includes MPI functions.

   cat > testppmp.F <<EOF
#define FOO 3
	program testppmp
	include 'mpif.h'
	integer rank,size,mpierr,sum
	call MPI_INIT(mpierr)
	call MPI_COMM_SIZE(MPI_COMM_WORLD,size,mpierr)
	call MPI_COMM_RANK(MPI_COMM_WORLD,rank,mpierr)
#ifdef FORTRAN_NO_UNDERSCORE
        sum = rank + size
#else
        sum = rank + rank
#endif
        call MPI_FINALIZE(mpierr)
        end 
EOF

   $MPIF77 -DBAR -c testppmp.F 
   if test -f testppmp.o; then 
      MPIF77NEEDSPP=no 
   else 
      MPIF77NEEDSPP=yes 
   fi

   echo $MPIF77NEEDSPP
   rm -f testppmp.o testppmp.F
   AC_SUBST(MPIF77NEEDSPP)
])dnl


dnl *********************************************************************
dnl * AXXPXX_SET_MPI sets up the needed MPI library and directory flags.   
dnl * The location of the file mpi.h is put into the variable MPIINCLUDE
dnl * as a -I flag.  The -l flags that specify the needed libraries and
dnl * the -L flags that specify the paths of those libraries are placed in
dnl * the variables MPILIBS and MPILIBDIRS, respectively.  To set the MPI
dnl * libraries and directories manually, use the --with-mpi-include,
dnl * --with-mpi-libs, and --with-mpi-lib-dirs command-line options when
dnl * invoking configure.  Only one directory should be specified with
dnl * --with-mpi-include, while any number of directories can be specified
dnl * by --with-mpi-lib-dirs.  Any number of libraries can be specified
dnl * with --with-mpi-libs, and the libraries must be referred to by their 
dnl * base names, so libmpi.a is just mpi.  It is adviseable to use all 
dnl * three --with flags whenever one is used, because it is likely that
dnl * when one is chosen it will mess up the automatic choices for the
dnl * other two.  If the architecture is unknown, or if the needed MPI
dnl * settings for the current architecture are not known, then the naive
dnl * settings of MPILIBS="-lmpi" and MPILIBDIRS="-L/usr/local/mpi/lib"
dnl * are tested, and if they exist they are used, otherwise the MPILIB*
dnl * variables are left blank.  In the case of rs6000, the variable
dnl * MPIFLAGS is also set. 
dnl **********************************************************************

AC_DEFUN(AXXPXX_SET_MPI,
[
   AC_ARG_WITH(mpi-include, [  --with-mpi-include=DIR     DIR is a space-seperated list of include paths for MPI e.g. \"-I/usr/include -I/usr/local/include\"] , [axxpxx_mpi_include_dir=$withval])

   AC_ARG_WITH(mpi-libs, [  --with-mpi-libs=LIBS       LIBS is space-separated list of library names needed for MPI, e.g. \"-lnsl -lsocket -lmpi\"],[axxpxx_mpi_libs=$withval])

   AC_ARG_WITH(mpi-lib-dirs, [  --with-mpi-lib-dirs=DIRS      DIRS is space-separated list of directories containing the libraries specified by \`--with-mpi-libs', e.g \"-L/usr/lib -L/usr/local/mpi/lib\"], [axxpxx_mpi_lib_dirs=$withval])

   AC_ARG_WITH(mpirun, [  --with-mpirun=ARG         ARG is the absolute path of the executable used to run MPI programs.],[mpirun=$withval])

   if test -z "$axxpxx_mpi_libs"; then

      dnl This test replaces axxpxx-GUESS-ARCH
      if test -z "$host_os"; then
        AC_MSG_ERROR([host_os has not been defined in macro axxpxx-SET-MPI])
      fi

      dnl * Set everything to known values
      echo "In AXXPXX SET MPI host_os = $host_os"
      case $host_os in

         sun* | solaris*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir="-I/usr/local/mpi/mpich-1.1.1/include -I/usr/local/mpi/mpich-1.1.1/lib/solaris/ch_p4"
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs="-L/usr/local/mpi/mpich-1.1.1/lib/solaris/ch_p4"
            fi
            
            if test -z "$mpirun"; then
               mpirun=/usr/local/mpi/mpich-1.1.1/bin/mpirun
            fi 

            dnl axxpxx_mpi_libs="-lnsl -lsocket -lmpi";;
            axxpxx_mpi_libs="-lpmpich -lmpich -lsocket -lnsl -lpthread -lnsl -laio -lc";;

         osf*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/opt/MPI190/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs="-L/usr/opt/MPI190/lib"
            fi

            if test -z "$mpirun"; then
               mpirun=/usr/opt/MPI190/bin/dmpirun
            fi 

            axxpxx_mpi_libs="-lmpi";;

         aix*) 
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/lpp/ppe.poe/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/lpp/ppe.poe/lib
            fi

            if test -z "$mpirun"; then
               mpirun=/usr/lpp/ppe.poe/bin/poe
            fi 

            axxpxx_mpi_libs=-lmpi

            MPIFLAGS="-binitfini:poe_remote_main";;

         irix*)
            if test -z "$axxpxx_mpi_include_dir"; then
             # axxpxx_mpi_include_dir="-I/usr/local/mpich-1.1.2/include -I/usr/local/mpich-1.1.2/build/IRIX64/ch_shmem/include"
               axxpxx_mpi_include_dir=
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
             # axxpxx_mpi_lib_dirs=-L/usr/local/mpich-1.1.2/lib/IRIX64/ch_p4
             # axxpxx_mpi_lib_dirs=-L/usr/local/mpich-1.1.2/build/IRIX64/ch_shmem/lib
             # axxpxx_mpi_lib_dirs=-L/usr/lib64
               axxpxx_mpi_lib_dirs=
            fi

            if test -z "$mpirun"; then
               mpirun=/usr/bin/mpirun
             # mpirun=/usr/local/mpich-1.1.2/bin/mpirun
            fi 

            axxpxx_mpi_libs=-lmpi;; 
          # axxpxx_mpi_libs=-lmpich;;

       # Added an entry for linux machines
         linux*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/casc/overture/local-i686-redhat-linux/opt/mpich/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/casc/overture/local-i686-redhat-linux/opt/mpich/lib
            fi

            if test -z "$mpirun"; then
               mpirun=/usr/casc/overture/local-i686-redhat-linux/opt/mpich/bin/mpirun
            fi 

            axxpxx_mpi_libs=-lmpich;; 
        
         *)
            AC_MSG_WARN([trying naive MPI settings - can use --with flags to change])
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/local/mpi/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/local/mpi/lib
            fi
            axxpxx_mpi_libs=-lmpi ;;
      esac

    fi

    if test -n "$axxpxx_mpi_include_dir"; then 
       MPIINCLUDE="$axxpxx_mpi_include_dir"
    else
       MPIINCLUDE=
    fi

  # This can just be set without a loop (which should be an error if there is more than one lib dir in the list!)
    if test -n "$axxpxx_mpi_lib_dirs"; then
       MPILIBDIRS="$axxpxx_mpi_lib_dirs"
    else
       MPILIBDIRS=
    fi

    for axxpxx_lib in $axxpxx_mpi_libs; do
       MPILIBS="$MPILIBS $axxpxx_lib"
    done

    echo "In AXXPXX SET MPI: MPIINCLUDE = $MPIINCLUDE"
    echo "In AXXPXX SET MPI: MPILIBDIRS = $MPILIBDIRS"
    echo "In AXXPXX SET MPI: MPILIBS    = $MPILIBS"
    echo "In AXXPXX SET MPI: MPIFLAGS   = $MPIFLAGS"
    echo "In AXXPXX SET MPI: mpirun     = $mpirun"

   AC_SUBST(MPIINCLUDE)
   AC_SUBST(MPILIBDIRS)
   AC_SUBST(MPILIBS)
   AC_SUBST(MPIFLAGS)
   AC_SUBST(mpirun)
])


# This is an older version of the AXXPXX SET MPI macro (it can be removed)
AC_DEFUN(OLD_AXXPXX_SET_MPI,
[
   AC_ARG_WITH(mpi-include, [  --with-mpi-include=DIR     DIR is a
space-seperated list of include paths for MPI e.g. \"-I/usr/include
-I/usr/local/include\"] , [axxpxx_mpi_include_dir=$withval])

   AC_ARG_WITH(mpi-libs, [  --with-mpi-libs=LIBS       LIBS is space-separated list of library names needed for MPI, e.g. \"-lnsl -lsocket -lmpi\"],[axxpxx_mpi_libs=$withval])

   AC_ARG_WITH(mpi-lib-dirs, [  --with-mpi-lib-dirs=DIRS      DIRS is space-separated list of directories containing the libraries specified by \`--with-mpi-libs', e.g \"-L/usr/lib -L/usr/local/mpi/lib\"], [axxpxx_mpi_lib_dirs=$withval])


   if test -z "$axxpxx_mpi_libs"; then

      dnl This test replaces axxpxx-GUESS-ARCH
      if test -z "$host_os"; then
        AC_MSG_ERROR([host_os has not been defined in macro axxpxx-SET-MPI])
      fi

      dnl * Set everything to known values
      echo "In AXXPXX SET MPI host_os = $host_os"
      case $host_os in

         sun* | solaris*)
            if test -z "$axxpxx_mpi_include_dir"; then
               dnl axxpxx_mpi_include_dir="-I/usr/local/mpi/mpich/include -I/usr/local/mpi/mpich/lib/solaris/ch_p4"
               axxpxx_mpi_include_dir="-I/usr/local/mpi/mpich-1.1.1/include -I/usr/local/mpi/mpich-1.1.1/lib/solaris/ch_p4"
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               dnl axxpxx_mpi_lib_dirs="-L/usr/local/mpi/mpich/lib/solaris/ch_p4 -L/usr/lib"
               axxpxx_mpi_lib_dirs="-L/usr/local/mpi/mpich-1.1.1/lib/solaris/ch_p4"
            fi
            
            dnl axxpxx_mpi_libs="-lnsl -lsocket -lmpi";;
            axxpxx_mpi_libs="-lpmpich -lmpich -lsocket -lnsl -lpthread -lnsl -laio -lc";;

         osf*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/opt/MPI190/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs="-L/usr/opt/MPI190/lib"
            fi

            axxpxx_mpi_libs="-lmpi";;

         aix*) 
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/lpp/ppe.poe/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/lpp/ppe.poe/lib
            fi

            axxpxx_mpi_libs=-lmpi

            MPIFLAGS="-binitfini:poe_remote_main";;

         irix*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/local/mpi/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/local/mpi/lib/IRIX64/ch_p4
            fi

            axxpxx_mpi_libs=-lmpi;; 

       # Added an entry for linux machines
         linux*)
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/casc/overture/local-i686-redhat-linux/opt/mpich/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=/usr/casc/overture/local-i686-redhat-linux/opt/mpich/lib
            fi

            axxpxx_mpi_libs=-lmpich;; 
        
         *)
AC_MSG_WARN([trying naive MPI settings - can use --with flags to change])
            if test -z "$axxpxx_mpi_include_dir"; then
               axxpxx_mpi_include_dir=-I/usr/local/mpi/include
            fi

            if test -z "$axxpxx_mpi_lib_dirs"; then
               axxpxx_mpi_lib_dirs=-L/usr/local/mpi/lib
            fi
            axxpxx_mpi_libs=-lmpi ;;
      esac

    fi

    if test -n "$axxpxx_mpi_include_dir"; then 
       MPIINCLUDE="$axxpxx_mpi_include_dir"
    else
       MPIINCLUDE=
    fi

  # This can just be set without a loop (which should be an error if there is more than one lib dir in the list!)
    if test -n "$axxpxx_mpi_lib_dirs"; then
       MPILIBDIRS="$axxpxx_mpi_lib_dirs"
       # for axxpxx_lib_dir in $axxpxx_mpi_lib_dirs; do
       #    MPILIBDIRS="$axxpxx_lib_dir $MPILIBDIRS"
       # done
    else
       MPILIBDIRS=
    fi

    for axxpxx_lib in $axxpxx_mpi_libs; do
       MPILIBS="$MPILIBS $axxpxx_lib"
    done

    echo "In AXXPXX SET MPI: MPIINCLUDE = $MPIINCLUDE"
    echo "In AXXPXX SET MPI: MPILIBDIRS = $MPILIBDIRS"
    echo "In AXXPXX SET MPI: MPILIBS    = $MPILIBS"
])



dnl ********************************************************************
dnl * AXXPXX_FIND_MPI will determine the libraries, directories, and other
dnl * flags needed to compile and link programs with MPI function calls.
dnl * This macro runs tests on the script found by the AXXPXX_PROG_MPICC
dnl * macro.  If there is no such mpicc-type script in the PATH and
dnl * MPICC is not set manually, then this macro will not work.
dnl *
dnl * One may question why these settings would need to be determined if
dnl * there already is mpicc available, and that is a valid question.  I
dnl * can think of a couple of reasons one may want to use these settings 
dnl * rather than using mpicc directly.  First, these settings allow you
dnl * to choose the C compiler you wish to use rather than using whatever
dnl * compiler is written into mpicc.  Also, the settings determined by
dnl * this macro should also work with C++ and Fortran compilers, so you
dnl * won't need to have mpiCC and mpif77 alongside mpicc.  This is
dnl * especially helpful on systems that don't have mpiCC.  The advantage
dnl * of this macro over AXXPXX_SET_MPI is that this one doesn't require
dnl * a test of the machine type and thus will hopefully work on unknown
dnl * architectures.  The main disadvantage is that it relies on mpicc.
dnl *
dnl * --with-mpi-include, --with-mpi-libs, and --with-mpi-lib-dirs can be
dnl * used to manually override the automatic test, just as with
dnl * AXXPXX_SET_MPI.  If any one of these three options are used, the
dnl * automatic test will not be run, so it is best to call all three
dnl * whenever one is called.  In addition, the option --with-mpi-flags is
dnl * available here to set any other flags that may be needed, but it
dnl * does not override the automatic test.  Flags set by --with-mpi-flags
dnl * will be added to the variable MPIFLAGS.  This way, if the macro, for
dnl * whatever reason, leaves off a necessary flag, the flag can be added 
dnl * to MPIFLAGS without eliminating anything else.  The other variables
dnl * set are MPIINCLUDE, MPILIBS, and MPILIBDIRS, just as in 
dnl * AXXPXX_SET_MPI.  This macro also incorporates AXXPXX_SET_MPI as a backup
dnl * plan, where if there is no mpicc, it will use the settings
dnl * determined by architecture name in AXXPXX_SET_MPI
dnl ********************************************************************

AC_DEFUN(AXXPXX_FIND_MPI,
[
   axxpxx_find_mpi_cache_used=yes

   AC_MSG_CHECKING([for MPI])
   AC_CACHE_VAL(axxpxx_cv_mpi_include, axxpxx_find_mpi_cache_used=no)
   AC_CACHE_VAL(axxpxx_cv_mpi_libs, axxpxx_find_mpi_cache_used=no)
   AC_CACHE_VAL(axxpxx_cv_mpi_lib_dirs, axxpxx_find_mpi_cache_used=no)
   AC_CACHE_VAL(axxpxx_cv_mpi_flags, axxpxx_find_mpi_cache_used=no)
   AC_MSG_RESULT( )

   if test "$axxpxx_find_mpi_cache_used" = "yes"; then

      echo "TEST A: Case of cached values"

      AC_MSG_CHECKING([for location of mpi.h])
      MPIINCLUDE=$axxpxx_cv_mpi_include
      AC_MSG_RESULT([\(cached\) $MPIINCLUDE])

      AC_MSG_CHECKING(for MPI library directories)
      MPILIBDIRS=$axxpxx_cv_mpi_lib_dirs
      AC_MSG_RESULT([\(cached\) $MPILIBDIRS])

      AC_MSG_CHECKING(for MPI libraries)
      MPILIBS=$axxpxx_cv_mpi_libs
      AC_MSG_RESULT([\(cached\) $MPILIBS])

      AC_MSG_CHECKING(for other MPI-related flags)
      MPIFLAGS=$axxpxx_cv_mpi_flags
      AC_MSG_RESULT([\(cached\) $MPIFLAGS])
   else
   
      echo "TEST B: Case of NON cached values"


      dnl * Set up user options.  If user uses any of the fist three options,
      dnl * then automatic tests are not run.

      axxpxx_user_chose_mpi=no
      AC_ARG_WITH(mpi-include, [  --with-mpi-include=DIR  mpi.h is in DIR],
                  for mpi_dir in $withval; do
                     MPIINCLUDE="$MPIINCLUDE -I$withval"
                  done; axxpxx_user_chose_mpi=yes)

      AC_ARG_WITH(mpi-libs,
[  --with-mpi-libs=LIBS    LIBS is space-separated list of library names 
                          needed for MPI, e.g. \"nsl socket mpi\"],  
                  for mpi_lib in $withval; do
                     MPILIBS="$MPILIBS -l$mpi_lib"
                  done; axxpxx_user_chose_mpi=yes)


      AC_ARG_WITH(mpi-lib-dirs,
[  --with-mpi-lib-dirs=DIRS
                          DIRS is space-separated list of directories
                          containing the libraries specified by
                          \`--with-mpi-libs', e.g \"/usr/lib /usr/local/mpi/lib\"],
                  for mpi_lib_dir in $withval; do
                     MPILIBDIRS="-L$mpi_lib_dir $MPILIBDIRS"
                  done; axxpxx_user_chose_mpi=yes)

      dnl * --with-mpi-flags only adds to automatic selections, 
      dnl * does not override

      AC_ARG_WITH(mpi-flags,
[  --with-mpi-flags=FLAGS  FLAGS is space-separated list of whatever flags other
                          than -l and -L are needed to link with mpi libraries],
                          MPIFLAGS=$withval)


      if test "$axxpxx_user_chose_mpi" = "no"; then

      dnl * Find an MPICC.  If there is none, call AXXPXX_SET_MPI to choose MPI
      dnl * settings based on architecture name.  If AXXPXX_SET_MPI fails,
      dnl * print warning message.  Manual MPI settings must be used.

         AC_ARG_WITH(MPICC,
[  --with-MPICC=ARG        ARG is mpicc or similar MPI C compiling tool],
            MPICC=$withval,
            [AC_CHECK_PROGS(MPICC, mpcc_r mpcc mpicc tmcc hcc)])

         if test -z "$MPICC"; then
            AC_MSG_WARN([no acceptable mpicc found in \$PATH])
            ##AXXPXX_SET_MPI
            if test -z "$MPILIBS"; then
             AC_MSG_WARN([MPI not found - must set manually using --with flags])
            fi

         dnl * When $MPICC is there, run the automatic test
         dnl * here begins the hairy stuff

         else      
 
dnl            changequote(, )dnl
  
            AC_MSG_CHECKING([for location of mpi.h])

            dnl * Create a minimal MPI program.  It will be compiled using
            dnl * $MPICC with verbose output.
            cat > mpconftest.c << EOF
#include "mpi.h"

main(int argc, char **argv)
{
   int rank, size;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Finalize();
   return 0;
}
EOF

            axxpxx_mplibs=
            axxpxx_mplibdirs=
            axxpxx_flags=
            axxpxx_lmpi_exists=no

            dnl * These are various ways to produce verbose output from $MPICC
            dnl * All of their outputs are stuffed into variable
            dnl * $axxpxx_mpoutput

            for axxpxx_command in "$MPICC -show"\
                                "$MPICC -v"\
                                "$MPICC -#"\
                                "$MPICC"; do

               axxpxx_this_output=`$axxpxx_command mpconftest.c -o mpconftest 2>&1`

               dnl * If $MPICC uses xlc, then commas must be removed from output
               xlc_p=`echo $axxpxx_this_output | grep xlcentry`
               if test -n "$xlc_p"; then
                  axxpxx_this_output=`echo $axxpxx_this_output | sed 's/,/ /g'`
               fi

               dnl * Turn on flag once -lmpi is found in output
               lmpi_p=`echo $axxpxx_this_output | grep "\-lmpi"`
               if test -n "$lmpi_p"; then
                  axxpxx_lmpi_exists=yes
               fi

               axxpxx_mpoutput="$axxpxx_mpoutput $axxpxx_this_output"
               axxpxx_this_output=

            done

            rm -rf mpconftest*

            dnl * little test to identify $CC as IBM's xlc
            echo "main() {}" > cc_conftest.c
            cc_output=`${CC-cc} -v -o cc_conftest cc_conftest.c 2>&1`
            xlc_p=`echo $cc_output | grep xlcentry`
            if test -n "$xlc_p"; then
               axxpxx_compiler_is_xlc=yes
            fi 
            rm -rf cc_conftest*

            dnl * $MPICC might not produce '-lmpi', but we still need it.
            dnl * Add -lmpi to $axxpxx_mplibs if it was never found
            if test "$axxpxx_lmpi_exists" = "no"; then
               axxpxx_mplibs="-lmpi"
            else
               axxpxx_mplibs=
            fi

            axxpxx_want_arg=

            dnl * Loop through every word in output to find possible flags.
            dnl * If the word is the absolute path of a library, it is added
            dnl * to $axxpxx_flags.  Any "-llib", "-L/dir", "-R/dir" and
            dnl * "-I/dir" is kept.  If '-l', '-L', '-R', '-I', '-u', or '-Y'
            dnl * appears alone, then the next word is checked.  If the next
            dnl * word is another flag beginning with '-', then the first
            dnl * word is discarded.  If the next word is anything else, then
            dnl * the two words are coupled in the $axxpxx_arg variable.
            dnl * "-binitfini:poe_remote_main" is a flag needed especially
            dnl * for IBM MPI, and it is always kept if it is found.
            dnl * Any other word is discarded.  Also, after a word is found
            dnl * and kept once, it is discarded if it appears again

            echo "TEST C: $axxpxx_mpoutput"

            for axxpxx_arg in $axxpxx_mpoutput; do

               echo "Top of loop"
               echo "axxpxx_arg = $axxpxx_arg"

               axxpxx_old_want_arg=$axxpxx_want_arg
               axxpxx_want_arg=  

               if test -n "$axxpxx_old_want_arg"; then
                  case "$axxpxx_arg" in
                  -*)
                     axxpxx_old_want_arg=
                  ;;
                  esac
               fi

               case "$axxpxx_old_want_arg" in
               '')
                  case $axxpxx_arg in
                  /*.a)
                     exists=false
                     for f in $axxpxx_flags; do
                        if test x$axxpxx_arg = x$f; then
                           exists=true
                        fi
                     done
                     if $exists; then
                        axxpxx_arg=
                     else
                        axxpxx_flags="$axxpxx_flags $axxpxx_arg"
                     fi
                  ;;
                  -binitfini:poe_remote_main)
                     exists=false
                     for f in $axxpxx_flags; do
                        if test x$axxpxx_arg = x$f; then
                           exists=true
                        fi
                     done
                     if $exists; then
                        axxpxx_arg=
                     else
                        axxpxx_flags="$axxpxx_flags $axxpxx_arg"
                     fi
                  ;;
                  -lang*)
                     axxpxx_arg=
                  ;;
                  -[lLR])
                     axxpxx_want_arg=$axxpxx_arg
                     axxpxx_arg=
                  ;;
                  -[lLR]*)
                     exists=false
                     for f in $axxpxx_flags; do
                        if test x$axxpxx_arg = x$f; then
                           exists=true
                        fi
                     done
                     if $exists; then
                        axxpxx_arg=
                     else
                       axxpxx_flags="$axxpxx_flags $axxpxx_arg"
                     fi
                  ;;
                  -u)
                     axxpxx_want_arg=$axxpxx_arg
                     axxpxx_arg=
                  ;;
                  -Y)
                     axxpxx_want_arg=$axxpxx_arg
                     axxpxx_arg=
                  ;;
                  -I)
                     axxpxx_want_arg=$axxpxx_arg
                     axxpxx_arg=
                  ;;
                  -I*)
                     exists=false
                     for f in $axxpxx_flags; do
                        if test x$axxpxx_arg = x$f; then
                           exists=true
                        fi
                     done
                     if $exists; then
                        axxpxx_arg=
                     else
                        axxpxx_flags="$axxpxx_flags $axxpxx_arg"
                     fi
                  ;;
                  *)
                     axxpxx_arg=
                  ;;
                  esac

               ;;
               -[lLRI])
                  axxpxx_arg="axxpxx_old_want_arg $axxpxx_arg"
               ;;
               -u)
                  axxpxx_arg="-u $axxpxx_arg"
               ;;
               -Y)
                  axxpxx_arg=`echo $axxpxx_arg | sed -e 's%^P,%%'`
                  SAVE_IFS=$IFS
                  IFS=:
                  axxpxx_list=
                  for axxpxx_elt in $axxpxx_arg; do
                     axxpxx_list="$axxpxx_list -L$axxpxx_elt"
                  done
                  IFS=$SAVE_IFS
                  axxpxx_arg="$axxpxx_list"
               ;;
               esac

               dnl * Still inside the big for loop, we separate each flag
               dnl * into includes, libdirs, libs, flags
               if test -n "$axxpxx_arg"; then
                  case $axxpxx_arg in
                  -I*)

                     dnl * if the directory given in this flag contains mpi.h
                     dnl * then the flag is assigned to $MPIINCLUDE
                     if test -z "$MPIINCLUDE"; then
                        axxpxx_cppflags="$axxpxx_cppflags $axxpxx_arg"
                        axxpxx_include_dir=`echo "$axxpxx_arg" | sed 's/-I//g'` 

                        SAVE_CPPFLAGS="$CPPFLAGS"
                        CPPFLAGS="$axxpxx_cppflags"
dnl                        changequote([, ])dnl

                        unset ac_cv_header_mpi_h
                        AC_CHECK_HEADER(mpi.h,
                                        MPIINCLUDE="$axxpxx_cppflags")

dnl                        changequote(, )dnl
                        CPPFLAGS="$SAVE_CPPFLAGS"

                     else
                        axxpxx_arg=
                     fi
                  ;;
                  -[LR]*)

                     dnl * These are the lib directory flags
                     axxpxx_mplibdirs="$axxpxx_mplibdirs $axxpxx_arg"
                  ;;
                  -l* | /*)

                     dnl * These are the libraries
                     axxpxx_mplibs="$axxpxx_mplibs $axxpxx_arg"
                  ;;
                  -binitfini:poe_remote_main)
                     if test "$axxpxx_compiler_is_xlc" = "yes"; then
                        axxpxx_mpflags="$axxpxx_mpflags $axxpxx_arg"
                     fi
                  ;;
                  *)
                     dnl * any other flag that has been kept goes here
                     axxpxx_mpflags="$axxpxx_mpflags $axxpxx_arg"
                  ;;
                  esac

                  dnl * Upcoming test needs $LIBS to contain the flags 
                  dnl * we've found
                  LIBS_SAVE=$LIBS
                  LIBS="$MPIINCLUDE $axxpxx_mpflags $axxpxx_mplibdirs $axxpxx_mplibs"

                  if test -n "`echo $LIBS | grep '\-R/'`"; then
                     LIBS=`echo $LIBS | sed 's/-R\//-R \//'`
                  fi

dnl                  changequote([, ])dnl


                  dnl * Test to see if flags found up to this point are
                  dnl * sufficient to compile and link test program.  If not,
                  dnl * the loop keeps going to the next word
                  AC_TRY_LINK(
dnl [#ifdef __cplusplus
dnl extern "C"
dnl #endif
dnl ]
[#include "mpi.h"
], [int rank, size;
   int argc;
   char **argv;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Finalize();
],
                     axxpxx_result=yes)

                  LIBS=$LIBS_SAVE

                  if test "$axxpxx_result" = yes; then
                     axxpxx_result=
                     break
                  fi
               fi

               echo "Bottom of loop"
               echo "axxpxx_flags = $axxpxx_flags"
               echo "axxpxx_mpflags = $axxpxx_mpflags"
               echo "axxpxx_mplibs = $axxpxx_mplibs"
               echo "axxpxx_mplibdirs = $axxpxx_mplibdirs"

            done

            echo "TEST D: After loop is done, set variables to be substituted"
            dnl * After loop is done, set variables to be substituted
            MPILIBS=$axxpxx_mplibs
            MPILIBDIRS=$axxpxx_mplibdirs
            MPIFLAGS="$MPIFLAGS $axxpxx_mpflags"

            dnl * IBM MPI uses /usr/lpp/ppe.poe/libc.a instead of /lib/libc.a
            dnl * so we need to make sure that -L/lib is not part of the 
            dnl * linking line when we use IBM MPI.  This only appears in
            dnl * configure when AXXPXX_FIND_MPI is called first.
            ifdef([AC_PROVIDE_AXXPXX_FIND_F77LIBS], 
               if test -n "`echo $F77LIBFLAGS | grep '\-L/lib '`"; then
                  if test -n "`echo $F77LIBFLAGS | grep xlf`"; then
                     F77LIBFLAGS=`echo $F77LIBFLAGS | sed 's/-L\/lib //g'`
                  fi
               fi
            )

            echo "TEST E:"

            if test -n "`echo $MPILIBS | grep pmpich`" &&
               test -z "`echo $MPILIBS | grep pthread`"; then
                  LIBS_SAVE=$LIBS
                  LIBS="$MPIINCLUDE $MPIFLAGS $MPILIBDIRS $MPILIBS -lpthread"
                  AC_TRY_LINK(
dnl                      ifelse(AC_LANG, CPLUSPLUS,
dnl [#ifdef __cplusplus
dnl extern "C"
dnl #endif
dnl ])dnl
[#include "mpi.h"
], [int rank, size;
   int argc;
   char **argv;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Finalize();
],
                     MPILIBS="$MPILIBS -lpthread")
                  LIBS=$LIBS_SAVE
            fi
          

            echo "TEST F:"

            AC_MSG_RESULT($MPIINCLUDE)
            AC_MSG_CHECKING([for MPI library directories])
            AC_MSG_RESULT($MPILIBDIRS)
            AC_MSG_CHECKING([for MPI libraries])
            AC_MSG_RESULT($MPILIBS)
            AC_MSG_CHECKING([for other MPI-related flags])
            AC_MSG_RESULT($MPIFLAGS)
         fi
      fi

      AC_CACHE_VAL(axxpxx_cv_mpi_include, axxpxx_cv_mpi_include=$MPIINCLUDE)
      AC_CACHE_VAL(axxpxx_cv_mpi_lib_dirs, axxpxx_cv_mpi_lib_dirs=$MPILIBDIRS)
      AC_CACHE_VAL(axxpxx_cv_mpi_libs, axxpxx_cv_mpi_libs=$MPILIBS)
      AC_CACHE_VAL(axxpxx_cv_mpi_flags, axxpxx_cv_mpi_flags=$MPIFLAGS)
   fi

   AC_SUBST(MPIINCLUDE)
   AC_SUBST(MPILIBDIRS)
   AC_SUBST(MPILIBS)
   AC_SUBST(MPIFLAGS)

])dnl

AC_DEFUN(PPP_MPIRUN_CHECK,
[
  dnl 
  dnl This macro exports via AC_SUBST the command for running 
  dnl mpi programs in the variable MPIRUN.  It also caches the
  dnl information whether we can compile and link a trivial mpi
  dnl program and whether we can run the simple mpi program on
  dnl 1,..,6 proceses.
  dnl
  dnl * CASC_FIND_MPI finds the include paths and library paths
  dnl and the libraries to compile and link a trivial mpi program.
  dnl We use these to test the running of a simple mpi program using
  dnl various numbers of processes.
  dnl *

  # Make sure PWD is correct.  Bourne shells do not set PWD, so there
  # is a potential that PWD is incorrect if this script is launched
  # by a Bourne shell.
  PWD=`pwd`
  echo PPP MPI RUN CHECK IS IN $PWD

  dnl We should not be looking for MPI in this macro or we will
  dnl be looking for it redundantly.  Look for it BEFORE calling
  dnl this macro.  BTNG
  dnl CASC_FIND_MPI

  dnl We should not be fixing the CPPFLAGS here.  It should be set
  dnl by a higher level script before calling this macro.  BTNG
  dnl CPPFLAGS="$mpi_INCLUDES $CPPFLAGS"

  dnl LDFLAGS should be prepended not reset
  dnl LDFLAGS="$MPILIBDIRS $LDFLAGS"
  dnl LDFLAGS="$MPILIBDIRS"
  dnl LDFLAGS should not be used to specify library locations.  And
  dnl it appears to be unused in this file, so I am removing this block
  dnl of codes.  BTNG.


  dnl LIBS should be prepended not reset
  dnl LIBS="$MPILIBS -lc -lm"
  dnl LIBS="$LIBS $MPILIBS -lc -lm"
  dnl In order to prevent redundant specification of libraries, assume
  dnl that LIBS is already accurately set.  If it is not, please correct
  dnl before calling this macro.

  # Make sure AR is defined, because it may be used in CXX_SHARED_LIB_UPDATE.
  if test ! "$AR"; then
    AC_CHECK_PROG(AR, ar,, ar)
  fi

  cxx_shared_lib_update=`eval "echo $CXX_SHARED_LIB_UPDATE"`
  cxx_static_lib_update=`eval "echo $CXX_STATIC_LIB_UPDATE"`

  AC_LANG_SAVE

# Test MPI using C compiler (Insure++ can't use -lpthread with C++)
# AC_LANG_CPLUSPLUS
  AC_LANG_C

  ppp_find_mpirun_cache_used=yes 

  AC_MSG_CHECKING(existence and behavior of mpirun)
  AC_CACHE_VAL(ppp_cv_mpirun,ppp_find_mpirun_cache_used=no)
  AC_CACHE_VAL(ppp_cv_mpirun_execution,ppp_find_mpirun_cache_used=no)
  AC_CACHE_VAL(ppp_cv_mpirun_machinefile,ppp_find_mpirun_cache_used=no)
  AC_CACHE_VAL(ppp_cv_build_shared_libs,ppp_find_mpirun_cache_used=no)
  AC_CACHE_VAL(ppp_cv_mpi_shared_lib_target,ppp_find_mpirun_cache_used=no)
  AC_CACHE_VAL(ppp_cv_pxx_rpath,ppp_find_mpirun_cache_used=no)
 
  AC_MSG_RESULT(finished checking for cached mpi variables)

  if test "$ppp_find_mpirun_cache_used" = yes;then
    AC_MSG_CHECKING(for mpirun)
    mpirun=$ppp_cv_mpirun
    AC_MSG_RESULT("\(cached\) $mpirun")

    AC_MSG_CHECKING(for successful execution of mpirun)
    MPIRUN_RESULT=$ppp_cv_mpirun_execution
    AC_MSG_RESULT("\(cached\) result of mpirun execution: $MPIRUN_RESULT")

    AC_MSG_CHECKING(for use of machinefile)
    MPIRUN_MACHINEFILE=$ppp_cv_mpirun_machinefile
    AC_MSG_RESULT("\(cached\) machinefile used: $MPIRUN_MACHINEFILE") 

    AC_MSG_CHECKING(for whether to build P++ shared library)
    PPP_BUILD_SHARED_LIBS=$ppp_cv_build_shared_libs
    AC_MSG_RESULT("\(cached\) $PPP_BUILD_SHARED_LIBS")

    AC_MSG_CHECKING(for P++ shared library target)
    PPP_BUILD_SHARED_LIB_TARGET=$ppp_cv_mpi_shared_lib_target
    AC_MSG_RESULT("\(cached\) P++ shared library target: $PPP_BUILD_SHARED_LIB_TARGET")

    AC_MSG_CHECKING(for P++ PXX_RPATH)
    PXX_RPATH=$ppp_cv_pxx_rpath
    AC_MSG_RESULT("\(cached\): $PXX_RPATH")

  else 

    MPIRUN_RESULT=no
    MPIRUN_MACHINEFILE=""

dnl if test "$ARCH" = rs6000; then
dnl     cat > conftest.$ac_ext << EOF
dnl #include "mpi.h"
dnl 
dnl main(int argc, char **argv)
dnl {
dnl    int rank, size;
dnl    MPI_Init(&argc, &argv);
dnl    MPI_Comm_size(MPI_COMM_WORLD, &size);
dnl    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
dnl    MPI_Finalize();
dnl    return 0;
dnl }
dnl EOF
dnl else
dnl    cat > conftest.$ac_ext << EOF

dnl changed file extension to .c to forrce use of C compiler
dnl    cat > conftest.C << EOF
    cat > conftest.c << EOF
dnl #ifdef __cplusplus
dnl extern "C"
dnl #endif
#include "mpi.h"
#include "stdio.h"

main(int argc, char **argv)
{
   int rank, size;
   printf ("At start of main in MPI test program! \n");
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   printf ("%d/%d\n", rank, size);
   fflush(stdout);
   MPI_Finalize();
   printf ("Program Terminated Normally! \n");
   return 0;
}
EOF
dnl fi

    dnl * note that the first part writes to the config.log file so we
    dnl have a record of the compile line that works *

    echo "DQ debugging compiler command for simple MPI program is ac_link = $ac_link"

    dnl if { (eval echo configure:__oline__: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; }; then
    if { (eval echo configure:__oline__: \"$ac_link\") 1>&5; (eval $ac_link); }; then

      dnl * here the program was successfully compiled and linked
      echo "DQ simple mpi program sucessfully compiled and linked"
 
      dnl
      dnl * look for mpirun command and machine file.
      dnl If we don't find a file named "machine.file" in the 
      dnl current or home directory, we build one in this directory
      dnl that consists of the machine name repeated once
      dnl *
      PATH_SAVE=$PATH
      dnl substitute : for -I and bin for include in the include path
      dnl and set the search path for the mpirun executable to this.
      IPATH=`echo $mpi_INCLUDES | sed -e 's/[[ 	]]\{1,\}-I\//:\//g' -e 's/include/bin/g'`
      PATH="$PATH_SAVE$IPATH"
      BTNG_AC_LOG("Looking in $PATH for mpirun-type programs")
      AC_ARG_WITH(mpirun,
                  [ --with-mpirun=ARG	ARG is mpirun or equivalent],
                    mpirun=$withval,
              dnl Skip the search for dmpirun since it requires linking to a different library (than the default)
              dnl [AC_CHECK_PROGS(mpirun,dmpirun poe mpirun)]
                  [AC_CHECK_PROGS(mpirun, poe mpirun)]
                 )
      PATH=$PATH_SAVE

      if test -z "$mpirun"; then
        AC_MSG_WARN(["no mpirun found in $PATH. Runtime tests will fail."])
        exit 1
      fi 

      dnl
      dnl set up some poefe runtime variables 
      dnl
      dnl if test "$ARCH" = rs6000; then
      case "$host_os" in
	aix*)
          NUMBER_OF_PROCS="-procs 2"
          NUMBER_OF_NODES="-nodes 1"
          RMPOOL="-rmpool 0"
	;;
	*)      
          NUMBER_OF_PROCS="-np 2"
          NUMBER_OF_NODES=""
          RMPOOL=""
	;;
      esac

      dnl 
      dnl * allow specification of an mpi machine file, or do hard work
      dnl to search for one, and if all else fails, build one in the
      dnl current directory
      dnl 
      AC_ARG_WITH(mpi-machinefile,
                  [--with-mpi-machinefile=FNAME    FNAME lists machines to run mpi progs on],
                  [MPIRUN_MACHINEFILE="-machinefile $withval"],
                  [MPIRUN_MACHINEFILE=""
                   dnl machine file not specified on configure line,
                   dnl we use MPIRUN on the test program without
                   dnl specifying a machinefile and see if it works.
                   dnl If so, we leave machinefile blank, if not,
                   dnl we build a file in this directory and try the
                   dnl mpirun command again.
                   AC_MSG_CHECKING(Testing 2 process mpirun without machinefile.)

                   PPP_NEEDS_NO_MACHINE_FILE="no"
                   dnl if test "$ARCH" = rs6000; then
		   # poe has different command line syntax from mpirun.
		   case "$mpirun" in
		     *poe) command="$mpirun ./conftest $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE" ;;
		     *) command="$mpirun $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE ./conftest" ;;
		   esac
                   echo executing $command 1>&5
                   $command > ./out.mpitest 2>&1
                   test_result=$?
		   echo "Results of mpi run:" 1>&5; cat ./out.mpitest 1>&5;
                   if test "$test_result" = 0; then
                     expect_proc=0
                     total_procs=`echo $NUMBER_OF_PROCS | sed 's/^.* //'`
                     while test $expect_proc -lt $total_procs; do
                       echo grepping for $expect_proc/$total_procs in out.mpitest. 1>&5
                       grep "$expect_proc/$total_procs$" out.mpitest 1>&5 2>&1
                       if test $? -ne 0 ; then
	                 cat <<_EOM_
ERROR: Processor $expect_proc of $total_procs is not reporting
the right rank or size or both in the mpi test.  There may be
something wrong with the way mpi applications are linked and run.
_EOM_
	                 exit 1
                       fi
                       expect_proc=`expr $expect_proc + 1`
                     done
                     rm -f out.mpitest
                     PPP_NEEDS_NO_MACHINE_FILE="yes"
                   fi
dnl                    else
dnl                      echo "$mpirun $RMPOOL $NUMBER_OF_PROCS $NUMBER_OF_NODES conftest"
dnl                      $mpirun $RMPOOL $NUMBER_OF_PROCS $NUMBER_OF_NODES conftest > ./out.mpitest 2>&1
dnl                      test_result=$?
dnl 		     echo "Results of mpi run:" 1>&5; cat ./out.mpitest 1>&5;
dnl                      if test "$test_result" = 0; then
dnl expect_proc=0
dnl total_procs=`echo $NUMBER_OF_PROCS | sed 's/^.* //'`
dnl while test $expect_proc -lt $total_procs; do
dnl     echo grepping for $expect_proc/$total_procs in out.mpitest. 1>&5
dnl     grep "$expect_proc/$total_procs$" out.mpitest > /dev/null 2>&1
dnl     if test $? -ne 0 ; then
dnl 	cat <<_EOM_
dnl ERROR: Processor $expect_proc of $total_procs is not reporting
dnl the right rank or size or both in the mpi test.  There may be
dnl something wrong with the way mpi applications are linked and run.
dnl _EOM_
dnl 	exit 1
dnl     fi
dnl     expect_proc=`expr $expect_proc + 1`
dnl done
dnl rm -f out.mpitest
dnl                        PPP_NEEDS_NO_MACHINE_FILE="yes"
dnl                      fi
dnl                    fi

                   if test "$PPP_NEEDS_NO_MACHINE_FILE" = yes; then
                     AC_MSG_RESULT([Passed using no machinefile.])
                   else
                     AC_MSG_RESULT([Failed using no machinefile, we'll find or build one.])
                     CURR_PATH=${PWD}

                     for m_f_path in "${PWD}" "${HOME}"; do
                       MACHINE_FILE="$m_f_path/machine.file"
		       AC_MSG_CHECKING(["for $MACHINE_FILE"])

                       if test -r "$MACHINE_FILE"; then
                         ppp_found_machine_file=yes
		         AC_MSG_RESULT(["Found $MACHINE_FILE"])
                         MPIRUN_MACHINEFILE="-machinefile $MACHINE_FILE"
                         break
                       else
                         ppp_found_machine_file=no
		         AC_MSG_RESULT(["Did not find $MACHINE_FILE"])
                       fi
                     done 

                     dnl * machine.file not found, we now build one in this
                     dnl directory which lists the name of this machine once.
                     dnl *
                     if test "$ppp_found_machine_file" = no; then
                       echo "Machine file not found in usual places, building one here."
                       MACHINE_NAME=`uname -n`

                   dnl Since the machine file is a cached value it can't use a relative path
                   dnl (or is would have to access a PXX_HOME variable setup differently in 
                   dnl each direction (which is not possible))
                       cat>"./machine.file"<<EOF
$MACHINE_NAME
EOF
                   dnl MACHINE_FILE="${PWD}/machine.file"
                   dnl MPIRUN_MACHINEFILE="-machinefile ${PWD}/machine.file"
                       dnl MACHINE_FILE="./machine.file"
		       MACHINE_FILE=`pwd`/machine.file
                       MPIRUN_MACHINEFILE="-machinefile $MACHINE_FILE"
                     fi
         
                     if test ! -r "$MACHINE_FILE"; then
                       AC_MSG_WARN([mpi-machinefile does not exist!  Runtime tests will fail])
                       exit 1
                     fi

                     AC_MSG_CHECKING(Testing 2 process mpirun with machinefile.)

		     # poe has different command line syntax from mpirun.
		     case "$mpirun" in
		       *poe) command="$mpirun ./conftest $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE" ;;
		       *) command="$mpirun $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE ./conftest" ;;
		     esac
                     echo "DQ debugging note: now run the comment = $command"
                     echo $command
                     $command > ./out.mpitest 2>&1
                     test_result=$?
		     echo "Results of mpi run:" 1>&5; cat ./out.mpitest 1>&5;
		     if test "$test_result" = 0; then
		       expect_proc=0
		       total_procs=`echo $NUMBER_OF_PROCS | sed 's/^.* //'`
		       while test $expect_proc -lt $total_procs; do
    		         echo grepping for $expect_proc/$total_procs in out.mpitest. 1>&5
    		         grep "$expect_proc/$total_procs$" out.mpitest 1>&5 2>&1
    		         if test $? -ne 0 ; then
			   cat <<_EOM_
ERROR: Processor $expect_proc of $total_procs is not reporting
the right rank or size or both in the mpi test.  There may be
something wrong with the way mpi applications are linked and run.
_EOM_
			   exit 1
    		         fi
    		         expect_proc=`expr $expect_proc + 1`
		       done
		       rm -f out.mpitest
                       AC_MSG_RESULT([Passed using machinefile.])
                     else
                       AC_MSG_RESULT([Failed using machinefile, exiting.])
                       exit 1
                     fi
                   fi
                  ])dnl

      dnl DQ: note for debugging
      echo "DQ Debugging note: Now run the trivial mpi program with various numbers of processes using a given machine file"

      dnl * run the trivial mpi program with various numbers of
      dnl processes using a given machine file *
      dnl
         
      dnl * dump a representative mpirun command line to the config.log file 
      dnl if test "$ARCH" = rs6000; then
      if echo $host_os | grep '^aix' > /dev/null; then
        eval echo configure:__oline__: \"$mpirun conftest $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE $NUMBER_OF_NODES $RMPOOL\" 1>&5
      else
      	eval echo configure:__oline__: \"$mpirun $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE $NUMBER_OF_NODES $RMPOOL conftest\" 1>&5
      fi

dnl Simplify for solaris
dnl   for i in "1" "2" "3" "4" "5" "6"; do
dnl   for i in "1" "2" "3" "4"; do
      for i in "1" "2"; do

        dnl if test "$ARCH" = rs6000; then 
        if echo $host_os | grep '^aix' > /dev/null; then
dnl YIKES!  double brackets are needed to keep one level left after quoting?!?
	  RMPOOL="-rmpool 0"
          if [[ "$i" -lt 5 ]]; then
            NUMBER_OF_NODES="-nodes 1"
          else
            NUMBER_OF_NODES="-nodes 2"
          fi
          NUMBER_OF_PROCS="-procs $i"
        else
          NUMBER_OF_PROCS="-np $i"
          RMPOOL=""
        fi

        AC_MSG_CHECKING(trivial mpi program using $i processes)
	# poe has different command line syntax from mpirun.
	case "$mpirun" in
	  *poe) command="$mpirun ./conftest $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE";;
	  *) command="$mpirun $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE ./conftest";;
	esac
	echo "executing $command" 1>&5
        $command 1>&5 /dev/null 2>&1
	test_result=$?
	if test $test_result = 0; then
          AC_MSG_RESULT([Passed])
        else
          AC_MSG_ERROR([Failed with exit value $exit_value])
	fi
dnl         if test "$ARCH" = rs6000; then
dnl         if echo $host_os | grep '^aix' > /dev/null; then
dnl           echo "$mpirun conftest $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE"
dnl           if $mpirun conftest $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE > /dev/null 2> /dev/null; then
dnl             AC_MSG_RESULT([Passed])
dnl           else
dnl             AC_MSG_RESULT([Failed])
dnl           fi
dnl         else
dnl           echo "$mpirun $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE conftest"
dnl           if $mpirun $RMPOOL $NUMBER_OF_NODES $NUMBER_OF_PROCS $MPIRUN_MACHINEFILE conftest > /dev/null 2> /dev/null; then
dnl             AC_MSG_RESULT([Passed])
dnl           else
dnl             AC_MSG_RESULT([Failed])
dnl           fi
dnl         fi

      done

      MPIRUN_RESULT=yes

    else
dnl    * here the compilation or linking failed
dnl      rm -rf conftest*
      echo "trivial mpi program NOT compiled and linked"
      exit 1
    fi

dnl    rm -f conftest* 
 
    AC_LANG_RESTORE

    dnl **********************************************
    dnl * here we build and test shared library for
    dnl * a trivial mpi program
    dnl ********************************************** 
    if test "$SHARED_LIBS" = yes; then

  echo "Testing shared libraries with MPI."
  echo "Testing shared libraries with MPI." 1>&5

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
#include "mpi.h"
#include "func1.h"
#include "func2.h"
#include "stdio.h"
int main(int argc, char **argv){
   int a1=func1(),a2=func2();
   int rank, size;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   printf ("%d/%d\n", rank, size);
   MPI_Finalize();
   return 0;
return 0;}
__EOM__
 
    PPP_BUILD_SHARED_LIBS="yes"

    command="$CC $C_DL_COMPILE_FLAGS -I./SrcForSharedLibTest -c ./SrcForSharedLibTest/func1.c"
    echo $command 1>&5
    if $command 1>&5 2>&1 ; then
      echo "Compilation of C file using $C_DL_COMPILE_FLAGS PIC flags passed." 1>&5
      mv func1.o ./SrcForSharedLibTest/func1.o
    else
      echo "Compilation of C file using $C_DL_COMPILE_FLAGS PIC flags FAILED." 1>&5
      PPP_BUILD_SHARED_LIBS="no"
    fi

    command="$CXX $CXX_DL_COMPILE_FLAGS -I./SrcForSharedLibTest -c ./SrcForSharedLibTest/func2.C"
    echo $command 1>&5
    if $command 1>&5 2>&1 ; then
      echo "Compilation of C++ file using $CXX_DL_COMPILE_FLAGS PIC flags passed." 1>&5
      mv func2.o ./SrcForSharedLibTest/func2.o
    else
      echo "Compilation of C++ file using $CXX_DL_COMPILE_FLAGS PIC flags FAILED." 1>&5
      PPP_BUILD_SHARED_LIBS="no"
    fi

dnl *
dnl * Build a static library from func1.o, build a shared library from func2.o
dnl *
dnl 
    echo "Build a static library from func1.o" 1>&5

    command="$cxx_static_lib_update libfunc1_static.a ./SrcForSharedLibTest/func1.o"
    echo $command 1>&5
    if $command 1>&5 2>&1 ; then
      echo "...OK" 1>&5
      mv libfunc1_static.a ./SrcForSharedLibTest/libfunc1_static.a
    else
      echo "...FAILED" 1>&5
      exit 1
    fi

    command="$cxx_static_lib_update libfunc2.a ./SrcForSharedLibTest/func2.o"
    echo $command 1>&5
    if $command 1>&5 2>&1 ; then
      echo "...OK" 1>&5
      mv libfunc2.a ./SrcForSharedLibTest/libfunc2.a
    else
      echo "...FAILED" 1>&5
      exit 1
    fi

    echo "Build a shared library from func2.o" 1>&5

    command="$cxx_shared_lib_update ./SrcForSharedLibTest/libfunc2.$SHARED_LIB_EXTENSION ./SrcForSharedLibTest/func2.o"
    echo $command 1>&5
dnl    if $cxx_shared_lib_update ./SrcForSharedLibTest/libfunc2.$SHARED_LIB_EXTENSION ./SrcForSharedLibTest/func2.o > /dev/null 2> /dev/null; then
    if $command 1>&5 2>&1 ; then
      echo "...OK" 1>&5
    else
      echo "...FAILED" 1>&5
      PPP_BUILD_SHARED_LIBS="no"
    fi

    # Define rpath to the directory of the shared lib
    # using full path specification (no relative path).
    rpath="${PWD}/SrcForSharedLibTest"
    case $host_os in
	solaris*)
	  # The solaris syntax for specifying RPATH.
	  TMP_RPATH="-R $rpath"
	;;
	*)
	  # Unless you know RPATH is required, set it to null.
	  TMP_RPATH=
	;;
    esac
dnl     dnl if test $ARCH = solaris; then
dnl     if echo $host_os | grep '^solaris' > /dev/null; then
dnl     dnl PXX_RPATH="-R ${PWD}/P++/config/SrcForSharedLibTest"
dnl     dnl PXX_RPATH="-R${PWD}/./../P++/config/SrcForSharedLibTest"
dnl 	PXX_RPATH="-R${PWD}/config/SrcForSharedLibTest"
dnl     else
dnl   	PXX_RPATH=
dnl     fi	

dnl *
dnl * compiling the main program with the shared library and static library
dnl *
    echo "Compiling the main program with the shared library and static library." 1>&5
    dnl command="$CXX $CXX_OPTIONS $RUNTIME_LOADER_FLAGS $mpi_INCLUDES -o ./SrcForSharedLibTest/test ./SrcForSharedLibTest/main.C $MPILIBDIRS -L./SrcForSharedLibTest -lfunc1_static $TMP_RPATH -lfunc2 $MPILIBS -lc -lm"
    command="$CXX $CXX_OPTIONS $RUNTIME_LOADER_FLAGS $mpi_INCLUDES -o ./SrcForSharedLibTest/test ./SrcForSharedLibTest/main.C -L./SrcForSharedLibTest -lfunc1_static $TMP_RPATH -lfunc2 $mpi_LIBS -lc -lm"
    echo $command 1>&5
    if $command 1>&5 2>&1 ; then
      echo "...OK" 1>&5
    else
      echo "...FAILED" 1>&5
      PPP_BUILD_SHARED_LIBS="no"
    fi

dnl *
dnl * building execute script to run the test
dnl *
    echo "Building execute script to run the test." 1>&5
    case $host_os in
    aix*)
      cat>./SrcForSharedLibTest/runTest<<EOF
#!/bin/sh
MP_RESD="YES" MP_HOSTFILE="" MP_EUILIB=us MP_EUIDEVICE=css0 poe ./SrcForSharedLibTest/test -rmpool 0 -nodes 1 -procs 1 
EOF
    ;;
    *)
      if test -n "$TMP_RPATH"; then
cat>./SrcForSharedLibTest/runTest<<EOF
#!/bin/sh
$mpirun -np 2 $MPIRUN_MACHINEFILE ./SrcForSharedLibTest/test
EOF
      else # TMP_RPATH is not defined.
cat>./SrcForSharedLibTest/runTest<<EOF
#!/bin/sh
LD_LIBRARY_PATH=./SrcForSharedLibTest:$LD_LIBRARY_PATH $mpirun -np 3 $MPIRUN_MACHINEFILE ./SrcForSharedLibTest/test
EOF
     fi # End if block on TMP_RPATH
    ;;
    esac

dnl     dnl if test "$ARCH" != rs6000; then
dnl     if echo $host_os | grep -v '^aix' > /dev/null; then
dnl       if test -n "$PXX_RPATH"; then
dnl cat>runTest<<EOF
dnl #!/bin/sh
dnl 
dnl # USe only two processors for this test
dnl # $mpirun -np 3 $MPIRUN_MACHINEFILE ./SrcForSharedLibTest/test
dnl $mpirun -np 2 $MPIRUN_MACHINEFILE ./SrcForSharedLibTest/test
dnl EOF
dnl 
dnl       else # PXX_RPATH is not defined.
dnl cat>runTest<<EOF
dnl #!/bin/sh
dnl  
dnl LD_LIBRARY_PATH=./SrcForSharedLibTest:$LD_LIBRARY_PATH 
dnl export LD_LIBRARY_PATH 
dnl $mpirun -np 3 $MPIRUN_MACHINEFILE ./SrcForSharedLibTest/test
dnl EOF
dnl 
dnl      fi # End if block on PXX_RPATH
dnl     else # host_os is aix
dnl     cat>runTest<<EOF
dnl #!/bin/sh
dnl export MP_RESD="YES"
dnl export MP_HOSTFILE=""
dnl export MP_EUILIB=us
dnl export MP_EUIDEVICE=css0
dnl poe ./SrcForSharedLibTest/test -rmpool 0 -nodes 1 -procs 1 
dnl EOF
dnl 
dnl     fi # End if block on host_os

    ( echo "The execute script is:"; cat ./SrcForSharedLibTest/runTest; echo "End of script" ) 1>&5


    chmod 770 ./SrcForSharedLibTest/runTest

    PPP_BUILD_SHARED_LIB_TARGET=
    dnl if test $ARCH = solaris; then
dnl     if echo $host_os | grep '^solaris' > /dev/null; then
dnl dnl    if true=false; then
dnl       echo "Assume solaris doesn't have working shared lib + MPI"
dnl       PPP_BUILD_SHARED_LIBS="no"
dnl     else 
      echo "Running script testing executable compiled with shared library."
      echo "Running script testing executable compiled with shared library." 1>&5
      if ./SrcForSharedLibTest/runTest 1>&5 2>&5 ; then
        echo "SrcForSharedLibTest/runTest script executed OK." 1>&5
        PPP_BUILD_SHARED_LIB_TARGET=libPpp.shared
        # rm -f ./SrcForSharedLibTest/runTest ./SrcForSharedLibTest/*.o ./SrcForSharedLibTest/*.a ./SrcForSharedLibTest/*.so ./SrcForSharedLibTest/test

	# Reset rpath to point to where P++ libraries are built.
        rpath=$PWD/src
        case $host_os in
	    solaris*)
	      # The solaris syntax for specifying PXX_RPATH.
	      PXX_RPATH="-R $rpath"
	    ;;
	    *)
	      # Unless you know PXX_RPATH is required, set it to null.
	      PXX_RPATH=
	    ;;
        esac
      else
        echo "SrcForSharedLibTest/runTest script FAILED. WILL NOT BUILD SHARED LIBRARY FOR P++" 1>&5
        echo "SrcForSharedLibTest/runTest script FAILED. WILL NOT BUILD SHARED LIBRARY FOR P++"
        PPP_BUILD_SHARED_LIBS="no"
      fi
dnl     fi
    # rm -rf SrcForSharedLibTest
    fi

    dnl *
    dnl * END OF MPI-shared library check
    dnl *

  fi
dnl * end of check for existance of cached values

  AC_CACHE_VAL(ppp_cv_mpirun,ppp_cv_mpirun=$mpirun)
  AC_CACHE_VAL(ppp_cv_mpirun_execution,ppp_cv_mpirun_execution=$MPIRUN_RESULT)
  AC_CACHE_VAL(ppp_cv_mpirun_machinefile,ppp_cv_mpirun_machinefile=$MPIRUN_MACHINEFILE)
  AC_CACHE_VAL(ppp_cv_build_shared_libs,ppp_cv_build_shared_libs=$PPP_BUILD_SHARED_LIBS)
  AC_CACHE_VAL(ppp_cv_mpi_shared_lib_target,ppp_cv_mpi_shared_lib_target=$PPP_BUILD_SHARED_LIB_TARGET)
  AC_CACHE_VAL(ppp_cv_pxx_rpath,ppp_cv_pxx_rpath=$PXX_RPATH)
  AC_CACHE_SAVE()

  AC_SUBST(mpirun)
  AC_SUBST(MPIRUN_MACHINEFILE)
  AC_SUBST(PPP_BUILD_SHARED_LIBS)
  AC_SUBST(PPP_BUILD_SHARED_LIB_TARGET)
  AC_SUBST(PXX_RPATH)
])dnl

dnl Define macro SUPPORT_Parti
dnl which does the following:
dnl handle enable-Parti configure flag.

AC_DEFUN(PADRE_SUPPORT_Parti, [

# Start macro PADRE_SUPPORT_Parti

dnl Check for enabling Parti
AC_MSG_CHECKING([whether Parti should be enabled under PADRE])
AC_ARG_ENABLE(Parti,
[--enable-Parti       Enable PADRE support for Parti.],
,
enable_Parti="yes" # enable-Parti was not given; on by default.
)
AC_MSG_RESULT($enable_Parti)

# If disabling Parti, do this.
if test "$enable_Parti" = "no"; then
  BTNG_AC_LOG(Parti is disabled under PADRE)
  NO_Parti=1
  AC_DEFINE([NO_Parti],1,[Define if not supporting Parti in PADRE])
else
  BTNG_AC_LOG(Parti is enabled under PADRE)
fi

# End macro PADRE_SUPPORT_Parti

])dnl	End definition of SUPPORT_Parti

dnl Define macro SUPPORT_Kelp
dnl which does the following:
dnl handle enable-Kelp configure flag.

AC_DEFUN(PADRE_SUPPORT_Kelp, [

# Start macro PADRE_SUPPORT_Kelp

dnl Check for enabling Kelp
AC_MSG_CHECKING([whether Kelp should be enabled under PADRE])
AC_ARG_ENABLE(Kelp,
[--enable-Kelp       Enable PADRE support for Kelp.],
,
enable_Kelp="no" # enable-Kelp was not given; off by default.
)
AC_MSG_RESULT($enable_Kelp)

# If disabling Kelp, do this.
if test "$enable_Kelp" = "no"; then
  BTNG_AC_LOG(Kelp is disabled under PADRE)
  NO_Kelp=1
  AC_DEFINE([NO_Kelp],1,[Define if not supporting Kelp in PADRE])
else
  BTNG_AC_LOG(Kelp is enabled under PADRE)
fi

# End macro PADRE_SUPPORT_Kelp

])dnl	End definition of SUPPORT_Kelp

dnl $Id: variable-header-filenames.m4,v 1.17 2002/01/03 22:04:20 gunney Exp $

AC_DEFUN(BTNG_FIND_CORRECT_HEADER_FILENAME,[
dnl There is no standard naming convention for STL header files.
dnl This macro helps to pick the right name out of a list.
dnl Arg1 is the variable to set to the found file name.
dnl Arg2 is the list of file names to search
dnl Arg3 are additional headers to include (for use by AC_TRY_COMPILE)
dnl Arg4 is the code body to test if the included file works.
# Start macro BTNG_FIND_CORRECT_HEADER_FILENAME
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  $1=
  AC_REQUIRE([BTNG_TYPE_NAMESPACE])
  AC_REQUIRE([BTNG_TYPE_BOOL])
  CPPFLAGS_SAVE=$CPPFLAGS
  for file in $2; do
    AC_CHECK_HEADER($file, btng_header_found=1, unset btng_header_found)
    if test -n "$btng_header_found"; then
      AC_MSG_CHECKING(whether $file is the header sought)
      BTNG_AC_LOG(found header file $file)
      CPPFLAGS="$CPPFLAGS_SAVE $CXX_OPTIONS"
      AC_TRY_COMPILE(
        [
#ifdef BOOL_IS_BROKEN
typedef int bool;
#define true 1
#define false 0
#endif
	$3
        #include <$file>
#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif
],
        $4,
	AC_MSG_RESULT(yes)
        $1="$file",
	AC_MSG_RESULT(no)
      )
    fi
    if test -n "${$1}"; then break; fi
  done
  AC_LANG_RESTORE
  CPPFLAGS=$CPPFLAGS_SAVE
# End macro BTNG_FIND_CORRECT_HEADER_FILENAME
])




AC_DEFUN(BTNG_TREAT_VARIABLE_HEADER_FILENAME,[
dnl BTNG_TREAT_VARIABLE_HEADER_FILENAME is a generic macro
dnl used by (and using) other macros in this file.
dnl It determines, from a given list, the correct name of
dnl a header file required to compile a test code body.
dnl It takes a list of possible of the header filenames.
dnl It reports whether each header file is the one sought
dnl until it finds the one that is.
dnl If none of the header filenames work:
dnl   It issues a warning.
dnl   It defines a ...IS_BROKEN C macro saying so.
dnl If it finds the first header filename that works:
dnl   It assigns a variable (..._HEADER_FILE) to the
dnl   correct filename and call AC_DEFINE for that variable.
dnl Arguments are:
dnl  1: a single name representing the header sought.
dnl  2: a list of possible header filenames.
dnl  3: other include lines (for use in AC_TRY_COMPILE).
dnl  4: code to test if the header file is the one being sought.
dnl
# Start macro BTNG_TREAT_VARIABLE_HEADER_FILENAME
AC_CACHE_VAL(btng_cv_[]translit($1,[-],[_])[]_header_filename, [
  AC_ARG_WITH($1-header-file,
  [  --with-$1-header-file	Specify name of the $1 header file.],
  btng_cv_[]translit($1,[-],[_])[]_header_filename=$with_[]translit($1,[-],[_])[]_header_file,
  [BTNG_FIND_CORRECT_HEADER_FILENAME(btng_cv_[]translit($1,[-],[_])[]_header_filename,$2,[$3],[[$4]])]
  )
])	dnl End AC_CACHE_VAL call
# We must be able to find the $1 header file or else.
translit($1,[-a-z],[_A-Z])[]_HEADER_FILE="$btng_cv_[]translit($1,[-],[_])[]_header_filename"
if test -z "$translit($1,[-a-z],[_A-Z])[]_HEADER_FILE"; then
  translit($1,[-],[_])[]_header_is_broken=1
  AC_MSG_WARN([cannot find a working $1 header file.
      Names tried: $2
      If you know the correct hame of this header file,
      use the option --with-[]$1[]-header-file=<filename>
      with configure.])
  AC_DEFINE(translit($1,[-a-z],[_A-Z])[]_IS_BROKEN,1,The $1 header file is broken)
  BTNG_AC_LOG(header file $1 is broken)
else
  unset translit($1,[-],[_])[]_header_is_broken
  AC_DEFINE_UNQUOTED(translit($1,[-a-z],[_A-Z])[]_HEADER_FILE,<$translit($1,[-a-z],[_A-Z])[]_HEADER_FILE>,
    [Header file for $1])
  BTNG_AC_LOG(header file $1 is ok)
fi
# End macro BTNG_TREAT_VARIABLE_HEADER_FILENAME
])	dnl end of BTNG_TREAT_VARIABLE_HEADER_FILENAME definition.




AC_DEFUN(BTNG_STL_STRING_HEADER_FILENAME,[
# Start macro BTNG_STL_STRING_HEADER_FILENAME
dnl dnl AC_MSG_CHECKING(name of the STL string header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-string,
  string strings string.h strings.h string.hxx strings.hxx,,
  [std::string s; s = "sample string";])
# End macro BTNG_STL_STRING_HEADER_FILENAME
])	dnl end of BTNG_STL_STRING_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_SET_HEADER_FILENAME,[
# Start macro BTNG_STL_SET_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL set header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-set, set set.h set.hxx,,
  [set<int> s; s.insert(1);])
# End macro BTNG_STL_SET_HEADER_FILENAME
])	dnl end of BTNG_STL_SET_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_STACK_HEADER_FILENAME,[
# Start macro BTNG_STL_STACK_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL stack header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-stack, stack stack.h stack.hxx,,
  [stack<int> s; s.push(1);])
# End macro BTNG_STL_STACK_HEADER_FILENAME
])	dnl end of BTNG_STL_STACK_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_VECTOR_HEADER_FILENAME,[
# Start macro BTNG_STL_VECTOR_HEADER_FILENAME
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-vector, vector vector.h vector.hxx,,
[vector<int> v; v.insert(v.begin(),1);
vector<char> s; s.insert( s.end(), 10, '\0' );])
# End macro BTNG_STL_VECTOR_HEADER_FILENAME
])	dnl end of BTNG_STL_VECTOR_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_LIST_HEADER_FILENAME,[
# Start macro BTNG_STL_LIST_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL list header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-list, list list.h list.hxx,,
  [list<int> v; v.insert(v.begin(),1);])
# End macro BTNG_STL_LIST_HEADER_FILENAME
])	dnl end of BTNG_STL_LIST_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_MAP_HEADER_FILENAME,[
# Start macro BTNG_STL_MAP_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL map header file)
AC_REQUIRE([BTNG_INFO_CXX_ID])
btng_stl_map_test_body='[map<int,int> v; v[0]=1;]'
# The Sun compiler version 4.2 does not treat default template
# arguments correctly.  The STL standard states that for map,
# only the first two arguments are required but the Sun compiler
# requires the third.
test "$CXX_ID" = "sunpro" && echo "$CXX_VERSION" | grep '^0x420' > /dev/null && \
btng_stl_map_test_body='[map<int,int,less<int> > v; v[0]=1;]'
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-map, map map.h map.hxx,,
  $btng_stl_map_test_body)
# End macro BTNG_STL_MAP_HEADER_FILENAME
])	dnl end of BTNG_STL_MAP_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_ITERATOR_HEADER_FILENAME,[
# Start macro BTNG_STL_ITERATOR_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL iterator header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-iterator,
  iterator iterator.h iterator.hxx,,
  [int a[10], size; size=distance(a,a+10);])
dnl  [ostream_iterator<int> v(cout," ");])
# End macro BTNG_STL_ITERATOR_HEADER_FILENAME
])	dnl end of BTNG_STL_ITERATOR_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_ALGO_HEADER_FILENAME,[
# Start macro BTNG_STL_ALGO_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL algo header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-algo,
  algo algorithm algo.h algorithm.h algo.hxx algorithm.hxx ,,
  [int n[10]; find(n,n+10,0);])
# End macro BTNG_STL_ALGO_HEADER_FILENAME
])	dnl end of BTNG_STL_ALGO_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_FUNCTION_HEADER_FILENAME,[
# Start macro BTNG_STL_FUNCTION_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL numeric header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-function,
  function function.h function.hxx ,,
  [int a=1, b=2, c; plus<int> adder; c=adder(a,b);])
# End macro BTNG_STL_FUNCTION_HEADER_FILENAME
])	dnl end of BTNG_STL_FUNCTION_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_NUMERIC_HEADER_FILENAME,[
# Start macro BTNG_STL_NUMERIC_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL numeric header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-numeric,
  numeric numeric.h numeric.hxx ,,
  [int n[10]; iota(n,n+10,0);])
# End macro BTNG_STL_NUMERIC_HEADER_FILENAME
])	dnl end of BTNG_STL_NUMERIC_HEADER_FILENAME definition.


AC_DEFUN(BTNG_IOSTREAM_HEADER_FILENAME,[
# Start macro BTNG_IOSTREAM_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the iostream header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(iostream,
  iostream iostream.h iostream.hxx,,
  [cout<<"test"<<endl;])
# End macro BTNG_IOSTREAM_HEADER_FILENAME
])	dnl end of BTNG_IOSTREAM_HEADER_FILENAME definition.


AC_DEFUN(BTNG_FSTREAM_HEADER_FILENAME,[
# Start macro BTNG_FSTREAM_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the fstream header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(fstream,
  fstream fstream.h fstream.hxx,,
  [fstream iost("theStream",ios::app);])
# End macro BTNG_FSTREAM_HEADER_FILENAME
])	dnl end of BTNG_FSTREAM_HEADER_FILENAME definition.


AC_DEFUN(BTNG_IOMANIP_HEADER_FILENAME,[
# Start macro BTNG_IOMANIP_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the iomanip header file)
AC_REQUIRE([BTNG_IOSTREAM_HEADER_FILENAME])
BTNG_TREAT_VARIABLE_HEADER_FILENAME(iomanip,
  iomanip iomanip.h iomanip.hxx,[#include IOSTREAM_HEADER_FILE],
  [cout<<setw(13)<<endl;])
# End macro BTNG_IOMANIP_HEADER_FILENAME
])	dnl end of BTNG_IOMANIP_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_STRINGSTREAM_HEADER_FILENAME,[
# Start macro BTNG_STL_STRINGSTREAM_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL string stream header file)
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stringstream,
  sstream stringstream sstream.h stringstream.h sstream.hxx stringstream.hxx strstream.h ,,
  [string i="istring"; istringstream ist(i);
   string o="ostring"; istringstream ost(o);
])
# End macro BTNG_STL_STRINGSTREAM_HEADER_FILENAME
])      dnl end of BTNG_STL_STRINGSTREAM_HEADER_FILENAME definition.


AC_DEFUN(BTNG_STL_MULTIMAP_HEADER_FILENAME,[
# Start macro BTNG_STL_MULTIMAP_HEADER_FILENAME
dnl AC_MSG_CHECKING(name of the STL multimap header file)
AC_REQUIRE([BTNG_INFO_CXX_ID])
btng_stl_multimap_test_body='[multimap<int,int > v; pair<const int,int> thePair(0,1); v.insert(thePair);]'
test "$CXX_ID" = "sunpro" && echo "$CXX_VERSION" | grep '^0x420' > /dev/null && \
btng_stl_multimap_test_body='[multimap<int,int,less<int> > v; pair<const int,int> thePair(0,1); v.insert(thePair);]'
BTNG_TREAT_VARIABLE_HEADER_FILENAME(stl-multimap,
    multimap mmap multimap.h mmap.h multimap.hxx mmap.hxx map map.h map.hxx,,
    $btng_stl_multimap_test_body)
# End macro BTNG_STL_MULTIMAP_HEADER_FILENAME
])      dnl end of BTNG_STL_MULTIMAP_HEADER_FILENAME definition.


dnl Define macro PADRE_SUPPORT_GlobalArrays
dnl which does the following:
dnl handle enable-GlobalArrays and with-GlobalArrays configure flags.
dnl Defines and AC_SUBST GlobalArrays_PREFIX, GlobalArrays_INCLUDES,
dnl GlobalArrays_LIBDIRS and GlobalArrays_LIBS.

AC_DEFUN(PADRE_SUPPORT_GlobalArrays, [

# Start macro PADRE_SUPPORT_GlobalArrays


dnl Check for enabling GlobalArrays
AC_MSG_CHECKING([whether GlobalArrays should be enabled])
AC_ARG_ENABLE(GlobalArrays,
[--enable-GlobalArrays       Enable PADRE support for GlobalArrays.],
,
enable_GlobalArrays="no" # enable-GlobalArrays was not given; off by default.
)
AC_MSG_RESULT($enable_GlobalArrays)


if test "$enable_GlobalArrays" = no; then
  # GlobalArrays has not been enabled.
  BTNG_AC_LOG(GlobalArrays disabled)
  AC_DEFINE([NO_GlobalArrays],1,[Define if not supporting GlobalArrays in PADRE])
else
  # GlobalArrays has been enabled.
  BTNG_AC_LOG(GlobalArrays enabled)

  # Get the installed location of GlobalArrays for PADRE.
  BTNG_ARG_WITH_PREFIX(GlobalArrays,GlobalArrays_PREFIX)
  BTNG_AC_LOG(GlobalArrays_PREFIX is $GlobalArrays_PREFIX)

  dnl # Get the location of GlobalArrays.
  dnl AC_CACHE_CHECK(for GlobalArrays installation, padre_cv_GlobalArrays_PREFIX,
  dnl   [BTNG_ARG_WITH_PREFIX(GlobalArrays,padre_cv_GlobalArrays_PREFIX)] )
  dnl BTNG_AC_LOG(padre_cv_GlobalArrays_PREFIX is $padre_cv_GlobalArrays_PREFIX)
  dnl GlobalArrays_PREFIX=$padre_cv_GlobalArrays_PREFIX 
 

  BTNG_AC_LOG(GlobalArrays_PREFIX is $GlobalArrays_PREFIX)
  if test -n "$GlobalArrays_PREFIX"; then
  # GlobalArrays is installed in a special location.
    GlobalArrays_INCLUDES="-I$GlobalArrays_PREFIX/include"
    GlobalArrays_LIBDIRS="-L$GlobalArrays_PREFIX/lib"
    GlobalArrays_LIBLINKS="-lglobal -lma -llinalg -larmci -ltcgmsg-mpi -lm"
    # Libraries installed with GlobalArrays are, global,
    # ma, linalg, armci, tcgmsg-mpi.
    # Depending on the OS and compiler, there are
    # other libraries that needs linking.
    case "$host_os" in
      solaris*) GlobalArrays_LIBLINKS="$GlobalArrays_LIBLINKS -lsocket -lnsl" ;;
      osf*) GlobalArrays_LIBLINKS="$GlobalArrays_LIBLINKS -lgs" ;;
    esac
    GlobalArrays_LIBS="$GlobalArrays_LIBDIRS $GlobalArrays_LIBLINKS"
  fi
  BTNG_AC_LOG(GlobalArrays_INCLUDES is $GlobalArrays_INCLUDES)
  BTNG_AC_LOG(GlobalArrays_LIBS is $GlobalArrays_LIBS)
  AC_SUBST(GlobalArrays_PREFIX)
  AC_SUBST(GlobalArrays_INCLUDES)
  AC_SUBST(GlobalArrays_LIBDIRS)
  AC_SUBST(GlobalArrays_LIBLINKS)
  AC_SUBST(GlobalArrays_LIBS)
  # Do not append to INCLUDES and LIBS because GlobalArrays
  # is not needed everywhere.


  # Check that we can compile a GlobalArrays application.
  # Optional and not yet implemented.


fi	; # End check if GlobalArray is enabled.

# End macro PADRE_SUPPORT_GlobalArrays

])dnl	End definition of PADRE_SUPPORT_GlobalArrays

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






AC_DEFUN(SUPPORT_FORTRAN, [

# Start macro SUPPORT_FORTRAN

dnl Use this macro if you have to link with Fortran codes.
dnl The macro AC_F77_LIBRARY_LDFLAGS, which was installed
dnl with autoconf, did not work for me.  BTNG.

AC_MSG_CHECKING([how to set library flags for linking to Fortran code])

AC_ARG_WITH(Fortran-libs,
[--with-Fortran-libs=LIBS       Specify library flags for linking with Fortran code.],
Fortran_LIBS=$with_Fortran_libs
)

if test -z "$Fortran_LIBS" ; then
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
	# linux*)		Fortran_LIBS=
	linux*)		Fortran_LIBS='-lg2c'
        if test "$CXX_VERSION" = "2.96" ; then
	  # Fix problem with RedHat distribution of experimental
	  # gcc version 2.96 not knowing where it put the g2c library.
          [ extra_path=`which $CXX | sed 's:bin/[^/]*$:lib:'` ]
      	  BTNG_AC_LOG_VAR(extra_path)
	  test -d "$extra_path/gcc-lib/i386-redhat-linux/2.96" && extra_path="$extra_path/gcc-lib/i386-redhat-linux/2.96"
      	  BTNG_AC_LOG_VAR(extra_path)
	  if test -d "$extra_path" ; then
	    BTNG_AC_LOG(Adding $extra_path to Fortran_LIBS)
	    Fortran_LIBS="-L$extra_path $Fortran_LIBS"
	  fi
        fi
	;;
	solaris*)	Fortran_LIBS='-lnsl -lg2c' ;;
      esac
    ;;
    kai)	# The KCC compiler.
      case "$host_os" in
	osf*)		Fortran_LIBS='-lfor' ;;
      esac
    ;;
    sgi)		# SGI compiler.
        Fortran_LIBS='-lftn'
    ;;
    sunpro)		# Sunpro compiler.
	Fortran_LIBS='-lF77 -lM77 -lV77 -lnsl -lsunmath'
    ;;
    dec)	# The DEC compiler.
	Fortran_LIBS='-lfor'
    ;;
  esac
fi

AC_MSG_RESULT([$Fortran_LIBS])

AC_SUBST(Fortran_LIBS)

# End macro SUPPORT_FORTRAN

])dnl	End definition of SUPPORT_FORTRAN

AC_DEFUN(PPP_PADRE_SPECIFIC_SETUP,
[
# PADRE honors both --enable-INTERNALDEBUG and --enable-PADRE-debug flags,
# with PADRE-debug overriding.
# So even when we check for enable-INTERNALDEBUG, we set PADRE_debug.
enable_PADRE_debug="no";	# Default.
AC_ARG_ENABLE(INTERNALDEBUG,[  --enable-INTERNALDEBUG  Include debugging code in compilation.],
  enable_PADRE_debug="$enableval")
AC_ARG_ENABLE(PADRE_debug,[  --enable-PADRE-debug  Include debugging code in compilation.])
if test -n "$enable_PADRE_debug" && test ! "$enable_PADRE_debug" = no ; then
  # Use the unquoted version of ac_define so that autoheader does not try to put the
  # macro name in config.h.  The macro name should go in PADRE_config.h.
  # t1=PADRE_DEBUG_IS_ENABLED
  # AC_DEFINE_UNQUOTED($t1)
  AC_DEFINE([PADRE_DEBUG_IS_ENABLED],1,[Define to enable PADRE debug codes])
fi


dnl Specify ar program and flags.  See specify-ar.m4.
BTNG_PROG_AR


# Check for enabling Parti
PADRE_SUPPORT_Parti
AM_CONDITIONAL(ENABLE_Parti, test "$enable_Parti" = yes)
if test "$enable_Parti" = "yes"; then
  BTNG_AC_LOG(Parti is enabled in PADRE)
  dnl Define and AC_SUBST Parti_PREFIX, Parti_INCLUDES,
  dnl Parti_LIBDIRS and Parti_LIBS.
  Parti_INCLUDES='-I$(top_srcdir)/PARTI -I../PARTI'
  # Parti_LIBS='-L${top_builddir}/PARTI -lPADRE_PARTI_Source'
  Parti_LIBS=
  BTNG_AC_LOG(Parti_INCLUDES is $Parti_INCLUDES)
  BTNG_AC_LOG(Parti_LIBS is $Parti_LIBS)
  AC_SUBST(Parti_INCLUDES)
  AC_SUBST(Parti_LIBS)
  BTNG_AC_LOG(Adding Parti variables to INCLUDES and LIBS)
  INCLUDES="$Parti_INCLUDES $INCLUDES"
  LIBS="$LIBS $Parti_LIBS"
fi



# Check for enabling Kelp
PADRE_SUPPORT_Kelp
AM_CONDITIONAL(ENABLE_Kelp, test "$enable_Kelp" = yes)
if test "$enable_Kelp" = "yes"; then
  BTNG_AC_LOG(Kelp is enabled in PADRE)
  dnl Define and AC_SUBST Kelp_PREFIX, Kelp_INCLUDES,
  dnl Kelp_LIBDIRS and Kelp_LIBS.
  Kelp_INCLUDES='-I$(top_srcdir)/KELP -I../KELP'
  Kelp_LIBS='-L${top_builddir}/KELP -lPADRE_KELP_Source'
  BTNG_AC_LOG(Kelp_INCLUDES is $Kelp_INCLUDES)
  BTNG_AC_LOG(Kelp_LIBS is $Kelp_LIBS)
  AC_SUBST(Kelp_INCLUDES)
  AC_SUBST(Kelp_LIBS)
  BTNG_AC_LOG(Adding Kelp variables to INCLUDES and LIBS)
  INCLUDES="$Kelp_INCLUDES $INCLUDES"
  LIBS="$LIBS $Kelp_LIBS"
fi



PGSLIB_INCLUDES='-I$(top_srcdir)/PGSLIB -I../PGSLIB'
AC_SUBST(PGSLIB_INCLUDES)
BTNG_AC_LOG(PGSLIB_INCLUDES is $PGSLIB_INCLUDES)
PGSLIB_LIBS='-L${top_builddir}/PGSLIB -lPADRE_PGSLIB_Source'
AC_SUBST(PGSLIB_LIBS)
BTNG_AC_LOG(PGSLIB_LIBS is $PGSLIB_LIBS)

INCLUDES="$INCLUDES $PGSLIB_INCLUDES"
# INCLUDES='$(Parti_INCLUDES) $(PGSLIB_INCLUDES)'
# Makefile.am
BTNG_AC_LOG(INCLUDES is $INCLUDES)


# Macro to support GlobalArrays.  See support-GlobalArrays.m4.
PADRE_SUPPORT_GlobalArrays
AM_CONDITIONAL(ENABLE_GlobalArrays, test "$enable_GlobalArrays" = yes)
BTNG_AC_LOG(INCLUDES is $INCLUDES)
if test "$enable_GlobalArrays" = "yes"; then
  test "$GlobalArrays_LIBS" && LIBS="$LIBS $GlobalArrays_LIBS"
  test "$GlobalArrays_INCLUDES" && INCLUDES="$GlobalArrays_INCLUDES $INCLUDES"
fi
BTNG_AC_LOG(INCLUDES is $INCLUDES)
BTNG_AC_LOG(LIBS is $LIBS)


# This causes certain package autoconf macros to guess at a default instead of leaving it blank.
guess_defaults=yes



# Commented out since this is handled in P++.m4 file's macro
# AC_MSG_CHECKING(whether to use LAM)
# BTNG_SUPPORT_LAM
# AC_MSG_RESULT($lam_PREFIX)
# # lam_PREFIX, lam_INCLUDES, lam_LIBS are defined if --with-lam is used.
# AC_SUBST(lam_PREFIX)
# AC_SUBST(lam_INCLUDES)
# AC_SUBST(lam_LIBS)
# if test "$with_lam" && test ! "$with_lam" = no; then
#   INCLUDES="$lam_INCLUDES $INCLUDES"
#   LIBS="$lam_LIBS $LIBS"
#   AC_DEFINE([MPI_IS_LAM],1,[Define if using LAM flavor of MPI])
# fi

#if test ! "$with_lam" || test "$with_lam" = no; then
#  # Only use MPICH if LAM is not specified.
#  PADRE_INHERIT_ENV_OR_CACHE(for mpi_INCLUDES, padre_cv_config_mpi_includes, mpi_INCLUDES)
#  PADRE_INHERIT_ENV_OR_CACHE(for mpi_LIBS, padre_cv_config_mpi_libs, mpi_LIBS)
#  BTNG_AC_LOG(mpi_INCLUDES)
#  test "$mpi_INCLUDES"	&& INCLUDES="$mpi_INCLUDES $INCLUDES"
#  test "$mpi_LIBS"	&& LIBS="$LIBS $mpi_LIBS"
#  AC_DEFINE(MPI_IS_MPICH,1,Define if using MPICH flavor of MPI)
#fi
#BTNG_AC_LOG(INCLUDES)


# Determine whether bool type works.  See compile-bool.m4.
# BTNG_TYPE_BOOL

# Determine whether namespace works.  See compile-namespace.m4.
# BTNG_TYPE_NAMESPACE

# if 0
# Determine location of STL.  See choose-stl.m4.
# STL uses bool, so make sure this goes after bool type is defined.
# BTNG_CHOOSE_STL
# AC_SUBST(STL_DIR)
# AC_SUBST(STL_INCLUDES)
# INCLUDES="$INCLUDES $STL_INCLUDES"

# # Determine pecularities of STL.  See compiling-stl.m4.
# # These should go after CHOOSE_STL so they would use the
# # header files defined by that macro.
# BTNG_STL_STRING_HEADER_FILENAME
# # BTNG_STL_STACK_HEADER_FILENAME
# BTNG_STL_LIST_HEADER_FILENAME
# BTNG_STL_VECTOR_HEADER_FILENAME
# BTNG_STL_ITERATOR_HEADER_FILENAME
# BTNG_STL_ALGO_HEADER_FILENAME
# BTNG_STL_FUNCTION_HEADER_FILENAME
# endif

# If GlobalArrays is enabled, support Fortran, because GlobalArrays needs it.
if test "$enable_GlobalArrays" = yes; then
  SUPPORT_FORTRAN
  LIBS="$LIBS $Fortran_LIBS"
  BTNG_AC_LOG_VAR(Fortran_LIBS)
  # AC_F77_LIBRARY_LDFLAGS
  # LIBS="$LIBS $FLIBS"
fi


# Optional support for indirect addressing.
AC_ARG_ENABLE(IndirectAddressing,
   [  --enable-IndirectAddressing .......................... enable parallel indirect addressing support],
   ,enable_IndirectAddressing=no )
# Tell make about enabling indirect addresing.
AM_CONDITIONAL(ENABLE_IndirectAddressing, test "$enable_IndirectAddressing" = yes)
# Set indirect addressing variables.
if test "$enable_IndirectAddressing" = yes; then
   IndirectAddressing_SUBDIRS="IndirectAddressing"
   IndirectAddressing_INCLUDES="-I\$(top_srcdir)/IndirectAddressing -I\$(top_builddir)/IndirectAddressing"
   INCLUDES="$INCLUDES $IndirectAddressing_INCLUDES"
fi
AC_SUBST(IndirectAddressing_SUBDIRS)



# Support Doxygen documentation system.
# Determine the doxygen binary programs.
BTNG_PATH_PROG(DOXYGEN_BIN,doxygen)
AC_SUBST(DOXYGEN_BIN)
BTNG_PATH_PROG(DOXYSEARCH_BIN,doxysearch)
AC_SUBST(DOXYSEARCH_BIN)
# Check if doxygen was enabled.
AC_ARG_ENABLE(doxygen,
[ --enable-doxygen	Enable generation of doxygen documentation.],,
[
# If enable-doxygen is not given, decide whether to enable it based on
# whether DOXYGEN_BIN and DOXYSEARCH_BIN were found above.
if test -n "${DOXYGEN_BIN}" && test -n "${DOXYSEARCH_BIN}" ; then
  enable_doxygen=yes
  AC_MSG_RESULT(doxygen is enabled)
else
  enable_doxygen=no
  AC_MSG_RESULT(doxygen is disabled)
fi
])
AM_CONDITIONAL(ENABLE_DOXYGEN, test "$enable_doxygen" = yes)


# User can specify the perl program to use.
# Perl is required to run doxygen.
BTNG_PATH_PROG(PERL,perl)
# Find perl if not specifed.
if test ! "$PERL"; then
  AC_PATH_PROG(PERL,perl)
  # Error if PERL still not defined.
  if test -z "$PERL" && test "$enable_doxygen" = yes; then
     AC_MSG_ERROR(Cannot find perl program.  Specify using --with-perl=PATH)
  fi
fi	# End block to find perl program.


# PADRE requires this defined.
# I is not configure-time dependent so I moved it to the bottom
# of acconfig.h.  BTNG.
# AC_DEFINE(PARALLEL_PADRE,1,I am not sure what this macro is for.  BTNG)

# Add current and top_builddir to INCLUDES.
# Do it late so that nothings comes before it.
# INCLUDES='-I. -I${top_builddir} '"$INCLUDES"


AC_SUBST(LIBS)
AC_SUBST(INCLUDES)

])dnl
















