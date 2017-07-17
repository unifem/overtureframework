# Type "make check" to run the regression tests
# NOTE: The file tests contains the list of test cases

common = ../../common
check: buildGrids
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgmp

check-quiet: buildGrids
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgmp -verbose=0

cgmp:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) cgmp


# build grids that are used by the regression tests (and not already in Overture/sampleGrids)
buildGrids: twoSquaresInterfacee1.order2.hdf elasticPistonGrid2.hdf diskDeformFixedBig2e.hdf deformingEllipseGride2.hdf
	ln -sf ../cmd/adDomain.h .
	ln -sf ../cmd/cnsDomain.h .
	ln -sf ../cmd/insDomain.h .
	ln -sf ../cmd/smDomain.h .

twoSquaresInterfacee1.order2.hdf:
	${Overture}/bin/ogen noplot ${Overture}/sampleGrids/twoSquaresInterface -factor=1 -yFactor=1 -interp=e

elasticPistonGrid2.hdf:
	${Overture}/bin/ogen noplot ${Overture}/sampleGrids/elasticPistonGrid -factor=2

diskDeformFixedBig2e.hdf:
	${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/diskDeform -fixedRadius=.25 -factor=2 -interp=e -xa=-2.5 -xb=2.5 -ya=-2.5 -yb=2.5 -name=diskDeformFixedBig2e.hdf


deformingEllipseGride2.hdf:
	${Overture}/bin/ogen -noplot ../runs/shockEllipse/deformingEllipseGrid -factor=2 -interp=e -width=.25
