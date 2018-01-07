#
# Main Makefile for cgcns
#
THISPKG := cns

# check for targets that do not need source file dependencies:
NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))

# ==== optional capabilities: use on/off ===

usePETSc := off

# to use Chemkin:
Chemkin  := off

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
PKGS := src ../common/chemistry src/godunov src/multicomponent src/jameson

#vpath % $(PKGS) $(GUIDIR)/src $(OvertureCheckout)/grid
#VPATH = $(PKGS) $(GUIDIR)/src $(OvertureCheckout)/grid
vpath % $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/oges $(OvertureCheckout)/ogshow ../common/chemistry
VPATH = $(PKGS) $(GUIDIR)/src $(Overture)/Oges $(OvertureCheckout)/oges $(OvertureCheckout)/ogshow ../common/chemistry

SRC_FILES :=
FSRC :=
FSRCO :=
FSRCNO :=
TESTS :=
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

all: $(BINDIR)cgcns

cgcns: $(BINDIR)cgcns

usage:
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|cgcns>"

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
DEP += cgcnsMain.d


.PHONY: doc cgcns

a:
	echo "OBJSO=$(OBJSO)"
	echo "FSRCSO=$(FSRCSO)"


# libCommon:=$(CGBUILDPREFIX)common/lib/libCgCommon.a
libCommon:=$(CGBUILDPREFIX)common/libCgCommon
# libCommon:=../common/lib/libCgCommon.so
# libCommon:=$(CGBUILDPREFIX)common/lib/libCgCommon.so

PETSC_FILES_OBJ := $(addprefix $(OBJPREFIX),$(PETSC_FILES))
# make cgcns in the bin dir:

CGCNS_EXTRA_FILES = $(PETSC_FILES_OBJ) /home/henshaw.0/Overture/amr/InterpolateRefinements.o /home/henshaw.0/Overture/amr/Interpolate.o /home/henshaw.0/Overture/gf/ExposedPoints.o
CGCNS_EXTRA_FILES = $(PETSC_FILES_OBJ) /home/henshaw.0/Overture/gf/ExposedPoints.o
CGCNS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/ogen/movingUpdate.o
CGCNS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/ogen/movingUpdateNew.o ../../Overture/ogen/classify.o
CGCNS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/mapping/Inverse.o ../../Overture/mapping/stencilWalk.o ../../Overture/mapping/Mapping.o
CGCNS_EXTRA_FILES = $(PETSC_FILES_OBJ) ../../Overture/ogen/updateRefineFillInterpNew.o
CGCNS_EXTRA_FILES = $(PETSC_FILES_OBJ)
$(BINDIR)cgcns: lib_build $(OBJDIR)cgcnsMain.o $(CGCNS_EXTRA_FILES) $(libCommon)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)cgcnsMain.o $(CGCNS_EXTRA_FILES) $(LIBS)
#	@echo "CGBUILDPREFIX=$(CGBUILDPREFIX)"

# make the library
# lib_build: $(LIBDIR)libCgcns
lib_build: $(LIBDIR)libCgcns
	touch lib_build

# build the Cgcns library
$(LIBDIR)libCgcns.a: $(OBJ) $(OBJN) $(OBJO) $(OBJSO)
	$(AR) $(ARFLAGS) $(LIBDIR)libCgcns.a $?

SHARED_LIBRARY = $(LIBDIR)libCgcns$(SHARED_LIBRARY_EXT)
LIB_OBJS = $(OBJ) $(OBJCO) $(OBJN) $(OBJO) $(OBJSO)
$(LIBDIR)libCgcns: $(LIB_OBJS)
	  $(SHARED_LIBRARY_COMMAND) -o $(SHARED_LIBRARY) $(SHARED_LIBRARY_FLAGS) $(LIB_OBJS) $(SHARED_LIBRARY_LOAD_LIBS)
	  touch $@

# build the library in cg/common
$(libCommon):; @cd ../common; $(MAKE)

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
	-rm -f *~ */*~ $(OBJPREFIX)*.o $(DEPPREFIX)*.d $(BINDIR)cgcns $(LIBDIR)*.a  \
         cmd/*.show* cmd/*.debug cmd/*.check cmd/*.log cmd/*.move cmd/cgcns.cmd
	@cd src; $(MAKE) clean
	-@cd doc; $(MAKE) clean

# remove all c and c++ .o files, dependencies and executables (but leave fortran .o files)
cleanc:
	-rm -f $(DEPPREFIX)*.d $(COBJ) $(BINDIR)cgcns $(LIBDIR)*.a

# include dependencies except for a list of commands
ifeq ($(NODEP),)
-include $(DEPDIR)depdep
-include $(DEP)
endif
