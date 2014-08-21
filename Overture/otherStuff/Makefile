include ../config/Makefile.$(VENDOR)

# CC = insure

# over-ride defaults here
# FFLAGSO= $(FFLAGS)
# FFLAGSO= $(FFLAGSO4)
current = .
grid = $(current)/../grid
gf = $(current)/../gf
mapping = $(current)/../mapping

VPATH = $(grid):$(gf):$(mapping)

# for version 2.2.1
# PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH) -I$(PETSC_DIR)/include/mpiuni
# usePPP = $(USE_PPP_FLAG)
# ifneq ($(strip $(usePPP)),)
# # P++ version
#  PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH)
#  PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc $(HYPRE_LIBS) $(LAPACK_LIBRARIES)
# else
#  PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc -lmpiuni
# 
# endif

all = 
all: $(all);

otherStuffFiles = OGgetIndex.C getFromADataBase.C OGPolyFunction.bC OGFunction.C OGTrigFunction.C display.C \
                  displayMask.C Integrate.C TridiagonalSolver.C ../mapping/sPrintF.C FortranIO.C \
                  OGPulseFunction.C ../gf/OvertureInit.C

otherStuff:
	extract.p $(otherStuffFiles)
	latex otherStuff
	makeindex otherStuff
	latex otherStuff
	dvips -P pdf -G0 -o otherStuff.ps otherStuff.dvi 
	ps2pdf otherStuff.ps
	cp otherStuff.pdf $(WEBPAGE)/documentation

