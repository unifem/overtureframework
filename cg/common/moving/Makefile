#
# Makefile for moving body routines.
#
THISPKG:=common/moving

# checkf or targets that do not need source file dependencies:
NODEP := $(strip $(filter usage doc clean cleano cleanc cleandep,$(MAKECMDGOALS)))


# ==== optional capabilities: use on/off ===

OS := $(OS)

ifeq ($(OS),)
OS := Linux
endif

include configFile
include $(CONFIGDIR)/make.dirs
include $(CONFIGDIR)/make.$(OS)
include $(CONFIGDIR)/make.rules
# Over-ride default compile flags here: 
#    compile:=[dbg/opt/optdbg]
#    profile:=[on/off]   
# compile:=dbg
include $(CONFIGDIR)/make.options
include make


# List of packages to build
PKGS := src

# include the src directory in paths to search for files
vpath % $(PKGS) 
VPATH = $(PKGS) 

SRC_FILES := 
TESTS := 
PKG_INCLUDE := $(foreach p, $(PKGS), $(p)/files)

all: $(BINDIR)trb

usage: 
	-@echo "usage : "
	-@echo "        % make " $(COPTS) "<all|trb>"

config_os:
	-@echo "using configuration for $(OS)"

# currently nothing here:
# include $(PKG_INCLUDE)

QSRC := $(SRC_FILES)

OBJ := $(patsubst %.cc, %.o, $(QSRC))
OBJ := $(patsubst %.c, %.o, $(OBJ))
OBJ := $(patsubst %.C, %.o, $(OBJ))
COBJ := $(OBJ)  # list of c and C++ objects
OBJ := $(patsubst %.f, %.o, $(OBJ)) 
OBJ := $(patsubst %.F, %.o, $(OBJ)) 

# fortran files that are compiled optimized
OBJO :=$(patsubst %.f, %.o, $(FSRCO))
OBJO :=$(patsubst %.F, %.o, $(OBJO))

# extract C and C++ files by removing .f and .F files 
CSRC := $(filter-out %.f, $(QSRC))
CSRC := $(filter-out %.F, $(CSRC))
# Dependency files for C and C++ files
DEP := $(patsubst %.cc, %.d, $(CSRC))
DEP := $(patsubst %.C, %.d, $(DEP))
DEP := $(patsubst %.c, %.d, $(DEP))
DEP += trb.d collide.d motion.d tp.d

OBJ  := $(addprefix $(OBJPREFIX),$(OBJ))
OBJO := $(addprefix $(OBJPREFIX),$(OBJO))
DEP  := $(addprefix $(DEPPREFIX),$(DEP)) 

COBJ := $(patsubst %.f, , $(COBJ))
COBJ := $(patsubst %.F, , $(COBJ))
COBJ := $(addprefix $(OBJPREFIX),$(COBJ))

# cobj:; -@echo "COBJ=$(COBJ)"

# "trb" is a phony file so that the make system does not look for a file "trb" after making the target trb
.PHONY: doc clean lib trb collide motion tp


# compile some fortran files optimized
$(OBJO) : $(OBJPREFIX)%.o : %.f
	$(FC) $(FC_FLAGSO) -o $@ -c $<


a:
	@echo "OBJSO=$(OBJSO)"
	@echo "FSRCSO=$(FSRCSO)"
	@echo "OBJDIR=$(OBJDIR)"
	@echo "BINDIR=$(BINDIR)"

# test routine for the rigid body motion class:
TESTS += $(BINDIR)trb
trb : $(BINDIR)trb
$(BINDIR)trb: $(OBJDIR)trb.o $(OBJDIR)RigidBodyMotion.o $(OBJDIR)TimeFunction.o
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)trb.o $(OBJDIR)RigidBodyMotion.o $(OBJDIR)TimeFunction.o ../obj/dbAccess.o ../obj/kk_ptr.o $(LIBS)

# test collisions
TESTS += $(BINDIR)collide
collide : $(BINDIR)collide
$(BINDIR)collide: $(OBJDIR)collide.o $(OBJDIR)RigidBodyMotion.o $(OBJDIR)detectCollisions.o
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)collide.o $(OBJDIR)RigidBodyMotion.o $(OBJDIR)detectCollisions.o $(LIBS)

# test matrix motions
TESTS += $(BINDIR)motion
motion : $(BINDIR)motion
$(BINDIR)motion: $(OBJDIR)motion.o $(OBJDIR)MatrixMotion.o $(OBJDIR)TimeFunction.o
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)motion.o $(OBJDIR)MatrixMotion.o $(OBJDIR)TimeFunction.o $(LIBS)


# test routine for the Piston class
TESTS += $(BINDIR)tp
tp : $(BINDIR)tp
$(BINDIR)tp: $(OBJDIR)tp.o $(OBJDIR)PistonMotion.o
	-@mkdir -p $(BINDIR)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJDIR)tp.o $(OBJDIR)PistonMotion.o ../obj/dfmin.o $(LIBS)


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
	-rm -f *~ */*~ $(DEPPREFIX)*.d $(OBJPREFIX)*.o $(BINDIR)trb $(BINDIR)collide $(BINDIR)motion $(BINDIR)tp \
         $(LIBDIR)*.a cmd/*.show* cmd/*.debug cmd/*.check cmd/*.log cmd/*.move 

# remove all c and c++ .o files, dependencies and executables (but leave fortran .o files)
cleanc: 
	-rm -f $(DEPPREFIX)*.d $(COBJ) $(BINDIR)cgins $(LIBDIR)*.a

# include dependencies except for a list of commands 
ifeq ($(NODEP),)
-include $(DEPDIR)depdep
-include $(DEP)
endif

