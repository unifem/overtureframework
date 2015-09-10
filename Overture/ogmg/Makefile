# Use apple: 
include ../config/Makefile.$(VENDOR)
# include ../../Overture/config/Makefile.intel
# over-ride:
# CC=insure

# setenv compile [opt|dbg]
# compile everything with optimization: (setenv compile opt)
ifeq ($(compile),opt)
CCFLAGS= $(CCFLAGSF)
else
# compile without opt:
CCFLAGSF= $(CCFLAGSG)
CCFLAGSO= $(CCFLAGSG)
endif
ifeq ($(compile),dbg)
FFLAGSF = $(FFLAGSG)
FFLAGSO = $(FFLAGSG)
endif

current = .
oges = $(current)/../oges
gf = $(current)/../gf
ogshow = $(current)/../ogshow
grid = $(current)/../grid
otherStuff = $(current)/../otherStuff
opsrc = $(current)/../op/src
mapping = $(current)/../mapping
parallel = $(current)/../parallel
# VPATH = $(gf):$(grid):$(otherStuff):$(opsrc):$(mapping)
VPATH = $(gf):$(grid):$(opsrc):$(mapping):$(ogshow):$(parallel)

# NOTE: I had to put flibs last on the dec to move -L/usr/local/lib to the end (conflicted with hdf, opengl)
LAPACK_LIBS = -L$(LAPACK) -llapack -lblas 
FLIBS=   $(FORTRAN_LIBS) $(AppLibraries) 
ifeq ($(VENDOR),apple)
  # MAC: 
  FLIBS=   -llapack -lblas $(FORTRAN_LIBS) $(AppLibraries) -Wl-framework,Accelerate  -L/usr/local/gfortran/lib -lgfortran 
endif

# apple:
# FORTRAN_LIBS = -lgfortran
# FLIBS=  -llapack -lblas $(FORTRAN_LIBS) $(AppLibraries) 


# FFLAGS = $(FFLAGSF)

Oges = ../oges

# useHypre := on
# 
# ifeq ($(strip $(usePPP)),)
# PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include  -I$(PETSC_DIR)/bmake/$(PETSC_ARCH) -I$(PETSC_DIR)/include/mpiuni
# # PETSC_LIBS = -L$(PETSC_LIB) -lpetscsles -lpetscdm -lpetscmat -lpetscvec -lpetsc -lmpiuni
# PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc -lmpiuni
# OGES_PETSC = buildEquationSolvers.o PETScEquationSolver.o 
# else
#  PETSC_INCLUDE := -I$(PETSC_DIR)/include  -I$(PETSC_DIR)/bmake/$(PETSC_ARCH) -DOVERTURE_USE_PETSC
#  PETSC_LIBS := -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc 
#  PETSC_LIBS += $(HYPRE_LIBS)
# OGES_PETSC := buildEquationSolvers.o \
#               PETScEquationSolver.o  \
#               MultigridEquationSolver.o \
# 	      PETScSolver.o
#   ifeq ($(useHypre),on)
#     HYPRE = $(PETSC_DIR)/externalpackages/hypre-1.11.1b/linux-gnu-opt
#     HYPRE_LIBS = -Wl,-rpath,$(HYPRE)/lib -L$(HYPRE)/lib -lHYPRE_DistributedMatrix -lHYPRE_DistributedMatrixPilutSolver -lHYPRE_Euclid -lHYPRE_IJ_mv -lHYPRE_LSI -lHYPRE_MatrixMatrix -lHYPRE_ParaSails -lHYPRE_krylov -lHYPRE_parcsr_ls -lHYPRE_parcsr_mv -lHYPRE_seq_mv -lHYPRE_sstruct_ls -lHYPRE_sstruct_mv -lHYPRE_struct_ls -lHYPRE_struct_mv
# 
#     PETSC_LIBS += $(HYPRE_LIBS)
#   endif
# 
# endif
# # apple: no petsc for now
# # PETSC_LIBS =
# # OGES_PETSC =

all = linkFiles ogmgt
all: $(all);


