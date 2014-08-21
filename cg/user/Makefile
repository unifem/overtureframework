#
# Makefile for building a locally altered version of cgcns, cgins, ... with changes made by a user. 
# You can, for example, make changes to some of the files and put the changes here so the originials are 
# not altered.
#
# This Makefile can also be used to build the applications
#       aero      : compute some aero-dynamic quantities for results from a show file
#                   (see file cg/user/src/aero.C)
#       comp      : compare the solutions from different grid resolutions (estimating convergence rates etc.)
#       getEnergy : Interpolate the solution from a showfile onto a specified array of points
#                   (see file cg/user/src/getEnergy.C)
#
THISPKG := user

# check for targets that do not need source file dependencies:
NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))

# ==== optional capabilities: use on/off ===

# to use PETSc
usePETSc   := off
# to use Chemkin:
Chemkin  := off

include configFile
include $(CONFIGDIR)/make.dirs
include $(CONFIGDIR)/make.$(OS)
include $(CONFIGDIR)/make.rules
include $(CONFIGDIR)/make.options
include make

# List of packages to build
PKGS := src 

vpath % $(PKGS) ../cns/src ../ins/src
VPATH = $(PKGS) ../cns/src ../ins/src

SRC_FILES := 
FSRC :=
FSRCO :=
FSRCNO :=
TESTS := 
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

all: $(BINDIR)cgcns

# Here are the application main programs that we can build here
APPS := cgcns cgcns aero getEnergy

# Short forms (so we can type "make cgcns" or "make aero") 
cgcns: $(BINDIR)cgcns
cgins: $(BINDIR)cgins
aero: $(BINDIR)aero
getEnergy: $(BINDIR)getEnergy

usage: 
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|cgcns>"

config_os:
	-@echo "using configuration for $(OS)"

include $(PKG_INCLUDE)

QSRC := $(SRC_FILES)

OBJ := $(patsubst %.cc, %.o, $(QSRC))
OBJ := $(patsubst %.c, %.o, $(OBJ))
OBJ := $(patsubst %.C, %.o, $(OBJ))
COBJ := $(OBJ)  # list of c and C++ objects
OBJ := $(patsubst %.f, %.o, $(OBJ)) 
OBJ := $(patsubst %.F, %.o, $(OBJ)) 

# fortran files that are compiled without auto-double:
OBJN :=$(patsubst %.f, %.o, $(FSRC))
OBJN :=$(patsubst %.F, %.o, $(OBJN))
 
# fortran files that are compiled optimized
OBJO :=$(patsubst %.f, %.o, $(FSRCO))
OBJO :=$(patsubst %.F, %.o, $(OBJO))

# fortran files that are compiled optimized and single precision
OBJSO :=$(patsubst %.f, %.o, $(FSRCSO))
OBJSO :=$(patsubst %.F, %.o, $(OBJSO))

# extract C and C++ files by removing .f and .F files 
CSRC := $(filter-out %.f, $(QSRC))
CSRC := $(filter-out %.F, $(CSRC))
# Dependency files for C and C++ files
DEP := $(patsubst %.cc, %.d, $(CSRC))
DEP := $(patsubst %.C, %.d, $(DEP))
DEP := $(patsubst %.c, %.d, $(DEP))
DEP += $(patsubst %.C, %.d, $(APPS))

OBJ := $(addprefix $(OBJPREFIX),$(OBJ))
OBJN := $(addprefix $(OBJPREFIX),$(OBJN))
OBJO := $(addprefix $(OBJPREFIX),$(OBJO))
OBJSO:= $(addprefix $(OBJPREFIX),$(OBJSO))
DEP := $(addprefix $(DEPPREFIX),$(DEP)) 


COBJ := $(patsubst %.f, , $(COBJ))
COBJ := $(patsubst %.F, , $(COBJ))
COBJ := $(addprefix $(OBJPREFIX),$(COBJ))

.PHONY: $(patsubst %.C, , $(APPS))

a:
	echo "OBJSO=$(OBJSO)"
	echo "FSRCSO=$(FSRCSO)"


# compile some fortran files optimized
$(OBJO) : $(OBJPREFIX)%.o : %.f
	$(FC) $(FC_FLAGSO) -o $@ -c $<
# compile some fortran files optimized and single precision
$(OBJSO) : $(OBJPREFIX)%.o : %.f
	$(FC) $(FC_FLAGSSO) -o $@ -c $<

EXTRAOBJ = $(OBJ) $(OBJN) $(OBJO) $(OBJSO)

libCommon:=$(CGBUILDPREFIX)common/libCgCommon
libCgcns:=$(LIBDIR)../cgcns/lib/libCgcns
libCgins:=$(LIBDIR)../cgins/lib/libCgins

# Make cgcns in the bin dir:
$(BINDIR)cgcns: $(libCommon) $(libCgcns) $(OBJDIR)cgcnsMain.o $(EXTRAOBJ)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)cgcnsMain.o $(EXTRAOBJ) $(CGCNS_EXTRA_FILES) $(CNSLIBS)

# Make cgins in the bin dir:
$(BINDIR)cgins: $(libCommon) $(libCgins) $(OBJDIR)cginsMain.o $(EXTRAOBJ)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)cginsMain.o $(EXTRAOBJ) $(CGINS_EXTRA_FILES) $(INSLIBS)


# Make aero in the bin dir:
$(BINDIR)aero: $(libCommon) $(libAero) $(OBJDIR)aero.o 
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)aero.o $(LIBS)

# Make getEnergy in the bin dir:
$(BINDIR)getEnergy: $(libCommon) $(libGetEnergy) $(OBJDIR)getEnergy.o 
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)getEnergy.o $(LIBS)

# build the Cgcns library
$(libCgcns):; @cd ../cgcns; $(MAKE) -$(MAKEFLAGS) lib

# build the Cgins library
$(libCgins):; @cd ../cgins; $(MAKE) -$(MAKEFLAGS) lib

# build the library in cg/common
$(libCommon):; @cd ../common; $(MAKE) -$(MAKEFLAGS) lib

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
	-rm -f *~ */*~ $(OBJPREFIX)*.o $(DEPPREFIX)*.d $(DEPPREFIX)depdep $(BINDIR)cgcns $(BINDIR)cgins $(LIBDIR)*.a  \
         $(BINDIR)aero $(BINDIR)getEnergy cmd/*.show* cmd/*.debug cmd/*.check cmd/*.log cmd/*.move cmd/cgcns.cmd cmd/cgins.cmd

# remove all c and c++ .o files, dependencies and executables (but leave fortran .o files)
cleanc: 
	-rm -f $(DEPPREFIX)*.d $(COBJ) $(BINDIR)cgcns $(BINDIR)cgins $(LIBDIR)*.a

# include dependencies except for a list of commands 
ifeq ($(NODEP),)
-include $(DEPDIR)depdep
-include $(DEP)
endif


