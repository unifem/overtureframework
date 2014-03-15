include ../../config/Makefile.$(VENDOR)

# compile without opt:
CCFLAGSF= $(CCFLAGSG)
CCFLAGSO= $(CCFLAGSG)
CCFLAGS = $(CCFLAGSG)
FFLAGSF = $(FFLAGSG)
FFLAGSO = $(FFLAGSG)


all:
	gDeriv.p
	bpp dsg.bf
	bpp assignBoundaryConditions.bf

# findInterpolationNeighbours.o: findInterpolationNeighbours.f

dsgc4.f : dsgc4.bf; bpp dsgc4.bf
dsgc6.f : dsgc6.bf; bpp dsgc6.bf

div.f: div.bf; bpp -clean -quiet div.bf

assignCornersOpt.f: assignCornersOpt.bf; bpp -clean -quiet assignCornersOpt.bf

fixBCOptEdge1.f: assignCornersOpt.bf; bpp -clean -quiet assignCornersOpt.bf
fixBCOptEdge2.f: assignCornersOpt.bf; bpp -clean -quiet assignCornersOpt.bf
fixBCOptEdge3.f: assignCornersOpt.bf; bpp -clean -quiet assignCornersOpt.bf
fixBCOptVerticies.f: assignCornersOpt.bf; bpp -clean -quiet assignCornersOpt.bf

corners: assignCornersOpt.o fixBCOptEdge1.o fixBCOptEdge2.o fixBCOptEdge3.o fixBCOptVerticies.o

extrapolateOpt.f : extrapolateOpt.bf; bpp -quiet -clean extrapolateOpt.bf 

# New version: 091123
findInterpNeighbours.f : findInterpNeighbours.bf; bpp findInterpNeighbours.bf

assignBoundaryConditions.f : assignBoundaryConditions.bf
	bpp -clean -quiet assignBoundaryConditions.bf
assignOptNormalComponent.f : assignBoundaryConditions.f
assignOptTangentialComponent.f : assignBoundaryConditions.f

clean:
	rm -f *.o
