## Process this file with automake to create Makefile.in

## include ../../config/Makefile.setup
include ../../config/Makefile.config.in

SUBDIRS = array_src OPERATORS MDI

## When we want to move the time consuming stuff to this level directory
## we can have it run only once rather than once per subdirectory (we can 
## use this as a way to make the configuration more efficient later).

## Read the dynamic library extension 
SO_EXT = @SHARED_LIB_EXTENSION@
CXX_DL = @CXX_DYNAMIC_LINKER@
CXX_DL_LINK_FLAGS = @CXX_DL_LINK_FLAGS@
STATIC_LINKER_FLAGS = @STATIC_LINKER_FLAGS@

## Specify the name of the library
# noinst_LIBRARIES = libPpp.a libPpp_static.a libPpp.$(SO_EXT)
# noinst_LIBRARIES = libPpp_static.a
lib_LIBRARIES = libPpp.a libPpp_static.a

# Exclude this to test compilation without PGSLIB (this is the only file 
# that requires PGSLIB and it is not required to compile P++ applications).
# array_src/indirect_addressing.o

Ppp_ARRAY_SRC_OBJS = \
     array_src/MPI_specific.o \
     array_src/allocate.o \
     array_src/arraylist.o \
     array_src/cases.o \
     array_src/conform_array_set.o \
     array_src/conform_enforce.o \
     array_src/map.o \
     array_src/p_array.o \
     array_src/p_abstract_op.o \
     array_src/p_descriptor.o \
     array_src/p_domain.o \
     array_src/p_error_checking.o \
     array_src/p_data_hash.o \
     array_src/p_index_operator.o \
     array_src/p_lazy_aggregate.o \
     array_src/p_lazy_operand.o \
     array_src/p_lazyarray.o \
     array_src/p_new_operators.o \
     array_src/partlist.o \
     array_src/array.o \
     array_src/descriptor.o \
     array_src/index_operator.o \
     array_src/new_operators.o \
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
     array_src/where.o

Ppp_OPERATOR_OBJS = \
     OPERATORS/pd_op_all.o \
     OPERATORS/pf_op_all.o \
     OPERATORS/pi_op_all.o \
     OPERATORS/d_op_all.o \
     OPERATORS/f_op_all.o \
     OPERATORS/i_op_all.o

double_OBJS = \
     MDI/double_MDI_1.o  MDI/double_MDI_2.o  MDI/double_MDI_3.o  MDI/double_MDI_4.o  MDI/double_MDI_5.o \
     MDI/double_MDI_6.o  MDI/double_MDI_7.o  MDI/double_MDI_8.o  MDI/double_MDI_9.o  MDI/double_MDI_10.o \
     MDI/double_MDI_11.o MDI/double_MDI_12.o MDI/double_MDI_13.o MDI/double_MDI_14.o MDI/double_MDI_15.o \
     MDI/double_MDI_16.o MDI/double_MDI_17.o MDI/double_MDI_18.o MDI/double_MDI_19.o MDI/double_MDI_20.o \
     MDI/double_MDI_21.o MDI/double_MDI_22.o MDI/double_MDI_23.o MDI/double_MDI_24.o MDI/double_MDI_25.o \
     MDI/double_MDI_26.o MDI/double_MDI_27.o MDI/double_MDI_28.o MDI/double_MDI_29.o MDI/double_MDI_30.o \
     MDI/double_MDI_31.o MDI/double_MDI_32.o MDI/double_MDI_33.o MDI/double_MDI_34.o MDI/double_MDI_35.o \
     MDI/double_MDI_36.o MDI/double_MDI_37.o MDI/double_MDI_38.o MDI/double_MDI_39.o MDI/double_MDI_40.o \
     MDI/double_MDI_41.o MDI/double_MDI_42.o MDI/double_MDI_43.o MDI/double_MDI_44.o MDI/double_MDI_45.o \
     MDI/double_MDI_46.o MDI/double_MDI_47.o MDI/double_MDI_48.o MDI/double_MDI_49.o MDI/double_MDI_50.o \
     MDI/double_MDI_51.o MDI/double_MDI_52.o MDI/double_MDI_53.o MDI/double_MDI_54.o MDI/double_MDI_55.o \
     MDI/double_MDI_56.o MDI/double_MDI_57.o MDI/double_MDI_58.o MDI/double_MDI_59.o MDI/double_MDI_60.o \
     MDI/double_MDI_61.o MDI/double_MDI_62.o MDI/double_MDI_63.o MDI/double_MDI_64.o MDI/double_MDI_65.o

