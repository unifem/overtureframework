Create Curve
new point 0 0
new point 0 1
arc segment 0 1
radius of curvature .5
arc segment 1 0
radius of curvature .5
exit
*dims 51,51
*dims 11,11
dxdy .1,.1
Create Unstructured Region
select outer 0
select outer 1
Done
Done
generate
Save Mesh
/home/chand/overture/2dmesh/test.msh

tb Plot Reference Grids 0
tb Plot Reference Grids 1
tb Plot Reference Grids 0
tb Plot Reference Grids 1
