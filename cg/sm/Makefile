#
# Main Makefile for cgsm -- Solid Mechanics
#
THISPKG := sm

# check for targets that do not need source file dependencies:
NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))

# ==== optional capabilities: use on/off ===

# set to "on" to build 6th and 8th order (but general BC's not implemented yet)
buildHigherOrder := off

# sparse solver packages:  -- these are not used yet
usePETSc   := off
useSuperLU := off

OS := $(OS)
ifeq ($(OS),)
OS := Linux
endif

include configFile
include $(CONFIGDIR)/make.dirs
include $(CONFIGDIR)/make.$(OS)
include $(CONFIGDIR)/make.rules
# Over-ride default compile flags here: (or setenv COMPILE [dbg/opt/optdbg] from the shell)
#    compile:=[default/dbg/opt/optdbg]
#    profile:=[on/off]   
# compile=dbg
include $(CONFIGDIR)/make.options
include make

# List of packages to build
PKGS := src src/godunov src/hemp

# include Overture/Oges to get the PETSc solver and Ogmg files
vpath % $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/oges
VPATH = $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/oges

SRC_FILES := 
TESTS := 
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

all: $(BINDIR)cgsm

cgsm: $(BINDIR)cgsm

usage: 
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|cgsm>"

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
DEP += cgsmMain.d

#* QSRC := $(SRC_FILES)
#* 
#* OBJ := $(patsubst %.cc, %.o, $(QSRC))
#* OBJ := $(patsubst %.c, %.o, $(OBJ))
#* OBJ := $(patsubst %.C, %.o, $(OBJ))
#* COBJ := $(OBJ)  # list of c and C++ objects
#* OBJ := $(patsubst %.f, %.o, $(OBJ)) 
#* OBJ := $(patsubst %.F, %.o, $(OBJ)) 
#* 
#* # fortran files that are compiled optimized
#* OBJO :=$(patsubst %.f, %.o, $(FSRCO))
#* OBJO :=$(patsubst %.F, %.o, $(OBJO))
#* 
#* # fortran files that are compiled optimized and single precision
#* OBJSO :=$(patsubst %.f, %.o, $(FSRCSO))
#* OBJSO :=$(patsubst %.F, %.o, $(OBJSO))
#* 
#* # extract C and C++ files by removing .f and .F files 
#* CSRC := $(filter-out %.f, $(QSRC))
#* CSRC := $(filter-out %.F, $(CSRC))
#* # Dependency files for C and C++ files
#* DEP := $(patsubst %.cc, %.d, $(CSRC))
#* DEP := $(patsubst %.C, %.d, $(DEP))
#* DEP := $(patsubst %.c, %.d, $(DEP))
#* DEP += cgsmMain.d
#* 
#* OBJ  := $(addprefix $(OBJPREFIX),$(OBJ))
#* OBJO := $(addprefix $(OBJPREFIX),$(OBJO))
#* OBJSO:= $(addprefix $(OBJPREFIX),$(OBJSO))
#* DEP  := $(addprefix $(DEPPREFIX),$(DEP)) 
#* 
#* COBJ := $(patsubst %.f, , $(COBJ))
#* COBJ := $(patsubst %.F, , $(COBJ))
#* COBJ := $(addprefix $(OBJPREFIX),$(COBJ))

.PHONY: doc cgsm

#* # compile some fortran files optimized
#* $(OBJO) : $(OBJPREFIX)%.o : %.f
#* 	$(FC) $(FC_FLAGSO) -o $@ -c $<
#* 
#* # compile some fortran files optimized and single precision
#* $(OBJSO) : $(OBJPREFIX)%.o : %.f
#* 	$(FC) $(FC_FLAGSSO) -o $@ -c $<


# Here is the common library
libCommon:=$(CGBUILDPREFIX)common/libCgCommon

SHARED_LIBRARY = $(LIBDIR)libCgsm$(SHARED_LIBRARY_EXT)

# make cgsm in the bin dir:
PETSC_FILES_OBJ := $(addprefix $(OBJPREFIX),$(PETSC_FILES))
CGSM_EXTRA_FILES = $(PETSC_FILES_OBJ)

$(BINDIR)cgsm: $(SHARED_LIBRARY) $(OBJDIR)cgsmMain.o $(CGSM_EXTRA_FILES) $(libCommon)
	-@mkdir $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)cgsmMain.o $(CGSM) $(CGSM_EXTRA_FILES) $(LIBS)


# make the library
lib_build: $(SHARED_LIBRARY)
	touch lib_build

LIB_OBJS = $(OBJ) $(OBJCO) $(OBJN) $(OBJO) $(OBJSO)

# build the shared library (also delete and build an almost empty static library so that only 
# out of date files will be added to the static lib if buildStaticLib := on)
$(LIBDIR)libCgsm$(SHARED_LIBRARY_EXT): $(LIB_OBJS)
	  $(SHARED_LIBRARY_COMMAND) -o $(SHARED_LIBRARY) $(SHARED_LIBRARY_FLAGS) $(LIB_OBJS) $(SHARED_LIBRARY_LOAD_LIBS)
# build the library in cg/common
$(libCommon):; @cd ../common; $(MAKE) lib


$(DEPDIR)depdep: $(DEPDIR) $(BINDIR) $(LIBDIR) $(OBJDIR)
	touch $(DEPDIR)depdep

$(BINDIR) $(LIBDIR) $(DEPDIR) $(OBJDIR):
	@-mkdir -p $@

doc:
	-@mkdir doc

check: 

cleano:
	-rm -f $(OBJPREFIX)*.o
cleandep:
	-rm -f $(OBJPREFIX)*.d
clean:
	-rm -f *~ */*~ $(OBJPREFIX)*.o $(DEPPREFIX)*.d $(DEPPREFIX)depdep $(BINDIR)cgsm  \
         cmd/*.show* cmd/*.debug cmd/*.check cmd/*.log cmd/*.move cmd/cgsm.cmd $(SHARED_LIBRARY)
	-@cd doc; $(MAKE) clean
	@cd src; $(MAKE) clean

# include dependencies except for a list of commands 
ifeq ($(NODEP),)
-include $(DEPDIR)depdep
-include $(DEP)
endif

