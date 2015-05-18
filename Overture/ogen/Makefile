include ../config/Makefile.$(VENDOR)
# include ../config/Makefile.intel

# over-ride defaults
# === use these for opt ===== (setenv compile opt)
ifeq ($(compile),opt)
  CCFLAGS= $(CCFLAGSO)
  CCFLAGSG= $(CCFLAGSO)
endif

current = .
mapping = $(current)/../mapping
hype = $(current)/../hype
otherStuff = $(current)/../otherStuff
# ogshow = $(current)/../ogshow

VPATH = $(mapping):$(hype):$(otherStuff)


# For use with fort77:
# FC = fort77
# F2C_INCLUDE = /home/henshaw/f2c/libf2c
# FFLAGS_FORT77= -r8 -g -I$(OvertureInclude) -fPIC -NC400 -Nn1604 -I$(F2C_INCLUDE)
# FFLAGSO= $(FFLAGS_FORT77)
# FORTRAN_LIBS_FORT77 =-lnsl -lg2c


all:      ogen
doc: ogenDoc

ogenUpdate.tex: checkOverlap.C computeOverlap.C
	extract.p checkOverlap.C computeOverlap.C
changeParametersInclude.tex: changeParameters.C
	extract.p changeParameters.C
ogenUpdateInclude.tex: checkOverlap.C computeOverlap.C
	extract.p checkOverlap.C computeOverlap.C


docIncludeFiles = changeParametersInclude.tex ogenInclude.tex ogenUpdateInclude.tex

ogenDoc: $(docIncludeFiles)
	pdflatex ogen
	bibtex ogen
	makeindex ogen.idx

ogenDocOld: $(docIncludeFiles)
	latex ogen
	makeindex ogen.idx
	latex ogen
	dvips -P pdf -G0 -o ogen.ps ogen.dvi
	ps2pdf ogen.ps
	cp ogen.pdf $(WEBPAGE)/documentation