linkFiles:
	ln -sf $(Overture)/include/defineDiffOrder2f.h .
	ln -sf $(Overture)/include/defineDiffOrder4f.h .
	ln -sf $(OvertureCheckout)/oges/buildEquationSolvers.C .
	ln -sf $(OvertureCheckout)/oges/MultigridEquationSolver.C .
	ln -sf $(OvertureCheckout)/oges/EquationSolver.C .
	ln -sf $(OvertureCheckout)/oges/PETScEquationSolver.C .
	ln -sf $(OvertureCheckout)/oges/PETScSolver.C .
	ln -sf $(Overture)/include/derivMacroDefinitions.h .
	ln -sf $(Overture)/include/defineParametricDerivMacros.h .
	ln -sf doc/neumannEquationLineBC3d.h .

# ../gf/doubleGridCollectionFunction.o \
#       ../gf/doubleMappedGridFunction.o  ../gf/doubleCompositeGridFunction.o

OGMG = Ogmg.o smooth.o defect.o fineToCoarse.o coarseToFine.o ogmgTests.o ogmgUtil.o lineSmooth.o  \
      boundaryConditions.o OgmgParameters.o operatorAveraging.o buildExtraLevels.o checkGrid.o displayMaskLaTeX.o \
      defectOpt.o smoothOpt.o singular.o averageOpt.o bcOpt.o predefined.o lineSmoothOpt.o smoothBoundary.o \
      assignBoundaryConditionCoefficients.o \
      defect2dOrder2.o defect2dOrder4.o defect3dOrder2.o defect3dOrder4.o  \
      smoothRB2dOrder2.o  smoothRB2dOrder4.o smoothRB3dOrder2.o smoothRB3dOrder4.o \
      smoothJAC2dOrder2.o  smoothJAC2dOrder4.o smoothJAC3dOrder2.o smoothJAC3dOrder4.o \
      smOpt.o smRB2dOrder2.o  smRB2dOrder4.o smRB3dOrder2.o smRB3dOrder4.o buildExtraLevelsNew.o bc3dOrder4.o


ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../mapping/stencilWalk.o ../mapping/Mapping.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../parallel/CanInterpolate.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) checkOverlappingGrid.o ../amr/Interpolate.o ../ogshow/CopyArray.o ../otherStuff/displayCoeff.o ../gf/Interpolant.o ../gf/ParallelOverlappingGridInterpolator.o ../ogen/classify.o ../parallel/CanInterpolate.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) checkOverlappingGrid.o ../op/src/fixBoundaryCorners.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) checkOverlappingGrid.o ../gf/Interpolant.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../../Overture/gf/Interpolant.o ../../Overture/gf/ParallelOverlappingGridInterpolator.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../../Overture/op/src/applyBCC.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../../Overture/mapping/Mapping.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../../Overture/parallel/CanInterpolate.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../../Overture/grid/MappedGrid.o ../../Overture/mapping/dpmMap.o ../../Overture/mapping/DataPointMapping.o ../../Overture/hype/HyperbolicMapping.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../../Overture/mapping/dpmInverse.o ../../Overture/mapping/StretchMapping.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../ogshow/gridOpt.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../../Overture/op/src/applyBCC.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../../Overture/ogshow/ParallelGridUtility.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) ../../Overture/grid/GridCollection.o ../../Overture/grid/CompositeGrid.o ../../Overture/ogshow/ParallelGridUtility.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) GridCollection.o CompositeGrid.o ParallelGridUtility.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) GridCollection.o LoadBalancer.o
ogmg= ogmgt.o $(OGMG) $(OGES_PETSC) 

# NOTE: put libGL last for parallel or else function perm() from libGL is used by mistake by PARTI
ogmgt: $(ogmg) linkFiles 
	$(CC) $(CCFLAGS) -o ogmgt $(ogmg) $(CLIBS_NoOgmg)  $(PETSC_LIBS) $(FLIBS) $(GLIBS) $(LAPACK_LIBS)

# ogmgp= ogmgt.o $(OGMG) $(OGES_PETSC) ../gf/Interpolant.o
# ogmgp: $(ogmgp)
#	$(CC) $(CCFLAGS) -o ogmgp $(ogmgp) $(CLIBS_NoOgmg)  $(PETSC_LIBS)  $(GLIBS) $(FLIBS)



