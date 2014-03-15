*box bounds 0,0,100,10
*dims 102,12
*dims 201,21
dxdy 1.,1.
Create Curve
new point 0.   0.
new point 0.   10.
new point 100. 0.
new point 100. 10.
new point 25.  8.
new point 25.  2.
new point 50.  8.
new point 50.  2.
new point 75.  8.
new point 75.  2.
arc segment 5 4
radius of curvature 3
arc segment 4 5
radius of curvature 3
arc segment 7 6
radius of curvature 3
arc segment 6 7
radius of curvature 3
arc segment 9 8
radius of curvature 3
arc segment 8 9
radius of curvature 3
point for interpolation 0
point for interpolation 1
stop picking
point for interpolation 0
point for interpolation 2
stop picking
point for interpolation 2
point for interpolation 3
stop picking
point for interpolation 3
point for interpolation 1
stop picking
exit
Create Unstructured Region
select outer 9
select outer 6
select outer 7
select outer 8
Done
select inner 1
select inner 0
Done
select inner 3
select inner 2
Done
select inner 5
select inner 4
Done
Done
use cutout 0
generate
Create Unstructured Region
select outer 1
select outer 0
Done
Done
use cutout 0
generate
Create Unstructured Region
select outer 3
select outer 2
Done
Done
use cutout 0
generate
Create Unstructured Region
select outer 4
select outer 5
Done
Done
use cutout 0
generate
Save Mesh
/home/chand/overture/2dmesh/test.msh