float_OBJS  = \
     MDI/float_MDI_1.o  MDI/float_MDI_2.o  MDI/float_MDI_3.o  MDI/float_MDI_4.o  MDI/float_MDI_5.o \
     MDI/float_MDI_6.o  MDI/float_MDI_7.o  MDI/float_MDI_8.o  MDI/float_MDI_9.o  MDI/float_MDI_10.o \
     MDI/float_MDI_11.o MDI/float_MDI_12.o MDI/float_MDI_13.o MDI/float_MDI_14.o MDI/float_MDI_15.o \
     MDI/float_MDI_16.o MDI/float_MDI_17.o MDI/float_MDI_18.o MDI/float_MDI_19.o MDI/float_MDI_20.o \
     MDI/float_MDI_21.o MDI/float_MDI_22.o MDI/float_MDI_23.o MDI/float_MDI_24.o MDI/float_MDI_25.o \
     MDI/float_MDI_26.o MDI/float_MDI_27.o MDI/float_MDI_28.o MDI/float_MDI_29.o MDI/float_MDI_30.o \
     MDI/float_MDI_31.o MDI/float_MDI_32.o MDI/float_MDI_33.o MDI/float_MDI_34.o MDI/float_MDI_35.o \
     MDI/float_MDI_36.o MDI/float_MDI_37.o MDI/float_MDI_38.o MDI/float_MDI_39.o MDI/float_MDI_40.o \
     MDI/float_MDI_41.o MDI/float_MDI_42.o MDI/float_MDI_43.o MDI/float_MDI_44.o MDI/float_MDI_45.o \
     MDI/float_MDI_46.o MDI/float_MDI_47.o MDI/float_MDI_48.o MDI/float_MDI_49.o MDI/float_MDI_50.o \
     MDI/float_MDI_51.o MDI/float_MDI_52.o MDI/float_MDI_53.o MDI/float_MDI_54.o MDI/float_MDI_55.o \
     MDI/float_MDI_56.o MDI/float_MDI_57.o MDI/float_MDI_58.o MDI/float_MDI_59.o MDI/float_MDI_60.o \
     MDI/float_MDI_61.o MDI/float_MDI_62.o MDI/float_MDI_63.o MDI/float_MDI_64.o MDI/float_MDI_65.o

int_OBJS    = \
     MDI/int_MDI_1.o  MDI/int_MDI_2.o  MDI/int_MDI_3.o  MDI/int_MDI_4.o  MDI/int_MDI_5.o \
     MDI/int_MDI_6.o  MDI/int_MDI_7.o  MDI/int_MDI_8.o  MDI/int_MDI_9.o  MDI/int_MDI_10.o \
     MDI/int_MDI_11.o MDI/int_MDI_12.o MDI/int_MDI_13.o MDI/int_MDI_14.o MDI/int_MDI_15.o \
     MDI/int_MDI_16.o MDI/int_MDI_17.o MDI/int_MDI_18.o MDI/int_MDI_19.o MDI/int_MDI_20.o \
     MDI/int_MDI_21.o MDI/int_MDI_22.o MDI/int_MDI_23.o MDI/int_MDI_24.o MDI/int_MDI_25.o \
     MDI/int_MDI_26.o MDI/int_MDI_27.o MDI/int_MDI_28.o MDI/int_MDI_29.o MDI/int_MDI_30.o \
     MDI/int_MDI_31.o MDI/int_MDI_32.o MDI/int_MDI_33.o MDI/int_MDI_34.o MDI/int_MDI_35.o \
     MDI/int_MDI_36.o MDI/int_MDI_37.o MDI/int_MDI_38.o MDI/int_MDI_39.o MDI/int_MDI_40.o \
     MDI/int_MDI_41.o MDI/int_MDI_42.o MDI/int_MDI_43.o MDI/int_MDI_44.o MDI/int_MDI_45.o \
     MDI/int_MDI_46.o MDI/int_MDI_47.o MDI/int_MDI_48.o MDI/int_MDI_49.o MDI/int_MDI_50.o \
     MDI/int_MDI_51.o MDI/int_MDI_52.o MDI/int_MDI_53.o MDI/int_MDI_54.o MDI/int_MDI_55.o \
     MDI/int_MDI_56.o MDI/int_MDI_57.o MDI/int_MDI_58.o MDI/int_MDI_59.o MDI/int_MDI_60.o \
     MDI/int_MDI_61.o MDI/int_MDI_62.o MDI/int_MDI_63.o MDI/int_MDI_64.o MDI/int_MDI_65.o \
     MDI/int_MDI_66.o MDI/int_MDI_67.o MDI/int_MDI_68.o MDI/int_MDI_69.o MDI/int_MDI_70.o \
     MDI/int_MDI_71.o MDI/int_MDI_72.o MDI/int_MDI_73.o MDI/int_MDI_74.o

