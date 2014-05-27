#
# Main Makefile for the unitTests
#
THISPKG:=unitTests

# check for targets that do not need source file dependencies:
NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))


# ==== optional capabilities: use on/off =================


# =======================================================


OS := $(OS)

ifeq ($(OS),)
OS := Linux
endif


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

SRC_FILES := 
TESTS := 
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

all: $(BINDIR)trb $(BINDIR)tbm

tbm: $(BINDIR)tbm
trb: $(BINDIR)trb

mytest: 
	-@echo "CGBUILDPREFIX=$(CGBUILDPREFIX)"
	-@echo "findstring: $(findstring //,$(CGBUILDPREFIX))"
	-@echo "subst: $(subst //,/,$(CGBUILDPREFIX))"

usage: 
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|tbm|trb>"

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
DEP += trb.d


.PHONY: doc clean lib trb tbm

# libCommon:=$(CGBUILDPREFIX)common/lib/libCgCommon.so
libCommon:=$(CGBUILDPREFIX)common/libCgCommon

# test routine for the BeamModel class
TESTS += $(BINDIR)tbm
tbm : $(BINDIR)tbm
$(BINDIR)tbm: $(OBJDIR)tbm.o   $(libCommon)
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)tbm.o $(LIBS)

# test routine for the rigid body motion class:
TESTS += $(BINDIR)trb
trb : $(BINDIR)trb
$(BINDIR)trb: $(OBJDIR)trb.o  $(libCommon)
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)trb.o $(LIBS)

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
         cmd/*.show* cmd/*.debug cmd/*.check cmd/*.log cmd/*.move cmd/trb.cmd
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



