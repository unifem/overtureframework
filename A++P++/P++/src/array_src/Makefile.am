## Process this file with automake to create Makefile.in

## include ../../../config/Makefile.setup
include ../../../config/Makefile.config.in

if DEVELOPMENT_SPECIFIC_DEPENDENCIES
# If this is the development version of A++P++ then include the 
# dependence upon the files in A++P++/Common_Code so that the
# source files in this directory are re-generated when the files
# in A++P++/Common_Code are updated.

# Note that white space is required before the "include" to avoid the automake error:
#      "automake: Makefile.am: unterminated conditionals: @PXX_MAINTAINER_MODE_TRUE@"
# This seems to be a bug in automake.
 include $(top_srcdir)/../Common_Code/make_dependence_info.inc
endif

# Libraries created in this directory.
noinst_LIBRARIES = libPppSource.a libPppSource_static.a

# Take this out to cutout use of PGSLIB
# indirect_addressing.C

# Specifications for libPppSource.a.
libPppSource_a_SOURCES = \
     MPI_specific.C \
     allocate.C \
     arraylist.C \
     cases.C \
     conform_array_set.C \
     conform_enforce.C \
     map.C \
     p_array.C \
     p_abstract_op.C \
     p_descriptor.C \
     p_domain.C \
     p_error_checking.C \
     p_data_hash.C \
     p_index_operator.C \
     p_lazy_aggregate.C \
     p_lazy_operand.C \
     p_lazyarray.C \
     p_new_operators.C \
     partlist.C \
     array.C \
     descriptor.C \
     index_operator.C \
     new_operators.C \
     abstract_op.C \
     comm_man.C \
     domain.C \
     descriptor_cache.C \
     error_checking.C \
     data_hash.C \
     index.C \
     lazy_aggregate.C \
     lazy_expression.C \
     lazy_operand.C \
     lazy_statement.C \
     lazy_task.C \
     lazy_taskset.C \
     lazyarray.C \
     optimization.C \
     partitioning.C \
     where.C

libPppSource.a: $(libPppSource_a_OBJECTS) $(libPppSource_a_DEPENDENCIES)
	-rm -f $@
	$(CXX_STATIC_LIB_UPDATE) $@ $(libPppSource_a_OBJECTS) $(libPppSource_a_LIBADD)
	test -f $@ || exit 1

# Specifications for libPppSource_static.a.
libPppSource_static_a_SOURCES = static_initialization.C
libPppSource_static.a: $(libPppSource_static_a_OBJECTS) $(libPppSource_static_a_DEPENDENCIES)
	-rm -f $@
	$(CXX_STATIC_LIB_UPDATE) $@ $(libPppSource_static_a_OBJECTS) $(libPppSource_static_a_LIBADD)
	test -f $@ || exit 1

## include dependency information
include $(srcdir)/Makefile.depend

## We don't have to include options since they will automatically be found in the 
## the config.h file (read automatically).  We use the makedepend program here
## because it seems to be installed on all the machines that we have checked.
## all-local: make_depend_run
## make_depend_run: depend
depend:
#	$(MAKE_DEPEND) $(MAKE_DEPEND_OPTIONS) $(SRCS) $(MAKE_DEPEND_OUTPUT)
	$(MAKE) -f makefile.build depend
##	touch make_depend_run


EXTRA_DIST = Makefile.depend


clean-local:
	$(clean_shared_libs_and_templates)

.NOEXPORT:
	