ogenhtml: $(docIncludeFiles)
	lh.p ogen.tex
	latex ogenHTML
	bibtex ogenHTML
	makeindex ogenHTML.idx
	latex ogenHTML
	latex2html -local_icons ogenHTML
	rm -rf $(WEBPAGE)/documentation/ogenHTML
	mkdir $(WEBPAGE)/documentation/ogenHTML
	cp ogenHTML/*.* $(WEBPAGE)/documentation/ogenHTML
	rm -rf ogenHTML

doc: ogenDoc


cartoon: 
	pdflatex cartoon



DIR=`pwd`
dir:; @echo "pwd = $(DIR)"

linkFiles:
	ln -s ../ugen/valve.hyb.cmd .
	ln -s ../ugen/obstacle.hyb.cmd .
	ln -s ../ugen/mastSail2d.hyb.cmd .
	ln -s ../ugen/twoBump.hyb.cmd .
	ln -s ../ugen/sib.hyb.cmd .

Oges = ../oges
linkFiles2:
	ln -sf $(Oges)/PETScEquationSolver.C .
	ln -sf $(Oges)/PETScSolver.C .
	ln -sf $(Oges)/buildEquationSolvers.C .

# ---- compile these next files with optimization----
#     stencilWalk is NB to optimize
# stencilWalk.o: $(Overture)/Mapping/stencilWalk.C
# stencilWalk.o: $(Overture)/Mapping/stencilWalk.C
#	 $(CC) $(CFLAGSF) -c $(Overture)/Mapping/stencilWalk.C

boundaryAdjustment.o: boundaryAdjustment.C
	 $(CC) $(CCFLAGS) -c boundaryAdjustment.C


#stencilWalk.o: $(Overture)/Mapping/stencilWalk.C
#	 $(CC) $(CCFLAGSF) -c $(Overture)/Mapping/stencilWalk.C

#Inverse.o: $(Overture)/Mapping/Inverse.C
#	 $(CC) $(CCFLAGSG) -c $(Overture)/Mapping/Inverse.C

#dpmInverse.o: $(Overture)/Mapping/dpmInverse.C
#	 $(CC) $(CCFLAGSO) -c $(Overture)/Mapping/dpmInverse.C

#DataPointMapping.o: $(Overture)/Mapping/DataPointMapping.C
#	$(CC) $(CCFLAGSO) -c $(Overture)/Mapping/DataPointMapping.C

#dpmScalar.o: $(Overture)/Mapping/dpmScalar.C
#	$(CC) $(CCFLAGSF) -c $(Overture)/Mapping/dpmScalar.C

updateRefinement.C : updateRefinement.bC; bpp updateRefinement.bC
updateRefinementNew.C : updateRefinementNew.bC; bpp updateRefinementNew.bC
updateRefinementNewer.C : updateRefinementNewer.bC; bpp updateRefinementNewer.bC
updateRefineFillInterp.C : updateRefineFillInterp.bC; bpp updateRefineFillInterp.bC

cutHolesNew.C: cutHolesNew.bC; bpp cutHolesNew.bC
cutHolesNewer.C: cutHolesNewer.bC; bpp cutHolesNewer.bC
markPointsNeeded.C: markPointsNeeded.bC; bpp markPointsNeeded.bC
lastChance.C: lastChance.bC; bpp lastChance.bC

movingUpdateNew.C: movingUpdateNew.bC; bpp movingUpdateNew.bC

BPP = bpp
%.C : %.bC
	$(BPP) -quiet -clean  $<

CCFLAGSOGEN = $(CCFLAGSF)
## CCFLAGSOGEN = $(CCFLAGSG)
# Here are ogen files we like to optimise
# **NOTE** pipes.cmd gives a different answer depending on compiling classify.C with opt or not
classify.o: classify.C; $(CC) $(CCFLAGSOGEN) -c classify.C
cutHoles.o: cutHoles.C; $(CC) $(CCFLAGSOGEN) -c cutHoles.C
updateRefinement.o: updateRefinement.C; $(CC) $(CCFLAGSG) -c updateRefinement.C
updateRefinementNew.o: updateRefinementNew.C; $(CC) $(CCFLAGSG) -c updateRefinementNew.C

hypgenFiles = ../mapping/HyperbolicSurfaceMapping.o ../mapping/hyper.o ../mapping/hypgen.o ../mapping/hypgen2.o \
              ../mapping/surgrd.o ../mapping/cmpltm.o
hypgenFiles =


# ***********************************************************************************

ogenFiles=     ogenFunction.o Ogen.o checkOverlap.o check.o changeParameters.o classify.o computeOverlap.o cutHoles.o \
               buildCutout.o boundaryAdjustment.o improveQuality.o movingUpdate.o updateRefinement.o \
               cutHolesNew.o markPointsNeeded.o lastChance.o adjustBoundary.o getHoleWidth.o movingUpdateNew.o \
               cutHolesNewer.o interpolatePoints.o checkOverlappingGrid.o explicitHoleCutting.o ExplicitHoleCutter.o

ogen=    ogenDriver.o $(ogenFiles) ../mapping/LoftedSurfaceMapping.o ../mapping/stretchUpdate.o
ogen=    ogenDriver.o $(ogenFiles) LoftedSurfaceMapping.o
ogen=    ogenDriver.o $(ogenFiles) ../grid/MappedGridGeometry2.o ../mapping/dpmInverse.o ../hype/update.o
ogen=    ogenDriver.o $(ogenFiles) $(ovjunk)/mapping/stencilWalk.o $(ovjunk)/mapping/dpmInverse.o $(ovjunk)/mapping/MappingBuilder.o
ogen=    ogenDriver.o $(ogenFiles) ../mapping/stretchUpdate.o
ogen=    ogenDriver.o $(ogenFiles) TridiagonalSolver.o TridiagonalFactor.o
ogen=    ogenDriver.o $(ogenFiles) GridStretcher.o generateParallel.o DataPointMapping.o ../hype/update.o
ogen=    ogenDriver.o $(ogenFiles) ../hype/update.o
ogen: $(ogen) ; $(CC) $(CCFLAGS) -o $@ $(ogen) $(PETSC_LIBS) $(CLIBS) $(FLIBS) $(FORTRAN_LIBS_FORT77) $(GLIBS)  

# **************************************************************************************


# ==== Kyle's routine for generating grids from shape files ========
ELEVEN = /home/henshaw.0/eleven/eleven_dist
ELEVEN_LIBS = -L$(ELEVEN)/lib -l11g -lexpat -lz $(ELEVEN)/shapelib/*.o
amrGridFromEleven.o: amrGridFromEleven.C; $(CC) $(CCFLAGSG) -I$(ELEVEN)/src -c amrGridFromEleven.C
amrGridFromEleven = amrGridFromEleven.o
amrGridFromEleven: $(amrGridFromEleven); $(CC) $(CCFLAGS) -o $@ $(amrGridFromEleven) $(ELEVEN_LIBS) $(PETSC_LIBS) $(CLIBS)  $(FLIBS) $(FORTRAN_LIBS_FORT77) $(GLIBS)



bug2=     bug2.o 
bug2:     $(bug2); $(CC) $(CFLAGS) -o $@ $(bug2) $(CLIBS) $(FLIBS) $(GLIBS)

#  Compare grids constructed with the serial and parallel version of ogen for consistency.
checkParallelGrid=     checkParallelGrid.o
checkParallelGrid:     $(checkParallelGrid); $(CC) $(CFLAGS) -o $@ $(checkParallelGrid) $(CLIBS)  $(FLIBS)  $(GLIBS)


hybrid=     hybrid.o
hybrid:     $(hybrid); $(CC) $(CFLAGS) -o $@ $(hybrid) $(CLIBS)  $(FLIBS)  $(GLIBS)

leak=     leak.o ../grid/MappedGrid.o ../gf/intMappedGridFunction.o
leak:     $(leak); $(CC) $(CFLAGS) -o $@ $(leak) $(CLIBS)  $(FLIBS)  $(GLIBS)


# check parallel memory usage
pmem=     pmem.o
pmem: $(pmem) ; $(CC) $(CCFLAGS) -o $@ $(pmem) $(CLIBS)  $(FLIBS) $(FORTRAN_LIBS_FORT77) $(GLIBS) 

# parallel write with hdf5
Dataset=     Dataset.o
Dataset: $(Dataset) ; $(CC) $(CCFLAGS) -o $@ $(Dataset) $(CLIBS)  $(FLIBS) $(FORTRAN_LIBS_FORT77) $(GLIBS) 



# These files we build dependency info for:
SRC = ogenDriver.C ogen.C Ogen.C checkOverlap.C check.C changeParameters.C classify.C computeOverlap.C cutHoles.C \
      buildCutout.C boundaryAdjustment.C improveQuality.C movingUpdate.C checkOverlappingGrid.C move.C refine.C \
      updateRefinement.C updateRefinementNew.C updateRefinementNewer.C updateRefineFillInterp.C


interpolateRefinements.o: ../gf/interpolateRefinements.C
	 $(CC) $(CFLAGS) -c ../gf/interpolateRefinements.C


copyCoarseToFine.C : copyCoarseToFine.bC; bpp copyCoarseToFine.bC

refine=   refine.o updateRefinementNew.o updateRefinement.o Ogen.o copyCoarseToFine.o
refine=   refine.o updateRefinementNewer.o updateRefinementNew.o updateRefinement.o checkInterpolation.o \
          updateRefineFillInterp.o ../ogshow/grid.o ../ogshow/gridOpt.o
refine:   $(refine); $(CC) $(CFLAGS) -o $@ $(refine) $(CLIBS)  $(FLIBS)  $(GLIBS)

refine2=   refine2.o computeOverlap.o ../otherStuff/displayMask.o ../gf/interpolateRefinements.o ../ogshow/contour.o \
           ../gf/doubleCompositeGridFunction.o
refine2:   $(refine2); $(CC) $(CFLAGS) -o $@ $(refine2) $(CLIBS)  $(FLIBS)  $(GLIBS)

refineOld=   refine.o computeOverlapOld.o checkOverlappingGrid.o interpolateRefinements.o ../gf/maskDisplay.o
refineOld:   $(refineOld); $(CC) $(CFLAGS) -o $@ $(refineOld) $(CLIBS)  $(FLIBS)  $(GLIBS)

adapt=   adapt.o $(ogenFiles) ../grid/GridCollection.o ../grid/CompositeGrid.o
adapt=   adapt.o $(ogenFiles)
adapt:   $(adapt); $(CC) $(CFLAGS) -o $@ $(adapt) $(CLIBS)  $(FLIBS)  $(GLIBS)


add=     add.o cutHoles.o ../grid/CompositeGrid.o
add:     $(add); $(CC) $(CFLAGS) -o $@ $(add) $(CLIBS) $(GLIBS)


fast=   fast.o computeOverlap.o 
fast:   $(fast); $(CC) $(CFLAGS) -o $@ $(fast) $(CLIBS) $(FLIBS) $(GLIBS)

move=     move.o $(ogenFiles) 
move:     $(move); $(CC) $(CFLAGS) -o $@ $(move) $(CLIBS) $(FLIBS) $(GLIBS)

# ===================================
PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH) -I$(PETSC_DIR)/include/mpiuni
LAPACK_LIBRARIES = -Wl,-rpath,$(PGI_LIB) -L$(PGI_LIB) -llapack -lblas
usePPP = $(USE_PPP_FLAG)
ifneq ($(strip $(usePPP)),)
# P++ version
 PETSC_INCLUDE = -DOVERTURE_USE_PETSC -I$(PETSC_DIR)/include -I$(PETSC_DIR)/bmake/$(PETSC_ARCH)
 PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc $(HYPRE_LIBS) $(LAPACK_LIBRARIES)
 PETSC =  buildEquationSolvers.o PETScEquationSolver.o PETScSolver.o
else
 PETSC_LIBS = -Wl,-rpath,$(PETSC_LIB) -L$(PETSC_LIB) -lpetscksp -lpetscdm -lpetscmat -lpetscvec -lpetsc -lmpiuni
 PETSC =  buildEquationSolvers.o PETScEquationSolver.o 
endif

# *wdh* 2014/01/17 -- do this for now
PETSC_LIBS =

buildEquationSolvers.o : buildEquationSolvers.C
	$(CC) $(CCFLAGS) -DOVERTURE_USE_PETSC -c buildEquationSolvers.C
# ===================================

OGMG = MultigridEquationSolver.o
OGMG_LIBS = -Wl,-rpath,$(Overture)/lib -L$(Overture)/lib -lOgmg

# subsched.c from P++/PARTI/
subsched.o: subsched.c
	 $(cc) -I$(MPI_ROOT)/include -I$(PPlusPlus)/../PARTI -g -c subsched.c


move2=     move2.o $(PETSC) $(OGMG) $(ogenFiles) ../grid/MappedGridGeometry2.o ../mapping/Mapping.o
move2=     move2.o $(PETSC) $(OGMG) $(ogenFiles) 
move2=     move2.o $(PETSC) $(OGMG) classify.o ../gf/ParallelOverlappingGridInterpolator.o 
move2=     move2.o $(PETSC) $(OGMG) classify.o 
move2=     move2.o $(PETSC) $(OGMG) $(ogenFiles) subsched.o 
move2=     move2.o $(PETSC) $(OGMG) $(ogenFiles) ../parallel/CanInterpolate.o
move2:     $(move2); $(CC) $(CFLAGS) -o $@ $(move2) $(OGMG_LIBS) $(CLIBS) $(PETSC_LIBS) $(LAPACK_LIBRARIES) $(FLIBS) $(GLIBS)

# check for leak in ogen:
move2g=     move2g.o $(PETSC) $(OGMG) $(ogenFiles) ../parallel/CanInterpolate.o
move2g:     $(move2g); $(CC) $(CFLAGS) -o $@ $(move2g) $(OGMG_LIBS) $(CLIBS) $(PETSC_LIBS) $(LAPACK_LIBRARIES) $(FLIBS) $(GLIBS)

#  2012/06/16 VERSION TO CHECK FOR LEAKS 
move2Leak=     move2Leak.o $(PETSC) $(OGMG) $(ogenFiles) 
move2Leak:     $(move2Leak); $(CC) $(CFLAGS) -o $@ $(move2Leak) $(OGMG_LIBS) $(CLIBS) $(PETSC_LIBS) $(LAPACK_LIBRARIES) $(FLIBS) $(GLIBS)

getLocalArrayLeak=  getLocalArrayLeak.o 
getLocalArrayLeak:     $(getLocalArrayLeak); $(CC) $(CFLAGS) -o $@ $(getLocalArrayLeak) $(CLIBS) $(FLIBS) $(GLIBS)


# Smoothing deforming grids: 
evolve=     evolve.o 
evolve:     $(evolve); $(CC) $(CFLAGS) -o $@ $(evolve) $(CLIBS) $(LAPACK_LIBRARIES) $(FLIBS) $(GLIBS)

tcm3=     tcm3.o $(PETSC)
tcm3:     $(tcm3); $(CC) $(CFLAGS) -o $@ $(tcm3) $(CLIBS) $(PETSC_LIBS) $(LAPACK_LIBRARIES) $(FLIBS) $(GLIBS)

# test of Mike Singer's turbine
moveTurbine=     moveTurbine.o 
moveTurbine:     $(moveTurbine); $(CC) $(CFLAGS) -o $@ $(moveTurbine) $(CLIBS) $(FLIBS) $(GLIBS)

# Test the moving update function
tmu=     tmu.o movingUpdate.o 
tmu:     $(tmu); $(CC) $(CFLAGS) -o $@ $(tmu) $(CLIBS) $(FLIBS) $(GLIBS)

bugp=     bugp.o computeOverlap.o
bugp:     $(bugp); $(CC) $(CFLAGS) -o $@ $(bugp) $(CLIBS) $(FLIBS) $(GLIBS)

bugGrid=     bugGrid.o 
bugGrid:     $(bugGrid); $(CC) $(CFLAGS) -o $@ $(bugGrid) $(CLIBS_NO_HYPGEN) $(FLIBS) $(GLIBS)

# **** test speed of elliptic solver ***
moveAndSolve=     moveAndSolve.o computeOverlap.o ../gf/laplaceFDerivCoefficients.o ../mapping/stencilWalk.o \
                  ../mapping/BoundingBox.o
moveAndSolve=     moveAndSolve.o 
moveAndSolve:     $(moveAndSolve); $(CC) $(CFLAGS) -o $@ $(moveAndSolve) $(CLIBS) $(FLIBS) $(GLIBS)

moveLeak=     moveLeak.o computeOverlap.leak.o cutHoles.leak.o
moveLeak=     moveLeak.o computeOverlap.o  cutHoles.o
moveLeak:     $(moveLeak); $(CC) $(CFLAGS) -o $@ $(moveLeak) $(CLIBS) $(FLIBS) $(GLIBS)

leakShow=     leakShow.o Mapping.o ../hdf/HDF_DataBase.o
leakShow=     leakShow.o ../hdf/HDF_DataBase.o
leakShow:     $(leakShow); $(CC) $(CFLAGS) -o $@ $(leakShow) $(CLIBS) $(FLIBS) $(GLIBS)

move3=     move3.o checkOverlap.o classify.o computeOverlap.o 
move3:     $(move3); $(CC) $(CFLAGS) -o $@ $(move3) $(CLIBS) $(GLIBS)

move4=     move4.o  
move4:     $(move4); $(CC) $(CFLAGS) -o $@ $(move4) $(CLIBS) $(FLIBS) $(GLIBS)

move1=     move1.o $(ogenFiles) ../ogshow/grid.o ../ogshow/gridOpt.o
move1=     move1.o classify.o cutHolesNew.o cutHolesNewer.o ../gf/ParallelOverlappingGridInterpolator.o
move1=     move1.o classify.o cutHolesNew.o cutHolesNewer.o 
move1=     move1.o 
move1:     $(move1); $(CC) $(CFLAGS) -o $@ $(move1) $(CLIBS) $(FLIBS) $(GLIBS)

# moving grids and AMR
moveAMR=     moveAMR.o $(ogenFiles) checkOverlappingGrid.o ../gf/interpolateExposedPoints.o MappedGrid.o MappingRC.o
moveAMR=     moveAMR.o $(ogenFiles) GridCollection.o GenericGridCollection.o MappedGrid.o
moveAMR=     moveAMR.o $(ogenFiles) ../grid/CompositeGrid.o
moveAMR:     $(moveAMR); $(CC) $(CFLAGS) -o $@ $(moveAMR) $(CLIBS) $(FLIBS) $(GLIBS)

tmove=     tmove.o $(ogenFiles)
tmove:     $(tmove); $(CC) $(CFLAGS) -o $@ $(tmove) $(CLIBS) $(FLIBS) $(GLIBS)

ml1=     ml1.o 
ml1:     $(ml1); $(CC) $(CFLAGS) -o $@ $(ml1) $(CLIBS) $(FLIBS) $(GLIBS)

ml2=     ml2.o ../gf/MappedGridGeometry.o ../gf/BoundaryOperators.o ../gf/GenericMappedGridOperators.o
ml2:     $(ml2); $(CC) $(CFLAGS) -o $@ $(ml2) $(CLIBS) $(FLIBS) $(GLIBS)

ml3=     ml3.o 
ml3:     $(ml3); $(CC) $(CFLAGS) -o $@ $(ml3) $(CLIBS) $(FLIBS) $(GLIBS)


amr=     amr.o 
amr:     $(amr); $(CC) $(CFLAGS) -o $@ $(amr) $(CLIBS) $(FLIBS) $(GLIBS)

amr2=     amr2.o 
amr2:     $(amr2); $(CC) $(CFLAGS) -o $@ $(amr2) $(CLIBS) $(FLIBS) $(GLIBS)

amr3=     amr3.o 
amr3:     $(amr3); $(CC) $(CFLAGS) -o $@ $(amr3) $(CLIBS) $(FLIBS) $(GLIBS)


amrExample1=     amrExample1.o  ../gf/doubleGridCollectionFunction.o
amrExample1:     $(amrExample1); $(CC) $(CFLAGS) -o $@ $(amrExample1) $(CLIBS) $(FLIBS) $(GLIBS)

bug=     bug.o 
bug:     $(bug); $(CC) $(CFLAGS) -o $@ $(bug) $(CLIBS) $(FLIBS) $(GLIBS)

k=     k.o 
k:     $(k); $(CC) $(CFLAGS) -o $@ $(k) $(CLIBS) $(FLIBS) $(GLIBS)

# linearized INS
lins = lins.o 
lins: $(lins)
	$(CC) $(CCFLAGS) -o lins $(lins) \
        $(CLIBS) $(FLIBS) $(GLIBS)

move1.o			: Ogen.h
tmove.o			: Ogen.h
tamr.o			: Ogen.h

# to remake the dependencies:  'make dep'
dep: ; 
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend

clean: 
	rm -f *.o cgsh amrExample1 move1 tmove ogen move2 move2Leak moveAndSolve moveLeak fort.1 move3 ml1 ml2 ml3 refine fast \
                  hybrid move216 refineOld move moveAMR evolve tpi

precious:
