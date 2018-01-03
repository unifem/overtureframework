## Process this file with automake to create Makefile.in

## This permits us to have a large common part of the 
## makefile be used in make different Makefile
## include ../../config/Makefile.setup

## include the A++P++ Makefile.config.in
include ../../config/Makefile.config.in


PPP_PATH_TO_SHARED_LIB = -L$(libdir)

noinst_PROGRAMS = riemann test_MPI_initialization test_Ppp_initialization test_Ppp_execution

test_MPI_initialization_SOURCES  = test_MPI_initialization.C
test_Ppp_initialization_SOURCES  = test_Ppp_initialization.C
test_Ppp_execution_SOURCES       = test_Ppp_execution.C

# Shock Tube Computation
riemann_SOURCES                  = riemann.C

## The use of templates requires that we include the path to the templates (in PADRE/src)
LDADD = $(PPP_PATH_TO_SHARED_LIB) $(PXX_RPATH)

## This would be defined to be the value of the _LDADD suffix variables and we want
## to avoid that dependency (since it gets in the way of tests using "make -n")
## DEPENDENCIES =
test_MPI_initialization_DEPENDENCIES =
test_Ppp_initialization_DEPENDENCIES =
test_Ppp_execution_DEPENDENCIES      =

## We want to have a separate template repository 
## locally (separate from the one used by P++)
## so the clean rule will not remove the P++ one!
CXX_TEMPLATE_REPOSITORY_PATH=.

# To check a program we use the automake TESTS_ENVIRONMENT variable
# to execute the generalized-parallel-run script using TEST_NPROCS processors.
TEST_NPROCS = @TEST_NPROCS@
# TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO=/dev/null sh ../generalized-parallel-run $(TEST_NPROCS)
TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO='$$program.$$nproc.stdout' REDIRECT_ERRORS_TO='$$program.$$nproc.error' NUM_PARALLEL_TRIES=2 sh ../generalized-parallel-run $(TEST_NPROCS)

## These tests may or may not have assumed the use 
## of some environment variables (we have to see)
TESTS = \
   test_MPI_initialization \
   test_Ppp_initialization \
   test_Ppp_execution

# Skip the test of riemann as an example program since it fails on 
# four processors this is something that I am looking into currently.
#   riemann

# A way to locally modify the C++ options for the Makefile built in this directory
AM_CXXFLAGS = -g

## CXXLINK = purify @CXXLINK@


# This checks the examples mechanism.
# It issues the commands as the user would.
# It tests whether we created Makefile.user and Makefile.user.defs correctly.
# For more info, see comments in Makefile.user.
# BTNG
check-local:
	$(MAKE) Makefile.user.defs
	if [ ! -r Makefile.user  ]; then	\
	  ln -s $(srcdir)/Makefile.user .;	\
	fi
	@echo Testing user example environments, Makefile.user, etc.
	@echo   Making clean
	$(MAKE) -f Makefile.user USER_APPLICATION=./test_Ppp_execution USER_APPLICATION_OBJECTS=test_Ppp_execution.o clean
	@echo   Making executable
	$(MAKE) -f Makefile.user USER_APPLICATION=./test_Ppp_execution USER_APPLICATION_OBJECTS=test_Ppp_execution.o compile
	@echo   Making run
	$(MAKE) -f Makefile.user USER_APPLICATION=./test_Ppp_execution USER_APPLICATION_OBJECTS=test_Ppp_execution.o run > user-run-output



EXTRA_DIST = \
  Makefile.user Makefile.user.defs.in

install-data-local:
	$(INSTALL_DATA) Makefile.user.defs $(DESTDIR)$(libdir)/..

uninstall-local:
	rm -f $(DESTDIR)$(libdir)/../Makefile.user.defs

# Rule to check the installation after it is run
# (test compilation using the new installed header files and libraries)
install-data-hook: installcheck
installcheck:
	@if test -r $(DESTDIR)$(libdir)/../Makefile.user.defs; then \
		echo "Verified presence of Makefile.user.defs file in installation"; \
	  else \
		echo "Can't find Makefile.user.defs file in installation"; \
		exit 1; \
	fi

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
	rm -f user-run-output
