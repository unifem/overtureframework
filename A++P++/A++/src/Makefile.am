## Process this file with automake to create Makefile.in

## include common setup (used by all A++/P++ Makefile.am files)
## include ../../config/Makefile.setup

## EXTRA_DIST = BUGS README-alpha
SUBDIRS = array_src OPERATORS MDI

## include $(top_srcdir)/config/Makefile.config.in
## where we place commonly used AutoMake code
## (used by the lower level Makefile.am files only).
include ../../config/Makefile.config.in

## Include all the file names from the array_src, OPERATORS, and MDI directories
## include FileNames.lst

## Read the dynamic library extension 
SO_EXT = @SHARED_LIB_EXTENSION@
CXX_DL = @CXX_DYNAMIC_LINKER@
CXX_DL_LINK_FLAGS = @CXX_DL_LINK_FLAGS@

# lib_LIBRARIES = libApp.a libApp_static.a
# noinst_LIBRARIES = libApp_static.a

# Build a library as a target to force instantiation of all templates (not needed for A++)
# noinst_LIBRARIES = libApp_nosublibraries.a
lib_LIBRARIES = libApp.a libApp_static.a

# Not sure if we need this!!!
STATIC_LINKER_FLAGS = @STATIC_LINKER_FLAGS@

# Rules for the regular library (without sublibraries).
# libApp_nosublibraries_a_SOURCES =
# libApp_nosublibraries_a_LIBADD = $(APP_ARRAY_OBJS_with_path) $(OPERATOR_OBJS_with_path) $(MDI_OBJS_with_path)
ALL_OBJS = \
     array_src/array_notemplates.o \
     array_src/descriptor_notemplates.o \
     array_src/index_operator_notemplates.o \
     array_src/new_operators_notemplates.o \
     array_src/abstract_op.o \
     array_src/comm_man.o \
     array_src/domain.o \
     array_src/descriptor_cache.o \
     array_src/error_checking.o \
     array_src/data_hash.o \
     array_src/index.o \
     array_src/lazy_aggregate.o \
     array_src/lazy_expression.o \
     array_src/lazy_operand.o \
     array_src/lazy_statement.o \
     array_src/lazy_task.o \
     array_src/lazy_taskset.o \
     array_src/lazyarray.o \
     array_src/optimization.o \
     array_src/partitioning.o \
     array_src/where.o \
     OPERATORS/*.o \
     MDI/*.o

# libApp_nosublibraries_a_LIBADD = $(ALL_OBJS)

## We require explicit rules here so that libraries can be built with
## anything other than "ar cru".  It turns out that the "cru" option
## is explicit within the automake perl script so the only way to
## build a library with anything but "ar cru" is to make these explicit rule.
## Redefining $(AR) is not sufficient since the options "cru" will remain.
## Note that C++ compiler must often be used to build the libraries since
## this is the only way to find the template instantiations.
#
# Rules for the static symbols library.
libApp_static_a_SOURCES = 
## libApp_static_a_LIBADD = $(STATIC_ARRAY_OBJS_with_path)
libApp_static_a_LIBADD = array_src/static_initialization.o
# libApp_static.a: libApp.a $(libApp_static_a_OBJECTS) $(libApp_static_a_LIBADD)
libApp_static.a: $(libApp_static_a_OBJECTS) $(libApp_static_a_LIBADD)
	-rm -f libApp_static.a
	$(CXX_STATIC_LIB_UPDATE) libApp_static.a $(libApp_static_a_OBJECTS) $(libApp_static_a_LIBADD)
	$(RANLIB) libApp_static.a

libApp_a_SOURCES =
libApp_a_LIBADD = $(ALL_OBJS)
libApp.a: $(ALL_OBJS)
	@echo Building $@, not using automake rule.
	$(CXX_STATIC_LIB_UPDATE) $@ $(ALL_OBJS)
	$(RANLIB) $@
if IF_BUILD_SHARED_LIBS
	$(CXX_SHARED_LIB_UPDATE) libApp.$(SO_EXT) $(ALL_OBJS)
endif

install-exec-local:
if IF_BUILD_SHARED_LIBS
	$(INSTALL_DATA) libApp.$(SO_EXT) $(DESTDIR)$(libdir)
endif

uninstall-local:
if IF_BUILD_SHARED_LIBS
	rm -f $(DESTDIR)$(libdir)/libApp.$(SO_EXT)
endif

####################################################
## OLD MAKE RULES THAT I WANT TO SAVE FOR A WHILE ##
####################################################

# libApp_nosublibraries.a: $(libApp_nosublibraries_a_OBJECTS) $(libApp_nosublibraries_a_LIBADD)
#	@echo Building $@, not using automake rule.
#	$(CXX_STATIC_LIB_UPDATE) $@ $(libApp_nosublibraries_a_OBJECTS) $(libApp_nosublibraries_a_LIBADD)
#	$(RANLIB) $@

# Here we build a tmp directory "tmp" to hold the object files
# that will be used to build the static and dynamic libraries
#libApp.a: libApp_nosublibraries.a $(AXX_SUBLIBS)
#	@echo "Making $@"	;\
#	tmpdir=tmp	;\
#	rm -rf $$tmpdir	;\
#	mkdir $$tmpdir	;\
#	for i in libApp_nosublibraries.a $(AXX_SUBLIBS)	;\
#	do \
#	  echo "Unpacking $$i";	\
#	  (cd $$tmpdir; $(AR_EXTRACT) ../$$i; chmod u+w *; chmod u+r *)	;\
#	done	;\
#	echo "Updating $@";	\
#	echo $(CXX_STATIC_LIB_UPDATE) $@ $$tmpdir/*	;\
#	$(CXX_STATIC_LIB_UPDATE) $@ $$tmpdir/*	;\
#	test -f $@ || exit 1	;\
#	$(RANLIB) $@	;\
#	: rm -rf $$tmpdir	;

# if IF_BUILD_SHARED_LIBS
# Here a directory is used to store the object files used to build the library.
# This directory should NOT be removed since on the SUN the dynamic library will
# reference the original object file in the direcotry to find the path to the source
# code and thus have the line number information for the debugger.  
# Note: dbx will cache the symbol information so this can be a tricky point to debug.
# libApp.$(SO_EXT): libApp.a
#	@echo "Making $@"	;\
#	tmpdir=tmp	;\
#	: rm -rf $$tmpdir	;\
#	mkdir -p $$tmpdir	;\
#	for i in libApp.a	;\
#	do \
#	  echo "Unpacking $$i";	\
#	  (cd $$tmpdir; $(AR_EXTRACT) ../$$i; chmod u+w *; chmod u+r *)	;\
#	done	;\
#	echo "Updating $@";	\
#	echo $(CXX_SHARED_LIB_UPDATE) $@ $$tmpdir/*.o	;\
#	$(CXX_SHARED_LIB_UPDATE) $@ $$tmpdir/*.o	;\
#	test -f $@ || exit 1	;\
#	: rm -rf $$tmpdir	;\
#	: touch libApp.a libApp_nosublibraries.a;\
#	: touch libApp.$(SO_EXT);
# endif


# After making the libraries, make the all-inclusive App library.
# all-local:
#	$(MAKE) libApp.a
# if IF_BUILD_SHARED_LIBS
#	$(MAKE) libApp.$(SO_EXT)
# endif

## This rule runs $(MAKE) depend in all subdirectories
alt_depend:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done; test -z "$$fail"

depend:
	cd array_src; $(MAKE) depend;
	cd OPERATORS; $(MAKE) depend;
	cd MDI; $(MAKE) depend;

# install-exec-local:
#	$(mkinstalldirs) $(DESTDIR)$(libdir)
#	$(INSTALL_DATA) libApp.a $(DESTDIR)$(libdir)
# if IF_BUILD_SHARED_LIBS
#	$(INSTALL_DATA) libApp.$(SO_EXT) $(DESTDIR)$(libdir)
# endif

# uninstall-local:
#	rm -f $(DESTDIR)$(libdir)/libApp.a
#	rm -f $(DESTDIR)$(libdir)/libApp.$(SO_EXT)

# Save the file name list into the distribution (not required anymore)
# EXTRA_DIST = FileNames.lst

# local clean rule
clean-local:
	rm -f libApp.a libApp_static.a
	$(clean_shared_libs_and_templates)
	rm -rf tmp

