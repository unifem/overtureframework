



AC_DEFUN(BTNG_STL_MAP_NEEDS,[
dnl On certain platforms, map needs differing numbers of
dnl template arguments.  This macro figures that out.
dnl It defines the C macro STL_MAP_NEEDS_LESS
dnl to indicate that the less template argument is required.
dnl It defines the C macro STL_MAP_NEEDS_ALLOCATOR
dnl to indicate that the allocator template argument is required.
dnl If map still does not work after being declared with
dnl less and allocator, it is declared broken and the C macro
dnl STL_MAP_IS_BROKEN is defined.
# Start macro BTNG_STL_MAP_NEEDS
  AC_REQUIRE([BTNG_TYPE_NAMESPACE])
  AC_REQUIRE([BTNG_TYPE_BOOL])
  AC_REQUIRE([BTNG_STL_MAP_HEADER_FILENAME])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  if test ! "${stl_map_header_is_broken+set}" = set; then
    CPPFLAGS_SAVE=$CPPFLAGS
    CPPFLAGS="$CPPFLAGS $CXX_OPTIONS"
    AC_TRY_COMPILE([
#ifdef BOOL_IS_BROKEN
typedef int bool;
#define true 1
#define false 0
#endif
#include STL_MAP_HEADER_FILE	
#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif
],
      [map<int,int> v; v[0]=1;],
      : Do nothing further because this is not a special case.
      ,
      # Did not compile so the less argument must be required.
      AC_DEFINE(STL_MAP_NEEDS_LESS,1,The STL map class needs less specified.)
      AC_TRY_COMPILE([
#ifdef BOOL_IS_BROKEN
typedef int bool;
#define true 1
#define false 0
#endif
#include STL_MAP_HEADER_FILE
#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif
],
        [map<int,int,less<int> > v; v[[0]]=1;],
        : Do nothing further because this is not a special case.
        ,
        # Did not compile so the allocator argument must be required.
        AC_DEFINE(STL_MAP_NEEDS_ALLOCATOR,1,The STL map class requires allocator specified.)
        AC_TRY_COMPILE([
#ifdef BOOL_IS_BROKEN
typedef int bool;
#define true 1
#define false 0
#endif
#include STL_MAP_HEADER_FILE
#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif
],
          [[map<int,int,less<int>,allocator<int> > v; v[[0]]=1;]],
          : Do nothing further because this is not a special case.
          ,
          # Did not compile so the map container must be broken.
          AC_DEFINE(STL_MAP_IS_BROKEN,1,The STL map class is broken)
        )
      )
    )
    CPPFLAGS=$CPPFLAGS_SAVE
  fi
  AC_LANG_RESTORE

# End macro BTNG_STL_MAP_NEEDS
])	dnl end of BTNG_STL_MAP_NEEDS



