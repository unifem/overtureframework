## Process this file with automake to create Makefile.in
include ../../config/Makefile.override.install.includeHEADERS

## include common setup (used by all A++/P++ Makefile.am files)
## include ../../config/Makefile.setup

## include ../config/Makefile.config.in
## where we place commonly used AutoMake code
## (used by the lower level Makefile.am files only).
include ../../config/Makefile.config.in

# If this is the development version of A++P++ then include the 
# dependence upon the files in A++P++/Common_Code so that the
# source files in this directory are re-generated when the files
# in A++P++/Common_Code are updated.

# Note that white space is required before the "include" to avoid the automake error:
#      "automake: Makefile.am: unterminated conditionals: @PXX_MAINTAINER_MODE_TRUE@"
# This seems to be a bug in automake.
if DEVELOPMENT_SPECIFIC_DEPENDENCIES

 include $(top_srcdir)/../Common_Code/make_dependence_info.inc

$(srcdir)/A++.h : $(srcdir)/../A++.h
	cp $(srcdir)/../A++.h $(srcdir)/A++.h

$(srcdir)/descriptor.h : $(srcdir)/../descriptor.h
	cp $(srcdir)/../descriptor.h $(srcdir)/descriptor.h

endif

## Add an install target to build a directory under A++/lib reflecting the
## platform and compiler.  Put the includes in an include directory under the 
## newly created one.
# PLATFORM = @ARCH@
# CC = @CC@
# CXX = @CXX@
# INSTALL_DIR = $(PLATFORM)_$(CC)_$(CXX)

## Test of overriding includedir so that 'make install' puts stuff
## in correct directory
# INSTALL_BASE = $(exec_prefix)/A++/lib/$(INSTALL_DIR)
# includedir = $(INSTALL_BASE)/include
# includedir = $(prefix)/$(PACKAGE)/lib/$(INSTALL_DIR)/include

##noinst_HEADERS = \
include_HEADERS = \
 A++.h A++_headers.h A++_templates.h A++_notemplates.h array_domain.h comm_man.h \
 constants.h data_hash.h descriptor.h domain.h descriptor_notemplates.h descriptor_templates.h \
 dimension_macros.h index.h expression_templates.h inline_func.h inline_expression_templates.h \
 inline_support.h lazy_aggregate.h lazy_operand.h lazy_expression.h lazy_statement.h \
 lazy_task.h lazy_taskset.h lazyarray.h machine.h max_array_dim.h mdi_typedef.h \
 optimization.h partitioning.h where.h 


# We have explicity install config.h.  We cannot place it in include_HEADERS
# because that list is only for files in the source tree.  (config.h is generated
# in the compile tree.)  BTNG
# DQ: (12/23/2000) Fixed this to permit compilation in the source tree which is 
#     the default tested configuration.
install-data-local:
	$(INSTALL_DATA) config.h $(DESTDIR)/$(includedir)
#install-data-local:
#	p="config.h"; \
#	if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
#	$(INSTALL_DATA) $$d$$p $(DESTDIR)$(includedir)/$$p;

uninstall-local:
	rm -f $(DESTDIR)$(includedir)/config.h

# DQ: (12/25/2000) Remove this to fix bug when using Makefile.user.defs 
# (which puts -I's in different order and tries to read this file).  
# This is a generated file and does not belong here after "make distclean".
DISTCLEANFILES= config.h


clean-local:
	rm -f *.h

