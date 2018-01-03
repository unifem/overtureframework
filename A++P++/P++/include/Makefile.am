include ../../config/Makefile.override.install.includeHEADERS
## Process this file with automake to create Makefile.in

## include ../../config/Makefile.setup

## When we want to move the time consuming stuff to this level directory
## we can have it run only once rather than once per subdirectory (we can 
## use this as a way to make the configuration more efficient later).

## include the A++ specific Makefile.config.in
include ../../config/Makefile.config.in

## include the P++ specific Makefile.config.in
## include ../config/Makefile.config.in

# If this is the development version of A++P++ then include the 
# dependence upon the files in A++P++/Common_Code so that the
# source files in this directory are re-generated when the files
# in A++P++/Common_Code are updated.

# Note that white space is required before the "include" to avoid the automake error:
#      "automake: Makefile.am: unterminated conditionals: @PXX_MAINTAINER_MODE_TRUE@"
# This seems to be a bug in automake.
if DEVELOPMENT_SPECIFIC_DEPENDENCIES
 include $(top_srcdir)/../Common_Code/make_dependence_info.inc

$(srcdir)/A++.h:             $(COMMON_CODE)/array.h_m4 \
                             $(COMMON_CODE)/descriptor_macro_template.m4 \
                             $(srcdir)/generate_A++.h_m4
	@echo "Rebuild the source file $*.h"
	$(M4_WITH_I) $(srcdir)/generate_A++.h_m4 > $(srcdir)/A++.h

$(srcdir)/descriptor.h:      $(COMMON_CODE)/descriptor.h_m4 \
                             $(COMMON_CODE)/descriptor_macro_template.m4 \
                             $(srcdir)/generate_descriptor.h_m4
	@echo "Rebuild the source file $*.h"
	$(M4_WITH_I) $(srcdir)/generate_descriptor.h_m4 > $(srcdir)/descriptor.h

endif

## This could alternatively been handled using the EXTRA_DIST (the whole directory
## could be specified in the higher level directory instead of listing the files separately).
include_HEADERS = \
   A++.h                  lazy_taskset.h         p_inline_func.h        \
   A++_headers.h          conform_array_set.h    index.h                lazyarray.h            p_inline_support.h     \
   conform_enforce.h      indirect_addressing.h  machine.h              p_lazy_aggregate.h     \
   P++.h                  constants.h            inline_func.h          max_array_dim.h        p_lazy_operand.h       \
   P++_headers.h          data_hash.h            inline_support.h       mdi_typedef.h          p_lazyarray.h          \
   descriptor.h           lazy_aggregate.h       \
   array_domain.h         dimension_macros.h     lazy_expression.h      optimization.h         partitioning.h         \
   arraylist.h            domain.h               lazy_operand.h         p_data_hash.h          partlist.h             where.h        \
   lazy_statement.h       p_descriptor.h         \
   comm_man.h             lazy_task.h            p_domain.h             maxProcessors.h


# We have to explicity install config.h.  We cannot place it in include_HEADERS
# because that list is only for files in the source tree.  (config.h is
# generated in the compile tree.)  BTNG
# DQ: (12/23/2000) Fixed this to permit compilation in the source tree which is 
#     the default tested configuration.
install-data-local:
	$(INSTALL_DATA) config.h $(DESTDIR)/$(includedir)
# install-data-local:
# 	p="config.h"; \
# 	if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
# 	$(INSTALL_DATA) $$d$$p $(DESTDIR)$(includedir)/$$p;

uninstall-local:
	rm -f $(DESTDIR)$(includedir)/config.h

# DQ: (12/25/2000) Remove this to fix bug when using Makefile.user.defs 
# (which puts -I's in different order and tries to read this file).  
# This is a generated file and does not belong here after "make distclean".
DISTCLEANFILES= config.h

clean-local:
	rm -f *.h