assignBoundaryConditions.f: assignBoundaryConditions.bf; bpp assignBoundaryConditions.bf
assignBoundaryConditions.o: assignBoundaryConditions.f
	$(FC) $(FFLAGSG) -c $<

averageOpt.f: averageOpt.bf; bpp averageOpt.bf
averageOpt.o: averageOpt.f
	$(FC) $(FFLAGSG) -c $<

buildExtraLevelsNew.C: buildExtraLevelsNew.bC; bpp buildExtraLevelsNew.bC

assignBoundaryConditionCoefficients.C: assignBoundaryConditionCoefficients.bC; bpp assignBoundaryConditionCoefficients.bC

fineToCoarse.C: fineToCoarse.bC; bpp fineToCoarse.bC
fineToCoarse.o: fineToCoarse.C
	$(CC) $(CCFLAGSF) -c $<

smoothRB2dOrder2.f : smoothOpt.bf
smoothRB2dOrder4.f : smoothOpt.bf
smoothRB3dOrder2.f : smoothOpt.bf
smoothRB3dOrder4.f : smoothOpt.bf

smRB2dOrder2.f : smOpt.bf
smRB2dOrder4.f : smOpt.bf
smRB3dOrder2.f : smOpt.bf
smRB3dOrder4.f : smOpt.bf

smoothJAC2dOrder2.f : smoothOpt.bf
smoothJAC2dOrder4.f : smoothOpt.bf
smoothJAC3dOrder2.f : smoothOpt.bf
smoothJAC3dOrder4.f : smoothOpt.bf

# smoothRB2dOrder2.o : smoothRB2dOrder2.f; $(FC) $(FFLAGSG) -c $<

smoothOpt.f: smoothOpt.bf; bpp smoothOpt.bf
smoothOpt.o: smoothOpt.f
#	$(FC) $(FFLAGS) -c $<
	$(FC) $(FFLAGSF) -c $<

smOpt.f: smOpt.bf; bpp smOpt.bf
smOpt.o: smOpt.f
	$(FC) $(FFLAGSF) -c $<

lineSmoothOpt.f: lineSmoothOpt.bf; $(BPP) -quiet -clean -I$(Overture)/include lineSmoothOpt.bf
lineSmoothOpt.o: lineSmoothOpt.f
	$(FC) $(FFLAGSF) -c $<

bcOpt.f: bcOpt.bf; $(BPP) -quiet -clean -I$(Overture)/include bcOpt.bf
bcOpt.o: bcOpt.f; $(FC) $(FFLAGSG) -c $<
# bcOpt.o: bcOpt.f; $(FC) $(FFLAGSF) -c $<

bc3dOrder4.f : bc3dOrder4.bf neumannEquationBC3d.h neumannEquationForcing.h; $(BPP) -quiet -clean -I$(Overture)/include -Idoc bc3dOrder4.bf
bc3dOrder4.o: bc3dOrder4.f; $(FC) $(FFLAGSG) -c $<

defect2dOrder2.f : defectOpt.bf; bpp defectOpt.bf
defect2dOrder4.f : defectOpt.bf; bpp defectOpt.bf
defect3dOrder2.f : defectOpt.bf; bpp defectOpt.bf
defect3dOrder4.f : defectOpt.bf; bpp defectOpt.bf

defectOpt.f: defectOpt.bf; bpp defectOpt.bf
defectOpt.o: defectOpt.f
#	$(FC) $(FFLAGS) -c $<
	$(FC) $(FFLAGSF) -c $<

# preprocessor version:
smooth.p.C: smooth.C; opp smooth.C
smooth.p.o: smooth.p.C
	$(CC) $(CCFLAGSF) -c $<

smooth.C: smooth.bC; bpp smooth.bC
smooth.o: smooth.C
#	$(CC) $(CCFLAGSG) -c $<
	$(CC) $(CCFLAGSF) -c $<

lineSmooth.C: lineSmooth.bC; bpp lineSmooth.bC
lineSmooth.o: lineSmooth.C
#	$(CC) $(CCFLAGSG) -c $<
	$(CC) $(CCFLAGSF) -c $<

buildExtraLevels.o: buildExtraLevels.C
#	$(CC) $(CCFLAGSG) -c $<
	$(CC) $(CCFLAGSF) -c $<

boundaryConditions.C: boundaryConditions.bC ; bpp boundaryConditions.bC
boundaryConditions.o: boundaryConditions.C
#	$(CC) $(CCFLAGS) -c $<
	$(CC) $(CCFLAGSF) -c $<

ogmgUtil.o: ogmgUtil.C
	$(CC) $(CCFLAGSF) -c $<

predefined.o: predefined.C
#	$(CC) $(CCFLAGS) -c $<
	$(CC) $(CCFLAGSF) -c $<

TridiagonalSolver.o: TridiagonalSolver.C
	$(CC) $(CCFLAGSF) -c $<

defect.p.C: defect.C; opp defect.C
defect.p.o: defect.p.C
	$(CC) $(CCFLAGSF) -c $<

boundaryConditions.p.C: boundaryConditions.C; opp boundaryConditions.C
boundaryConditions.p.o: boundaryConditions.p.C
	$(CC) $(CCFLAGSF) -c $<

fixBoundaryCorners.p.o: fixBoundaryCorners.p.C
	$(CC) $(CCFLAGSF) -c $<
Interpolant.p.o: Interpolant.p.C
	$(CC) $(CCFLAGSO) -c $<


# Here is the library 
AR = ar
ARFLAGS = ruv
liba: 
	rm -f libOgmg.a
	$(AR) $(ARFLAGS) libOgmg.a ${OGMG}


mgExample1= mgExample1.o 
mgExample1: $(mgExample1) 
	$(CC) $(CCFLAGS) -o mgExample1 $(mgExample1) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# show how Galerkin averaging averages the grid
galerkin= galerkin.o $(OGMG) $(OGES_PETSC)
galerkin: $(galerkin) 
	$(CC) $(CCFLAGS) -o galerkin $(galerkin) \
            $(CLIBS)  $(PETSC_LIBS) $(FLIBS) $(GLIBS)


# test 1D elliptic solver
ell= ell.o 
ell: $(ell) 
	$(CC) $(CCFLAGS) -o ell $(ell) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# test 1D singular problems
sing= sing.o
sing: $(sing) 
	$(CC) $(CCFLAGS) -o sing $(sing) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# test poor man's MG
pmmg= pmmg.o ../grid/CompositeGrid.o operatorAveraging.o
pmmg= pmmg.o $(OGMG)
pmmg: $(pmmg) 
	$(CC) $(CCFLAGS) -o pmmg $(pmmg) \
        $(CLIBS) $(FLIBS) $(GLIBS)

bug= bug.o $(OGMG)
bug: $(bug) 
	$(CC) $(CCFLAGS) -o bug $(bug) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# find a bug in the move and solve with MG
moveAndSolve= moveAndSolve.o $(OGMG) $(OGES_PETSC) $(OvertureCheckout)/oges/MultigridEquationSolver.o
moveAndSolve: $(moveAndSolve) 
	$(CC) $(CCFLAGS) -o moveAndSolve $(moveAndSolve) \
            $(CLIBS)  $(PETSC_LIBS) $(FLIBS) $(GLIBS)


LIB_FILES = Ogmg.o smooth.o defect.o fineToCoarse.o coarseToFine.o ogmgTests.o ogmgUtil.o lineSmooth.o \
       boundaryConditions.o OgmgParameters.o operatorAveraging.o buildExtraLevels.o checkGrid.o \
       displayMaskLaTeX.o singular.o \
       defectOpt.o smoothOpt.o averageOpt.o bcOpt.o

# AR = ar
# ARFLAGS = ruv
# lib: $(LIB_FILES)
# 	$(AR) $(ARFLAGS) libOgmg.a $(LIB_FILES)


clean:;	rm -f ogmgt ogmgp mgExample1 *.o pmmg sing ell *.debug 
cleano:; rm -f *.o 

cleanc:;	rm -f check/*.check check/*.info


#		
#		DEPENDENCIES
#		

SRC = *.C 

# to remake the dependencies: make dep
dep:;
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend


lineSmoothOpt.f : neumannEquationBC.h neumannEquationForcing.h neumannEquationBC.new.h doc/neumannEquationLineBC3d.h
bcOpt.f : neumannEquationBC.h neumannEquationForcing.h neumannEquationBC.new.h


.PRECIOUS:

