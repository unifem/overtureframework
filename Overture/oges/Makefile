include ../config/Makefile.$(VENDOR)

SuperLU = /home/henshaw/software/SuperLU_3.0
SuperLU_Includes = -I$(SuperLU)/SRC	

SuperLU = /home/henshaw/software/SuperLU_DIST_2.0
SuperLU_Includes = -I$(SuperLU)/src	

CCFLAGS  += $(SuperLU_Includes)
CCFLAGSG += $(SuperLU_Includes)
CCFLAGSO += $(SuperLU_Includes)/src

# over-ride:
# FLIBS=   -L$(LAPACK) -llapack -lblas $(FORTRAN_LIBS) 
FFLAGSO = $(FFLAGSG)

# LAPACK_LIBRARIES = -Wl,-rpath,/nfs/apps/pgi/6.1-3/linux86-64/6.1/lib -L/nfs/apps/pgi/6.1-3/linux86-64/6.1/lib -llapack -lblas
PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include  -I$(PETSC_DIR)/bmake/$(PETSC_ARCH)  -I$(PETSC_DIR)/src/sys/src/mpiuni
# for version 2.2.1
PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH) -I$(PETSC_DIR)/include/mpiuni
usePPP = $(USE_PPP_FLAG)
ifneq ($(strip $(usePPP)),)
# P++ version
 PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH)
 PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc $(HYPRE_LIBS) $(LAPACK_LIBRARIES)
else
 PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc -lmpiuni

endif
# PETSC_INCLUDE =

# HYPRE = /home/henshaw.0/petsc/petsc-2.3.2-mpi/externalpackages/hypre-1.11.1b/linux-gnu-opt
# HYPRE_LIBS -Wl,-rpath,$(HYPRE)/lib -L$(HYPRE)/lib -lHYPRE_DistributedMatrix -lHYPRE_DistributedMatrixPilutSolver -lHYPRE_Euclid -lHYPRE_IJ_mv -lHYPRE_LSI -lHYPRE_MatrixMatrix -lHYPRE_ParaSails -lHYPRE_krylov -lHYPRE_parcsr_ls -lHYPRE_parcsr_mv -lHYPRE_seq_mv -lHYPRE_sstruct_ls -lHYPRE_sstruct_mv -lHYPRE_struct_ls -lHYPRE_struct_mv


Oges = ../oges
OGES_PETSC = buildEquationSolvers.o PETScEquationSolver.o



current = .
oges = $(current)/../oges
gf = $(current)/../gf
grid = $(current)/../grid
otherStuff = $(current)/../otherStuff
opsrc = $(current)/../op/src
VPATH = $(oges):$(gf):$(grid):$(otherStuff):$(opsrc)


all = buildEquationSolvers.o  MultigridEquationSolver.o  PETScEquationSolver.o
all: $(all);

texFiles = determineErrors.C ogesFiles.C Oges.C OgesParameters.C buildEquationSolvers.C
oges:
	extract.p $(texFiles)
	latex oges
	makeindex oges.idx
	latex oges
	dvips -P pdf -G0 -o oges.ps oges.dvi 
	ps2pdf oges.ps

ogesweb: oges
	cp oges.pdf $(WEBPAGE)/documentation