otherStuffhtml: 
	lh.p otherStuff.tex
	latex otherStuffHTML
	# bibtex otherStuffHTML
	makeindex otherStuffHTML.idx
	latex otherStuffHTML
	latex2html -local_icons otherStuffHTML
	rm -rf $(WEBPAGE)/documentation/otherStuffHTML
	mkdir $(WEBPAGE)/documentation/otherStuffHTML
	cp otherStuffHTML/*.* $(WEBPAGE)/documentation/otherStuffHTML
	rm -rf otherStuffHTML

cutCellsDoc:
	extract.p CutCells.C
	latex cutCells
	makeindex cutCells


doc: otherStuff 

SRC = tz.C OGFunction.C OGPolyFunction.C OGTrigFunction.C OGPulseFunction.C \
      td.C intDisplay.C floatDisplay.C doubleDisplay.C DisplayParameters.C \
      tfio.C FortranIO.C tri.C TridiagonalSolver.C TridiagonalFactor.C testIntegrate.C  Integrate.C \
      cutCells.C divide.C removeCoveredPoints.C dicer.C tdicer.C \
      cut.C CutCells.C Polygon.C dice.C triangulate.C buildCutCells.C cutBoundary.C 


BodyForce.C : BodyForce.bC; $(BPP) -quiet -clean $<

# 
polyFunction1D.f : polyFunction.bf; $(BPP) polyFunction.bf

polyEvaluate1D.F : polyFunction.bf; $(BPP) polyFunction.bf
OGPolyFunction.C : OGPolyFunction.bC; $(BPP) OGPolyFunction.bC
displayMask.C    : displayMask.bC; $(BPP) displayMask.bC


# test out the twilight-zone functions
tz = tz.o OGFunction.o OGPolyFunction.o OGTrigFunction.o OGPulseFunction.o \
     polyEvaluate1D.o polyEvaluate2D.o polyEvaluate3D.o \
     polyFunction.o polyFunction1D.o polyFunction2D.o polyFunction2D5.o polyFunction2D6.o \
     polyFunction3D.o polyFunction3D4.o polyFunction3D5.o polyFunction3D6.o 
# tz = tz.o 
# tz = tz.o OGFunction.o OGPolyFunction.o OGTrigFunction.o OGPulseFunction.o 
tz: $(tz)
	$(CC) $(CCFLAGS) -o tz $(tz) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# test the display function
td= td.o intDisplay.o floatDisplay.o doubleDisplay.o DisplayParameters.o
td: $(td) 
	$(CC) $(CCFLAGS) -o td $(td) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# test fortran IO 
tfio= tfio.o FortranIO.o fortranIO.o
tfio: $(tfio) 
	$(CC) $(CCFLAGS) -o tfio $(tfio) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# test Bill's tridiagonal solver
pentaDiagonal.f : pentaDiagonal.bf; $(BPP) pentaDiagonal.bf
triDiagonal.f : triDiagonal.bf; $(BPP) triDiagonal.bf

# TridiagonalFactor.o : TridiagonalFactor.C
# 	$(CC) $(CCFLAGSF) -c TridiagonalFactor.C

TridiagonalFactor.o : TridiagonalFactor.C
	$(CC) $(CCFLAGSG) -c TridiagonalFactor.C

ParallelTridiagonalSolver.o : ParallelTridiagonalSolver.C
	$(CC) $(CCFLAGSG) -c ParallelTridiagonalSolver.C

# Test the tridiagonal solver: 
tri= tri.o TridiagonalSolver.o TridiagonalFactor.o pentaDiagonal.o
tri: $(tri) 
	$(CC) $(CCFLAGS) -o tri $(tri) $(CLIBS) $(FLIBS) $(GLIBS)

# Test the (TEMP) PARALLEL tridiagonal solver: 
parallelTri= parallelTri.o ParallelTridiagonalSolver.o TridiagonalSolver.o TridiagonalFactor.o pentaDiagonal.o
parallelTri: $(parallelTri) 
	$(CC) $(CCFLAGS) -o parallelTri $(parallelTri) $(CLIBS) $(FLIBS) $(GLIBS)

# Test the NESTED DISECTION PARALLEL tridiagonal solver: FINISH ME
trip= trip.o TridiagonalSolver.o TridiagonalFactor.o pentaDiagonal.o
trip: $(trip) 
	$(CC) $(CCFLAGS) -o trip $(trip) $(CLIBS) $(FLIBS) $(GLIBS)

# test templates
tm= tm.o
tm: $(tm) 
	$(CC) $(CCFLAGS) -o tm $(tm) -lm


# these next files allow PETSc to be used with Overture. No need to link to these otherwise.
Oges = ../oges
buildEquationSolvers.o : $(Oges)/buildEquationSolvers.C
	$(CC) $(CCFLAGS) -DOVERTURE_USE_PETSC -c $(Oges)/buildEquationSolvers.C
PETScEquationSolver.o : $(Oges)/PETScEquationSolver.C
	$(CC) $(CCFLAGS) -DOVERTURE_USE_PETSC -c $(Oges)/PETScEquationSolver.C
PETScSolver.o : $(Oges)/PETScSolver.C
	$(CC) $(CCFLAGS) -DOVERTURE_USE_PETSC -c $(Oges)/PETScSolver.C

# PETSC =  buildEquationSolvers.o PETScEquationSolver.o
# PETSC = 

# Test the Integrate class
testIntegrate= testIntegrate.o  Integrate.o  BodyDefinition.o $(PETSC) 
testIntegrate= testIntegrate.o  Integrate.o BodyDefinition.o ../ugen/TriangleSource.o \
           ../mapping/IntersectionMapping.o ../ugen/CompositeGridHybridConnectivity.o ../ogshow/grid.o
testIntegrate= testIntegrate.o  Integrate.o ../ugen/SurfaceStitcher.o ../ugen/Ugen.o ../mapping/IntersectionMapping.o \
        ../ugen/sealHoles.o
testIntegrate= testIntegrate.o  Integrate.o BodyDefinition.o $(PETSC)
testIntegrate: $(testIntegrate)
	$(CC) $(CCFLAGS) -o testIntegrate $(testIntegrate) $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)

testIntegrateOld= testIntegrate.o  IntegrateOld.o $(PETSC)
testIntegrateOld: $(testIntegrateOld)
	$(CC) $(CCFLAGS) -o testIntegrateOld $(testIntegrateOld) $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)

# -- compute integration weights and save with the grid file.
ciw = ciw.o Integrate.o BodyDefinition.o $(PETSC)
ciw: $(ciw)
	$(CC) $(CCFLAGS) -o ciw $(ciw) $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)



# from Kara Maki:
testint= testint.o  Integrate.o $(PETSC) 
testint: $(testint)
	$(CC) $(CCFLAGS) -o testint $(testint) $(CLIBS) $(PETSC_LIBS) $(FLIBS) $(GLIBS)

cutCells= cut.o CutCells.o divide.o removeCoveredPoints.o dicer.o Polygon.o dice.o triangulate.o buildCutCells.o \
     cutBoundary.o BodyDefinition.o
cutCells: $(cutCells) 
	$(CC) $(CCFLAGS) -o cutCells $(cutCells) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# old cut cells:
cut0= cutCells.o divide.o removeCoveredPoints.o dicer.o ../mapping/dpmInverse.o
cut0= cutCells.o divide.o removeCoveredPoints.o dicer.o 
cut0: $(cut0) 
	$(CC) $(CCFLAGS) -o cut0 $(cut0) \
        $(CLIBS) $(FLIBS) $(GLIBS)

tdicer= tdicer.o dice.o Polygon.o triangulate.o 
tdicer: $(tdicer) 
	$(CC) $(CCFLAGS) -o tdicer $(tdicer) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# olde dicer
tdicer0= tdicer0.o divide.o dicer0.o
tdicer0: $(tdicer0) 
	$(CC) $(CCFLAGS) -o tdicer0 $(tdicer0) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# test the polygon triangulator
polytri= polytri.o triangulate.o construct.o misc.o monotone.o Polygon.o
polytri= polytri.o triangulate.o Polygon.o dice.o
polytri: $(polytri) 
	$(CC) $(CCFLAGS) -o polytri $(polytri) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# test unstructured mapping
tu= tu.o 
tu: $(tu) 
	$(CC) $(CCFLAGS) -o tu $(tu) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# check memory usage and sizeOf() functions in Overture class.
memoryUsage= memoryUsage.o MappedGrid.o MappedGridGeometry1.o
memoryUsage= memoryUsage.o doubleMappedGridFunction.o doubleGridCollectionFunction.o doubleCompositeGridFunction.o
memoryUsage= memoryUsage.o lap.o CompositeGrid.o GridCollection.o 
mu: $(memoryUsage) 
	$(CC) $(CCFLAGS) -o mu $(memoryUsage) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# test of sparse array class
sa = sa.o
sa: $(sa)
	$(CC) $(CCFLAGS) -o sa $(sa) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS)


ciw.o : Integrate.h
testIntegrate.o : Integrate.h
Integrate.o : Integrate.h


clean:;	rm -f *.o tri testIntegrate tfio td memoryUsage sa

# to remake the dependencies:  make dep
dep: 
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend

.PRECIOUS:

