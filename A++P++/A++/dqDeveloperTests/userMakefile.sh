#! /bin/csh -f
#
#  this script 
if ( -e Makefile.user ) then
    rm -f Makefile.user
endif
echo "# " > Makefile.user
echo "# This is a simple makefile that users can use to compile P++">>Makefile.user
echo "# applications.  Fill in the name of your application, ">>Makefile.user
echo "# the Object files it depends upon and any dependencies.">>Makefile.user
echo "USER_APPLICATION=  $1">>Makefile.user
echo "USER_APPLICATION_OBJECTS=  $2">>Makefile.user
echo "USER_APPLICATION_DEPENDENCIES=  ">>Makefile.user
echo "USER_LIBS= ">>Makefile.user
echo "">>Makefile.user
echo "include Makefile.user.defs">>Makefile.user
echo "">>Makefile.user
echo ".SUFFIXES:.o .C">>Makefile.user
echo "">>Makefile.user
echo ".C.o:">>Makefile.user
echo "	"\$\(CXX_COMPILE\) -c \$\< >> Makefile.user
echo "">>Makefile.user
echo \$\(USER_APPLICATION\): \$\(USER_APPLICATION_DEPENDENCIES\) \$\(USER_APPLICATION_OBJECTS\)>>Makefile.user
echo "	"\$\(CXX_LINK\) -o \$\(USER_APPLICATION\) \$\(USER_APPLICATION_OBJECTS\) \$\(LIBS\) \$\(USER_LIBS\)>>Makefile.user
