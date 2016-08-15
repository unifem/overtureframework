ogen = ${Overture}/bin/ogen

# generate some grids
grids : grids2d risingGrids

# 2D grids: 
grids2d:
	$(ogen) -noplot fallingBodyGrid -cy=-0.0 -interp=e -order=2 -factor=2
	$(ogen) -noplot fallingBodyGrid -cy=-0.0 -interp=e -order=2 -factor=4
	$(ogen) -noplot fallingBodyGrid -cy=-0.0 -interp=e -order=2 -factor=8
	$(ogen) -noplot fallingBodyGrid -cy=-0.0 -interp=e -order=2 -factor=16
	$(ogen) -noplot fallingBodyGrid -cy=-0.0 -interp=i -order=2 -factor=2
	$(ogen) -noplot fallingBodyGrid -cy=-0.0 -interp=i -order=2 -factor=4
	$(ogen) -noplot fallingBodyGrid -cy=-0.0 -interp=i -order=2 -factor=8
	$(ogen) -noplot fallingBodyGrid -cy=-0.0 -interp=i -order=2 -factor=16


#  Top -refinement:
#     ogen -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=e -order=2 -factor=2
risingGrids:
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=e -order=2 -factor=1
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=e -order=2 -factor=2
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=e -order=2 -factor=4
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=e -order=2 -factor=8
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=e -order=2 -factor=16
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=e -order=2 -factor=32
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=i -order=2 -factor=2
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=i -order=2 -factor=4
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=i -order=2 -factor=8
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=i -order=2 -factor=16
	$(ogen) -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=i -order=2 -factor=32

# two falling bodies

twoFallingBodies:
	$(ogen) -noplot twoFallingBodiesGrid -interp=e -order=2 -factor=2
	$(ogen) -noplot twoFallingBodiesGrid -interp=e -order=2 -factor=4
	$(ogen) -noplot twoFallingBodiesGrid -interp=e -order=2 -factor=8
	$(ogen) -noplot twoFallingBodiesGrid -interp=e -order=2 -factor=16
	$(ogen) -noplot twoFallingBodiesGrid -interp=i -order=2 -factor=2
	$(ogen) -noplot twoFallingBodiesGrid -interp=i -order=2 -factor=4
	$(ogen) -noplot twoFallingBodiesGrid -interp=i -order=2 -factor=8
	$(ogen) -noplot twoFallingBodiesGrid -interp=i -order=2 -factor=16
