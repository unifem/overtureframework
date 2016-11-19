#
# Main Makefile for cgins
#
THISPKG:=ins

# check for targets that do not need source file dependencies:
NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))


# ==== optional capabilities: use on/off =================

# sparse solver packages:
usePETSc   := on
useSuperLU := off

# turbulence models: (these don't work yet)
BaldwinLomax   := on
SpalartAlmaras := on
kEpsilon       := on

# visco-plastic model 
viscoPlastic   := on

# two-phase flow model 
twoPhase := off

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
# Over-ride default compile flags here: (or setenv COMPILE [dbg/opt/optdbg] from the shell)
#    compile:=[dbg/opt/optdbg]
#    profile:=[on/off]   
# compile:=dbg
include $(CONFIGDIR)/make.options
include make


# List of packages to build
PKGS := src 

# include the src directory in path to search for files, also include Overture/Oges to get the PETSc solver 
# and Ogmg files
vpath % $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/op/bc $(OvertureCheckout)/oges 
VPATH = $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/op/bc $(OvertureCheckout)/oges
#vpath % $(PKGS) $(GUIDIR)/src $(Overture)/Oges /home/henshaw/Overture/op/bc
#VPATH = $(PKGS) $(GUIDIR)/src $(Overture)/Oges /home/henshaw/Overture/op/bc

SRC_FILES := 
TESTS := 
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

all: $(BINDIR)cgins

cgins: $(BINDIR)cgins

#CGBUILDPREFIX := $(CGBUILDPREFIX)/
#ifeq "$(findstring //,$(CGBUILDPREFIX))" "//"
# CGBUILDPREFIX := $(subst //,/,$(CGBUILDPREFIX))
#endif

test: 
	-@echo "CGBUILDPREFIX=$(CGBUILDPREFIX)"
	-@echo "findstring: $(findstring //,$(CGBUILDPREFIX))"
	-@echo "subst: $(subst //,/,$(CGBUILDPREFIX))"

usage: 
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|cgins>"

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
DEP += cginsMain.d


.PHONY: doc clean lib cgins

#* # compile some fortran files optimized
#* $(OBJO) : $(OBJPREFIX)%.o : %.f
#* 	$(FC) $(FC_FLAGSO) -o $@ -c $<

# libCommon:=$(CGBUILDPREFIX)common/lib/libCgCommon.so
libCommon:=$(CGBUILDPREFIX)common/libCgCommon


#  optionally link in PETSc and/or Ogmg (these are setup common/make)
PETSC_FILES_OBJ := $(addprefix $(OBJPREFIX),$(PETSC_FILES))
## Ogmg is now always included with PETSc: 
## OGMG_FILES_OBJ := $(addprefix $(OBJPREFIX),$(OGMG_FILES))


# compile PETSc files optimized
$(PETSC_FILES_OBJ) : $(OBJPREFIX)%.o : %.C
	$(CXX) $(CXX_FLAGSO) -o $@ -c $<

# Here is where I optionally link in other files for debugging: 
CGINS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/gf/ParallelOverlappingGridInterpolator.o
CGINS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/op/bc/neumann.o
CGINS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/otherStuff/BodyForce.o
CGINS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/mapping/UnstructuredMapping.o
CGINS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/mapping/stencilWalk.o
CGINS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/hype/applyBoundaryConditions.o
CGINS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/oges/generateMatrix.o
CGINS_EXTRA_FILES = $(PETSC_FILES_OBJ) 
# make cgins in the bin dir:
$(BINDIR)cgins: lib_build $(OBJDIR)cginsMain.o $(CGINS_EXTRA_FILES) $(libCommon)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)cginsMain.o $(CGINS_EXTRA_FILES) $(LIBS)

a: $(OBJDIR)a.o 
	$(CXX) $(CXX_FLAGS) -o $(BINDIR)$@ $(OBJDIR)a.o 

# make the library
# lib_build: $(LIBDIR)libCgins
lib_build: $(CginsBuildLibrary)
	touch lib_build

LIB_OBJS = $(OBJ) $(OBJCO) $(OBJN) $(OBJO) $(OBJSO) $(FNOBJO)

e:; -@echo "FNOBJO=$(FNOBJO)"

f:; -@echo "OBJCO=$(OBJCO)"

# build the Cgins static library (only put in files that are out of date since the dynamic lib was built)
$(LIBDIR)libCginsStatic.a: $(LIB_OBJS)
	$(AR) $(AR_FLAGS) $(LIBDIR)libCginsStatic.a $?

# build the shared library (also delete and build an almost empty static library so that only 
# out of date files will be added to the static lib if buildStaticLib := on)
SHARED_LIBRARY = $(LIBDIR)libCgins$(SHARED_LIBRARY_EXT)
$(LIBDIR)libCgins: $(LIB_OBJS)
	  $(SHARED_LIBRARY_COMMAND) -o $(SHARED_LIBRARY) $(SHARED_LIBRARY_FLAGS) $(LIB_OBJS) $(SHARED_LIBRARY_LOAD_LIBS)
	  rm -f $(LIBDIR)libCginsStatic.a
	  $(AR) $(AR_FLAGS) $(LIBDIR)libCginsStatic.a $(OBJPREFIX)Cgins.o
	  touch $@

# build the library in cg/common
$(libCommon):; @cd ../common; $(MAKE) lib

$(DEPDIR)depdep: $(DEPDIR) $(BINDIR) $(LIBDIR) $(OBJDIR)
	touch $(DEPDIR)depdep

$(BINDIR) $(LIBDIR) $(DEPDIR) $(OBJDIR):
	@-mkdir -p $@

doc:
	-@mkdir doc

check:
	@cd check; make 

cleano:
	-rm -f $(OBJPREFIX)*.o
cleandep:
	-rm -f $(OBJPREFIX)*.d

clean: 
	-rm -f *~ */*~ $(DEPPREFIX)*.d $(OBJPREFIX)*.o $(BINDIR)cgins $(BINDIR)checkCheckFiles $(LIBDIR)*.a \
         cmd/*.show* cmd/*.debug cmd/*.check cmd/*.log cmd/*.move cmd/cgins.cmd
	-@cd doc; $(MAKE) clean
	@cd src; $(MAKE) clean

# remove all c and c++ .o files, dependencies and executables (but leave fortran .o files)
cleanc: 
	-rm -f $(DEPPREFIX)*.d $(COBJ) $(BINDIR)cgins $(LIBDIR)*.a

# include dependencies except for a list of commands 
ifeq ($(NODEP),)
-include $(DEPDIR)depdep
-include $(DEP)
endif



