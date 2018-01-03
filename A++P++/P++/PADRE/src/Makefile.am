## Process this file with automake to create Makefile.in

include ../../../config/Makefile.config.in
include ../config/Makefile.override.install.includeHEADERS

# This is required for some rules (e.g. all-recursive, and all in some directories)
# It is defined in A++P++/config/Makefile.config.in (but this Makefile.am does not
# include that file).
# MAKE=gmake

# CXXFLAGS      = @CXXFLAGS@ $(CXX_DEBUG) $(CXX_OPTIONS) $(CXX_OPT) $(CXX_WARNINGS) $(CXX_DL_COMPILE_FLAGS) $(CXX_TEMPLATES)
# CFLAGS        = @CFLAGS@ $(C_DEBUG) $(C_OPTIONS) $(C_OPT) $(C_WARNINGS) $(C_DL_COMPILE_FLAGS)

# Optionally defined variables for using GlobalArrays.
if ENABLE_GlobalArrays
     GlobalArrays_src = PADRE_GlobalArrays.C
endif

## Optionally defined variables for using Parti.
if ENABLE_Parti
     Parti_src = PADRE_Parti_Template.C PADRE_Parti_NonTemplate.C
     Parti_lib = ../../PARTI/libPARTI_Source.a

  ## Parti objects
     PADRE_PARTI_OBJS = \
        ../../PARTI/Extras.o \
        ../../PARTI/bsparti.o \
        ../../PARTI/exchsched.o \
        ../../PARTI/hash.o \
        ../../PARTI/print.o \
        ../../PARTI/utils.o \
        ../../PARTI/List.o \
        ../../PARTI/datamove.o \
        ../../PARTI/fort_wrap.o \
        ../../PARTI/helper.o \
        ../../PARTI/subsched.o \
        ../../PARTI/addition_by_quinlan.o \
        ../../PARTI/decomp.o \
        ../../PARTI/getprocnum.o \
        ../../PARTI/mpi-port.o
endif

## Optional use of IndirectAddressing.
if ENABLE_IndirectAddressing

## Indirect Addressing objects
     PADRE_IndirectAddressing_OBJS = \
        ../IndirectAddressing/CommunicationScheduleRecord.o \
        ../IndirectAddressing/ReferenceCounting.o \
        ../IndirectAddressing/MPI_wrappers.o
endif

## PGSLIB objects
PADRE_PGSLIB_OBJS = \
     ../PGSLIB/attempt-receive-double.o \
     ../PGSLIB/constrained-send-rcv-int.o \
     ../PGSLIB/gather-real.o \
     ../PGSLIB/random-send-rcv-double.o \
     ../PGSLIB/scatter-int.o \
     ../PGSLIB/attempt-receive-int.o \
     ../PGSLIB/constrained-send-rcv-real.o \
     ../PGSLIB/random-send-rcv-int.o \
     ../PGSLIB/scatter-real.o \
     ../PGSLIB/attempt-receive-real.o \
     ../PGSLIB/gs-setup-c.o \
     ../PGSLIB/random-send-rcv-real.o \
     ../PGSLIB/gather-double.o \
     ../PGSLIB/gs-util-c.o \
     ../PGSLIB/utility-c.o \
     ../PGSLIB/constrained-send-rcv-double.o \
     ../PGSLIB/gather-int.o \
     ../PGSLIB/pgshash.o \
     ../PGSLIB/scatter-double.o

# These were included redundently (which is an error when the library is built using KCC)
# PADRE_Parti_Template.C
# PADRE_Parti_NonTemplate.C

## PADRE source files
libPADRE_a_SOURCES = \
     PADRE_CommonInterface.C \
     PADRE_Global.C \
     PADRE_Descriptor.C \
     PADRE_Representation.C \
     PADRE_Distribution.C \
     $(Parti_src) $(GlobalArrays_src)

# Use object files from subdirectories
# libPADRE_a_LIBADD = $(PADRE_PARTI_OBJS) $(PADRE_PGSLIB_OBJS) $(PADRE_IndirectAddressing_OBJS)
libPADRE_a_LIBADD = $(PADRE_IndirectAddressing_OBJS)

# Specify the name of the library
noinst_LIBRARIES = libPADRE.a

libPADRE.a: $(libPADRE_a_OBJECTS) $(libPADRE_a_LIBADD)
	-rm -f $@
	$(CXX_STATIC_LIB_UPDATE) $@ $(libPADRE_a_OBJECTS) $(libPADRE_a_LIBADD)
	test -f $@ || exit 1

# noinst_LIBRARIES = libPADRE.a
#
## Make the PADRE_withsubs library.
## It includes all the objects from PADRE and its sublibraries.
## This is done by extracting the sublibraries into a temporary
## directory and rebuilding from the extracted object files.
#padre_lib_with_subs = libPADRE.a $(Parti_lib) ../PGSLIB/libPADRE_PGSLIB_Source.a
#libPADRE_withsubs.a: $(padre_lib_with_subs)
#	@echo "Making $@"	;\
#	tmpdir=tmp-$$$$	;\
#	rm -rf $$tmpdir	;\
#	mkdir $$tmpdir	;\
#	for i in $(padre_lib_with_subs)	;\
#	do \
#	  echo "Unpacking $$i";	\
#	  (cd $$tmpdir; $(AR_EXTRACT) ../$$i; chmod u+w *; chmod u+r *)	;\
#	done	;\
#	echo "Updating $@";	\
#	$(CXX_STATIC_LIB_UPDATE) $@ $$tmpdir/*	;\
#	$(RANLIB) $@	;\
#	rm -rf $$tmpdir	;
#
## DQ (1/1/2002): Soon we will not require this library
#all-local:
## After making the libraries, make the all-inclusive PADRE library.
#	$(MAKE) libPADRE_withsubs.a

clean-local:
	rm -f libPADRE_withsubs.a
	rm -rf Templates.DB cxx_repository SunWS_cache PI* ti_files ii_files


depend:
	$(MAKE) -f makefile.build depend
#	touch make_depend_run

template_source_to_install = \
     PADRE_Descriptor.C \
     PADRE_Representation.C \
     PADRE_Distribution.C \
     PADRE_Parti_Template.C

include_HEADERS = \
 User_Local_Descriptor.h  \
 PADRE.h                 PADRE_CommonInterface.h   PADRE_Descriptor.h \
 PADRE_Distribution.h    PADRE_Global.h            PADRE_Incomplete_Domain.h   PADRE_Kelp.h \
 PADRE_Kelp_Descriptor.h PADRE_Kelp_Distribution.h PADRE_Kelp_Representation.h PADRE_Ordering.h \
 PADRE_PSGLib.h          PADRE_Parti.h             PADRE_Representation.h      PADRE_Structured_Domain.h	\
 PADRE_SublibraryNames.h PADRE_GlobalArrays.h	   PADRE_macros.h	       PADRE_forward-declarations.h \
 $(template_source_to_install)










