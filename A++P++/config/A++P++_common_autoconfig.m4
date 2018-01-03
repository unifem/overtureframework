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
dnl *wdh* 100924   gnu)		CXX_WARNINGS='-Wstrict-prototypes' ;;
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
dnl *wdh* 100924    gnu)		C_WARNINGS='-Wstrict-prototypes' ;;
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
