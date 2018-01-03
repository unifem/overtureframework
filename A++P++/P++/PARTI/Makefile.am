# include ../config/Makefile.override.install.includeHEADERS
include ../../config/Makefile.config.in

# This is required for some rules (e.g. all-recursive, and all in some directories)
# It is defined in A++P++/config/Makefile.config.in (but this Makefile.am does not
# include that file).
# MAKE=gmake

ACLOCAL = @ACLOCAL@ -I ../config
# Note: make sure aclocal options match those in build.

# CFLAGS        = @CFLAGS@ $(C_DEBUG) $(C_OPTIONS) $(C_OPT) $(C_WARNINGS) $(C_DL_FLAGS) $(C_DL_COMPILE_FLAGS)

libPARTI_Source_a_SOURCES = \
     Extras.c \
     bsparti.c \
     exchsched.c \
     hash.c \
     print.c \
     utils.c \
     List.c \
     datamove.c \
     fort_wrap.c \
     helper.c \
     subsched.c \
     addition_by_quinlan.c \
     decomp.c \
     getprocnum.c \
     mpi-port.c

noinst_LIBRARIES = libPARTI_Source.a

# All PADRE/PARTI header files
include_HEADERS = List.h bsparti.h hash.h helper.h mpi-port.h port.h print.h utils.h
# noinst_HEADERS = List.h bsparti.h hash.h helper.h mpi-port.h port.h print.h utils.h

# install-data-local:
#	$(INSTALL_DATA) Parti_config.h $(DESTDIR)$(includedir)
#	$(INSTALL_DATA) $(DESTDIR)$(includedir)

# uninstall-local:
#	rm -f $(DESTDIR)$(includedir)/Parti_config.h

dist-hook:
	echo "Inside of PARTI dist-hook rule"
