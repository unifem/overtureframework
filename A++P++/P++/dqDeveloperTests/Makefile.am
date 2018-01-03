## Process this file with automake to create Makefile.in

## This permits us to have a large common part of the 
## makefile be used in make different Makefile
## include ../../config/Makefile.setup

## include the A++ specific Makefile.config.in
## include ../config/APP_Makefile_config.in
include ../../config/Makefile.config.in

## Avoid overwriting these rules for A++ and P++!
## overwrite CXXFLAGS and CFLAGS to not use PIC compilation
## and turn off optimization
## DQ note: see if there is a better way of handling this so that we don't have to overwrite the CXXFLAGS variable
## CXXFLAGS      = @RUNTIME_LOADER_FLAGS@ $(CXX_DEBUG) $(CXX_DEFINES) $(CXX_OPTIONS) $(CXX_WARNINGS) $(CXX_OPT)
## CFLAGS        = $(C_DEBUG) $(C_WARNINGS)

## Force faster compilation without optimization
CXX_OPT =

## Construct path to shared lib
# ARCH = @ARCH@
# CC = @CC@
# CXX = @CXX@
# PPP_PATH_TO_SHARED_LIB = -L$(top_srcdir)/lib/$(ARCH)_$(CC)_$(CXX)/lib
PPP_PATH_TO_SHARED_LIB = -L$(libdir)

## Test of overriding libdir and bin dir so that 'make install' puts stuff
## in correct directory
INSTALL_BASE = $(exec_prefix)/P++/lib/$(INSTALL_DIR)
libdir = $(INSTALL_BASE)/lib


# ***********************************************
if DQ_DEVELOPER_TESTS

# place the test code that I'm currently working on first to implify my life
TESTCODES = \
     test2001_03 \
     test2001_05 test2001_06 \
     test2001_07 test2001_08 test2001_09 \
     test2000_01 test2000_02 test2000_03 test2000_04 test2000_05 \
     test2000_06 test2000_07 test2000_08 test2000_09 test2000_10 \
     test2000_11 test2000_12 test2000_13 test2000_14 test2000_15 \
     test2000_16 test2000_17 test2000_18 test2000_19 test2000_20 \
     test2000_21 test2000_22 test2000_22B test2000_23 test2000_24 test2000_25 \
     test2000_26 test2000_27 test2000_28 test2000_29 test2000_30 \
     test2000_31 test2000_32 test2000_33 test2000_34 test2000_35 \
     test2000_36 test2000_37 test2000_38 test2000_39 test2000_40 \
     test2000_41 test2000_42 test2000_43 test2000_44 test2000_45 \
     test2000_46 test2000_47 test2000_48 test2000_49 test2000_50 \
     test2000_51 \
     test_MPI_initialization \
     test_Ppp_initialization \
     test_Ppp_execution

# Leave these out sincefeatures in P++ that they test are not ready yet
#    test2001_01 test2001_02 test2001_04
#    riemann

endif

noinst_PROGRAMS = $(TESTCODES)
## bin_PROGRAMS = $(TESTCODES)

# test2001_01_SOURCES  = test2001_01.C
# test2001_02_SOURCES  = test2001_02.C
# test2001_04_SOURCES  = test2001_04.C
# riemann_SOURCES      = riemann.C

test2001_03_SOURCES  = test2001_03.C

# if DQ_DEVELOPER_TESTS

# ZZZ = yyy

# else

test2001_05_SOURCES  = test2001_05.C
test2001_06_SOURCES  = test2001_06.C
test2001_07_SOURCES  = test2001_07.C
test2001_08_SOURCES  = test2001_08.C
test2001_09_SOURCES  = test2001_09.C

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
test2000_22B_SOURCES = test2000_22B.C
test2000_23_SOURCES  = test2000_23.C
test2000_24_SOURCES  = test2000_24.C
test2000_25_SOURCES  = test2000_25.C
test2000_26_SOURCES  = test2000_26.C
test2000_27_SOURCES  = test2000_27.C
test2000_28_SOURCES  = test2000_28.C
test2000_29_SOURCES  = test2000_29.C
test2000_30_SOURCES  = test2000_30.C
test2000_31_SOURCES  = test2000_31.C
test2000_32_SOURCES  = test2000_32.C
test2000_33_SOURCES  = test2000_33.C
test2000_34_SOURCES  = test2000_34.C
test2000_35_SOURCES  = test2000_35.C
test2000_36_SOURCES  = test2000_36.C
test2000_37_SOURCES  = test2000_37.C
test2000_38_SOURCES  = test2000_38.C
test2000_39_SOURCES  = test2000_39.C
test2000_40_SOURCES  = test2000_40.C
test2000_41_SOURCES  = test2000_41.C
test2000_42_SOURCES  = test2000_42.C
test2000_43_SOURCES  = test2000_43.C
test2000_44_SOURCES  = test2000_44.C
test2000_45_SOURCES  = test2000_45.C
test2000_46_SOURCES  = test2000_46.C
test2000_47_SOURCES  = test2000_47.C
test2000_48_SOURCES  = test2000_48.C
test2000_49_SOURCES  = test2000_49.C
test2000_50_SOURCES  = test2000_50.C
test2000_51_SOURCES  = test2000_51.C

