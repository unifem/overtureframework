# Type "make check" to run the regression tests
common = ../../common

check:  buildGrids
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgmx

check-quiet:  buildGrids
	ln -sf $(common)/src/check.p .
	./check.p -solver=cgmx -verbose=0

tube1.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/tube1.order4.cmd

square16.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/square16.order4.cmd

cici1.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/cicArg -factor=1 interp=i -order=4

box16.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/box16.order4.cmd

innerOuter4.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/innerOuter -factor=4 -order=4 -deltaRad=.5 -interp=e -name="innerOuter4.order4.hdf"

twoBoxesInterfacee111.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/twoBoxesInterface -order=4 -interp=e -factor=1

# nore ghost points for sosup: 
square20.order4.ng3.hdf:
	$(Overture)/bin/ogen -noplot  $(Overture)/sampleGrids/squareArg -order=4 -nx=20 -numGhost=3

square20.order6.ng4.hdf:
	$(Overture)/bin/ogen -noplot  $(Overture)/sampleGrids/squareArg -order=6 -nx=20 -numGhost=4

solidSphereInABoxe2.order2.hdf:
	$(Overture)/bin/ogen -noplot  $(Overture)/sampleGrids/solidSphereInABox -order=2 -interp=e -factor=2

solidSphereInABoxe2.order4.ng3.hdf:
	$(Overture)/bin/ogen -noplot  $(Overture)/sampleGrids/solidSphereInABox -order=4 -interp=e -numGhost=3 -factor=2


# build grids that are used by the regression tests (and not already in Overture/sampleGrids)
buildGrids: tube1.order4.hdf square16.order4.hdf cici1.order4.hdf box16.order4.hdf innerOuter4.order4.hdf \
            twoBoxesInterfacee111.order4.hdf square20.order4.ng3.hdf square20.order6.ng4.hdf \
            solidSphereInABoxe2.order2.hdf solidSphereInABoxe2.order4.ng3.hdf
	touch buildGrids


# checkCheckFiles:	
# 	@cd $(common); $(MAKE) -$(MAKEFLAGS) $(BINDIR)checkCheckFiles

cgmx:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) cgmx

clean:
	rm -f *.hdf



