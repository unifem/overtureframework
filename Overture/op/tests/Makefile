# include ../../config/Makefile.$(VENDOR)
include ../../config/Makefile.intel

# over-ride defaults here
# PURIFY  = purify $(PFLAGS) 

# over-ride some flags
FFLAGSO=     $(FFLAGS)
EXTRA_CC_FLAGS = -DUSE_OPT_OPERATORS
EXTRA_FFLAGS = -I../include

# for version 2.2.1
# PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH) -I$(PETSC_DIR)/include/mpiuni
# usePPP = $(USE_PPP_FLAG)
# ifneq ($(strip $(usePPP)),)
# P++ version
#  PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH)
# endif

# PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH) -I$(PETSC_DIR)/include/mpiuni
# LAPACK_LIBRARIES = -Wl,-rpath,/nfs/apps/pgi/6.1-3/linux86-64/6.1/lib -L/nfs/apps/pgi/6.1-3/linux86-64/6.1/lib -llapack -lblas



Oges = ../../oges
# PETSC_LIBS = -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc -lmpiuni
OGES_PETSC = buildEquationSolvers.o PETScEquationSolver.o


current = .
opsrc = $(current)/../src
deriv = $(current)/../deriv
coeff = $(current)/../coeff
fortranDeriv = $(current)/../fortranDeriv
fortranCoeff = $(current)/../fortranCoeff
bc = $(current)/../bc
VPATH = $(opsrc):$(deriv):$(coeff):$(fortranDeriv):$(fortranCoeff):$(bc)

all = tderivatives
all: $(all);


linkFiles:
	ln -sf $(Oges)/PETScEquationSolver.C .
	ln -sf $(Oges)/PETScSolver.C .
	ln -sf $(Oges)/buildEquationSolvers.C .


derivFiles = xFDeriv.o yFDeriv.o zFDeriv.o xxFDeriv.o xyFDeriv.o xzFDeriv.o yyFDeriv.o yzFDeriv.o  zzFDeriv.o \
             laplacianFDeriv.o

coeffFiles = opcoeff.o laplacianCoeff2.o xCoeff2.o yCoeff2.o zCoeff2.o xxCoeff2.o xyCoeff2.o xzCoeff2.o yyCoeff2.o \
              yzCoeff2.o zzCoeff2.o identityCoeff2.o divScalarGradCoeff2.o \
              laplacianCoeff4.o xCoeff4.o yCoeff4.o zCoeff4.o xxCoeff4.o xyCoeff4.o \
              xzCoeff4.o yyCoeff4.o yzCoeff4.o zzCoeff4.o identityCoeff4.o divScalarGradCoeff4.o \
              rCoeff2.o sCoeff2.o tCoeff2.o rrCoeff2.o rsCoeff2.o rtCoeff2.o ssCoeff2.o \
              stCoeff2.o ttCoeff2.o \
              rCoeff4.o sCoeff4.o tCoeff4.o rrCoeff4.o rsCoeff4.o \
              rtCoeff4.o ssCoeff4.o stCoeff4.o ttCoeff4.o


# dsg.f : dsg.bf; bpp dsg.bf

# dsg.o : dsg.f; $(FC) $(FFLAGSG) -c $*.f

# *** test of new coefficient matrix macros ***
# getcop.f : getcop.bf; bpp -quiet -clean -I$(Overture)/include -I../src $<
getcop.f : getcop.bf ../src/conservativeCoefficientMacros.h; bpp -quiet -clean -I$(Overture)/include -I../src $<
# getcop.o : getcop.f; pgf77 -o getcop.o -c getcop.f


tderivatives.C: tderivatives.bC; bpp tderivatives.bC


# test out all derivatives:
tderivatives= tderivatives.o coeff.o MGOD.o dsg.o
tderivatives= tderivatives.o Checker.o  $(derivFiles) coeff.o MGOD.o \
              dsg.o dsg2.o dsg3.o dsgr.o laplaceNC.o divScalarGradNC.o derivativeScalarDerivativeNC.o \
              $(coeffFiles)
tderivatives= tderivatives.o Checker.o  dsgc4.o ../fortranDeriv/dsg.o ../src/MGOD.o ../fortranDeriv/standardDerivative.o \
           ../fortranDeriv/dsgc6.o ../fortranDeriv/laplaceNC.o ../fortranDeriv/divScalarGradNC.o ../fortranDeriv/derivativeScalarDerivativeNC.o
tderivatives= tderivatives.o Checker.o  ../../mapping/AnnulusMapping.o
tderivatives= tderivatives.o Checker.o ../fortranDeriv/dsg.o ../fortranDeriv/dsgc4.o ../fortranDeriv/dsgc6.o \
              ../fortranDeriv/dsgr.o
tderivatives= tderivatives.o Checker.o ../src/MGOD.o ../fortranDeriv/dsg.o ../fortranDeriv/div.o 

tderivatives= tderivatives.o Checker.o ../fortranDeriv/dsg.o ../fortranDeriv/dsg2.o ../fortranDeriv/dsgr.o
tderivatives= tderivatives.o Checker.o ../fortranDeriv/dsg.o ../fortranDeriv/dsgr.o
tderivatives= tderivatives.o Checker.o 
tderivatives: $(tderivatives)
	$(CC) $(CCFLAGS) -o tderivatives $(tderivatives) $(CLIBS) $(FLIBS) $(GLIBS)

# Test the Dx1( s Dx2) operator
tdsd.C: tdsd.bC; bpp tdsd.bC
tdsd= tdsd.o Checker.o ../src/MGOD.o derivativeScalarDerivativeNC.o dsg.o
tdsd= tdsd.o Checker.o 
tdsd: $(tdsd)
	$(CC) $(CCFLAGS) -o tdsd $(tdsd) $(CLIBS) $(FLIBS) $(GLIBS)



# Test the div(tensor grad) operator
dtg.C: dtg.bC; bpp dtg.bC
dtg= dtg.o Checker.o ../fortranDeriv/dsg.o ../fortranDeriv/dsgc4.o ../fortranDeriv/dsgc6.o \
              ../fortranDeriv/dsgr.o ../fortranDeriv/dsg3.o
dtg: $(dtg)
	$(CC) $(CCFLAGS) -o dtg $(dtg) $(CLIBS) $(FLIBS) $(GLIBS)


# Test the symmetry of the symmetric (conservative) Laplace and div(s grad) operators
tdsg= tdsg.o Checker.o ../fortranDeriv/dsg.o ../fortranDeriv/dsg3.o ../src/MGOD.o
tdsg: $(tdsg)
	$(CC) $(CCFLAGS) -o tdsg $(tdsg) $(CLIBS) $(FLIBS) $(GLIBS)

# compute convergence rates for operators
crop= crop.o Checker.o 
crop: $(crop)
	$(CC) $(CCFLAGS) -o crop $(crop) $(CLIBS) $(FLIBS) $(GLIBS)

# %.o : %.C ; $(CC) $(CCFLAGS) -c $*.C

# test the coefficient matrix OPERATORS AND boundary conditions.
# perform timings too
tbcc.C: tbcc.bC; bpp tbcc.bC
finishBoundaryConditions.C : finishBoundaryConditions.bC; bpp finishBoundaryConditions.bC

tbcc.o: tbcc.C 
	$(CC) $(CCFLAGS) -c $<

tbcc= tbcc.o MGOC.o coeff.o applyBCC.o $(coeffFiles)
tbcc= tbcc.o Checker.o ../fortranDeriv/fixBCOptEdge1.o ../fortranDeriv/fixBCOptEdge2.o ../fortranDeriv/fixBCOptEdge3.o ../src/finishBoundaryConditions.o
tbcc= tbcc.o Checker.o setCoefficients.o getcop.o getCoeffOpOrder2Dim2.o getCoeffOpOrder2Dim3.o getCoeffOpOrder4Dim2.o getCoeffOpOrder4Dim3.o
tbcc= tbcc.o Checker.o ../src/finishBoundaryConditions.o
tbcc: $(tbcc) 
	$(CC) $(CCFLAGS) -o tbcc $(tbcc) \
        $(CLIBS) $(FLIBS) $(GLIBS)




# test order of convergence of derivatives
td= derivativeScalarDerivativeFDerivative.o
td= divScalarGradFDerivative.o derivativeScalarDerivativeFDerivative.o
td= divScalarGradFDerivative.o derivativeScalarDerivativeFDerivative.o
td= 
td: td.o $(td)
	$(CC) $(CCFLAGS) -o td td.o \
        $(tderivatives) $(CLIBS) $(FLIBS) $(GLIBS)



derivatives = xFDerivative.o yFDerivative.o zFDerivative.o xxFDerivative.o xyFDerivative.o xzFDerivative.o \
              yyFDerivative.o yzFDerivative.o zzFDerivative.o laplaceFDerivative.o \
              rDerivative.o sDerivative.o tDerivative.o rrDerivative.o rsDerivative.o rtDerivative.o \
              ssDerivative.o stDerivative.o ttDerivative.o divFDerivative.o gradFDerivative.o \
              identityFDerivative.o

derivCoefficients = \
   xFDerivCoefficients.o yFDerivCoefficients.o zFDerivCoefficients.o xxFDerivCoefficients.o \
   xyFDerivCoefficients.o xzFDerivCoefficients.o \
   yyFDerivCoefficients.o yzFDerivCoefficients.o zzFDerivCoefficients.o laplaceFDerivCoefficients.o \
   rDerivCoefficients.o sDerivCoefficients.o tDerivCoefficients.o rrDerivCoefficients.o \
   rsDerivCoefficients.o rtDerivCoefficients.o \
   ssDerivCoefficients.o stDerivCoefficients.o ttDerivCoefficients.o divFDerivCoefficients.o \
   gradFDerivCoefficients.o identityFDerivCoefficients.o

# test the differentiable grid functions:
dmgf= $(derivatives) (derivCoefficients)
dmgf= MGOD.o doubleMappedGridFunction.o
dmgf= ../xcog/hdf_stuff.o ../xcog/c_array.o ../cgsh/getFromADataBase.o ../xcog/xCogToOverture.o
dmgf= 

# Example showing the use of MappedGridOperators
tmgo: $(dmgf) tmgo.o 
	$(CC) $(CCFLAGS) -o tmgo tmgo.o \
        $(dmgf) $(CLIBS) $(FLIBS) $(GLIBS)

# Example showing the use of CompositeGridOperators
tcgo = tcgo.o 
tcgo: $(tcgo) 
	$(CC) $(CCFLAGS) -o tcgo $(tcgo) \
        $(dmgf) $(CLIBS) $(FLIBS) $(GLIBS)

# test the coefficients returned by the MappedGridOperators
cfiles= MappedGridOperators.o MGOC.C
cfiles= laplacianFDCoefficients.o
tc: tc.o $(cfiles)
	$(CC) $(CCFLAGS) -o tc tc.o \
        $(cfiles) $(CLIBS) $(FLIBS) $(GLIBS)

bcgf= bcgf.o 
bcgf: $(bcgf)
	$(CC) $(CCFLAGS) -o bcgf $(bcgf) $(CLIBS) $(FLIBS) $(GLIBS)

applyBoundaryConditions.f : applyBoundaryConditions.bf; bpp applyBoundaryConditions.bf
applyBoundaryConditions.o : applyBoundaryConditions.f; $(FC) $(FFLAGS) -c $<

# assignBoundaryConditions.f : assignBoundaryConditions.bf; bpp assignBoundaryConditions.bf
# assignBoundaryConditions.o : assignBoundaryConditions.f; $(FC) $(FFLAGS) -c $<

# test the boundary conditions:
tbc= tbc.o Checker.o  ../bc/neumann.o
tbc= tbc.o Checker.o  ../src/applyBCC.o ../bc/normalDerivative.o ../fortranDeriv/assignBoundaryConditions.o \
           ../bc/symmetry.o ../src/fixBoundaryCorners.o
tbc= tbc.o Checker.o ../fortranDeriv/fixBCOptEdge1.o ../fortranDeriv/fixBCOptEdge2.o ../fortranDeriv/fixBCOptEdge3.o ../src/finishBoundaryConditions.o
tbc= tbc.o Checker.o ../src/finishBoundaryConditions.o
tbc= tbc.o Checker.o tangential.o assignBoundaryConditions.o assignOptTangentialComponent.o
tbc: $(tbc) 
	$(CC) $(CCFLAGS) -o tbc $(tbc) \
        $(CLIBS) $(FLIBS) $(GLIBS)

tbc2= tbc2.o BoundaryOperators.o
tbc2: $(tbc2) 
	$(CC) $(CCFLAGS) -o tbc2 $(tbc2) \
        $(CLIBS) $(FLIBS) $(GLIBS)


edge=   edge.o floatMappedGridFunction.o
edge:   $(edge)
	$(CC) $(CCFLAGS) -o edge $(edge) \
        $(dmgf) $(CLIBS) $(FLIBS) $(GLIBS)

findInterpNeighbours.f : findInterpNeighbours.bf; bpp findInterpNeighbours.bf

testExtrapInterpNeighbours= testExtrapInterpNeighbours.o Checker.o AssignInterpNeighbours.o findInterpNeighbours.o ../fortranDeriv/extrapolateOpt.o BoundaryOperators.o
testExtrapInterpNeighbours= testExtrapInterpNeighbours.o Checker.o ../fortranDeriv/extrapolateOpt.o
testExtrapInterpNeighbours: $(testExtrapInterpNeighbours) 
	$(CC) $(CCFLAGS) -o testExtrapInterpNeighbours $(testExtrapInterpNeighbours) $(CLIBS) $(FLIBS) $(GLIBS)


# test out all spectral derivatives:
testSpectral= testSpectral.o spectralDerivatives.o MappedGridOperators.o FourierOperators.o MGOD.o
testSpectral= testSpectral.o spectralDerivatives.o FourierOperators.o
testSpectral: testSpectral.o $(testSpectral)
	$(CC) $(CCFLAGS) -o testSpectral \
        $(testSpectral) $(CLIBS) $(FLIBS) $(GLIBS)

# Test the Integrate class
ti= testIntegrate.o  Integrate.o 
ti: $(ti)
	$(CC) $(CCFLAGS) -o ti $(ti) \
        $(CLIBS) $(FLIBS) $(GLIBS)


gc = gc.o 
gc: $(gc)
	$(CC) $(CCFLAGS) -o gc $(gc) \
        $(CLIBS) $(FLIBS) $(GLIBS)

example2 = example2.o 
example2: $(example2)
	$(CC) $(CCFLAGS) -o example2 $(example2) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# example3 : test interpolation 
files3 = example3.o 
example3: $(files3)
	$(CC) $(CCFLAGS) -o example3 $(files3) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# example3 : test interpolation 
filesEx3 = ex3.o Interpolant.o
ex3: $(filesEx3)
	$(CC) $(CCFLAGS) -o ex3 $(filesEx3) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# output stuff about a grid
query = query.o
query: $(query)
	$(CC) $(CCFLAGS) -o query $(query) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# these next files allow PETSc to be used with OverBlown. No need to link to these otherwise.
Oges = ../../oges
buildEquationSolvers.o : $(Oges)/buildEquationSolvers.C
	$(CC) $(CCFLAGS) -DOVERTURE_USE_PETSC -c $(Oges)/buildEquationSolvers.C
# PETScEquationSolver.o : $(Oges)/PETScEquationSolver.C
# 	$(CC) $(CCFLAGS) -DOVERTURE_USE_PETSC -c $(Oges)/PETScEquationSolver.C



# Test coefficient matrices
tcm = tcm.o SparseRep.o BoundaryOperators.o floatMappedGridFunction.o ../oges/oges.o
tcm = tcm.o SparseRep.o MappedGridOperators.o MGOC.o laplaceFDerivCoefficients.o

tcm = tcm.o SparseRep.o BoundaryOperators.o xFDerivCoefficients.o yFDerivCoefficients.o \
      laplaceFDerivCoefficients.o ../oges/oges.o
tcm = tcm.o ../oges/ogce.o
tcm = tcm.o ../oges/Oges.o
tcm = tcm.o 
tcm: $(tcm)
	$(CC) $(CCFLAGS) -o tcm $(tcm) \
        $(CLIBS) $(FLIBS) $(GLIBS)

convectionDiffusion = convectionDiffusion.o applyBCC.o
convectionDiffusion = convectionDiffusion.o
convectionDiffusion: $(convectionDiffusion)
	$(CC) $(CCFLAGS) -o convectionDiffusion $(convectionDiffusion) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# memory usage:
memory = memory.o
memory: $(memory)
	$(CC) $(CCFLAGS) -o memory $(memory) $(CLIBS) $(FLIBS) $(GLIBS)


# Bobby's bug
test19 = test19.o coeff.o MGOC.o ../mapping/ReparameterizationTransform.o ../grid/MappedGrid.o
test19: $(test19)
	$(CC) $(CCFLAGS) -o test19 $(test19) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# bug test:
tcmt = tcmt.o
tcmt: $(tcmt)
	$(CC) $(CCFLAGS) -o tcmt $(tcmt) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# solve a system of equations on a MappedGrid
tcm2 = tcm2.o SparseRep.o BoundaryOperators.o xFDerivCoefficients.o yFDerivCoefficients.o \
      laplaceFDerivCoefficients.o ../oges/oges.o
tcm2 = tcm2.o laplacianFDCoefficients.o xFDCoefficients.o yFDCoefficients.o
tcm2 = tcm2.o floatDisplay.o ../oges/ogesl.o
tcm2 = tcm2.o 
tcm2: $(tcm2)
	$(CC) $(CCFLAGS) -o tcm2 $(tcm2) \
        $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)

# solve a scalar equation on a CompositeGrid
tcm3 = tcm3.o Checker.o $(PETSC) formMatrix.o
tcm3 = tcm3.o Checker.o $(PETSC)  ../../grid/MappedGridGeometry2.o
tcm3 = tcm3.o Checker.o $(PETSC)
tcm3: $(tcm3)
	$(CC) $(CCFLAGS) -o tcm3 $(tcm3) \
         $(CLIBS) $(PETSC_LIBS) $(FLIBS) $(GLIBS)

# Test changing the singular problem by changing an equation at one point
tcm3a = tcm3a.o Checker.o $(PETSC)
tcm3a: $(tcm3a)
	$(CC) $(CCFLAGS) -o tcm3a $(tcm3a) \
         $(CLIBS) $(PETSC_LIBS) $(FLIBS) $(GLIBS)

# special version for Mike Singer test
tcm3n = tcm3n.o Checker.o $(PETSC)
tcm3n: $(tcm3n)
	$(CC) $(CCFLAGS) -o tcm3n $(tcm3n) \
        $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)

# version that check memory usage
tcm3m = tcm3m.o $(PETSC) 
tcm3m: $(tcm3m)
	$(CC) $(CCFLAGS) -o tcm3m $(tcm3m) \
        $(PETSC_LIBS) $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)

testSolve = testSolve.o GenericMappedGridOperators.o
testSolve: $(testSolve)
	$(CC) $(CCFLAGS) -o testSolve $(testSolve) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# solve a system of equations on an overlapping grid
tcm4 = tcm4.o GenericGridCollectionOperators.o SparseRep.o BoundaryOperators.o \
       floatMappedGridFunction.o ../oges/oges.o
tcm4 = tcm4.o CompositeGridOperators.o GenericCompositeGridOperators.o \
              GridCollectionOperators.o GenericGridCollectionOperators.o
tcm4 = tcm4.o  $(PETSC) formMatrix.o
tcm4: $(tcm4)
	$(CC) $(CCFLAGS) -o tcm4 $(tcm4) \
        $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)

# test the block solves
tcm4b = tcm4b.o  $(PETSC) 
tcm4b: $(tcm4b)
	$(CC) $(CCFLAGS) -o tcm4b $(tcm4b) \
        $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)

tcmOrder4 = tcmOrder4.o Interpolant.o
tcmOrder4 = tcmOrder4.o $(PETSC) 
tcmOrder4: $(tcmOrder4)
	$(CC) $(CCFLAGS) -o tcmOrder4 $(tcmOrder4) \
        $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)

# tcmge: Shows how to fill in the coefficient matrix with general equations 
tcmge = tcmge.o  $(PETSC) 
tcmge: $(tcmge)
	$(CC) $(CCFLAGS) -o tcmge $(tcmge) \
        $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)



# linearized INS
lins = lins.o BoundaryOperators.o applyBCC.o
lins = lins.o 
lins: $(lins)
	$(CC) $(CCFLAGS) -o lins $(lins) \
        $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(GLIBS)


krister = krister.o floatCompositeGridFunction.o floatGridCollectionFunction.o
krister: $(krister)
	$(CC) $(CCFLAGS) -o krister $(krister) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# **** test coefficient matrixes and time creation *****
tcmT = tcmTime.o laplacianFDCoefficients.o MGOC.o floatMappedGridFunction.o
tcmT = tcmTime.o laplacianFDCoefficients.o floatMappedGridFunction.o intMappedGridFunction.o
tcmT = tcmTime.o laplacianFDCoefficients.o xFDCoefficients.o yFDCoefficients.o zFDCoefficients.o ../ogmg/display.o \
       xxFDCoefficients.o yyFDCoefficients.o zzFDCoefficients.o xyFDCoefficients.o xzFDCoefficients.o yzFDCoefficients.o \
       identityFDCoefficients.o MappedGridOperators.o
tcmTest = tcmTest.o laplacianFDCoefficients.o xyFDCoefficients.o xzFDCoefficients.o yzFDCoefficients.o
tcmTest = tcmTest.o
tcmTest: $(tcmTest)
	$(CC) $(CCFLAGS) -o tcmTest $(tcmTest) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# Test coefficient matrices
exCM1 = exCM1.o 
exCM1: $(exCM1)
	$(CC) $(CCFLAGS) -o exCM1 $(exCM1) \
        $(CLIBS) $(FLIBS) $(GLIBS)


example7 = example7.o 
example7: $(example7)
	$(CC) $(CCFLAGS) -o example7 $(example7) \
        $(CLIBS) $(FLIBS) $(GLIBS)


Interpolant.p.o: Interpolant.p.C
	$(CC) $(CCFLAGSO) -c $*.C

tint = testInterpolant.o Interpolant.p.o
tint = testInterpolant.o Interpolant.p.o interpOpt.o
tint: $(tint)
	$(CC) $(CCFLAGS) -o tint $(tint) \
        $(CLIBS) $(FLIBS) $(GLIBS)

testInterpolant: tint

# test special interpolation routines
tsi = tsi.o Interpolant.o interpOpt.o
tsi: $(tsi)
	$(CC) $(CCFLAGS) -o tsi $(tsi) \
        $(CLIBS) $(FLIBS) $(GLIBS)


gcfTest = gcfTest.o doubleGridCollectionFunction.o
gcfTest: $(gcfTest)
	$(CC) $(CCFLAGS) -o gcfTest $(gcfTest) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# test the fortran read of the data file generated by display
tdf= tdf.o
tdf: $(tdf)
	f77 -o tdf $(tdf) $(FLIBS)


createCG = createCG.o Interpolant.o
createCG: $(createCG)
	$(CC) $(CCFLAGS) -o createCG $(createCG) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# solver1D
solver1D = solver1D.o 
solver1D: $(solver1D)
	$(CC) $(CCFLAGS) -o solver1D $(solver1D) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# generic test routine
tFiles = t.o floatMappedGridFunction.o ../grid/MappedGrid.o
t: $(tFiles) 
	$(CC) $(CCFLAGS) -o t $(tFiles) \
        $(CLIBS) $(FLIBS) $(GLIBS)

dan3d = dan3d.o ../grid/MappedGrid.o
dan3d: $(dan3d) 
	$(CC) $(CCFLAGS) -o dan3d $(dan3d) \
        $(CLIBS) $(FLIBS) $(GLIBS)

int: int.o
	$(CC) $(CCFLAGS) -o int int.o \
        $(CLIBS) $(FLIBS)

# test routine for the ListOfReferencedCountedObjects class
trc =  ListOfReferenceCountedObjects.o trc.o
trc: $(trc)
	$(CC) $(CCFLAGS) -o trc \
        $(trc)  \
        -L$(A++) $(LIBAPP) $(LIVERLIB) \
        -lF77 -lM77 -lV77

rc =  rc.o
rc: $(rc)
	$(CC) $(CCFLAGS) -o rc \
        $(rc)  \
        -L$(A++) $(OVERTURE_LIB) $(LIBAPP) $(LIVERLIB) $(FLIBS) \
        -lF77 -lM77 -lV77



#
amrExample1 = amrExample1.o
amrExample1: $(amrExample1)
	$(CC) $(CCFLAGS) -o amrExample1 $(amrExample1) \
        $(CLIBS) $(FLIBS) $(GLIBS)

gridCollectionExample1 = gridCollectionExample1.o  ../grid/GenericGridCollection.o ../ogshow/grid.o
gridCollectionExample1 = gridCollectionExample1.o  
gridCollectionExample1: $(gridCollectionExample1)
	$(CC) $(CCFLAGS) -o gridCollectionExample1 $(gridCollectionExample1) \
        $(CLIBS) $(FLIBS) $(GLIBS)

