## Process this file with automake to create Makefile.in

# Location of A++ directory (used as a reference for all relative paths in the Makefile)
# AXX_HOME = $(top_srcdir)

## This permits us to have a large common part of the 
## makefile be used in make different Makefile
## include ../../config/Makefile.setup

## include A++/P++ specific Makefile.config.in
include ../../config/Makefile.config.in

## I don't know why we have this here!!! (for a special case on the IBM)
## CXXFLAGS = @RUNTIME_LOADER_FLAGS@ $(CXX_OPT) $(CXX_DEBUG) $(CXX_DEFINES) $(CXX_OPTIONS) $(CXX_TEMPLATES) $(CXX_WARNINGS)
## CFLAGS        = $(C_DEBUG) $(C_WARNINGS)

## We want to have a separate template repository 
## locally (separate from the one used by A++)
## This also prevents "make clean" from removing the A++/src/$(CXX_TEMPLATE_REPOSITORY)
CXX_TEMPLATE_REPOSITORY_PATH=.

## Construct path to shared lib
# ARCH = @ARCH@
# CC = @CC@
# CXX = @CXX@
# APP_PATH_TO_SHARED_LIB = -L$(top_srcdir)/lib/$(ARCH)_$(CC)_$(CXX)/lib
APP_PATH_TO_SHARED_LIB = -L$(libdir)

# LDADD = $(APP_PATH_TO_SHARED_LIB) -lApp_static
LDADD = $(APP_PATH_TO_SHARED_LIB) $(AXX_RPATH)

## We would like to avoid a redundent setting of the INCLUDE variable
## to ../include (since this is done automatically with AutoMake).
## INCLUDES = 

## Add bugs to this line as they are fixed so that they can be used as test problems
## At least we should not reintroduce these fixed bugs again later.
if DQ_DEVELOPER_TESTS
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
     test2000_19 \
     test2000_20 \
     test2000_21 \
     test2000_22 \
     test2000_23 \
     test2000_24 \
     test2000_25 \
     test2000_26 \
     test2000_27 \
     test2000_28 \
     test2000_29 \
     test2000_30 \
     test2000_31 \
     test2001_01 \
     resize
endif

noinst_PROGRAMS = $(TESTCODES)
## bin_PROGRAMS = $(TESTCODES)

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
test2000_19_SOURCES  = test2000_19.C
test2000_20_SOURCES  = test2000_20.C
test2000_21_SOURCES  = test2000_21.C
test2000_22_SOURCES  = test2000_22.C
test2000_23_SOURCES  = test2000_23.C
test2000_24_SOURCES  = test2000_24.C
test2000_25_SOURCES  = test2000_25.C
test2000_26_SOURCES  = test2000_26.C
test2000_27_SOURCES  = test2000_27.C
test2000_28_SOURCES  = test2000_28.C
test2000_29_SOURCES  = test2000_29.C
test2000_30_SOURCES  = test2000_30.C
test2000_31_SOURCES  = test2000_31.C
test2001_01_SOURCES  = test2001_01.C
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
test2000_19_DEPENDENCIES =
test2000_20_DEPENDENCIES =
test2000_21_DEPENDENCIES =
test2000_22_DEPENDENCIES =
test2000_23_DEPENDENCIES =
test2000_24_DEPENDENCIES =
test2000_25_DEPENDENCIES =
test2000_26_DEPENDENCIES =
test2000_27_DEPENDENCIES =
test2000_28_DEPENDENCIES =
test2000_29_DEPENDENCIES =
test2000_30_DEPENDENCIES =
test2000_31_DEPENDENCIES =
test2001_01_DEPENDENCIES =
resize_DEPENDENCIES =

## Run each test with this script (required to get the
## LD_LIBRARY_PATH correct when using shared libraries).
## TESTS_ENVIRONMENT = sh serial_test_script
## TESTS_ENVIRONMENT = sh ./A++_test_script
## TESTS_ENVIRONMENT = sh ../generalized-serial-run
TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO='$$program.$$nproc.stdout' REDIRECT_ERRORS_TO='$$program.$$nproc.error' sh ../generalized-serial-run

## These tests may or may not have assumed the use
## of some environment variables (we have to see)
TESTS = $(TESTCODES)

# This is no longer required
# tarch

EXTRA_DIST = \
  makefileDefs.sh \
  userMakefile.sh \
  purify_access_errors \
  purify_bytes_leaked \
  purify_bytes_potentially_leaked \
  purify_check

DISTCLEANFILES = $(TESTCODES) serial_test_script

## Uncomment to use with purify
# Purify related flags
# PURIFY_USE_WINDOWS = -windows=yes
# PURIFY_FLAGS  = $(PURIFY_USE_WINDOWS) -recursion-depth-limit=40000 -chain-length=12 -first-only=yes -leaks-at-exit=yes -inuse-at-exit=yes -always-use-cache-dir=yes -cache-dir=/home/dquinlan/tmp
# PURIFY        = purify4.5 $(PURIFY_FLAGS)
# New version of purify
# PURIFY        = /usr/local/pure/purify-5.1-solaris2/purify $(PURIFY_FLAGS)

## Uncomment to use with purify
## CXXLD = $(PURIFY) $(CXX)










