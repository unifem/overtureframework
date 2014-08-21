# Type "make check" to run the regression tests
# NOTE: The file tests contains the list of test cases
#
# To replace existing check files with current:
#  check.p -solver=cgsm replace
# 
common = ../../common
check: buildGrids
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgsm

check-quiet: buildGrids
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgsm -verbose=0

square10.order2.hdf:
	${Overture}/bin/ogen noplot ${Overture}/sampleGrids/squareArg -order=2 -nx=10 

# build grids that are used by the regression tests (and not already in Overture/sampleGrids)
buildGrids: square10.order2.hdf


cgsm:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) cgsm