amrTest = amrTest.o floatGridCollectionFunction.o ../grid/GenericGridCollection.o
amrTest = amrTest.o ../grid/GenericGridCollection.o ../grid/GridCollection.o ../grid/CompositeGrid.o
amrTest: $(amrTest)
	$(CC) $(CCFLAGS) -o amrTest $(amrTest) \
        $(CLIBS) $(FLIBS) $(GLIBS)

amrTest2 = amrTest2.o floatGridCollectionFunction.o ../grid/GenericGridCollection.o \
     ../grid/CompositeGrid.o floatCompositeGridFunction.o
amrTest2 = amrTest2.o  ../grid/GenericGridCollection.o ../grid/GridCollection.o ../grid/CompositeGrid.o
amrTest2: $(amrTest2)
	$(CC) $(CCFLAGS) -o amrTest2 $(amrTest2) \
        $(CLIBS) $(FLIBS) $(GLIBS)

tcg2 = testCompositeGridFunction-2.o doubleCompositeGridFunction.o
tcg2 = testCompositeGridFunction-2.o 
tcg2: $(tcg2)
	$(CC) $(CCFLAGS) -o tcg2 $(tcg2) \
        $(CLIBS) $(FLIBS) $(GLIBS)

tcg1 = testCompositeGridFunction-1.o doubleCompositeGridFunction.o
tcg1: $(tcg1)
	$(CC) $(CCFLAGS) -o tcg1 $(tcg1) \
        $(CLIBS) $(FLIBS) $(GLIBS)

static= static.o OvertureInit.o
static: $(static)
	$(CC) $(CCFLAGS) -o static $(static) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# hybrid grid example
hybrid = hybrid.o doubleMappedGridFunction.o ../grid/MappedGrid.o
hybrid = hybrid.o ../grid/MappedGrid.o ../ogshow/grid.o ../mapping/UnstructuredMapping.o \
         ../grid/UnstructuredGridGeometry.o ../ogshow/contour.o
hybrid = hybrid.o ../mapping/UnstructuredMapping.o 
hybrid = hybrid.o doubleMappedGridFunction.o ../grid/MappedGrid.o ../grid/UnstructuredGridGeometry.o
hybrid = hybrid.o 
hybrid: $(hybrid)
	$(CC) $(CCFLAGS) -o hybrid $(hybrid) \
        $(CLIBS) $(FLIBS) $(GLIBS)

lins2= lins2.o
lins2: $(lins2) 
	$(CC) $(CCFLAGS) -o lins2 $(lins2) \
        $(CLIBS) $(FLIBS) $(GLIBS)

amrLeak= amrLeak.o doubleGridCollectionFunction.o OvertureInit.o ../grid/MappedGrid.o
amrLeak= amrLeak.o 
amrLeak: $(amrLeak) 
	$(CC) $(CCFLAGS) -o amrLeak $(amrLeak) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# timing test
time = time.o timef.o
time: $(time)
	$(CC) $(CCFLAGS) -o time $(time) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# test out BoxLib
tbox= tbox.o
tbox: $(tbox)
	$(CC) $(CCFLAGS) -o tbox $(tbox) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# test out pseudo-spectral
ps= ps.o FourierOperators.o ncarfft.o
ps: $(ps)
	$(CC) $(CCFLAGS) -o ps $(ps) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# test prototype unstructured operators
unstest= UnstructuredOperators.o unstructuredExample.o 
unstest: $(unstest)
	$(CC) $(CCFLAGS) -o unstest $(unstest) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# advection-diffusion example using unstructured operators
uexample= UnstructuredOperators.o unstructuredMappedGridExample2.o
uexample: $(uexample)
	$(CC) $(CCFLAGS) -o uexample $(uexample) \
        $(CLIBS) $(FLIBS) $(GLIBS)

DYNAMIC=  floatGridCollectionFunction.o 
lib.so:   $(DYNAMIC)
	  ld -o lib.so -assert pure-text $(DYNAMIC)



clean:
	rm -rf *.o fort.1 tcm tcm2 tcm3 tc3m tcm4 tcmge lins tcgo tderivatives tbc tmgo \
                  testSpectral ps tbc2 time tcmTest td bcgf tz tcmt tbcc  tcmOrder4 

SRC = *.C

# to remake the dependencies:  'make dep'
dep: ; 
	makeDepend -fdepend -Y -- $(CFLAGS) -- $(SRC)

# include the dependencies here
include depend


.PRECIOUS:
