#
# Type 'make check' to run regression tests
#
# Type 
#   check.p -replace                   : to replace all check files with new ones
#   check.p -test=cic4.check           : only run this check
#   check.p -replace -test=cic4.check  : only run and replace this check
#
# NOTE: file "tests" defines the executable and regression tests. 
#       file "parallel_tests.pm" holds the parallel tests
check:  buildGrids
	ln -sf $(CG)/common/src/check.p .
	check.p -solver=ogmgt


square64.order2.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/squareArg -nx=64

cic.bbmg2.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/cic.bbmg2.cmd

rhombus2.order2.ml2.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhombus.cmd -order=2 -factor=2 -ml=2

rhomboid2.order2.ml2.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhomboid.cmd -order=2 -factor=2 -ml=2

rhombus2.order4.ml2.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhombus.cmd -order=4 -factor=2 -ml=2

rhomboid2.order4.ml2.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhomboid.cmd -order=4 -factor=2 -ml=2

valvee.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/valvee.cmd

box32.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/box32.cmd

sib.bbmg.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/sib.bbmg.cmd


ellipsoid.bbmg.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/ellipsoid.bbmg.cmd

box32.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/box32.cmd

cice4.order4.ml3.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/cicArg -order=4 -interp=e -ml=3 -factor=4

sibe2.order4.ml3.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/sibArg -order=4 -interp=e -factor=2 -ml=3

square32p.order2.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/squareArg -periodic=p -nx=32
square32p.order4.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/squareArg -periodic=p -order=4 -nx=32

square32np.order2.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/squareArg -periodic=np -nx=32
square32np.order4.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/squareArg -periodic=np -order=4 -nx=32

square32.order2.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/squareArg -nx=32 
square32.order4.ng3.hdf:; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/squareArg -order=4 -nx=32 -numGhost=3

# build grids that are used by the regression tests (and not already in Overture/sampleGrids)
buildGrids: square64.order2.hdf cic.bbmg2.hdf rhombus2.order2.ml2.hdf rhomboid2.order2.ml2.hdf rhombus2.order4.ml2.hdf rhomboid2.order4.ml2.hdf valvee.hdf box32.hdf sib.bbmg.hdf ellipsoid.bbmg.hdf cice4.order4.ml3.hdf sibe2.order4.ml3.hdf square32p.order2.hdf square32p.order4.hdf square32np.order2.hdf square32np.order4.hdf square32.order2.hdf square32.order4.ng3.hdf