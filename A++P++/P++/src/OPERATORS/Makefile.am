## Process this file with automake to create Makefile.in

## include ../../../config/Makefile.setup
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

libPppOperators_a_SOURCES = pd_op_all.C pf_op_all.C pi_op_all.C d_op_all.C f_op_all.C i_op_all.C 
libPppOperators.a: $(libPppOperators_a_OBJECTS) $(libPppOperators_a_DEPENDENCIES)
	-rm -f libPppOperators.a
	$(CXX_STATIC_LIB_UPDATE) libPppOperators.a $(libPppOperators_a_OBJECTS) $(libPppOperators_a_LIBADD)
	test -f $@ || exit 1

noinst_LIBRARIES = libPppOperators.a

## include dependency information
include $(srcdir)/Makefile.depend

# We don't have to include options since they will automatically be found in the 
# the config.h file (read automatically).  We use the makedepend program here
# because it seems to be installed on all the machines that we have checked.
# old version: $(MAKE_DEPEND) $(MAKE_DEPEND_OPTIONS) $(OPERATOR_SRCS) $(MAKE_DEPEND_OUTPUT)
# all-local: make_depend_run
# make_depend_run: depend
depend: 
	$(MAKE) -f makefile.build depend

EXTRA_DIST = Makefile.depend

clean-local:
	$(clean_shared_libs_and_templates)
