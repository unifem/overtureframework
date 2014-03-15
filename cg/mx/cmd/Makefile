# Make for the cmd directory to make it easier to re-make things in the parent directory

cgmx:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) cgmx


# Here are targets to build grids for some of the examples. 

# Grids for afm.cmd
#  ogen noplot afm -interp=e -order=4 -factor=4

# grids for cyleigen.cmd: 
tube1.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/tube1.order4.cmd
tube2.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/tube2.order4.cmd


# grids for cic.planeWaveBC
cice3.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/cicArg -factor=3 -order=4 -interp=e 
cice16.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/cicArg -factor=16 -order=4 -interp=e 


# grids for dielectricCyl
innerOuter4.order2.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/innerOuter -factor=4 -order=2 -deltaRad=.5 -interp=e -name="innerOuter4.order2.hdf"
innerOuter4.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/innerOuter -factor=4 -order=4 -deltaRad=.5 -interp=e -name="innerOuter4.order4.hdf"
innerOuter8.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/innerOuter -factor=8 -order=4 -deltaRad=.5 -interp=e -name="innerOuter8.order4.hdf"
innerOuter16.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/innerOuter -factor=16 -order=4 -deltaRad=.5 -interp=e -name="innerOuter16.order4.hdf"

# grids for scat.cmd 
trianglee4.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/triangleArg -factor=4 -order=4 -interp=e
trianglee8.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/triangleArg -factor=8 -order=4 -interp=e

# grids for sib.planeWaveBC
sib1.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/sib1.order4.cmd
sib2.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/sib2.order4.cmd
sib4.order4.hdf:
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/sib4.order4.cmd


# Grids for rbc:

# ogen -noplot squareArg -order=4 -nx=128
# ogen noplot boxArg -order=4 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -factor=4 -name="boxLx2Ly2Lz2Factor4.order4.hdf"


# Grids for lattice.cmd
#   ogen -noplot lattice -order=4 -interp=e -nCylx=3 -nCyly=3 -factor=2 -name="lattice3x3yFactor2.order4.hdf"
#   ogen -noplot lattice -order=4 -interp=e -nCylx=3 -nCyly=3 -factor=4 -name="lattice3x3yFactor4.order4.hdf"