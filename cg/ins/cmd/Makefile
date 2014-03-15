# Make for the cmd directory to make it easier to re-make things in the parent directory

cgins:
	@cd ..;      $(MAKE) -$(MAKEFLAGS) cgins

# Here are targets to build grids for some of the examples. 

# sphereMove.cmd:
sphereInABoxe1.order2.hdf:
	ln -s $(Overture)/sampleGrids/sphereThreePatch.h . 
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/sphereInABox -order=2 -interp=e -factor=2

sphereInABoxe1.order4.ml1.hdf:
	ln -s $(Overture)/sampleGrids/sphereThreePatch.h . 
	$(Overture)/bin/ogen noplot $(Overture)/sampleGrids/sphereInABox -order=4 -interp=e -factor=1 -ml=1