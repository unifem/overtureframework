#
# Main Makefile for cgasf
#
THISPKG:=asf

# check for targets that do not need source file dependencies:
NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))

# ==== optional capabilities: use on/off ===

# sparse solver packages:
ifeq ($(usePETSc),)
usePETSc   := off
endif
useSuperLU := off
useOgmg    := off

# to use Chemkin:
Chemkin  := off

include configFile
include $(CONFIGDIR)/make.dirs
include $(CONFIGDIR)/make.$(OS)
include $(CONFIGDIR)/make.rules
# Over-ride defaults here: 
#    compile:=[dbg/opt/optdbg]
#    profile:=[on/off]      
# compile=opt
include $(CONFIGDIR)/make.options
include make

# List of packages to build
PKGS := src ../common/chemistry

# include Overture/Oges to get the PETSc solver and Ogmg files
vpath % $(PKGS) $(GUIDIR)/src  ../common/chemistry $(Overture)/Oges $(OvertureCheckout)/op/bc $(OvertureCheckout)/oges
VPATH = $(PKGS) $(GUIDIR)/src  ../common/chemistry $(Overture)/Oges $(OvertureCheckout)/op/bc $(OvertureCheckout)/oges

SRC_FILES := 
FSRC :=
FSRCO :=
FSRCNO :=
TESTS := 
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

all: $(BINDIR)cgasf

cgasf: $(BINDIR)cgasf

usage: 
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|cgasf>"

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
DEP += cgasfMain.d


#* DEP := $(patsubst %.o, %.d, $(OBJ))
#* 
#* OBJ := $(addprefix $(OBJPREFIX),$(OBJ))
#* OBJN := $(addprefix $(OBJPREFIX),$(OBJN))
#* OBJO := $(addprefix $(OBJPREFIX),$(OBJO))
#* OBJSO:= $(addprefix $(OBJPREFIX),$(OBJSO))
#* DEP := $(addprefix $(DEPPREFIX),$(DEP)) 
#* 
#* 
#* COBJ := $(patsubst %.f, , $(COBJ))
#* COBJ := $(patsubst %.F, , $(COBJ))
#* COBJ := $(addprefix $(OBJPREFIX),$(COBJ))

.PHONY: doc cgasf

a:
	echo "OBJSO=$(OBJSO)"
	echo "FSRCSO=$(FSRCSO)"

#* 
#* # compile some fortran files optimized
#* $(OBJO) : $(OBJPREFIX)%.o : %.f
#* 	$(FC) $(FC_FLAGSO) -o $@ -c $<
#* # compile some fortran files optimized and single precision
#* $(OBJSO) : $(OBJPREFIX)%.o : %.f
#* 	$(FC) $(FC_FLAGSSO) -o $@ -c $<
#* 

# libCommon:=$(CGBUILDPREFIX)common/lib/libCgCommon.so
libCommon:=$(CGBUILDPREFIX)common/libCgCommon

PETSC_FILES_OBJ := $(addprefix $(OBJPREFIX),$(PETSC_FILES))
OGMG_FILES_OBJ := $(addprefix $(OBJPREFIX),$(OGMG_FILES))
CGASF_EXTRA_FILES = $(PETSC_FILES_OBJ) $(OGMG_FILES_OBJ) 
# make cgasf in the bin dir:
$(BINDIR)cgasf: $(LIBDIR)libCgasf $(OBJDIR)cgasfMain.o $(CGASF_EXTRA_FILES) $(libCommon)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)cgasfMain.o $(CGASF_EXTRA_FILES) $(LIBS)

lib_build: $(LIBDIR)libCgasf
	touch lib_build

# here is the shared library:
SHARED_LIBRARY = $(LIBDIR)libCgasf$(SHARED_LIBRARY_EXT)
LIB_OBJS = $(OBJ) $(OBJN) $(OBJO) $(OBJSO)
$(LIBDIR)libCgasf: $(LIB_OBJS)
	  $(SHARED_LIBRARY_COMMAND) -o $(SHARED_LIBRARY) $(SHARED_LIBRARY_FLAGS) $(LIB_OBJS) $(SHARED_LIBRARY_LOAD_LIBS)
	  touch $@

# here is the static Cgasf library
$(LIBDIR)libCgasf.a: $(OBJ) $(OBJN) $(OBJO) $(OBJSO)
	$(AR) $(AR_FLAGS) $(LIBDIR)libCgasf.a $?


# build the library in cg/common
$(libCommon):; @cd ../common; $(MAKE) lib

$(DEPDIR)depdep: $(DEPDIR) $(BINDIR) $(LIBDIR) $(OBJDIR)
	touch $(DEPDIR)depdep

$(BINDIR) $(LIBDIR) $(DEPDIR) $(OBJDIR):
	@-mkdir -p $@

# compile some files without auto double precision
$(OBJN) : $(OBJPREFIX)%.o : %.f
	$(FC) $(FC_FLAGSS) -o $@ -c $<

doc:
	-@mkdir doc

check: 

cleano:
	-rm -f $(OBJPREFIX)*.o
cleandep:
	-rm -f $(OBJPREFIX)*.d
clean:
	-rm -f *~ */*~ $(OBJPREFIX)*.o $(DEPPREFIX)*.d $(BINDIR)cgasf $(LIBDIR)*.a \
         cmd/*.show* cmd/*.debug cmd/*.check cmd/*.log cmd/*.move cmd/cgasf.cmd
	@cd src; $(MAKE) clean

# remove all c and c++ .o files, dependencies and executables (but leave fortran .o files)
cleanc: 
	-rm -f $(DEPPREFIX)*.d $(COBJ) $(BINDIR)cgasf $(LIBDIR)*.a

# include dependencies except for a list of commands 
ifeq ($(NODEP),)
-include $(DEPDIR)depdep
-include $(DEP)
endif


