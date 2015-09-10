#
#  This direcory holds results for oges
#
#
include ../../config/Makefile.$(VENDOR)

SuperLU = /home/henshaw/software/SuperLU_3.0
SuperLU_Includes = -I$(SuperLU)/SRC	

SuperLU = /home/henshaw/software/SuperLU_DIST_2.0
SuperLU_Includes = -I$(SuperLU)/src	

CCFLAGS  += $(SuperLU_Includes)
CCFLAGSG += $(SuperLU_Includes)
CCFLAGSO += $(SuperLU_Includes)/src

# over-ride:
# FLIBS=   -L$(LAPACK) -llapack -lblas $(FORTRAN_LIBS) 
# FFLAGSO = $(FFLAGSG)

PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include  -I$(PETSC_DIR)/bmake/$(PETSC_ARCH)  -I$(PETSC_DIR)/src/sys/src/mpiuni
# for version 2.2.1
PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH) -I$(PETSC_DIR)/include/mpiuni
usePPP = $(USE_PPP_FLAG)
ifneq ($(strip $(usePPP)),)
# P++ version
 PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH)
 PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc $(HYPRE_LIBS)
else
 PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc -lmpiuni

endif


oges = ..
VPATH = $(oges)

linkFiles:
	ln -sf $(oges)/PETScEquationSolver.C .
	ln -sf $(oges)/PETScSolver.C .
	ln -sf $(oges)/buildEquationSolvers.C .


all = tos
all: $(all);

# These files we build dependency info for:
SRC = tos.C 

PETScEquationSolver.o: PETScEquationSolver.C
	$(CC) $(CCFLAGSF) -c $<

PETScSolver.o: PETScSolver.C
	$(CC) $(CCFLAGSF) -c $<

# test the parallel interface to PETSc

SUPER_LU_DIST_LIB = /home/henshaw/software/SuperLU_DIST_2.0/superlulinux.a
SUPER_LU_DIST_LIB = 

# *********** test the parallel interface to PETSc from Overture *************
# tos = tos.o PETScSolver.o ../buildEquationSolvers.o ../PETScEquationSolver.o
tos = tos.o PETScSolver.o buildEquationSolvers.o PETScEquationSolver.o
tos: $(tos) 
	$(CC) $(CCFLAGS) -o tos $(tos)  \
              $(PETSC_LIBS) \
	      $(SUPER_LU_DIST_LIB) \
	       $(GLIBS) $(CLIBS) -L$(LAPACK) -llapack  $(FLIBS)



clean:;	rm -rf *.o tos

# to remake the dependencies: make dep
dep:;
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend

.PRECIOUS: 