# Ppp_MDI_OBJS = MDI/common_func.o $(double_OBJS) $(float_OBJS) $(int_OBJS)

# This is far simplier to specify than all those objects individually!
Ppp_MDI_OBJS = MDI/*.o

## Optionally defined variables for using Parti.
if ENABLE_Parti
  ## Parti objects
     PADRE_PARTI_OBJS = \
        ../PARTI/Extras.o \
        ../PARTI/bsparti.o \
        ../PARTI/exchsched.o \
        ../PARTI/hash.o \
        ../PARTI/print.o \
        ../PARTI/utils.o \
        ../PARTI/List.o \
        ../PARTI/datamove.o \
        ../PARTI/fort_wrap.o \
        ../PARTI/helper.o \
        ../PARTI/subsched.o \
        ../PARTI/addition_by_quinlan.o \
        ../PARTI/decomp.o \
        ../PARTI/getprocnum.o \
        ../PARTI/mpi-port.o
endif

## Optional use of IndirectAddressing.
# if ENABLE_IndirectAddressing
## Indirect Addressing objects
#     PADRE_IndirectAddressing_OBJS = \
#        ../PADRE/IndirectAddressing/CommunicationScheduleRecord.o \
#        ../PADRE/IndirectAddressing/ReferenceCounting.o \
#        ../PADRE/IndirectAddressing/MPI_wrappers.o
#else
     PADRE_IndirectAddressing_OBJS =
#endif

## PGSLIB objects
PADRE_PGSLIB_OBJS = \
     ../PADRE/PGSLIB/attempt-receive-double.o \
     ../PADRE/PGSLIB/constrained-send-rcv-int.o \
     ../PADRE/PGSLIB/gather-real.o \
     ../PADRE/PGSLIB/random-send-rcv-double.o \
     ../PADRE/PGSLIB/scatter-int.o \
     ../PADRE/PGSLIB/attempt-receive-int.o \
     ../PADRE/PGSLIB/constrained-send-rcv-real.o \
     ../PADRE/PGSLIB/random-send-rcv-int.o \
     ../PADRE/PGSLIB/scatter-real.o \
     ../PADRE/PGSLIB/attempt-receive-real.o \
     ../PADRE/PGSLIB/gs-setup-c.o \
     ../PADRE/PGSLIB/random-send-rcv-real.o \
     ../PADRE/PGSLIB/gather-double.o \
     ../PADRE/PGSLIB/gs-util-c.o \
     ../PADRE/PGSLIB/utility-c.o \
     ../PADRE/PGSLIB/constrained-send-rcv-double.o \
     ../PADRE/PGSLIB/gather-int.o \
     ../PADRE/PGSLIB/pgshash.o \
     ../PADRE/PGSLIB/scatter-double.o

## PADRE source files
PADRE_SRC_OBJS = \
     ../PADRE/src/PADRE_CommonInterface.o \
     ../PADRE/src/PADRE_Global.o \
     ../PADRE/src/PADRE_Descriptor.o \
     ../PADRE/src/PADRE_Representation.o \
     ../PADRE/src/PADRE_Distribution.o \
     ../PADRE/src/PADRE_Parti_Template.o \
     ../PADRE/src/PADRE_Parti_NonTemplate.o

PADRE_OBJS = $(PADRE_PARTI_OBJS) $(PADRE_PGSLIB_OBJS) $(PADRE_IndirectAddressing_OBJS) $(PADRE_SRC_OBJS)

PARTI_OBJS = \
     ../PARTI/Extras.o \
     ../PARTI/bsparti.o \
     ../PARTI/exchsched.o \
     ../PARTI/hash.o \
     ../PARTI/print.o \
     ../PARTI/utils.o \
     ../PARTI/List.o \
     ../PARTI/datamove.o \
     ../PARTI/fort_wrap.o \
     ../PARTI/helper.o \
     ../PARTI/subsched.o \
     ../PARTI/addition_by_quinlan.o \
     ../PARTI/decomp.o \
     ../PARTI/getprocnum.o \
     ../PARTI/mpi-port.o

## Use object files from PADRE
if COMPILE_PADRE_DIRECTORY
     ## Use object files from PADRE (add in the template objects too!)
     PARALLEL_DISTRIBUTION_SUPPORT_OBJS = $(PADRE_OBJS)
else
     ## Use object files from PADRE (notice that we leave out any template objects since there should be none)
     PARALLEL_DISTRIBUTION_SUPPORT_OBJS = $(PARTI_OBJS)
endif

# libPpp_a_LIBADD = $(Ppp_ARRAY_SRC_OBJS) $(Ppp_OPERATOR_OBJS) $(Ppp_MDI_OBJS) $(PARALLEL_DISTRIBUTION_SUPPORT_OBJS)
ALL_OBJS = $(Ppp_ARRAY_SRC_OBJS) $(Ppp_OPERATOR_OBJS) $(Ppp_MDI_OBJS) $(PARALLEL_DISTRIBUTION_SUPPORT_OBJS)
libPpp_a_LIBADD = $(ALL_OBJS)

# Need to define the SOURCES variable even though there are no local sources for this library
libPpp_a_SOURCES =

## Rule to build static library
if COMPILE_PADRE_DIRECTORY
# If we are using PADRE then we want to force the generation of all templates
# before building the library (so that all templates will be present in the final libPpp.a).
libPppForceTemplateGeneration.a: $(ALL_OBJS)
	-rm -f $@
	$(CXX_STATIC_LIB_UPDATE) $@ $(ALL_OBJS)
	test -f $@ || exit 1

# Now build the final static library (shared library is build if required)
libPpp.a: libPppForceTemplateGeneration.a $(ALL_OBJS)
	-rm -f $@
	$(CXX_STATIC_LIB_UPDATE) $@ $(ALL_OBJS) $(CXX_TEMPLATE_OBJECT_FILES)
if IF_PPP_BUILD_SHARED_LIBS
	$(CXX_SHARED_LIB_UPDATE) libPpp.$(SO_EXT) $(ALL_OBJS) $(CXX_TEMPLATE_OBJECT_FILES)
endif
	test -f $@ || exit 1
else
# Don't include the template objects if we are not using PADRE
libPpp.a: $(ALL_OBJS)
	-rm -f $@
	$(CXX_STATIC_LIB_UPDATE) $@ $(ALL_OBJS)
if IF_PPP_BUILD_SHARED_LIBS
	$(CXX_SHARED_LIB_UPDATE) libPpp.$(SO_EXT) $(ALL_OBJS)
endif
	test -f $@ || exit 1
endif

# Rule for building shared P++ library
# if IF_PPP_BUILD_SHARED_LIBS
# libPpp.$(SO_EXT): libPpp.a $(libPpp_a_OBJECTS) $(libPpp_a_LIBADD)
#	$(CXX_SHARED_LIB_UPDATE) $@ $(libPpp_a_OBJECTS) $(libPpp_a_LIBADD)
# endif

# Need to define the SOURCES variable even though there are no local sources for this library
libPpp_static_a_SOURCES =

## No local P++ objects for static library
# libPpp_static_a_OBJECTS =
libPpp_static_a_LIBADD = array_src/static_initialization.o
libPpp_static.a: $(libPpp_static_a_OBJECTS) $(libPpp_static_a_LIBADD)
	-rm -f $@ ;\
	$(CXX_STATIC_LIB_UPDATE) $@ $(libPpp_static_a_OBJECTS) $(libPpp_static_a_LIBADD) ;\
	test -f $@ || exit 1 ;

install-exec-local:
if IF_PPP_BUILD_SHARED_LIBS
	$(INSTALL_DATA) libPpp.$(SO_EXT) $(DESTDIR)$(libdir)
endif

uninstall-local:
if IF_PPP_BUILD_SHARED_LIBS
	rm -f $(DESTDIR)$(libdir)/libPpp.$(SO_EXT)
endif

clean-local:
	rm -f libPpp.a libPppForceTemplateGeneration.a  libPpp_static.a
	$(clean_shared_libs_and_templates)

depend:
	cd array_src; $(MAKE) depend;
	cd OPERATORS; $(MAKE) depend;
	cd MDI; $(MAKE) depend;













#################################################
## OLD MAKE Rules that I want to save for now! ##
#################################################

# if IF_PPP_BUILD_SHARED_LIBS
## Here is the rule to build the shared library.
## It uses the linker, flags, file extension
## defined in the APP_PPP_common.m4 macro.
# libPpp.$(SO_EXT): libPpp.a
#	@echo "Making $@"	;\
#	$(CXX_SHARED_LIB_UPDATE) ../$@ *.o ;\
#	test -f $@ || exit 1	;
# endif

#########################################################################
#########################################################################
###  Makefile source code to unpack libraries and repack the objects  ###
#########################################################################
#########################################################################

# This code was Brian Gunney's approach to handling the construction of
# libraries from other projects without explicit lists of file names.
# I (DQ) am not so sure it has been a success.  It is clever, but takes
# a long time and the contributes to a long build process for PADRE
# development.

#all-local:
## Make the libraries not using automake rules.
#	$(MAKE) libPpp_nosublibraries.a
#	$(MAKE) libPpp_static.a
#	$(MAKE) libPpp.a
#if IF_PPP_BUILD_SHARED_LIBS
#	$(MAKE) libPpp.$(SO_EXT)
#endif

# Rules for the static symbols library.
#lower_Ppp_static_libs = array_src/libPppSource_static.a
#libPpp_static.a: $(lower_Ppp_static_libs)
#	@echo "Making $@"	;\
#	tmpdir=tmp-$$$$	;\
#	rm -rf $$tmpdir	;\
#	mkdir $$tmpdir	;\
#	for i in $(lower_Ppp_static_libs)	;\
#	do \
#	  echo "Unpacking $$i";	\
#	  (cd $$tmpdir; $(AR_EXTRACT) ../$$i; chmod u+w *; chmod u+r *)	;\
#	done	;\
#	echo "Updating $@";	\
#	( cd $$tmpdir && echo $(CXX_STATIC_LIB_UPDATE) ../$@ * )	;\
#	( cd $$tmpdir && $(CXX_STATIC_LIB_UPDATE) ../$@ * )	;\
#	test -f $@ || exit 1	;\
#	$(RANLIB) $@	;\
#	rm -rf $$tmpdir	;

# Rules for the regular library (without sublibraries).
#lower_Ppp_libs = array_src/libPppSource.a OPERATORS/libPppOperators.a MDI/libMDI.a
#libPpp_nosublibraries.a: $(lower_Ppp_libs)
#	@echo "Making $@"	;\
#	tmpdir=tmp-$$$$	;\
#	rm -rf $$tmpdir	;\
#	mkdir $$tmpdir	;\
#	for i in $(lower_Ppp_libs)	;\
#	do \
#	  echo "Unpacking $$i";	\
#	  (cd $$tmpdir; $(AR_EXTRACT) ../$$i; chmod u+w *; chmod u+r *)	;\
#	done	;\
#	echo "Copying template object files to $$tmpdir"	;\
#	test -n "$(CXX_TEMPLATE_OBJECT_FILES)" &&	\
#        test -r $(CXX_TEMPLATE_OBJECT_FILES) &&	\
#        cp -pf $(CXX_TEMPLATE_OBJECT_FILES) $$tmpdir	;\
#	echo "Updating $@";	\
#	( cd $$tmpdir && echo $(CXX_STATIC_LIB_UPDATE) ../$@ * )	;\
#	( cd $$tmpdir && $(CXX_STATIC_LIB_UPDATE) ../$@ * )	;\
#	test -f $@ || exit 1	;\
#	$(RANLIB) $@	;\
#	rm -rf $$tmpdir	;

# Rules for the all-inclusive libPpp.a library (with sublibraries).
# It includes all the objects from Ppp_nosublibraries and its sublibraries.
# This is done by extracting the sublibraries into a temporary directory
# and rebuilding from the extracted object files.
# Note: some environments add additional files to the library archive
# and set the permission for those files to zero, causing read problem
# when reusing the files.  That is the reason for the chmod command.

# Notes on unpacking of object files: We used to unpack the object files
# into a uniquely named temporary directory (tmp-$$$$), generate a new
# library from the unpacked objects then delete the temporary directory.
# But that caused problems for the Sun 4.2 compiler which could not find
# source files during debugging if the object files used to build the
# library files are deleted.  So now we preserve the temporary directory.
# And because we preserve them, we cannot use unique names (else there
# would be a new one each time we rebuild the library), so we use the
# generic name (tmp).  BTNG. 19Apr01.
#libPpp.a: libPpp_nosublibraries.a $(PXX_SUBLIBS)
#	@echo "Making $@"	;\
#	tmpdir=tmp	;\
#	rm -rf $$tmpdir	;\
#	mkdir -p $$tmpdir	;\
#	for i in libPpp_nosublibraries.a $(PXX_SUBLIBS)	;\
#	do \
#	  echo "Unpacking $$i";	\
#	  (cd $$tmpdir; $(AR_EXTRACT) ../$$i; chmod u+w *; chmod u+r *)	;\
#	done	;\
#	echo "Updating $@";	\
#	( cd $$tmpdir && echo $(CXX_STATIC_LIB_UPDATE) $@ * )	;\
#	( cd $$tmpdir && $(CXX_STATIC_LIB_UPDATE) ../$@ * )	;\
#	test -f $@ || exit 1	;\
#	$(RANLIB) $@	;\
#	: rm -rf $$tmpdir	;

#if IF_PPP_BUILD_SHARED_LIBS
## Here is the rule to build the shared library.
## It uses the linker, flags, file extension
## defined in the APP_PPP_common.m4 macro.

## Here a directory is used to store the object files used to build the library.
## This directory should NOT be removed since on the SUN the dynamic library will
## reference the original object file in the direcotry to find the path to the source
## code and thus have the line number information for the debugger.  
## Note: dbx will cache the symbol information so this can be a tricky point to debug.

#libPpp.$(SO_EXT): libPpp.a
#	@echo "Making $@"	;\
#	tmpdir=tmp	;\
#	: rm -rf $$tmpdir	;\
#	mkdir -p $$tmpdir	;\
#	for i in libPpp.a	;\
#	do \
#	  echo "Unpacking $$i";	\
#	  (cd $$tmpdir; $(AR_EXTRACT) ../$$i; chmod u+w *; chmod u+r *)	;\
#	done	;\
#	echo "Updating $@";	\
#	( cd $$tmpdir && echo $(CXX_SHARED_LIB_UPDATE) ../$@ *.o )	;\
#	( cd $$tmpdir && $(CXX_SHARED_LIB_UPDATE) ../$@ *.o )	;\
#	test -f $@ || exit 1	;\
#	: rm -rf $$tmpdir	;
#endif

## make an extension of the existing 'install' target to put a link to P++/include
## in the platform specific lib directory.
#install-exec-local:
#	$(mkinstalldirs) $(DESTDIR)$(libdir)
#	$(INSTALL_DATA) libPpp_static.a $(DESTDIR)$(libdir)
#	$(INSTALL_DATA) libPpp.a $(DESTDIR)$(libdir)
#if IF_PPP_BUILD_SHARED_LIBS
#	$(INSTALL_DATA) libPpp.$(SO_EXT) $(DESTDIR)$(libdir)
#endif

#uninstall-local:
#	rm -f $(DESTDIR)$(libdir)/libPpp_static.a
#	rm -f $(DESTDIR)$(libdir)/libPpp.a
#	rm -f $(DESTDIR)$(libdir)/libPpp.$(SO_EXT)












