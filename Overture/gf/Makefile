# include ../config/Makefile.$(VENDOR)
include ../config/Makefile.intel

FFLAGSO = $(FFLAGSG)

current = .
op = $(current)/../op
VPATH = .:$(op)


all = tgf
all: $(all);

# Test the grid functions
#        ../A++/array.o
# ggf = doubleGenericGridFunction.o floatGenericGridFunction.o intGenericGridFunction.o
gcgf= doubleGenericGridCollectionFunction.o floatGenericGridCollectionFunction.o \
      intGenericGridCollectionFunction.o
mgf = doubleMappedGridFunction.o floatMappedGridFunction.o intMappedGridFunction.o
gcf = doubleGridCollectionFunction.o floatGridCollectionFunction.o intGridCollectionFunction.o
cgf = doubleCompositeGridFunction.o floatCompositeGridFunction.o intCompositeGridFunction.o
mcgf= doubleMultigridCompositeGridFunction.o floatMultigridCompositeGridFunction.o \
      intMultigridCompositeGridFunction.o

G1: $(gcgf)
G2: $(mgf)
G3: $(gcf)
G4: $(cgf)
G5: $(mcgf)

GRIDS= $(Overture)/MappedGrid.o $(Overture)/GenericGridCollection.o \
       $(Overture)/GridCollection.o $(Overture)/CompositeGrid.o \
       $(Overture)/MultigridCompositeGrid.o

GRIDS= $(Overture)/GRID.o
GRIDS=

gfFiles = mappedGridFunction.C Interpolant.C gridCollectionFunction.C \
                  interpPoints.C interpolateExposedPoints.C 

opFiles = MappedGridOperators.C GenericMappedGridOperators.C MGOD.C MGOC.C BoundaryOperators.C applyBCC.C  \
                  GridCollectionOperators.C GenericGridCollectionOperators.C SparseRep.C  \
                  FourierOperators.C generalizedDivergence.C BoundaryConditionParameters.C

otherStuffFiles = OGgetIndex.C ../ogen/getFromADataBase.C OGPolyFunction.C OGFunction.C OGTrigFunction.C display.C \
                  displayMask.C Integrate.C


gf.pdf:
	extract.p $(gfFiles)
	latex gf
	bibtex gf
	makeindex gf.idx
	dvips -P pdf -G0 -o gf.ps gf.dvi
	ps2pdf gf.ps

gf:
	extract.p $(gfFiles)
	latex gf
	bibtex gf
	makeindex gf.idx
	dvips -P pdf -G0 -o gf.ps gf.dvi
	ps2pdf gf.ps
	cp gf.pdf $(WEBPAGE)/documentation

gfhtml: 
	lh.p gf.tex
	latex gfHTML
	bibtex gfHTML
	makeindex gfHTML.idx
	latex gfHTML
	latex2html -local_icons gfHTML

