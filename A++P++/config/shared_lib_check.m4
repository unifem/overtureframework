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
