#
# Main Makefile for cgad
#
THISPKG:=ad

# checkf or targets that do not need source file dependencies:
NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))

# ==== optional capabilities: use on/off =================

# sparse solver packages:
usePETSc   := on
useSuperLU := off
useOgmg    := on

include configFile
include $(CONFIGDIR)/make.dirs
include $(CONFIGDIR)/make.$(OS)
include $(CONFIGDIR)/make.rules
include $(CONFIGDIR)/make.options
include make

# List of packages to build
PKGS := src 

# include the src directory in path to search for files, also include Overture/Oges to get the PETSc solver 
# and Ogmg files
vpath % $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/op/bc $(OvertureCheckout)/oges 
VPATH = $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/op/bc $(OvertureCheckout)/oges
# vpath % $(PKGS) $(GUIDIR)/src
# VPATH = $(PKGS) $(GUIDIR)/src

SRC_FILES := 
TESTS := 
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

.PHONY: doc clean lib cgad libCommon


all: $(BINDIR)cgad

cgad: $(BINDIR)cgad

test:; -@echo "NODEP=$(NODEP), MAKECMDGOALS=$(MAKECMDGOALS)"

usage: 
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|cgad>"

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
DEP += cgadMain.d

# libCommon:=$(CGBUILDPREFIX)common/lib/libCgCommon.so
libCommon:=$(CGBUILDPREFIX)common/libCgCommon

#  optionally link in PETSc and/or Ogmg (these are setup common/make)
PETSC_FILES_OBJ := $(addprefix $(OBJPREFIX),$(PETSC_FILES))
# compile PETSc files optimized
$(PETSC_FILES_OBJ) : $(OBJPREFIX)%.o : %.C
	$(CXX) $(CXX_FLAGSO) -o $@ -c $<

# make cgad in the bin dir:
# CGAD_EXTRA_FILES = $(PETSC_FILES_OBJ) /home/henshw/overtureFramework/Overture/mapping/DataPointMapping.o \
#           /home/henshw/overtureFramework/Overture/mapping/NurbsMapping.o
CGAD_EXTRA_FILES = $(PETSC_FILES_OBJ)
$(BINDIR)cgad: $(LIBDIR)libCgad.a $(OBJDIR)cgadMain.o $(OBJ)  $(CGAD_EXTRA_FILES) $(libCommon)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)cgadMain.o  $(CGAD_EXTRA_FILES) $(LIBS)

lib_build: $(LIBDIR)libCgad.a
	touch lib_build

# build the Cgad library
$(LIBDIR)libCgad.a: $(OBJ)
	$(AR) $(AR_FLAGS) $(LIBDIR)libCgad.a $?

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
	-rm -f *~ */*~ $(DEPPREFIX)*.d $(OBJPREFIX)*.o $(BINDIR)cgad $(BINDIR)checkCheckFiles $(LIBDIR)*.a \
             cmd/*.show* cmd/*.debug cmd/*.check cmd/*.log cmd/*.move cmd/cgad.cmd

# remove all c and c++ .o files, dependencies and executables (but leave fortran .o files)
cleanc: 
	-rm -f $(DEPPREFIX)*.d $(COBJ) $(BINDIR)cgad $(LIBDIR)*.a

# include dependencies except for a list of commands 
ifeq ($(NODEP),)
-include $(DEPDIR)depdep
-include $(DEP)
endif



