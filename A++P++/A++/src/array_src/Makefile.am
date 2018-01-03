## Process this file with automake to create Makefile.in

## include common setup (used by all A++/P++ Makefile.am files)
## include ../../../config/Makefile.setup

## include ../../config/Makefile.config.in
## where we place commonly used AutoMake code
## (used by the lower level Makefile.am files only).
include ../../../config/Makefile.config.in

## include all the relavant filenames (with and without relative paths)
# include FileNames.lst

# If this is the development version of A++P++ then include the 
# dependence upon the files in A++P++/Common_Code so that the
# source files in this directory are re-generated when the files
# in A++P++/Common_Code are updated.
# Note: that white space is required before the "include" to avoid the automake error:
#      "automake: Makefile.am: unterminated conditionals: @PXX_MAINTAINER_MODE_TRUE@"
# This seems to be a bug in automake.
if DEVELOPMENT_SPECIFIC_DEPENDENCIES
 include $(top_srcdir)/../Common_Code/make_dependence_info.inc
endif

# SRCS = $(APP_ARRAY_SRCS_with_path) \
#        $(STATIC_ARRAY_SRCS_with_path)

## libAppSource_a_SOURCES = abstract_op.C
## libAppSource_a_SOURCES = $(ARRAY_SRCS)
## libAppSource_a_SOURCES = $(SRCS)
libAppSource_a_SOURCES = \
 array_notemplates.C \
 descriptor_notemplates.C \
 index_operator_notemplates.C \
 new_operators_notemplates.C \
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
 where.C \
 static_initialization.C

## include dependency information
include $(srcdir)/Makefile.depend

## We don't have to include options since they will automatically be found in the 
## the config.h file (read automatically).  We use the makedepend program here
## because it seems to be installed on all the machines that we have checked.
## all-local: make_depend_run
## make_depend_run: depend
depend:
	$(MAKE) -f makefile.build depend

## Name the library we will build
noinst_LIBRARIES = libAppSource.a

EXTRA_DIST = Makefile.depend

clean-local:
	$(clean_shared_libs_and_templates)






