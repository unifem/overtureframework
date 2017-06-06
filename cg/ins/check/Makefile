# Type "make check" to run the regression tests
# NOTE: The file tests contains the list of test cases

common = ../../common

check:  buildGrids
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgins

check-quiet:  buildGrids
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgins -verbose=0

cgins:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) cgins


shortHalfCylinder.hdf:
	${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/halfCylinder -name="shortHalfCylinder" -xa=-2.5 -xb=2.5 -factor=.5 -stretch=2.0

slideri1.order2.hdf:
	${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/slider -factor=1

square10.order4.hdf:
	${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/squareArg -order=4 -nx=10 

box10.order4.hdf:
	${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/box -order=4 -nx=10 

turek_hron_fsi2e2.hdf:
	${Overture}/bin/ogen -noplot ../runs/TurekHronFSI2/turek_hron_fsi2_grid -interp=e -factor=2

bicFixede1.order2.hdf:
	${Overture}/bin/ogen -noplot ../runs/beamPaperRuns/oneBeamTests/oneBeamInAChannelGrid -xb=5 -fixedRadius=0.3 -prefix=bic -interp=e -factor=1

plug4.hdf:
	${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/plug -factor=4

shearBlock4.order2.hdf:
	${Overture}/bin/ogen -noplot ../runs/piston/sliderGrid -prefix=shearBlock -nx=3 -factor=4

cylOffsetGridFixede2.order2.s3.hdf:
	${Overture}/bin/ogen -noplot ../runs/cylDrop/cylDropGrid -interp=e -radius=.2 -xa=-1 -xb=1 -ya=-3 -yb=1 -cx=.5 -cy=0 -rgd=fixed -deltaRadius0=.2 -prefix=cylOffsetGrid -factor=2

square.cmd: 
	ln -sf ${Overture}/sampleGrids/square.cmd $@

annulus.cmd: 
	ln -sf ${Overture}/sampleGrids/annulus.cmd $@

box.cmd:
	ln -sf ${Overture}/sampleGrids/box.cmd $@

cylinder.cmd: 
	ln -sf ${Overture}/sampleGrids/cylinder.cmd $@

# build grids that are used by the regression tests (and not already in Overture/sampleGrids)
buildGrids: shortHalfCylinder.hdf slideri1.order2.hdf box.cmd square.cmd annulus.cmd cylinder.cmd \
            square10.order4.hdf box10.order4.hdf turek_hron_fsi2e2.hdf bicFixede1.order2.hdf \
	    plug4.hdf shearBlock4.order2.hdf cylOffsetGridFixede2.order2.s3.hdf