ogeshtml: 
	lh.p oges.tex
	latex ogesHTML
	bibtex ogesHTML
	makeindex ogesHTML.idx
	latex ogesHTML
	latex2html -local_icons ogesHTML
	rm -rf $(WEBPAGE)/documentation/ogesHTML
	mkdir $(WEBPAGE)/documentation/ogesHTML
	cp ogesHTML/*.* $(WEBPAGE)/documentation/ogesHTML
	rm -rf ogesHTML

doc: ogesweb 

# These files we build dependency info for:
SRC = Oges.C ogesFiles.C OgesParameters.C formMatrix.C generateMatrix.C buildEquationSolvers.C \
      EquationSolver.C PETScEquationSolver.C YaleEquationSolver.C SlapEquationSolver.C HarwellEquationSolver.C \
      MultigridEquationSolver.C determineErrors.C \
      toges.C subset.C SparseSolver.C tss.C


PETScEquationSolver.o: PETScEquationSolver.C
	$(CC) $(CCFLAGSF) -c $<

PETScSolver.o: PETScSolver.C
	$(CC) $(CCFLAGSG) -c $<

PETSC_SOLVER = PETScEquationSolver.o
# parallel:
# PETSC_SOLVER = PETScEquationSolver.o PETScSolver.o

# Tests Overlapping Grid Equation Solver: oges

# Here are the files we are working on
Oges= Oges.o oges.o OgesParameters.o formMatrix.o generateMatrix.o buildEquationSolvers.o \
      EquationSolver.o PETScEquationSolver.o YaleEquationSolver.o SlapEquationSolver.o HarwellEquationSolver.o \
      MultigridEquationSolver.o determineErrors.o \
      obsolete.o csort.o ../ogmg/Ogmg.o ../ogmg/boundaryConditions.o ../ogmg/defect.o
Oges= Oges.o ogesFiles.o OgesParameters.o formMatrix.o generateMatrix.o buildEquationSolvers.o \
      EquationSolver.o PETScEquationSolver.o YaleEquationSolver.o SlapEquationSolver.o HarwellEquationSolver.o \
      MultigridEquationSolver.o determineErrors.o PETScSolver.o \
      obsolete.o csort.o 
Oges= buildEquationSolvers.o PETScEquationSolver.o PETScSolver.o
Oges= buildEquationSolvers.o PETScEquationSolver.o HarwellEquationSolver.o Oges.o
Oges= Oges.o ogesFiles.o OgesParameters.o formMatrix.o generateMatrix.o buildEquationSolvers.o \
      EquationSolver.o $(PETSC_SOLVER) YaleEquationSolver.o SlapEquationSolver.o HarwellEquationSolver.o \
      MultigridEquationSolver.o determineErrors.o 
toges= toges.o $(Oges) 
toges: $(toges) 
	$(CC) $(CCFLAGS) -o toges \
        $(toges) $(CLIBS)  $(PETSC_LIBS) $(LAPACK_LIBRARIES) $(FLIBS) $(GLIBS)

subset= $(Oges) subset.o 
subset: $(subset) 
	$(CC) $(CCFLAGS) -o subset \
        $(subset) $(CLIBS) $(GLIBS)

tcm3 = $(Oges) tcm3.o
tcm3: $(tcm3)
	$(CC) $(CCFLAGS) -o tcm3 \
        $(tcm3) $(CLIBS) $(PETSC_LIBS) $(FLIBS) $(GLIBS)


tcm4= $(Oges) tcm4.o 
tcm4: $(tcm4) 
	$(CC) $(CCFLAGS) -o tcm4 \
        $(tcm4) $(CLIBS) $(GLIBS)

tcmPartial= $(Oges) tcmPartial.o 
tcmPartial: $(tcmPartial) 
	$(CC) $(CCFLAGS) -o tcmPartial \
        $(tcmPartial) $(CLIBS) $(GLIBS)

# check for leaks
tcmLeak = $(Oges) tcmLeak.o
tcmLeak: $(tcmLeak)
	$(CC) $(CCFLAGS) -o tcmLeak \
        $(tcmLeak) $(CLIBS) $(PETSC_LIBS) $(FLIBS) $(GLIBS)


lins= $(Oges) lins.o 
lins: $(lins) 
	$(CC) $(CCFLAGS) -o lins \
        $(lins) $(CLIBS) $(GLIBS)

tcm0= $(Oges) tcm0.o
tcm0: $(tcm0) 
	$(CC) $(CCFLAGS) -o tcm0 \
        $(tcm0) $(CLIBS) $(GLIBS)

# performance testing
perf= $(Oges) perf.o residual.o
perf= perf.o residual.o
perf: $(perf) 
	$(CC) $(CCFLAGS) -o perf \
        $(perf) $(CLIBS) $(FLIBS) $(GLIBS)

# --- Test the sparse interface solver ----
tis= tis.o $(Oges) cgesl1234.o yalesp.o
tis= tis.o $(Oges) 
tis: $(tis) 
	$(CC) $(CCFLAGS) -o tis \
        $(tis) $(CLIBS)  $(PETSC_LIBS) $(FLIBS) $(GLIBS)


#        ../A++/array.o 
oges1 = toges1.o 
toges1: $(oges1)
	$(CC) $(CCFLAGS) -o toges1 $(oges1) $(CLIBS) $(FLIBS)

ogesInt = togesInt.o
togesInt: $(ogesInt) Oges.a
	  $(CC) $(CCFLAGS) -o togesInt $(ogesInt) $(CLIBS) $(GLIBS)

# test user supplied coefficients -- vertex grid
ogesUSC = togesUSC.o 
togesUSC: $(ogesUSC)
	$(CC) $(CCFLAGS) -o togesUSC $(ogesUSC) $(CLIBS) $(GLIBS)

# test user supplied coefficients -- cell-centred grid
ogesUSCC = togesUSCC.o
togesUSCC: $(ogesUSCC)
	$(CC) $(CCFLAGS) -o togesUSCC $(ogesUSCC) $(CLIBS) $(GLIBS)

# test refactor option
ogesRF = togesRF.o 
togesRF: $(ogesRF)
	$(CC) $(CCFLAGS) -o togesRF $(ogesRF) $(CLIBS) $(GLIBS)

example7 = example7.o discreteVertex.o
example7: $(example7)
	$(CC) $(CCFLAGS) -o example7 $(example7) $(CLIBS) $(GLIBS)

# test refactor option
fvo = fvo.o
fvo: $(fvo)
	$(CC) $(CCFLAGS) -o fvo $(fvo) $(CLIBS) $(FLIBS) $(FVO)/lib/libFVO.a $(GLIBS)

iw = integrationWeights.o ogic.o ../gf/BoundaryOperators.o ../gf/SparseRep.o ogif.C cgesl1234.o oges.o
iw = integrationWeights.o ogic.o
iw: $(iw) 
	$(CC) $(CCFLAGS) -o iw $(iw) \
        $(CLIBS) $(GLIBS)

sample = sample.o 
sample: $(sample) 
	$(CC) $(CCFLAGS) -o sample $(sample) $(CLIBS) $(FLIBS)

testPetscInterface = testPetscInterface.o petscOverture.o csort.o
testPetscInterface: $(testPetscInterface) 
	$(CC) $(CCFLAGS) -o testPetscInterface \
        $(testPetscInterface) -L$(PETSC_LIB) -lpetscsles -lpetscdm -lpetscmat -lpetscvec -lpetsc  \
        $(CLIBS) $(FLIBS) -L/usr/local/lib -llapack $(GLIBS) -L$(PETSC_LIB) -lmpiuni 

# test the parallel interface to PETSc

SUPER_LU_DIST_LIB = /home/henshaw/software/SuperLU_DIST_2.0/superlulinux.a
SUPER_LU_DIST_LIB = 

# -----------------------------------------------------
# -- the tos program now sits in the results directory
# -----------------------------------------------------

tp2 = tp2.o MGOC.o 
tp2: $(tp2) 
	$(CC) $(CCFLAGS) -o tp2 \
        $(tp2)  -Wl,-rpath,/home/henshaw/petsc/petsc-2.2.1-mpi/lib/libg/linux_gcc_pgf90 -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc  \
	-Wl,-rpath,/usr/apps/MathWorks/matlab6.5R13/extern/lib/glnx86:/usr/apps/MathWorks/matlab6.5R13/sys/os/glnx86 -L/usr/apps/MathWorks/matlab6.5R13/extern/lib/glnx86 -L/usr/apps/MathWorks/matlab6.5R13/bin/glnx86 -leng -lmx -lmat -lut \
        $(CLIBS) $(FLIBS) -L/usr/local/lib -llapack $(GLIBS) 


# PETSc ex1.C 
ex1 = ex1.o 
ex1: $(ex1) 
	$(CC) $(CCFLAGS) -o ex1 \
        $(ex1) -L$(PETSC_LIB) -lpetscsles -lpetscdm -lpetscmat -lpetscvec -lpetsc  \
        $(CLIBS) $(FLIBS) -L/usr/local/lib -llapack $(GLIBS) -L$(PETSC_LIB) -lmpiuni 

# parallel PETSc examples
# src/ksp/examples/tutorials/ex2.c
ex2 = ex2.o 
ex2: $(ex2) 
	$(CC) $(CCFLAGS) -o ex2 $(ex2) -Wl,-rpath,$(PETSC_LIB) $(PETSC_LIBS) \
	         -L$(LAPACK) -llapack  $(CLIBS) $(FLIBS) $(GLIBS) 

# solve 3D Poisson with MG
# /home/henshaw.0/petsc/petsc-2.3.2-mpi/src/ksp/ksp/examples/tutorials/ex22.c
# PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscsnes -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc $(HYPRE_LIBS)

ex22 = ex22.o 
ex22: $(ex22) 
	$(CC) $(CCFLAGS) -o ex22 $(ex22) -Wl,-rpath,$(PETSC_LIB) $(PETSC_LIBS) \
	         -L$(LAPACK) -llapack  $(CLIBS) $(FLIBS) $(GLIBS) 



d: d.o
	$(CC) $(CCFLAGS) -o d d.o \
        -L$(A++) $(LIBAPP) $(LIVERLIB) -lF77 -lM77 -lV77


SUPER_LU_LIBS = $(SuperLU)/superlu_linux.a -L/usr/apps/pgi/5.1/linux86/5.1/lib -lpgftnrtl -lpgmp  -lpgc -lstd -lblas -lm
# test of SuperLU (from superlu.c)
exSLU = exSLU.o $(SuperLU)/EXAMPLE/sp_ienv.o
exSLU: $(exSLU)
	$(CC) $(CCFLAGS) -o $@ $(exSLU) $(SUPER_LU_LIBS)

# this test came from dlinsol.c
# ex2SLU < /home/henshaw/software/SuperLU_3.0/EXAMPLE/g10
ex2SLU = ex2SLU.o $(SuperLU)/EXAMPLE/sp_ienv.o
ex2SLU: $(ex2SLU)
	$(CC) $(CCFLAGS) -o $@ $(ex2SLU) $(SUPER_LU_LIBS)

# this test came from dlinsolx1.c
# ex3SLU < /home/henshaw/software/SuperLU_3.0/EXAMPLE/g10
ex3SLU = ex3SLU.o $(SuperLU)/EXAMPLE/sp_ienv.o
ex3SLU: $(ex3SLU)
	$(CC) $(CCFLAGS) -o $@ $(ex3SLU) $(SUPER_LU_LIBS)


# test the SparseSolver class -- interface to SuperLU and Yale

tss = tss.o SparseSolver.o $(SuperLU)/EXAMPLE/sp_ienv.o yalesp.o
tss: $(tss) 
	$(CC) $(CCFLAGS) -o $@ $(tss)  \
        $(CLIBS) $(FLIBS) $(SUPER_LU_LIBS) -L/usr/local/lib -llapack $(GLIBS) 


clean:;	rm -rf *.o toges iw tcm3 testPetscInterface perf lins ex2 tcmPartial exSLU ex2SLU ex3SLU 

# to remake the dependencies: make dep
dep:;
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend

.PRECIOUS: Oges.a
# DO NOT DELETE

Oges.o: OgesParameters.h
