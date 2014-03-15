# include ../config/Makefile.$(VENDOR)
include ../config/Makefile.intel

# over-ride defaults here
# PURIFY  = purify $(PFLAGS) 
# add this line for insure++
# CC = insure
FFLAGSO = $(FFLAGSG)

current = .
primer = $(current)/../primer
# VPATH = .:$(primer)

all:     hype

RegridInclude.tex: Regrid.C
	extract.p Regrid.C
ErrorEstimatorInclude.tex: ErrorEstimator.C
	extract.p ErrorEstimator.C
InterpolateRefinementsInclude.tex: InterpolateRefinements.C
	extract.p InterpolateRefinements.C


docIncludeFiles = ErrorEstimatorInclude.tex RegridInclude.tex InterpolateRefinementsInclude.tex

amr: $(docIncludeFiles)
	latex amr
	makeindex amr.idx
	latex amr
	dvips -P pdf -G0 -o amr.ps amr.dvi
	ps2pdf amr.ps
	cp amr.pdf $(WEBPAGE)/documentation

amrhtml: 
	lh.p amr.tex
	latex amrHTML
	bibtex amrHTML
	makeindex amrHTML.idx
	latex amrHTML
	latex2html -local_icons amrHTML
	rm -rf $(WEBPAGE)/documentation/amrHTML
	mkdir $(WEBPAGE)/documentation/amrHTML
	cp amrHTML/*.* $(WEBPAGE)/documentation/amrHTML
	rm -rf amrHTML

doc: amrDoc


fig:
	latex fig.tex
	dvips -P pdf -G0 -o fig.ps fig.dvi
	ps2pdf fig.ps


SRC = tamr.C hype.C ir.C AdaptiveMeshRefinement.C Regrid.C amrGrid.C ErrorEstimator.C InterpolateRefinements.C \
      RotatedBox.C ListOfRotatedBox.C Interpolate.C amrh.C util.C rk.C

Interpolate.C : Interpolate.bC; bpp Interpolate.bC

getDt.o: $(Overture)/primer/getDt.C
	 $(CC) $(CCFLAGSF) -c $(Overture)/primer/getDt.C

tamr=     tamr.o AdaptiveMeshRefinement.o regrid.o tile.o setRefinementMask.o AMR_Regrid.o
tamr=     tamr.o AdaptiveMeshRefinement.o regrid.o tile.o
tamr:     $(tamr); $(CC) $(CFLAGS) -o $@ $(tamr) $(CLIBS_AMR) $(GLIBS) 

testCommonGridInitialisation= testCommonGridInitialisation.o
testCommonGridInitialisation:  $(testCommonGridInitialisation); $(CC) $(CFLAGS) -o $@ $(testCommonGridInitialisation) $(CLIBS_AMR) $(GLIBS) 



# tregrid=     tregrid.o tile.o
# tregrid:     $(tregrid); $(CC) $(CFLAGS) -o $@ $(tregrid) $(CLIBS) $(FLIBS) $(GLIBS) 

# Test the amr regrid function
amrGrid=     amrGrid.o   ErrorEstimator.o Regrid.o ../ogen/classify.o ../ogen/computeOverlap.o
amrGrid=     amrGrid.o   ErrorEstimator.o Regrid.o ../ogshow/grid3d.o ../ogshow/grid.o
amrGrid=     amrGrid.o   ErrorEstimator.o Regrid.o ../ogshow/grid3d.o 
amrGrid:     $(amrGrid); $(CC) $(CFLAGS) -o $@ $(amrGrid) $(CLIBS) $(FLIBS) $(GLIBS) 

amrGrid0=     amrGrid.o Regrid.old.C ErrorEstimator.o 
amrGrid0:     $(amrGrid0); $(CC) $(CFLAGS) -o $@ $(amrGrid0) $(CLIBS) $(FLIBS) $(GLIBS) 

# convert an AMR grid to one with all base grids
amrToBase=     amrToBase.o ../ogen/classify.o ../ogen/computeOverlap.o ../grid/MappedGridGeometry2.o \
               ../mapping/ReparameterizationTransform.o
amrToBase=     amrToBase.o 
amrToBase:     $(amrToBase); $(CC) $(CFLAGS) -o $@ $(amrToBase) $(CLIBS) $(FLIBS) $(GLIBS) 

# Templates.DB: Regrid.C; rm -rf Templates.DB

pcsFiles = ChildInfo.o ParentChildSiblingBase.o ParentInfo.o ListOfParentChildSiblingInfo.o \
           ParentChildSiblingInfo.o SiblingInfo.o

pppFiles = datamove.o  partitioning.o conform_enforce.o p_array.o array.o p_abstract_op.o 

# datamove.c from P++/PARTI/
# datamove.o: datamove.c
# 	 $(cc) -I$(MPI_ROOT)/include -I$(PPlusPlus)/../PARTI -g -c datamove.c

# ----------- here is how we compile P++ files with -g --------------------------
# P = /home/henshaw2/P++-3.3.3/A++P++-0.7.9d/P++/src/array_src
P = $(PPlusPlus)/../src/array_src
PFLAGS = -DHAVE_CONFIG_H -I. -I$(P)/../.. -I$(P)/../../include -I$(P)/../../PARTI -I$(P)/../PARTI -I$(MPI_ROOT)/include    -I$(P)/../../PADRE/src -I$(P)/../../PADRE -I$(P)/../../PARTI -I$(P)/../../PARTI -I$(P)/../../PADRE/PGSLIB -I$(P)/../../PADRE/PGSLIB -I$(P)/../../PGSLIB -I$(P)/../PGSLIB -g -fPIC -Wno-deprecated 
# P = /home/henshaw2/P++-3.2.2/A++P++-0.7.9d/P++/src/array_src
# PFLAGS = -DHAVE_CONFIG_H -I. -I$(P)/../.. -I$(P)/../../include -I$(P)/../../PARTI -I$(P)/../PARTI -I/usr/apps/mpich/default/include    -I$(P)/../../PADRE/src -I$(P)/../../PADRE -I$(P)/../../PARTI -I$(P)/../../PARTI -I$(P)/../../PADRE/PGSLIB -I$(P)/../../PADRE/PGSLIB -I$(P)/../../PGSLIB -I$(P)/../PGSLIB -I/usr/apps/mpich/default/include  -g  -Wstrict-prototypes -fPIC -Wno-deprecated

PADRE_Parti_Template.o: PADRE_Parti_Template.C
	$(CC) $(PFLAGS) -c PADRE_Parti_Template.C

PADRE_Descriptor.o: PADRE_Descriptor.C
	$(CC) $(PFLAGS) -c PADRE_Descriptor.C

partitioning.o: partitioning.C
	$(CC) $(PFLAGS) -c partitioning.C
conform_enforce.o: conform_enforce.C
	$(CC) $(PFLAGS) -c conform_enforce.C
p_array.o: p_array.C
	$(CC) $(PFLAGS) -c p_array.C

array.o: array.C
	$(CC) $(PFLAGS) -c array.C
p_abstract_op.o: p_abstract_op.C
	$(CC) $(PFLAGS) -c p_abstract_op.C
p_descriptor.o: p_descriptor.C
	$(CC) $(PFLAGS) -c p_descriptor.C
p_descriptor.060504.o: p_descriptor.060504.C
	$(CC) $(PFLAGS) -c p_descriptor.060504.C

index.o: index.C
	$(CC) $(PFLAGS) -c index.C

allocate.o: allocate.C
	$(CC) $(PFLAGS) -c allocate.C

datamove.o: datamove.c
	$(cc) $(PFLAGS) -c datamove.c

# .o: .C
# 	$(CC) $(PFLAGS) -c .C

# *** amrh is the main amr test routine ***
amrh=     amrh.o rk.o util.o getDt.o Regrid.o ../parallel/LoadBalancer.o
amrh=     amrh.o rk.o util.o getDt.o ../ogen/updateRefineFillInterpNew.o
amrh=     amrh.o rk.o util.o getDt.o Regrid.o ../parallel/updateRefinementNewer.o ../parallel/updateRefineFillInterpNew.o \
               ../parallel/updateRefinement.o ../parallel/updateRefinementNew.o
amrh=     amrh.o rk.o util.o getDt.o Regrid.o ../ogen/updateRefinement.o ../ogen/updateRefineFillInterp.o
amrh=     amrh.o rk.o util.o getDt.o Regrid.o 
amrh:     $(amrh); $(CC) $(CFLAGS) -o $@ $(amrh) $(CLIBS) $(FLIBS) $(GLIBS) 



# put this here temporarily while debugging
ParallelUtility.C: ParallelUtility.bC; bpp ParallelUtility.bC

# test functions in Interpolate AND InterpolateRefinements
#      -- test parallel interpolation
ir=     ir.o Regrid.o RotatedBox.o ListOfRotatedBox.o ErrorEstimator.o getDt.o InterpolateRefinements.o  \
            /home/henshaw/Overture/gf/interpolateRefinements.o /home/henshaw/Overture/gf/Interpolant.o
ir=     ir.o util.o Regrid.o RotatedBox.o ListOfRotatedBox.o ErrorEstimator.o getDt.o InterpolateRefinements.o  \
        Interpolate.o InterpolateParameters.o $(pcsFiles) 

ir=     ir.o util.o Interpolate.o InterpolateRefinements.o
ir=     ir.o util.o Interpolate.o InterpolateRefinements.o ParallelUtility.o $(pppFiles)
ir=     ir.o util.o Interpolate.o InterpolateRefinements.o $(pppFiles)
ir=     ir.o util.o Interpolate.o ../ogshow/CopyArray.o
ir=     ir.o util.o
ir:     $(ir); $(CC) $(CFLAGS) -o $@ $(ir) $(CLIBS) $(FLIBS) $(GLIBS) 

# OLD: (use ir instead) test Interpolate class (from dlb)
ti=   testInterpolateFineToCoarse.o InterpolateHelpfulFunctions.o testUtils.o
ti:     $(ti); $(CC) $(CFLAGS) -o $@ $(ti) $(CLIBS) $(FLIBS) $(GLIBS) 

interpFineFromCoarse.o : interpFineFromCoarse.f; $(FC) $(FFLAGSG) -c $*.f

interpFineFromCoarseNoMask.o : interpFineFromCoarse.f; $(FC) $(FFLAGSG) -c $*.f
interpFineFromCoarseMask.o : interpFineFromCoarse.f; $(FC) $(FFLAGSG) -c $*.f

# OLD: (use ir instead) test Interpolate class InterpolateCoarseToFine (from dlb)
ti2=   testInterp.o interpFineFromCoarseNoMask.o interpFineFromCoarseMask.o Interpolate.o InterpolateParameters.o \
       interpCoarseFromFine.o
ti2:     $(ti2); $(CC) $(CFLAGS) -o $@ $(ti2) $(CLIBS) $(FLIBS) $(GLIBS) 



test.o:		test.f rundat.h
	f90 $(FFLAGS90) -c test.f
tile.o:		tile.f rundat.h refine.h
	f90 $(FFLAGS90) -c tile.f


test= test.o tile.o
test:   $(test); f90 $(FFLAGS90) -o $@ $(test) $(FLIBS)


# to remake the dependencies:  make dep
dep: 
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)
	touch dependDate

# include the dependencies here
include depend

clean: 
	rm -f *.o tamr amrGrid amrh ht ti ti2 ir amrToBase

cleano: 
	rm -f *.o 

precious: depend