# endif

## The use of templates requires that we include the path to the templates (in PADRE/src)
## LDADD = $(PPP_PATH_TO_SHARED_LIB) $(PXX_LIB_PATH) $(PXX_LIBS) -lPpp_static
## LDADD = $(PPP_PATH_TO_SHARED_LIB) $(RPATH) $(PXX_LIB_PATH) $(PXX_LIBS) -lPpp_static
## LDADD = $(PPP_PATH_TO_SHARED_LIB) $(RPATH) -lPpp_static
LDADD = $(PPP_PATH_TO_SHARED_LIB) $(PXX_RPATH) -lPpp_static

## Talk to Brian about this!!!
## LDADD = $(PADRE_INCLUDES) $(PXX_LIBS) -lPpp_static

## This would be defined to be the value of the _LDADD suffix variables and we want
## to avoid that dependency (since it gets in the way of tests using "make -n")
DEPENDENCIES =

## We want to have a separate template repository 
## locally (separate from the one used by P++)
## so the clean rule will not remove the P++ one!
CXX_TEMPLATE_REPOSITORY_PATH=.

# TEST_NPROCS = 1,2,3,4,5
TEST_NPROCS = 1,2

# To check a program we use the automake TESTS_ENVIRONMENT variable
# to execute the generalized-parallel-run script using TEST_NPROCS processors.
## TESTS_ENVIRONMENT = REDIRECT_RESULTS_TO=/dev/null sh generalized-parallel-run @TEST_NPROCS@
## TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO=/dev/null REDIRECT_ERRORS_TO=/dev/null sh generalized-parallel-run $(TEST_NPROCS)

## A quick hack to make this work would be to write another shell script 
## which hard codes the value of the TESTS_ENVIRONMENT variable directly.
## TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO=\$$program.\$$nproc.stdout REDIRECT_ERRORS_TO=\$$program.\$$nproc.error sh ../generalized-parallel-run $(TEST_NPROCS)
TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO='$$program.$$nproc.stdout' REDIRECT_ERRORS_TO='$$program.$$nproc.error' sh ../generalized-parallel-run $(TEST_NPROCS)

# Working use of PETF using TESTS_ENVIRONMENT variable with make check maechanism
# MY_RUNTESTS = $(PERL) -I$(PERL_INCLUDE) -e 'use petf::Auditor qw(&runtests $$srcdir $$allow_exitstatus_only); $$srcdir="$(srcdir)"; $$allow_exitstatus_only=1; runtests ( @ARGV );'
# TESTS_ENVIRONMENT = $(MY_RUNTESTS) PxxTests \"PETF_NAME=xxx.c $? $* $< $@ $% \"

# Used for testing the value of the TESTS_ENVIRONMENT variable
echoTestEnv:
	@echo testing environment is: $(TESTS_ENVIRONMENT)

TESTS = $(TESTCODES)
# TESTS = test2001_03

# A way to locally modify the C++ options for the Makefile built in this directory
# AM_CXXFLAGS = -g

## CXXLINK = purify @CXXLINK@

##
## the user-makefile target is designed to write out a simple
## Makefile.User that has none of the complications of an automake
## generated makefile but can build an application code.
##
user-makefile:
	./makefileDefs.sh "$(CC)" "$(CXX)" "$(CXX_DEBUG)" "$(CXXOPT)" "$(CXXOPTIONS)" \
	"$(CXXDEFINES)" "$(C_DEBUG)" "$(COPT)" "$(COPTIONS)" "$(MPIINCLUDE)" \
	"$(MPILIBDIRS)" "$(MPILIBS)" "$(PADRE_INCLUDES)" "$(PXX_INCLUDES)" "$(PXX_LIBS)" \
	"$(PXX_LIB_PATH)" "$(PPP_PATH_TO_SHARED_LIB)" "$(DEFS)" "$(top_srcdir)" \
	"$(CXXFLAGS)" "$(CFLAGS)" "$(LDFLAGS)" "$(RPATH)"
	./userMakefile.sh "test_Ppp_execution" "test_Ppp_execution.o"

