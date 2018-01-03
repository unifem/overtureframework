## Process this file with automake to create Makefile.in

# Location of A++ directory (used as a reference for all relative paths in the Makefile)

## This permits us to have a large common part of the 
## makefile be used in make different Makefile
## include ../../config/Makefile.setup

## include P++ specific Makefile.config.in
include ../../config/Makefile.config.in

# CXXFLAGS = @CXXFLAGS@ @RUNTIME_LOADER_FLAGS@ $(CXX_OPT) $(CXX_DEBUG) $(CXX_DEFINES) $(CXX_OPTIONS) $(CXX_TEMPLATES) $(CXX_WARNINGS)
## CFLAGS        = $(C_DEBUG) $(C_WARNINGS)
CXX_OPT =

## We want to have a separate template repository 
## locally (separate from the one used by A++)
## This also prevents "make clean" from removing the A++/src/$(CXX_TEMPLATE_REPOSITORY)
CXX_TEMPLATE_REPOSITORY_PATH=.

## Avoid testing the wrong libraries!!! (this has been a problem twice and it wastes huge amounts of time)
APP_PATH_TO_SHARED_LIB = -L$(libdir)
## APP_PATH_TO_SHARED_LIB =


noinst_PROGRAMS  = riemann testcode
testcode_SOURCES = testcode.C
riemann_SOURCES  = riemann.C

## testcode_LDADD = ../src/libApp.a
## Avoid testing the wrong libraries!!! (this has been a problem twice and it wastes huge amounts of time)
# DQ: (12/25/2000) need to avoid early specification of -lApp_static $(PTHREADS_LIB)
## LDADD = $(APP_PATH_TO_SHARED_LIB) $(AXX_LIB_PATH) $(AXX_LIBS) -lApp_static $(PTHREADS_LIB)
# LDADD = $(APP_PATH_TO_SHARED_LIB) $(AXX_LIB_PATH) $(AXX_LIBS)
LDADD = $(APP_PATH_TO_SHARED_LIB) $(AXX_RPATH)

## This would be defined to be the value of the _LDADD suffix variables and we want
## to avoid that dependency (since it gets in the way of tests using "make -n")
testcode_DEPENDENCIES =

## Run each test with this script (required to get the 
## LD_LIBRARY_PATH correct when using shared libraries).
## TESTS_ENVIRONMENT = sh ../generalized-serial-run
TESTS_ENVIRONMENT = REDIRECT_OUTPUT_TO='$$program.$$nproc.stdout' REDIRECT_ERRORS_TO='$$program.$$nproc.error' sh ../generalized-serial-run

## These tests may or may not have assumed the use 
## of some environment variables (we have to see)
TESTS = testcode


# This checks the examples mechanism.
# It issues the commands as the user would.
# It tests whether we created Makefile.user and Makefile.user.defs correctly.
# For mor info, see comments in Makefile.user.
check-local:
	$(MAKE) Makefile.user.defs
	if [ ! -r Makefile.user  ]; then	\
	  ln -s $(srcdir)/Makefile.user .;	\
	fi
	@echo Testing user example environments, Makefile.user, etc.
	@echo   Making clean
	$(MAKE) -f Makefile.user USER_APPLICATION=./testcode USER_APPLICATION_OBJECTS=testcode.o clean
	@echo   Making executable
	$(MAKE) -f Makefile.user USER_APPLICATION=./testcode USER_APPLICATION_OBJECTS=testcode.o compile
	@echo   Making run
	$(MAKE) -f Makefile.user USER_APPLICATION=./testcode USER_APPLICATION_OBJECTS=testcode.o run > user-run-output


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


clean-local:
	rm -f *.*.stdout *.*.error
	$(clean_shared_libs_and_templates)
	rm -f user-run-output












