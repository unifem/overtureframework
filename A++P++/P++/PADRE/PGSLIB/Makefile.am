include ../config/Makefile.override.install.includeHEADERS

# This is required for some rules (e.g. all-recursive, and all in some directories)
# It is defined in A++P++/config/Makefile.config.in (but this Makefile.am does not
# include that file).
MAKE=gmake

ACLOCAL = @ACLOCAL@ -I ../config
# Note: make sure aclocal options match those in build.

CXXFLAGS      = @CXXFLAGS@ $(CXX_DEBUG) $(CXX_DEFINES) $(CXX_OPTIONS) $(CXX_OPT) $(CXX_TEMPLATES) $(CXX_WARNINGS) $(CXX_DL_FLAGS) $(CXX_DL_COMPILE_FLAGS)

CFLAGS        = @CFLAGS@ $(C_DEBUG) $(C_OPTIONS) $(C_OPT) $(C_WARNINGS) $(C_DL_FLAGS) $(C_DL_COMPILE_FLAGS)

CXX_TEMPLATE_REPOSITORY_PATH = @CXX_TEMPLATE_REPOSITORY_PATH@

libPADRE_PGSLIB_Source_a_SOURCES = \
 attempt-receive-double.c \
 constrained-send-rcv-int.c \
 gather-real.c \
 random-send-rcv-double.c \
 scatter-int.c \
 attempt-receive-int.c \
 constrained-send-rcv-real.c \
 random-send-rcv-int.c \
 scatter-real.c \
 attempt-receive-real.c \
 gs-setup-c.c \
 random-send-rcv-real.c \
 gather-double.c \
 gs-util-c.c \
 utility-c.c \
 constrained-send-rcv-double.c \
 gather-int.c \
 pgshash.c \
 scatter-double.c

noinst_LIBRARIES = libPADRE_PGSLIB_Source.a

# All PADRE/PGSLIB header files.
include_HEADERS = pgshash.h pgslib-names.h send-rcv.h gs-c.h pgslib-include-c.h pgslib-types.h utility-c.h

## files that are included in either the Makefile or some of the C source files
EXTRA_DIST = attempt-receive.c constrained-send-rcv.c random-send-rcv.c scatter.c gather.c



