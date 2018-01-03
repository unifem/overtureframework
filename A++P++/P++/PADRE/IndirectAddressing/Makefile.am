include ../config/Makefile.override.install.includeHEADERS

# This is required for some rules (e.g. all-recursive, and all in some directories)
# It is defined in A++P++/config/Makefile.config.in (but this Makefile.am does not
# include that file).
MAKE=gmake

ACLOCAL = @ACLOCAL@ -I ../config
# Note: make sure aclocal options match those in build.

CXXFLAGS      = @CXXFLAGS@ $(CXX_DEBUG) $(CXX_OPTIONS) $(CXX_WARNINGS) $(CXX_DL_COMPILE_FLAGS) $(CXX_TEMPLATES)
# CFLAGS        = @CFLAGS@ $(C_DEBUG) $(C_OPTIONS) $(C_OPT) $(C_WARNINGS) $(C_DL_FLAGS) $(C_DL_COMPILE_FLAGS)

# EXTRA_PROGAMS = doc

# bin_PROGRAMS = CommSchedTest2D

##CommSchedTest2D_SOURCES= \
#	CommSchedTest2D.C \
#	test10.C \
#	test11.C \
#	test12.C \
#	test14.C \
#	test15.C \
#	test16.C
bug5_objs = bug5.o

bug5: $(bug5_objs)
	$(CXXLINK) -o bug5 $(bug5_objs) -L/usr/opt/MPI190/lib -L../../src/ -rpath ../../src -lPpp -lPpp_static -lPpp -lmpi -lm -lc


CommSchedTest2D_depends = \
	CommunicationSchedule.h \
	CommunicationSchedule.C \
	CommunicationScheduler.h \
	CommunicationScheduler.C

CommSchedTest2D_objs= \
	CommSchedTest2D.o \
	test10.o \
	test11.o \
	test12.o \
	test14.o \
	test15.o \
	test16.o \
	test20.o \
	test25.o \
	test26.o \
	test27.o

CommSchedTest2D: $(CommSchedTest2D_objs) $(CommSchedTest2D_depends)
	$(CXXLINK) -o CommSchedTest2D $(CommSchedTest2D_objs) -L/usr/opt/MPI190/lib -L../../src/ -rpath ../../src -lPpp -lPpp_static -lPpp -lmpi -lm -lc

libIndirectAddressing_a_SOURCES = \
     CommunicationScheduleRecord.C \
     ReferenceCounting.C \
     MPI_wrappers.C

LDADD = $(PPP_PATH_TO_SHARED_LIB) $(PXX_RPATH) -lPpp_static

noinst_LIBRARIES = libIndirectAddressing.a

# All PADRE/PARTI header files.
include_HEADERS = \
     CommunicationScheduleRecord.h \
     ReferenceCountedTypes.h  \
     ReferenceTypes.h \
     CommunicationSchedule.h \
     CommunicationScheduler.h \
     ReferenceCounting.h

AM_CXXFLAGS= -g

install-data-local:
##	$(INSTALL_DATA) $(DESTDIR) $(includedir)

uninstall-local:
##	rm -f $(DESTDIR)$(includedir)/PARTI_config.h


dist-hook:
	ln -s ../../../STL-link $(distdir)













