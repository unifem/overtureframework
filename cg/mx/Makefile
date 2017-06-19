#
# Main Makefile for cgmx -- Maxwell's Equations
#
THISPKG := mx

# check for targets that do not need source file dependencies:
NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))

# ==== optional capabilities: use on/off ===

# set to "on" to build 6th and 8th order (but general BC's not implemented yet)
buildHigherOrder := off
# buildHigherOrder := on

# sparse solver packages:  -- these are not used yet
usePETSc   := off
useSuperLU := off

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

# include Overture/Oges to get the PETSc solver and Ogmg files
vpath % $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/oges
VPATH = $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/oges

SRC_FILES := 
TESTS := 
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

all: $(BINDIR)cgmx

cgmx: $(BINDIR)cgmx

usage: 
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|cgmx>"

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
DEP += cgmx.d

# objco:; -@echo "OBJCO=$(OBJCO)"

.PHONY: doc cgmx

# Cgmx needs the data base files (but does not link to libCommon) so make a link to them here:
# src/dbAccess.C : ../../common/dataBase/dbAccess.C
# 	ln -sf ../../common/dataBase/dbAccess.C src/dbAccess.C
# 	ln -sf ../../common/dataBase/dbFunctions.f src/dbFunctions.f
# 	ln -sf ../../common/dataBase/kk_ptr.cc src/kk_ptr.cc

libCommon:=$(CGBUILDPREFIX)common/libCgCommon

# make cgmx in the bin dir:
PETSC_FILES_OBJ := $(addprefix $(OBJPREFIX),$(PETSC_FILES))
CGMX_EXTRA_FILES = $(PETSC_FILES_OBJ)

# $(BINDIR)cgmx: $(OBJDIR)cgmx.o $(OBJALL) $(CGMX_EXTRA_FILES) src/dbAccess.C
$(BINDIR)cgmx: $(OBJDIR)cgmx.o $(OBJALL) $(CGMX_EXTRA_FILES) $(libCommon)
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)cgmx.o $(CGMX) $(OBJALL) $(CGMX_EXTRA_FILES) $(LIBS)


# build the library in cg/common
$(libCommon):; @cd ../common; $(MAKE) lib

$(DEPDIR)depdep: $(DEPDIR) $(BINDIR) $(LIBDIR) $(OBJDIR)
	touch $(DEPDIR)depdep

$(BINDIR) $(LIBDIR) $(DEPDIR) $(OBJDIR):
	@-mkdir -p $@

doc:
	-@mkdir doc

check: 

# remove all c and c++ .o files, dependencies and executables (but leave fortran .o files)
cleanc: 
	-rm -f $(DEPPREFIX)*.d $(COBJ) $(OBJCO) $(BINDIR)cgmx $(LIBDIR)*.a

cleano:
	-rm -f $(OBJPREFIX)*.o
cleandep:
	-rm -f $(OBJPREFIX)*.d
clean:
	-rm -f *~ */*~ $(OBJPREFIX)*.o $(DEPPREFIX)*.d $(BINDIR)cgmx  \
         cmd/*.show* cmd/*.debug cmd/*.check cmd/*.log cmd/*.move cmd/cgmx.cmd 

# include dependencies except for a list of commands 
ifeq ($(NODEP),)
-include $(DEPDIR)depdep
-include $(DEP)
endif