##
## check-local depends on user-makefile, so the user makefile info is then built.
## we then use this makefile to 
## Skip running the user-makefile stuff (delete this at a later point)
# check-local: user-makefile
# check-local:
#	if test -f test_Ppp_execution.o; then rm test_Ppp_execution.o; fi;
#	$(MAKE) -f Makefile.user test_Ppp_execution;
#	./parallel_test_script test_Ppp_execution "1 2 3 4"

# install-data-local:
#	cp Makefile.user.defs $(prefix)/P++/lib/$(ARCH)_$(CC)_$(CXX)/
#	cp Makefile.user $(prefix)/P++/lib/$(ARCH)_$(CC)_$(CXX)/

##dist-hook:
##	cp -rp $(srcdir)/MG $(distdir)/MG

DISTCLEANFILES= $(TESTCODES) \
	parallel_test_script \
        test_MPI_initialization_script \
	test_Ppp_initialization_script \
	test_Ppp_execution_script

#  user-makefile
#  test_MPI_initialization_script 
#  test_Ppp_initialization_script 
#  test_Ppp_execution_script 
#  makefileDefs.sh
#  userMakefile.sh

EXTRA_DIST = \
  purify_access_errors \
  purify_bytes_leaked \
  purify_bytes_potentially_leaked \
  purify_check

clean-local:
	rm -f *.error *.stdout

## Uncomment to use with purify
# Purify related flags
# PURIFY_USE_WINDOWS = -windows=yes
# PURIFY_FLAGS  = $(PURIFY_USE_WINDOWS) -recursion-depth-limit=40000 -chain-length=24 -first-only=yes -leaks-at-exit=yes -inuse-at-exit=yes -always-use-cache-dir=yes -cache-dir=/home/dquinlan/tmp
# PURIFY        = purify4.5 $(PURIFY_FLAGS)
# New version of purify
# PURIFY        = /usr/local/pure/purify-5.1-solaris2/purify $(PURIFY_FLAGS)

## Uncomment to use with purify
## CXXLD = $(PURIFY) $(CXX)

# *********************************
# PerlEnabledTestingFramework stuff
# *********************************

# PERL=@PERL@
# PERL=/bin/perl

# PERL_INCLUDE=$(top_srcdir)/../CUTE/PerlEnabledTestingFramework/perl5lib
# PETF_MODULES= petf/Runner.pm petf/Reporter.pm petf/Auditor.pm
# RUNPERLTEST=$(PERL) -I$(PERL_INCLUDE) -e 'use petf::Auditor qw(&runtests $$srcdir $$allow_exitstatus_only); $$srcdir="$(srcdir)"; $$allow_exitstatus_only=1; runtests( @ARGV );'

# Build the list of object files
# TEST_EXECUTABLES = ${TESTCODES:.C=}
# ALLTESTS=$(TEST_EXECUTABLES)

# working example
# /bin/perl -I/home/dquinlan/A++P++/A++P++Source/A++P++/CUTE/PerlEnabledTestingFramework/perl5lib -e 'use petf::Auditor qw(&runtests $srcdir $allow_exitstatus_only); $srcdir="/home/dquinlan/A++P++/A++P++Source/A++P++/P++/dqDeveloperTests"; $allow_exitstatus_only=1; runtests( @ARGV );' mytests "PETF_NAME=test2001_01 echo test2001_01.C"

# RUNTESTS = $(PERL) -I$(PERL_INCLUDE) -e 'use petf::Auditor qw(&runtests $$srcdir $$allow_exitstatus_only); $$srcdir="$(srcdir)"; $$allow_exitstatus_only=1; runtests ( @ARGV );'

# check-local:
#	$(RUNPERLTEST) PxxCheck \
# "PETF_NAME=test2001_03 test2001_03" \
# "PETF_NAME=test2001_05 test2001_05"

# check-local-failing:
# 	$(RUNPERLTEST) PxxCheck \
# "PETF_NAME=test2001_03 $(TESTS_ENVIRONMENT) test2001_03" \
# "PETF_NAME=test2001_05 $(TESTS_ENVIRONMENT) test2001_05"

#	cmds=""
#        for i in $(FILES); do \
#          cmds="$$cmds  \"PETF_NAME=$$i $(ROSE) $$i\" "; \
#	   echo "\$$cmds = $$cmds"; \
#        done; \
