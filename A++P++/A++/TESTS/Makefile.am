## Process this file with automake to create Makefile.in

## This permits us to have a large common part of the 
## makefile be used in make different Makefile
## include ../../config/Makefile.setup

## include P++ specific Makefile.config.in
include ../../config/Makefile.config.in

## I don't know why we have this here!!! (for a special case for the IBM)
## CXXFLAGS = @CXXFLAGS@ @RUNTIME_LOADER_FLAGS@ $(CXX_DEBUG) $(CXX_DEFINES) $(CXX_OPTIONS) $(CXX_OPT) $(CXX_TEMPLATES) $(CXX_WARNINGS) $(CXX_DL_COMPILE_FLAGS)

## We want to have a separate template repository 
## locally (separate from the one used by A++)
## This also prevents "make clean" from removing the A++/src/$(CXX_TEMPLATE_REPOSITORY)
CXX_TEMPLATE_REPOSITORY_PATH=.

## Construct path to shared lib
# ARCH = @ARCH@
# CC = @CC@
# CXX = @CXX@
APP_PATH_TO_SHARED_LIB = -L$(libdir)

# DQ: (12/24/2000) need to avoid early specification of -lApp_static $(PTHREADS_LIB)
# LDADD = $(APP_PATH_TO_SHARED_LIB) $(AXX_LIB_PATH) $(AXX_LIBS) -lApp_static $(PTHREADS_LIB)
# LDADD = $(APP_PATH_TO_SHARED_LIB) $(AXX_LIB_PATH)
LDADD = $(APP_PATH_TO_SHARED_LIB) $(AXX_RPATH)

## Add bugs to this line as they are fixed so that they can be used as test problems
## At least we should not reintroduce these fixed bugs again later.
TESTCODES = testcode \
     test2000_01 \
     test2000_02 \
     test2000_03 \
     test2000_04 \
     test2000_05 \
     test2000_06 \
     test2000_07 \
     test2000_08 \
     test2000_09 \
     test2000_10 \
     test2000_11 \
     test2000_12 \
     test2000_13 \
     test2000_14 \
     test2000_15 \
     test2000_16 \
     test2000_17 \
     test2000_18 \
     resize

## noinst_PROGRAMS = $(TESTCODES)
noinst_PROGRAMS = $(TESTCODES)

testcode_SOURCES     = testcode.C
test2000_01_SOURCES  = test2000_01.C
test2000_02_SOURCES  = test2000_02.C
test2000_03_SOURCES  = test2000_03.C
test2000_04_SOURCES  = test2000_04.C
test2000_05_SOURCES  = test2000_05.C
test2000_06_SOURCES  = test2000_06.C
test2000_07_SOURCES  = test2000_07.C
test2000_08_SOURCES  = test2000_08.C
test2000_09_SOURCES  = test2000_09.C
test2000_10_SOURCES  = test2000_10.C
test2000_11_SOURCES  = test2000_11.C
test2000_12_SOURCES  = test2000_12.C
test2000_13_SOURCES  = test2000_13.C
test2000_14_SOURCES  = test2000_14.C
test2000_15_SOURCES  = test2000_15.C
test2000_16_SOURCES  = test2000_16.C
test2000_17_SOURCES  = test2000_17.C
test2000_18_SOURCES  = test2000_18.C
resize_SOURCES       = resize.C

## This would be defined to be the value of the _LDADD suffix variables and we want
## to avoid that dependency (since it gets in the way of tests using "make -n")
testcode_DEPENDENCIES =
test2000_01_DEPENDENCIES =
test2000_02_DEPENDENCIES =
test2000_03_DEPENDENCIES =
test2000_04_DEPENDENCIES =
test2000_05_DEPENDENCIES =
test2000_06_DEPENDENCIES =
test2000_07_DEPENDENCIES =
test2000_08_DEPENDENCIES =
test2000_09_DEPENDENCIES =
test2000_10_DEPENDENCIES =
test2000_11_DEPENDENCIES =
test2000_12_DEPENDENCIES =
test2000_13_DEPENDENCIES =
test2000_14_DEPENDENCIES =
test2000_15_DEPENDENCIES =
test2000_16_DEPENDENCIES =
test2000_17_DEPENDENCIES =
test2000_18_DEPENDENCIES =
resize_DEPENDENCIES =

## Run each test with this script (required to get the 
## LD_LIBRARY_PATH correct when using shared libraries).
## TESTS_ENVIRONMENT = sh ../serial_test_script
## TESTS_ENVIRONMENT = sh ../generalized-serial-run
TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO='$$program.$$nproc.stdout' REDIRECT_ERRORS_TO='$$program.$$nproc.error' sh ../generalized-serial-run

## These tests may or may not have assumed the use 
## of some environment variables (we have to see)
TESTS = $(TESTCODES)

# Rule to check the installation after it is run
# (test compilation using the new installed header files and libraries)
install-exec-hook: installcheck
installcheck:
	$(CXX) $(DEFS) $(CXX_OPTIONS) -I$(includedir) -c $(srcdir)/test2000_01.C -o alt_test2000_01.o
#	Dec cxx compiler can't handle -R option
#	$(CXXLD) -o alt_test2000_01  alt_test2000_01.o -L$(libdir) -R $(libdir) -lApp -lApp_static -lm
	$(CXXLD) $(CXX_OPTIONS) -o alt_test2000_01  alt_test2000_01.o -L$(libdir) -lApp -lApp_static -lm


##
## the user-makefile target is designed to write out a simple
## Makefile.User that has none of the complications of an automake
## generated makefile but can build an application code.
##
#user-makefile:
#	./makefileDefs.sh "$(CC)" "$(CXX)" "$(CXX_DEBUG)" "$(CXXOPT)" "$(CXXOPTIONS)" \
#	"$(CXXDEFINES)" "$(C_DEBUG)" "$(COPT)" "$(COPTIONS)" \
#	"$(AXX_INCLUDES)" "$(AXX_LIBS)"	"$(AXX_LIB_PATH)" \
#	"$(APP_PATH_TO_SHARED_LIB)" "$(DEFS)" "$(top_srcdir)" \
#	"$(CXXFLAGS)" "$(CFLAGS)" "$(LDFLAGS)"
#	./userMakefile.sh "testcode" "testcode.o"

##
## check-local depends on user-makefile, so the user makefile info is then built.
## we then use this makefile to
## Skip running the user-makefile stuff (delete this at a later point)
## check-local: user-makefile
## check-local:
## 	if test -f testcode.o; then rm testcode.o; fi;
## 	$(MAKE) -f Makefile.user testcode
## 	./serial_test_script testcode

#install-data-local:
#	cp Makefile.user.defs $(prefix)/A++/lib/$(ARCH)_$(CC)_$(CXX)/
#	cp Makefile.user $(prefix)/A++/lib/$(ARCH)_$(CC)_$(CXX)/

# EXTRA_DIST = \
#   tarch

# Need to remove all the tests codes plus the extra alt_test2000_01 we build to test the installed A++ library
DISTCLEANFILES = $(TESTCODES) alt_test2000_01

## Uncomment to use with purify
# Purify related flags
# PURIFY_USE_WINDOWS = -windows=yes
# PURIFY_FLAGS  = $(PURIFY_USE_WINDOWS) -recursion-depth-limit=40000 -chain-length=12 -first-only=yes -leaks-at-exit=yes -inuse-at-exit=yes -always-use-cache-dir=yes -cache-dir=/home/dquinlan/tmp
# PURIFY        = purify4.5 $(PURIFY_FLAGS)
# New version of purify
# PURIFY        = /usr/local/pure/purify-5.1-solaris2/purify $(PURIFY_FLAGS)

## Uncomment to use with purify
## CXXLD = $(PURIFY) $(CXX)



clean-local:
	rm -f *.*.stdout *.*.error
	$(clean_shared_libs_and_templates)

