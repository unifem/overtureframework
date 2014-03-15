common = $(CG)/common

# The convergence rate script is called conv.p and found in the Cg directory
all:  buildGrids
	ln -sf $(common)/src/conv.p .


rhombus1.order4.ml1.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhombus -order=4 -factor=1 -ml=1
rhombus2.order4.ml2.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhombus -order=4 -factor=2 -ml=2
rhombus4.order4.ml3.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhombus -order=4 -factor=4 -ml=3
rhombus8.order4.ml4.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhombus -order=4 -factor=8 -ml=4

rhomboid1.order4.ml1.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhomboid -order=4 -factor=1 -ml=1
rhomboid2.order4.ml2.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhomboid -order=4 -factor=2 -ml=2
rhomboid4.order4.ml3.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhomboid -order=4 -factor=4 -ml=3
rhomboid8.order4.ml4.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rhomboid -order=4 -factor=8 -ml=4

orthoSphere1.order4.ml1.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/orthoSphere -order=4 -factor=1 -ml=1
orthoSphere2.order4.ml2.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/orthoSphere -order=4 -factor=2 -ml=2
orthoSphere4.order4.ml3.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/orthoSphere -order=4 -factor=4 -ml=3
orthoSphere8.order4.ml4.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/orthoSphere -order=4 -factor=8 -ml=4

rotatedBox8.order4.hdf :;  ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rotatedBox -order=4 -n=8
rotatedBox16.order4.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rotatedBox -order=4 -n=16
rotatedBox32.order4.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rotatedBox -order=4 -n=32
rotatedBox64.order4.hdf :; ${Overture}/bin/ogen -noplot ${Overture}/sampleGrids/rotatedBox -order=4 -n=64

buildGrids : rhombus1.order4.ml1.hdf rhombus2.order4.ml2.hdf rhombus4.order4.ml3.hdf rhombus8.order4.ml4.hdf \
             rhomboid1.order4.ml1.hdf rhomboid2.order4.ml2.hdf rhomboid4.order4.ml3.hdf rhomboid8.order4.ml4.hdf \
             orthoSphere1.order4.ml1.hdf orthoSphere2.order4.ml2.hdf orthoSphere4.order4.ml3.hdf orthoSphere8.order4.ml4.hdf \
             rotatedBox8.order4.hdf rotatedBox16.order4.hdf rotatedBox32.order4.hdf rotatedBox64.order4.hdf

ogmgt:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) ogmgt
