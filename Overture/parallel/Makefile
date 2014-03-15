include ../config/Makefile.$(VENDOR)

# over-ride defaults
# === use these for opt ===== (setenv compile opt)
ifeq ($(compile),opt)
  CCFLAGS= $(CCFLAGSO)
  CCFLAGSG= $(CCFLAGSO)
endif

current = .
mapping = $(current)/../mapping
# ogshow = $(current)/../ogshow

VPATH = $(mapping):$(ogshow)


all:      refine

DIR=`pwd`
dir:; @echo "pwd = $(DIR)"

linkFiles:;
	ln -sf ../ogen/refine.C .
	ln -sf ../ogen/checkInterpolation.C .
	ln -sf ../ogen/updateRefinement.bC .
	ln -sf ../ogen/updateRefinement.C .
	ln -sf ../ogen/updateRefinementNew.bC
	ln -sf ../ogen/updateRefinementNew.C
	ln -sf ../ogen/updateRefinementNewer.bC
	ln -sf ../ogen/updateRefinementNewer.C
	ln -sf ../ogen/updateRefineFillInterp.bC
	ln -sf ../ogen/updateRefineFillInterp.C
	ln -sf ../ogen/updateRefineFillInterpNew.bC
	ln -sf ../ogen/updateRefineFillInterpNew.C
	ln -sf ../ogen/updateRefinementMacros.h
	ln -sf ../primer/getDt.C .
	ln -sf ../ogshow/contour.C
	ln -sf ../ogshow/grid.C
	ln -sf ../ogshow/ParallelUtility.C




updateRefinement.C : updateRefinement.bC; bpp updateRefinement.bC
updateRefinementNew.C : updateRefinementNew.bC; bpp updateRefinementNew.bC
updateRefinementNewer.C : updateRefinementNewer.bC; bpp updateRefinementNewer.bC
updateRefineFillInterp.C : updateRefineFillInterp.bC; bpp updateRefineFillInterp.bC
updateRefineFillInterpNew.C : updateRefineFillInterpNew.bC; bpp updateRefineFillInterpNew.bC

aggregate.C : aggregate.bC; bpp aggregate.bC

CanInterpolate.C : CanInterpolate.bC; bpp CanInterpolate.bC

CCFLAGSOGEN = $(CCFLAGSF)
# CCFLAGSOGEN = $(CCFLAGSG)
# Here are ogen files we like to optimise
# **NOTE** pipes.cmd gives a different answer depending on compiling classify.C with opt or not

updateRefinement.o: updateRefinement.C; $(CC) $(CCFLAGSG) -c updateRefinement.C
updateRefinementNew.o: updateRefinementNew.C; $(CC) $(CCFLAGSG) -c updateRefinementNew.C

updateRefineFillInterpNew.o: updateRefineFillInterpNew.C; $(CC) $(CCFLAGSF) -c updateRefineFillInterpNew.C
CanInterpolate.o: CanInterpolate.C; $(CC) $(CCFLAGSF) -c CanInterpolate.C
## CanInterpolate.o: CanInterpolate.C; $(CC) $(CCFLAGSG) -c CanInterpolate.C

Interpolate.C : Interpolate.bC; bpp Interpolate.bC

ParallelOverlappingGridInterpolator.C: ParallelOverlappingGridInterpolator.bC; bpp -quiet ParallelOverlappingGridInterpolator.bC
ParallelOverlappingGridInterpolator.o: ParallelOverlappingGridInterpolator.C
	 $(CC) $(CCFLAGSG) -c ParallelOverlappingGridInterpolator.C

# These files we build dependency info for:
SRC = refine.C updateRefinement.C updateRefinementNew.C updateRefinementNewer.C tlb.C LoadBalancer.C \
      tci.C CanInterpolate.C updateRefineFillInterp.C updateRefineFillInterpNew.C


copyCoarseToFine.C : copyCoarseToFine.bC; bpp copyCoarseToFine.bC

refine=   refine.o checkInterpolation.o updateRefinementNewer.o updateRefineFillInterpNew.o CanInterpolate.o \
           ../ogen/checkOverlappingGrid.o 
refine=   refine.o checkInterpolation.o updateRefinementNewer.o updateRefineFillInterpNew.o CanInterpolate.o
refine=   refine.o checkInterpolation.o ../amr/InterpolateRefinements.o
refine=   refine.o checkInterpolation.o ../gf/ParallelOverlappingGridInterpolatorNew.o
refine=   refine.o checkInterpolation.o ../grid/CompositeGrid.o
refine=   refine.o checkInterpolation.o ../op/fortranDeriv/extrapolateOpt.o
refine=   refine.o checkInterpolation.o updateRefinementNewer.o
refine=   refine.o checkInterpolation.o updateRefinementNewer.o updateRefineFillInterpNew.o CanInterpolate.o
refine:   $(refine); $(CC) $(CFLAGS) -o $@ $(refine) $(CLIBS)  $(FLIBS)  $(GLIBS)



# Test the load balancer
tlb=   tlb.o LoadBalancer.o GridDistribution.o
tlb:   $(tlb); $(CC) $(CFLAGS) -o $@ $(tlb) $(CLIBS)  $(FLIBS)  $(GLIBS)

# Another test routine for the load balancer
tload=   tload.o LoadBalancer.o ../grid/GenericGridCollection.o ../grid/GridCollection.o \
         ../grid/CompositeGrid.o
tload=   tload.o LoadBalancer.o MappedGrid.o GridCollection.o GenericGridCollection.o
tload:   $(tload); $(CC) $(CFLAGS) -o $@ $(tload) $(CLIBS)  $(FLIBS)  $(GLIBS)


# Parallel wave equation solver
# Compile optimized:
pwave.o : pwave.C; $(CC) $(CCFLAGSG) -c $*.C
pwave = pwave.o getDt.o MappedGrid.o CompositeGrid.o LoadBalancer.o contour.o ParallelUtility.o grid.o
pwave = pwave.o getDt.o 
pwave: $(pwave) 
	$(CC) $(CCFLAGS) -o pwave $(pwave) $(CLIBS) $(FLIBS) $(GLIBS) 


# test the parallel canInterpolate
tci = tci.o ../ogshow/ParallelUtility.o
tci = tci.o CanInterpolate.o updateRefinementNew.o updateRefinementNewer.o \
          updateRefineFillInterp.o updateRefineFillInterpNew.o
tci = tci.o CanInterpolate.o
tci: $(tci) 
	$(CC) $(CCFLAGS) -o tci $(tci) $(CLIBS) $(FLIBS) $(GLIBS) 

# for tracking down a bug:  MPI_REAL --> is the same as MPI_FLOAT, use MPI_Real
tcii = tcii.o 
tcii: $(tcii) 
	$(CC) $(CCFLAGS) -o tcii $(tcii) $(CLIBS) $(FLIBS) $(GLIBS) 



# to remake the dependencies:  'make dep'
dep: ; 
	makeDepend -fdepend -Y -- $(CCFLAGS) -- $(SRC)

# include the dependencies here
include depend

clean: 
	rm -f *.o refine tlb tload pwave tci tcii

precious:
