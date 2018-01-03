## Process this file with automake to create Makefile.in

## Permit the MDI code to have specially setup options
## (typically no -g because it makes the libraries too large).
## This value will override the value specificed for the normal C code.
C_OPT    = @MDI_C_OPT@
C_DEBUG  = @MDI_C_DEBUG@

## include common setup (used by all A++/P++ Makefile.am files)
## include ../../../config/Makefile.setup

## include ../../config/Makefile.config.in
## where we place commonly used AutoMake code
## (used by the lower level Makefile.am files only).
include ../../../config/Makefile.config.in

## include all the relavant filenames (with and without relative paths)
## include FileNames.lst

INCLUDES = -I$(top_srcdir)/include

double_SRCS = \
 double_MDI_1.c double_MDI_2.c double_MDI_3.c double_MDI_4.c double_MDI_5.c \
 double_MDI_6.c double_MDI_7.c double_MDI_8.c double_MDI_9.c double_MDI_10.c \
 double_MDI_11.c double_MDI_12.c double_MDI_13.c double_MDI_14.c double_MDI_15.c \
 double_MDI_16.c double_MDI_17.c double_MDI_18.c double_MDI_19.c double_MDI_20.c \
 double_MDI_21.c double_MDI_22.c double_MDI_23.c double_MDI_24.c double_MDI_25.c \
 double_MDI_26.c double_MDI_27.c double_MDI_28.c double_MDI_29.c double_MDI_30.c \
 double_MDI_31.c double_MDI_32.c double_MDI_33.c double_MDI_34.c double_MDI_35.c \
 double_MDI_36.c double_MDI_37.c double_MDI_38.c double_MDI_39.c double_MDI_40.c \
 double_MDI_41.c double_MDI_42.c double_MDI_43.c double_MDI_44.c double_MDI_45.c \
 double_MDI_46.c double_MDI_47.c double_MDI_48.c double_MDI_49.c double_MDI_50.c \
 double_MDI_51.c double_MDI_52.c double_MDI_53.c double_MDI_54.c double_MDI_55.c \
 double_MDI_56.c double_MDI_57.c double_MDI_58.c double_MDI_59.c double_MDI_60.c \
 double_MDI_61.c double_MDI_62.c double_MDI_63.c double_MDI_64.c double_MDI_65.c

float_SRCS  = \
 float_MDI_1.c float_MDI_2.c float_MDI_3.c float_MDI_4.c float_MDI_5.c \
 float_MDI_6.c float_MDI_7.c float_MDI_8.c float_MDI_9.c float_MDI_10.c \
 float_MDI_11.c float_MDI_12.c float_MDI_13.c float_MDI_14.c float_MDI_15.c \
 float_MDI_16.c float_MDI_17.c float_MDI_18.c float_MDI_19.c float_MDI_20.c \
 float_MDI_21.c float_MDI_22.c float_MDI_23.c float_MDI_24.c float_MDI_25.c \
 float_MDI_26.c float_MDI_27.c float_MDI_28.c float_MDI_29.c float_MDI_30.c \
 float_MDI_31.c float_MDI_32.c float_MDI_33.c float_MDI_34.c float_MDI_35.c \
 float_MDI_36.c float_MDI_37.c float_MDI_38.c float_MDI_39.c float_MDI_40.c \
 float_MDI_41.c float_MDI_42.c float_MDI_43.c float_MDI_44.c float_MDI_45.c \
 float_MDI_46.c float_MDI_47.c float_MDI_48.c float_MDI_49.c float_MDI_50.c \
 float_MDI_51.c float_MDI_52.c float_MDI_53.c float_MDI_54.c float_MDI_55.c \
 float_MDI_56.c float_MDI_57.c float_MDI_58.c float_MDI_59.c float_MDI_60.c \
 float_MDI_61.c float_MDI_62.c float_MDI_63.c float_MDI_64.c float_MDI_65.c

int_SRCS    = \
 int_MDI_1.c int_MDI_2.c int_MDI_3.c int_MDI_4.c int_MDI_5.c \
 int_MDI_6.c int_MDI_7.c int_MDI_8.c int_MDI_9.c int_MDI_10.c \
 int_MDI_11.c int_MDI_12.c int_MDI_13.c int_MDI_14.c int_MDI_15.c \
 int_MDI_16.c int_MDI_17.c int_MDI_18.c int_MDI_19.c int_MDI_20.c \
 int_MDI_21.c int_MDI_22.c int_MDI_23.c int_MDI_24.c int_MDI_25.c \
 int_MDI_26.c int_MDI_27.c int_MDI_28.c int_MDI_29.c int_MDI_30.c \
 int_MDI_31.c int_MDI_32.c int_MDI_33.c int_MDI_34.c int_MDI_35.c \
 int_MDI_36.c int_MDI_37.c int_MDI_38.c int_MDI_39.c int_MDI_40.c \
 int_MDI_41.c int_MDI_42.c int_MDI_43.c int_MDI_44.c int_MDI_45.c \
 int_MDI_46.c int_MDI_47.c int_MDI_48.c int_MDI_49.c int_MDI_50.c \
 int_MDI_51.c int_MDI_52.c int_MDI_53.c int_MDI_54.c int_MDI_55.c \
 int_MDI_56.c int_MDI_57.c int_MDI_58.c int_MDI_59.c int_MDI_60.c \
 int_MDI_61.c int_MDI_62.c int_MDI_63.c int_MDI_64.c int_MDI_65.c \
 int_MDI_66.c int_MDI_67.c int_MDI_68.c int_MDI_69.c int_MDI_70.c \
 int_MDI_71.c int_MDI_72.c int_MDI_73.c int_MDI_74.c

libMDI_a_SOURCES = common_func.c $(double_SRCS) $(float_SRCS) $(int_SRCS)

noinst_LIBRARIES = libMDI.a

noinst_HEADERS = array_domain.h constants.h machine.h

## include dependency information
include $(srcdir)/Makefile.depend

## We don't have to include options since they will automatically be found in the 
## the config.h file (read automatically).  We use the makedepend program here
## because it seems to be installed on all the machines that we have checked.
## all-local: make_depend_run
## make_depend_run: depend
depend:
#	$(MAKE_DEPEND) $(MAKE_DEPEND_OPTIONS) $(MDI_SRCS) $(MAKE_DEPEND_OUTPUT)
	$(MAKE) -f makefile.build depend
#	touch make_depend_run

# A way to locally modify the C++ and C options for the Makefile built in this directory
# AM_CXXFLAGS = -g
# AM_CFLAGS   = -g

# EXTRA_DIST = FileNames.lst Makefile.depend
EXTRA_DIST = Makefile.depend

clean-local:
	$(clean_shared_libs_and_templates)