gfweb: gf gfhtml
	dvips -P pdf -G0 -o gf.ps gf.dvi 
	ps2pdf gf.ps
	mv gf.pdf $(WEBPAGE)/documentation
	rm -rf $(WEBPAGE)/documentation/gfHTML
	mkdir $(WEBPAGE)/documentation/gfHTML
	cp gfHTML/*.* $(WEBPAGE)/documentation/gfHTML
	rm -rf gfHTML


doc: gf

App.C: App.bC; bpp -quiet App.bC


intMappedGridFunction.C : mappedGridFunction.C; gf.p
floatMappedGridFunction.C : mappedGridFunction.C; gf.p
doubleMappedGridFunction.C : mappedGridFunction.C; gf.p


# -y_APP_Unit_Range
# *** test grid functions ***
tgf = tgf.o 
tgf: $(tgf)
	$(CC) $(CCFLAGS) -o tgf \
        $(tgf) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) \
        -lF77 -lM77 -lV77


tgf3 = tgf3.o MappedGridGeometry.o
tgf3 = tgf3.o 
tgf3: $(tgf3)
	$(CC) $(CCFLAGS) -o tgf3 \
        $(tgf3) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) \
        -lF77 -lM77 -lV77

tlink = tlink.o floatMappedGridFunction.o
tlink: $(tlink)
	$(CC) $(CCFLAGS) -o tlink \
        $(tlink) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) \
        -lF77 -lM77 -lV77

bug1 = bug1.o MappedGrid.o
bug1: $(bug1)
	$(CC) $(CCFLAGS) -o bug1 \
        $(bug1) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) \
        -lF77 -lM77 -lV77

leak = leak.o ../mapping/Mapping.o
leak = leak.o floatGridCollectionFunction.o
leak = leak.o ../amr/p_array_fixed.o ../amr/p_descriptor.o ../amr/array.o
leak = leak.o ../amr/p_descriptor.060504.o
leak = leak.o 
leak: $(leak)
	$(CC) $(CCFLAGS) -o leak $(leak) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

# look for a leak in pogi
tpi=     tpi.o Interpolant.o ParallelOverlappingGridInterpolator.o 
tpi:     $(tpi); $(CC) $(CFLAGS) -o $@ $(tpi) $(CLIBS) $(FLIBS) $(GLIBS)


# bug in shock hitting a zero mass ellipse
shockEllipseBug = shockEllipseBug.o
shockEllipseBug: $(shockEllipseBug)
	$(CC) $(CCFLAGS) -o shockEllipseBug $(shockEllipseBug) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

# P++ bug from John Lyons
lyon1 = lyon1.o 
lyon1: $(lyon1)
	$(CC) $(CCFLAGS) -o lyon1 $(lyon1) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

lyon2 = lyon2.o 
lyon2: $(lyon2)
	$(CC) $(CCFLAGS) -o lyon2 $(lyon2) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

# test operations with long's
tlong = tlong.C
tlong: $(tlong)
	$(CC) $(CCFLAGS) -o tlong $(tlong) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

# test the grid classes
tgrid = tgrid.o
tgrid: $(tgrid)
	$(CC) $(CCFLAGS) -o tgrid  $(tgrid) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

# test for memory leaks
tml = tml.o 
tml: $(tml)
	$(CC) $(CCFLAGS) -o tml \
        $(tml) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) \
        -lF77 -lM77 -lV77

# test out cell-centre and face-centre properties:
cellFace = floatMappedGridFunction.o  intMappedGridFunction.o cellFace.o
cellFace = cellFace.o OGgetIndex.o floatCompositeGridFunction.o 
cellFace = cellFace.o
cellFace: $(cellFace)
	$(CC) $(CCFLAGS) -o cellFace \
        $(cellFace) $(GRIDS) $(CLIBS) $(FLIBS)  $(GLIBS) \
        -lF77 -lM77 -lV77

cf=cf.o 
cf: $(cf)
	$(CC) $(CCFLAGS) -o cf $(cf) $(GRIDS) $(CLIBS) $(FLIBS)  $(GLIBS) \
        -lF77 -lM77 -lV77

dlb = dlb.o
dlb: $(dlb)
	$(CC) $(CCFLAGS) -o dlb \
        $(dlb) $(GRIDS) $(CLIBS) $(FLIBS)  $(GLIBS) -lF77 -lM77 -lV77

InterpolatePoints.o: InterpolatePoints.C
	 $(CC) $(CCFLAGSG) -c InterpolatePoints.C

# New (parallel) interpolate points: 
InterpolatePointsOnAGrid.C: InterpolatePointsOnAGrid.bC; bpp InterpolatePointsOnAGrid.bC
pogip.C: pogip.bC; bpp pogip.bC


# test InterpolatePoints 
testip = testip.o interpolatePoints.o InterpolatePoints.o
testip = testip.o InterpolatePoints.o ../ugen/SurfaceStitcher.o ../ugen/Ugen.o
testip = testip.o InterpolatePoints.o InterpolatePointsOnAGrid.o pogip.o ../parallel/CanInterpolate.o
testip = testip.o InterpolatePoints.o InterpolatePointsOnAGrid.o pogip.o findNearestValidGridPoint.o
testip: $(testip)
	$(CC) $(CCFLAGS) -o testip \
        $(testip) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

# interpolateExposedPoints.C: interpolateExposedPoints.bC; bpp interpolateExposedPoints.bC
# interpolateExposedPoints.o: interpolateExposedPoints.C
# 	 $(CC) $(CCFLAGSG) -c interpolateExposedPoints.C

ExposedPoints.C: ExposedPoints.bC; bpp ExposedPoints.bC
ExposedPoints.o: ExposedPoints.C
	 $(CC) $(CCFLAGSG) -c ExposedPoints.C

# test  interpolateExposedPoints *OLD*
texpose = texpose.o interpolateExposedPoints.o InterpolatePoints.o ../ogen/movingUpdate.o
texpose = texpose.o ExposedPoints.o InterpolatePoints.o ../ogen/movingUpdate.o
texpose = texpose.o ExposedPoints.o InterpolatePointsOnAGrid.o pogip.o 
texpose: $(texpose)
	$(CC) $(CCFLAGS) -o texpose \
        $(texpose) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 

# ***** here is the new way to test ExposedPoints
move2 = move2.o ExposedPoints.o InterpolatePointsOnAGrid.o pogip.o 
move2: $(move2)
	$(CC) $(CCFLAGS) -o move2 \
        $(move2) $(GRIDS) $(CLIBS) $(FLIBS) $(GLIBS) 


gc = gc.o 
gc: $(gc)
	$(CC) $(CCFLAGS) -o gc $(gc) \
        $(CLIBS) $(FLIBS) $(GLIBS)

# output stuff about a grid
query = query.o
query: $(query)
	$(CC) $(CCFLAGS) -o query $(query) \
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



testSolve = testSolve.o GenericMappedGridOperators.o
testSolve: $(testSolve)
	$(CC) $(CCFLAGS) -o testSolve $(testSolve) \
        $(CLIBS) $(FLIBS) $(GLIBS)


krister = krister.o floatCompositeGridFunction.o floatGridCollectionFunction.o
krister: $(krister)
	$(CC) $(CCFLAGS) -o krister $(krister) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# Grid function norms: 
normOpt.f: normOpt.bf; bpp -quiet -clean normOpt.bf
tgfn = tgfn.o gridFunctionNorms.o normOpt.o
tgfn: $(tgfn)
	 $(CC) $(CCFLAGS) -o tgfn $(tgfn) $(CLIBS) $(FLIBS) $(GLIBS)

# program to compare solutions in show files compute convergence rates 
interpPoints.o : interpPoints.C; $(CC) $(CCFLAGSF) -c $*.C
comp = comp.o gridFunctionNorms.o normOpt.o interpPoints.o InterpolatePoints.o
comp = comp.o gridFunctionNorms.o
comp: $(comp)
	 $(CC) $(CCFLAGS) -o comp $(comp) $(CLIBS) $(FLIBS) $(GLIBS)

# program to transfer a solution from one grid to another
#   transferSolution ts.cmd
transferSolution = transferSolution.o 
transferSolution: $(transferSolution)
	 $(CC) $(CCFLAGS) -o transferSolution $(transferSolution) $(CLIBS) $(FLIBS) $(GLIBS)


interpOpt.f: interpOpt.bf; bpp interpOpt.bf
interpOpt.o : interpOpt.f; $(FC) $(FFLAGSG) -c $*.f
interpOptSP.o : interpOptSP.f; $(FC) $(FFLAGSG) -c $*.f

initExplicitInterp.f: initExplicitInterp.bf; bpp initExplicitInterp.bf
initExplicitInterp.o : initExplicitInterp.f; $(FC) $(FFLAGSF) -c $*.f

Interpolant.p.o: Interpolant.p.C
	$(CC) $(CCFLAGSO) -c $*.C
Interpolant.o: Interpolant.C
	 $(CC) $(CCFLAGSG) -c Interpolant.C


tint = testInterpolant.o Interpolant.o interpOpt.o initExplicitInterp.o interpOptSP.o interpOptFull.o interpOptTP.o 
tint = testInterpolant.o Interpolant.o 
tint: $(tint)
	$(CC) $(CCFLAGS) -o tint $(tint) $(CLIBS) $(FLIBS) $(GLIBS)

testInterpolant: tint

# test special interpolation routines
tsi = tsi.o Interpolant.o interpOpt.o
tsi: $(tsi)
	$(CC) $(CCFLAGS) -o tsi $(tsi) $(CLIBS) $(FLIBS) $(GLIBS)

ParallelOverlappingGridInterpolator.C: ParallelOverlappingGridInterpolator.bC; bpp -quiet ParallelOverlappingGridInterpolator.bC
ParallelOverlappingGridInterpolator.o: ParallelOverlappingGridInterpolator.C
	 $(CC) $(CCFLAGSG) -c ParallelOverlappingGridInterpolator.C

ParallelOverlappingGridInterpolatorNew.C: ParallelOverlappingGridInterpolatorNew.bC; bpp -quiet ParallelOverlappingGridInterpolatorNew.bC

# parallel interpolator
pogi = pogi.o ParallelOverlappingGridInterpolatorNew.o
pogi = pogi.o ParallelOverlappingGridInterpolator.o
pogi: $(pogi)
	$(CC) $(CCFLAGS) -o pogi $(pogi) $(CLIBS) $(FLIBS) $(GLIBS)


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
hybrid = hybrid.o  ../ogshow/contourOpt.o ../ogshow/contour.o  ../mapping/UnstructuredMapping.o 
hybrid = hybrid.o  ../ogshow/contourOpt.o ../ogshow/contour.o  ../mapping/UnstructuredMapping.o 
hybrid: $(hybrid)
	$(CC) $(CCFLAGS) -o hybrid $(hybrid) \
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


# general rigid motion around multiple axes (to go into cg)
motion= motion.C TimeFunction.C MatrixMotion.C
motion: $(motion)
	$(CC) $(CCFLAGS) -o motion $(motion) \
        $(CLIBS) $(FLIBS) $(GLIBS)


# test passing opaque pointers to fortarn
passingPointers= passingPointers.o mySolver.o
passingPointers: $(passingPointers)
	$(CC) $(CCFLAGS) -o passingPointers $(passingPointers) \
        $(CLIBS) $(FLIBS) $(GLIBS)


clean:
	rm -rf *.o fort.1 tgf2 leak cellFace testInterpolant edge createCG time td amrExample1 dan ti \
                  tgf amrTest amrTest2 static convectionDiffusion \
	*Include.tex *.ind *.dvi *.toc *.blg *.bbl *.log *.idx *.aux *.ilg *.lof

SRC = *.C

# to remake the dependencies:  'make dep'
dep: ; 
	makeDepend -fdepend -Y -- $(CFLAGS) -- $(SRC)

# include the dependencies here
include depend


.PRECIOUS:
