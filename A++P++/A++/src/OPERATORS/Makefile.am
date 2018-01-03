## Process this file with automake to create Makefile.in

## include common setup (used by all A++/P++ Makefile.am files)
## include ../../../config/Makefile.setup

## where we place commonly used AutoMake code
## (used by the lower level Makefile.am files only).
include ../../../config/Makefile.config.in

# If this is the development version of A++P++ then include the 
# dependence upon the files in A++P++/Common_Code so that the
# source files in this directory are re-generated when the files
# in A++P++/Common_Code are updated.
# Note that white space is required before the "include" to avoid the automake error:
#      "automake: Makefile.am: unterminated conditionals: @PXX_MAINTAINER_MODE_TRUE@"
# This seems to be a bug in automake.
if DEVELOPMENT_SPECIFIC_DEPENDENCIES
 include $(top_srcdir)/../Common_Code/make_dependence_info.inc
endif

INCLUDES = -I$(top_srcdir)/include

libAppOperators_a_SOURCES = d_op_all.C f_op_all.C i_op_all.C

# Name the library we will build
noinst_LIBRARIES = libAppOperators.a

# include dependency information
include $(srcdir)/Makefile.depend

## We don't have to include options since they will automatically be found in the 
## the config.h file (read automatically).  We use the makedepend program here
## because it seems to be installed on all the machines that we have checked.
## all-local: make_depend_run
## make_depend_run: depend
depend:
	$(MAKE) -f makefile.build depend

EXTRA_DIST = Makefile.depend

clean-local:
	$(clean_shared_libs_and_templates)
