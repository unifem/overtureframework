#!/bin/csh
#
# This makefile takes as arguments the makefile variables needed 
# to compile and link an application and writes them out to a file
# called userMakefile.defs
# 
# The items specified as arguments to this script are (in order):
# CC
# CXX
# CXXDEBUG
# CXXOPT
# CXXOPTIONS
# CXXDEFINES
# CDEBUG
# COPT
# COPTIONS
# AXX_INCLUDES
# AXX_LIBS
# AXX_LIB_PATH
# APP_PATH_TO_SHARED_LIB
# DEFS
# AXX_HOME
# CXXFLAGS
# CFLAGS
# LDFLAGS
# 
# From these variables, we build up default values:
# 
# CXX_COMPILE CXX_LINK LIBS 
# 
#
if ( -e Makefile.user.defs ) then
    rm -f Makefile.user.defs
endif

echo "CC                     = $1"  >&! Makefile.user.defs
echo "CXX                    = $2"  >>& Makefile.user.defs
echo "CXXDEBUG               = $3"  >>& Makefile.user.defs
echo "CXXOPT                 = $4"  >>& Makefile.user.defs
echo "CXXOPTIONS             = $5"  >>& Makefile.user.defs
echo "CXXDEFINES             = $6"  >>& Makefile.user.defs
echo "CDEBUG                 = $7"  >>& Makefile.user.defs
echo "COPT                   = $8"  >>& Makefile.user.defs
echo "COPTIONS               = $9"  >>& Makefile.user.defs
echo "AXX_INCLUDES           = $10" | sed 's:-I\.\./:-I$(AXX_HOME)\/:'     >>& Makefile.user.defs
echo "AXX_LIBS               = $11" >>& Makefile.user.defs 
echo "AXX_LIB_PATH           = $12" | sed 's:-L\.\./:-L$(AXX_HOME)\/:g'    >>& Makefile.user.defs
echo "APP_PATH_TO_SHARED_LIB = $13" | sed 's:-L\.\./:-L$(AXX_HOME)\/:g'    >>& Makefile.user.defs
echo "DEFS                   = $14" | sed 's:-I\.\/\.\./:-I$(AXX_HOME)\/:' >>& Makefile.user.defs
echo "AXX_HOME               = $15" >>& Makefile.user.defs
echo "CXXFLAGS               = $16" >>& Makefile.user.defs
echo "CFLAGS                 = $17" >>& Makefile.user.defs
echo "LDFLAGS                = $18" >>& Makefile.user.defs

# This won't work since in the Makefile CXXLINK = $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@
# echo "CXXLINK= $20" >>& Makefile.user.defs
echo "" >>& Makefile.user.defs

echo "C_COMPILE   = " \$\(CC\) \$\(DEFS\) \$\(CDEBUG\) \$\(COPT\) \
                      \$\(COPTIONS\) \$\(AXX_INCLUDES\)        >>& Makefile.user.defs
echo "CXX_COMPILE = " \$\(CXX\) \$\(DEFS\) \$\(CXXDEBUG\) \$\(CXXOPT\) \$\(CXXOPTIONS\) \
                      \$\(CXXDEFINES\) \$\(AXX_INCLUDES\)      >>& Makefile.user.defs
echo "C_LINK      = " \$\(CC\) \$\(CFLAGS\) \$\(LDFLAGS\)      >>& Makefile.user.defs
echo "CXX_LINK    = " \$\(CXX\) \$\(CXXFLAGS\) \$\(LDFLAGS\)   >>& Makefile.user.defs
echo "LIBS        = " \$\(AXX_LIB_PATH\) \$\(APP_PATH_TO_SHARED_LIB\) \
                      \$\(AXX_LIBS\) -lApp_static              >>& Makefile.user.defs