AC_DEFUN(BTNG_STL_MULTIMAP_NEEDS,[
dnl On certain platforms, multimap needs differing numbers of
dnl template arguments.  This macro figures that out.
dnl It defines the C macro STL_MULTIMAP_NEEDS_LESS
dnl to indicate that the less template argument is required.
dnl It defines the C macro STL_MULTIMAP_NEEDS_ALLOCATOR
dnl to indicate that the allocator template argument is required.
dnl If multimap still does not work after being declared with
dnl less and allocator, it is declared broken and the C macro
dnl STL_MULTIMAP_IS_BROKEN is defined.
# Start macro BTNG_STL_MULTIMAP_NEEDS
  AC_REQUIRE([BTNG_TYPE_NAMESPACE])
  AC_REQUIRE([BTNG_TYPE_BOOL])
  AC_REQUIRE([BTNG_STL_MULTIMAP_HEADER_FILENAME])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  if test ! "${stl_multimap_header_is_broken+set}" = set; then
    CPPFLAGS_SAVE=$CPPFLAGS
    CPPFLAGS="$CPPFLAGS $CXX_OPTIONS"
    AC_TRY_COMPILE([
#ifdef BOOL_IS_BROKEN
typedef int bool;
#define true 1
#define false 0
#endif
#include STL_MULTIMAP_HEADER_FILE	
#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif
],
      [multimap<int,int> v; pair<const int,int> thePair(0,1); v.insert(thePair);],
      : Do nothing further because this is not a special case.
      ,
      # Did not compile so the less argument must be required.
      AC_DEFINE(STL_MULTIMAP_NEEDS_LESS,1,The STL multimap class needs less specified.)
      AC_TRY_COMPILE([
#ifdef BOOL_IS_BROKEN
typedef int bool;
#define true 1
#define false 0
#endif
#include STL_MULTIMAP_HEADER_FILE
#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif
],
        [multimap<int,int,less<int> > v; pair<const int,int> thePair(0,1); v.insert(thePair);],
        : Do nothing further because this is not a special case.
        ,
        # Did not compile so the allocator argument must be required.
        AC_DEFINE(STL_MULTIMAP_NEEDS_ALLOCATOR,1,The STL multimap class requires allocator specified.)
        AC_TRY_COMPILE([
#ifdef BOOL_IS_BROKEN
typedef int bool;
#define true 1
#define false 0
#endif
#include STL_MULTIMAP_HEADER_FILE
#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif
],
          [[multimap<int,int,less<int>,allocator<int> > v; pair<const int,int> thePair(0,1); v.insert(thePair);]],
          : Do nothing further because this is not a special case.
          ,
          # Did not compile so the multimap container must be broken.
          AC_DEFINE(STL_MULTIMAP_IS_BROKEN,1,The STL multimap class is broken)
        )
      )
    )
    CPPFLAGS=$CPPFLAGS_SAVE
  fi
  AC_LANG_RESTORE

# End macro BTNG_STL_MULTIMAP_NEEDS
])	dnl end of BTNG_STL_MULTIMAP_NEEDS







AC_DEFUN(BTNG_STRUCT_SET_NEEDS_LESS,[

# Start macro BTNG_STRUCT_SET_NEEDS_LESS

AC_MSG_CHECKING(whether STL set container needs the less functor explicitly)
dnl AC_REQUIRE(BTNG_STL_SET_HEADER_FILENAME)

AC_CACHE_VAL(btng_cv_struct_set_needs_less, [

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS

  AC_TRY_COMPILE(
  #include <set.h>
  ,
  set<int> si;
  ,
  # do nothing if set<int> works.
  btng_cv_struct_set_needs_less=no
  ,
  btng_cv_struct_set_needs_less=yes
  )	dnl End AC_TRY_COMPILE call

  AC_LANG_RESTORE

])	dnl End AC_CACHE_VAL call


AC_MSG_RESULT($btng_cv_struct_set_needs_less)

if test "$btng_cv_struct_set_needs_less" = yes; then
  AC_DEFINE(STL_SET_NEEDS_EXPLICIT_LESS)
fi

# End macro BTNG_STRUCT_SET_NEEDS_LESS

])	dnl end of BTNG_STRUCT_SET_NEEDS_LESS definition.










AC_DEFUN(BTNG_STL_STACK_REFUSES_CONTAINER_ARGUMENT,[

# Start macro BTNG_STL_STACK_REFUSES_CONTAINER_ARGUMENT

AC_MSG_CHECKING(whether STL stack refuses optional container argument)


AC_CACHE_VAL(btng_cv_stl_stack_refuses_container_argument, [

  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS

  btng_cv_stl_stack_refuses_container_argument=
  AC_TRY_COMPILE(
    #include <stack.h>
    ,
    stack<int,vector<int> > s; s.push(0);
    ,
    btng_cv_stl_stack_refuses_container_argument=no
    ,
    btng_cv_stl_stack_refuses_container_argument=yes
  )
    if test -n "$btng_cv_stl_stack_refuses_container_argument"; then break; fi
  done

  AC_LANG_RESTORE

])	dnl End AC_CACHE_VAL call

AC_MSG_RESULT($btng_cv_stl_stack_refuses_container_argument)

if test "$btng_cv_stl_stack_refuses_container_argument" = yes; then
  AC_DEFINE(STL_STACK_REFUSES_CONTAINER_ARGUMENT)
fi

# End macro BTNG_STL_STACK_REFUSES_CONTAINER_ARGUMENT

])	dnl end of BTNG_STL_STACK_REFUSES_CONTAINER_ARGUMENT definition.

