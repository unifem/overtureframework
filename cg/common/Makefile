#
# Makefile for building the library of common routines
#

# check for targets that do not need source file dependencies:
THISPKG:=common

NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))

# ==== optional capabilities: use on/off =================

# if buildStaticLib:=on then all out of date object files are added to the static library. 
# This option is useful when developing new code. First build the dynamic libraries with buildStaticLib:=off. 
# Then set buildStaticLib:=on and any files that are changed are put in the static lib. This gives a faster turn around.
# (buildStaticLib is used in the file "make" )
buildStaticLib := off

# =======================================================

include configFile
include $(CONFIGDIR)/make.dirs
include $(CONFIGDIR)/make.$(OS)
include $(CONFIGDIR)/make.rules
include $(CONFIGDIR)/make.options
include make

# List of packages to build
PKGS := src moving/src dataBase multiComponent/src

vpath % $(PKGS) $(Overture)/bin $(OvertureCheckout)/ogen
VPATH = $(PKGS) $(Overture)/bin $(OvertureCheckout)/ogen

SRC_FILES := 
FSRC :=
TESTS := 
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

all: lib $(BINDIR)checkCheckFiles $(BINDIR)convRate $(BINDIR)createBodyMask

junk:
	-@echo "OV_PARALLEL_INCLUDE=$(OV_PARALLEL_INCLUDE)"
	-@echo "PERL_DIR=$(PERL_DIR)"
	-@echo "PERL_LIBS=$(PERL_LIBS)"

usage: 
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|lib|checkCheckFiles>"

config_os:
	-@echo "using configuration for $(OS)"

include $(PKG_INCLUDE)

# This next include defines how to compile the lists: 
#  SRC_FILES : list of C++, C, and fortran files that are to be compiled in the default way.
#  CSRCO : list of C++ files that are to be compiled optimized by default
#  FSCRO : list of Fortran files that are to be compiled optimized by default
#  FSRC   : list of Fortran files that are to be compiled without auto-dbl
#  FSCRSO : list of Fortran files that are to be compiled optimized and without auto-dbl
include $(CONFIGDIR)/make.src

# add dependencies for main programs that are not in the previous source file lists
# DEP += 

# QSRC := $(SRC_FILES)
# OBJ := $(patsubst %.cc, %.o, $(QSRC))
# OBJ := $(patsubst %.c, %.o, $(OBJ))
# OBJ := $(patsubst %.C, %.o, $(OBJ))
# OBJ := $(patsubst %.f, %.o, $(OBJ)) 
# OBJ := $(patsubst %.F, %.o, $(OBJ)) 
# 
# # fortran files that are compiled without auto-double:
# OBJN :=$(patsubst %.f, %.o, $(FSRC))
# OBJN :=$(patsubst %.F, %.o, $(OBJN))
# 
# # extract C and C++ files by removing .f and .F files 
# CSRC := $(filter-out %.f, $(QSRC))
# CSRC := $(filter-out %.F, $(CSRC))
# 
# # Dependency files for C and C++ files
# DEP := $(patsubst %.cc, %.d, $(CSRC))
# DEP := $(patsubst %.C, %.d, $(DEP))
# DEP := $(patsubst %.c, %.d, $(DEP))
# # DEP := $(patsubst %.o, %.d, $(OBJ))
# 
# OBJ := $(addprefix $(OBJPREFIX),$(OBJ))
# DEP := $(addprefix $(DEPPREFIX),$(DEP)) 

.PHONY: doc 

# make the library
# lib: $(LIBDIR)libCgCommon.a
# lib: $(LIBDIR)libCgCommon
# lib: $(LIBDIR)libCgCommon
lib: $(CgCommonBuildLibrary)

LIB_OBJS = $(OBJ) $(OBJN) $(OBJO) $(OBJSO)

aa:
	@echo "CSRC=$(CSRC)"
	@echo "DEP=$(DEP)"
	@echo "OBJO=$(OBJO)"
	@echo "LIB_OBJS=$(LIB_OBJS)"


# $(LIBDIR)libCgCommon.a: $(OBJ)
#	$(AR) $(AR_FLAGS) $(LIBDIR)libCgCommon.a $?
# build the CgCommon static library (only put in files that are out of date since the dynamic lib was built)
$(LIBDIR)libCgCommonStatic.a: $(LIB_OBJS) 
	$(AR) $(AR_FLAGS) $(LIBDIR)libCgCommonStatic.a $?

# build the shared library (also delete and build an almost empty static library so that only 
# out of date files will be added to the static lib if buildStaticLib := on)
SHARED_LIBRARY = $(LIBDIR)libCgCommon$(SHARED_LIBRARY_EXT)
$(LIBDIR)libCgCommon: $(LIB_OBJS)
	  $(SHARED_LIBRARY_COMMAND) -o $(SHARED_LIBRARY) $(SHARED_LIBRARY_FLAGS) $(LIB_OBJS) $(SHARED_LIBRARY_LOAD_LIBS)
	  rm -f $(LIBDIR)libCgCommonStatic.a
	  $(AR) $(AR_FLAGS) $(LIBDIR)libCgCommonStatic.a $(OBJPREFIX)DomainSolver.o
	  touch $@

# Here is the routine used by the regression tests in the check directory
TESTS += $(BINDIR)checkCheckFiles
$(BINDIR)checkCheckFiles: $(OBJDIR)checkCheckFiles.o
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)checkCheckFiles.o $(LIBS)

# Here is the routine that computes convergence rates (and makes a TeX table), used in conv directories
TESTS += $(BINDIR)convRate
$(BINDIR)convRate: lib $(OBJDIR)convRate.o
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)convRate.o $(LIBS)

# test routine for the BoundaryData class
TESTS += $(BINDIR)tbd
$(BINDIR)tbd: $(LIBDIR)libCgCommon.a $(OBJDIR)tbd.o
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)tbd.o -L./lib -lCgCommon $(LIBS)

# Define libraries used by the tests that need linCgCommon
ifneq ($(OV_RPATH),)
  TEST_LIBS := $(OV_RPATH)$(LIBDIR) -L$(LIBDIR) -lCgCommon $(LIBS)
else
  TEST_LIBS := -L$(LIBDIR) -lCgCommon $(LIBS)
endif

# routine to create a body force mask
TESTS += $(BINDIR)createBodyMask
$(BINDIR)createBodyMask: lib $(OBJDIR)createBodyMask.o
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)createBodyMask.o $(TEST_LIBS)



# compile some files without auto double precision
$(OBJN) : $(OBJPREFIX)%.o : %.f
	$(FC) $(FC_FLAGSS) -o $@ -c $<

$(DEPDIR)depdep: $(DEPDIR) $(BINDIR) $(LIBDIR) $(OBJDIR)
	touch $(DEPDIR)depdep

$(BINDIR) $(LIBDIR) $(DEPDIR) $(OBJDIR):
	@-mkdir -p $@

# src/updateOpt.f : src/updateOpt.bf; bpp $<


doc:
	-@mkdir doc

check: 

cleano:
	-rm -f $(OBJPREFIX)*.o
cleandep:
	-rm -f $(OBJPREFIX)*.d
clean:
	-rm -f *~ */*~ $(OBJPREFIX)*.o $(DEPPREFIX)*.d $(LIBDIR)*.a $(TESTS)
	@cd src; $(MAKE) clean

# include dependencies except for a list of commands 
ifeq ($(NODEP),)
-include $(DEPDIR)depdep
-include $(DEP)
endif
