## Process this file with automake to create Makefile.in

## This permits us to have a large common part of the 
## makefile be used in make different Makefile
## include ../../config/Makefile.setup

## include the A++ specific Makefile.config.in
include ../../config/Makefile.config.in


PPP_PATH_TO_SHARED_LIB = -L$(libdir)

# ***********************************************
TESTCODES = test2000_01 test2000_02 test2000_03 test2000_04 test2000_05 \
            test2000_06 test2000_07 test2000_08 test2000_09 test2000_10 \
            test2000_11 test2000_12 test2000_13 test2000_14 test2000_15 \
            test2000_16 test2000_17 test2000_18 test2000_19 test2000_20 \
            test2000_21 test2000_22 test2000_23 \
            test_MPI_initialization \
            test_Ppp_initialization \
            test_Ppp_execution

noinst_PROGRAMS = $(TESTCODES)
## bin_PROGRAMS = $(TESTCODES)

# This is an A++ test code!
#testcode_SOURCES               = testcode.C

test_MPI_initialization_SOURCES = test_MPI_initialization.C
test_Ppp_initialization_SOURCES = test_Ppp_initialization.C
test_Ppp_execution_SOURCES      = test_Ppp_execution.C
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

## This would be defined to be the value of the _LDADD suffix variables and we want
## to avoid that dependency (since it gets in the way of tests using "make -n")
#testcode_DEPENDENCIES = ../src/libPpp.a
test_MPI_initialization_DEPENDENCIES =
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

LDADD = $(PPP_PATH_TO_SHARED_LIB) $(PXX_RPATH)

## Talk to Brian about this!!!
## LDADD = $(PADRE_INCLUDES) $(PXX_LIBS) -lPpp_static

## This would be defined to be the value of the _LDADD suffix variables and we want
## to avoid that dependency (since it gets in the way of tests using "make -n")
DEPENDENCIES =

## We want to have a separate template repository 
## locally (separate from the one used by P++)
## so the clean rule will not remove the P++ one!
CXX_TEMPLATE_REPOSITORY_PATH=.

# To check a program we use the automake TESTS_ENVIRONMENT variable
# to execute the generalized-parallel-run script using TEST_NPROCS processors.
# TEST_NPROCS = @TEST_NPROCS@
TEST_NPROCS = 1
# TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO=/dev/null sh ../generalized-parallel-run $(TEST_NPROCS)
TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO='$$program.$$nproc.stdout' REDIRECT_ERRORS_TO='$$program.$$nproc.error' N_PARALLEL_TRIES=2 sh ../generalized-parallel-run $(TEST_NPROCS)

## These tests may or may not have assumed the use 
## of some environment variables (we have to see)
TESTS = $(TESTCODES)
#        test_MPI_initialization_script \
#        test_Ppp_initialization_script \
#        test_Ppp_execution_script

# A way to locally modify the C++ options for the Makefile built in this directory
# AM_CXXFLAGS = -g

# Rule to check the installation after it is run
# (test compilation using the new installed header files and libraries)
install-exec-hook: installcheck
installcheck:
	$(CXX) $(DEFS) $(CXX_OPTIONS) -I$(includedir) $(CPPFLAGS) $(STL_INCLUDES) -c $(srcdir)/test2000_01.C -o alt_test2000_01.o
	$(CXXLD) $(CXX_OPTIONS) -o alt_test2000_01  alt_test2000_01.o -L$(libdir) -lPpp -lPpp_static $(mpich_LIBS) -lm



# Need to remove all the tests codes plus the extra alt_test2000_01 we build to test the installed P++ library
DISTCLEANFILES= $(TESTCODES) \
        alt_test2000_01 \
	parallel_test_script \
        test_MPI_initialization_script \
	test_Ppp_initialization_script \
	test_Ppp_execution_script

## Uncomment to use with purify
# Purify related flags
# PURIFY_USE_WINDOWS = -windows=yes
# PURIFY_FLAGS  = $(PURIFY_USE_WINDOWS) -recursion-depth-limit=40000 -chain-length=24 -first-only=yes -leaks-at-exit=yes -inuse-at-exit=yes -always-use-cache-dir=yes -cache-dir=/home/dquinlan/tmp
# PURIFY        = purify4.5 $(PURIFY_FLAGS)
# New version of purify
# PURIFY        = /usr/local/pure/purify-5.1-solaris2/purify $(PURIFY_FLAGS)

## Uncomment to use with purify
## CXXLD = $(PURIFY) $(CXX)


clean-local:
	rm -f *.*.stdout *.*.error
	$(clean_shared_libs_and_templates)
