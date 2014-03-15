*dxdy .2 .2 
Create Curve 
new point 0 0 
new point 1 0 
new point 1 1 
new point 0 1 
*new point .25 .25 
*new point .25 .75 
*new point .75 .75 
*new point .75 .25 
new point .2 .2 
new point .2 .8 
new point .8 .8 
new point .8 .2 
point for interpolation 0 
Mouse Mode Interpolate Curve 
point for interpolation 1 
stop picking 
point for interpolation 0 
point for interpolation 3 
stop picking 
point for interpolation 1 
point for interpolation 2 
stop picking 
point for interpolation 2 
point for interpolation 3 
stop picking 
point for interpolation 4 
point for interpolation 5 
stop picking 
point for interpolation 4 
point for interpolation 7 
stop picking 
point for interpolation 7 
point for interpolation 6 
stop picking 
point for interpolation 5 
point for interpolation 6 
stop picking 
exit 
DISPLAY AXES:0 0 
Create Unstructured Region 
select outer 0 
select outer 1 
select outer 2 
select outer 3 
Done 
select inner 4 
select inner 5 
select inner 6 
select inner 7 
Done 
Done 
generate 
Save Mesh 
/home/chand/overture/2dmesh/test.msh 
tb Plot Reference Grids 0
Create Curves
Mouse Mode Build Point
new point 4.562405e-01 6.106556e-01
new point 3.473007e-01 4.564213e-01
new point 5.530701e-01 3.302314e-01
new point 6.219552e-01 4.536961e-01
new point 5.865880e-01 6.337447e-01
Mouse Mode Interpolate Curve
point for interpolation 8
point for interpolation 9
point for interpolation 10
point for interpolation 11
point for interpolation 12
point for interpolation 8
stop picking
stop picking
exit
mm nCurvePts
points on curve 10  24
DISPLAY AXES:0 1
dxdy ..1,..1
Create Unstructured Region
select outer 10
Done
Done
use cutout 0
generate
mm delRegion
delete region 1
dxdy 0.01, 0.01
Create Unstructured Region
select outer 10
Done
Done
use cutout 0
generate
set view:0 0.0499306 -0.0263523 0 3.79695 1 0 0 0 1 0 0 0 1
smaller:0
delete region 2
dxdy 0.05, 0.05
Create Unstructured Region
select outer 10
Done
Done
use cutout 0
generate
mm optimize
optimize region 3
optimize region 3
optimize region 3
optimize region 3
optimize region 3
optimize region 3
mm delRegion
delete region 3
dxdy 0.03, 0.03
Create Unstructured Region
select outer 10
Done
Done
use cutout 0
generate
dxdy 0.08, 0.08
delete region 4
Create Unstructured Region
select outer 10
Done
Done
generate
mm optimize
optimize region 5
optimize region 5
optimize region 5
optimize region 5
dxdy 0.05, 0.05
mm delRegion
delete region 5
Create Unstructured Region
select outer 10
Done
Done
use cutout 0
generate
mm optimize
optimize region 6
optimize region 6
optimize region 6
optimize region 6
optimize region 6
optimize region 6
optimize region 6
optimize region 6
optimize region 6
optimize region 6
optimize region 6
