ogen = ${Overture}/bin/ogen

doc:
	pdflatex freeSurface

grids:
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -factor=2 -ml=1
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -factor=4 -ml=1
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -factor=8 -ml=2
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -factor=16 -ml=3
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -factor=32 -ml=3
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -periodic=p -factor=2 
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -periodic=p -factor=4
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -periodic=p -factor=8 
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -periodic=p -factor=16
	$(ogen) -noplot freeSurfaceGrid2d -interp=e -periodic=p -factor=32 
