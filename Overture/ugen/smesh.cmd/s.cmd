Create Curves 
Mouse Mode Build Point 
new point 9.925371e-02 6.506476e-01 
new point 3.996265e-01 8.234406e-01 
new point 7.365000e-01 6.344963e-01 
new point 8.320803e-01 3.572776e-01 
Mouse Mode Interpolate Curve 
point for interpolation 0 
point for interpolation 1 
point for interpolation 2 
point for interpolation 3 
stop picking 
exit 
Create Curves 
Mouse Mode Build Point 
new point 1.801515e-01 1.425515e-01 
Mouse Mode Interpolate Curve 
point for interpolation 0 
point for interpolation 4 
point for interpolation 3 
stop picking 
exit 
Create Unstructured Region 
select outer 0 
select outer 1 
Done 
Done 
use cutout 0 
generate 
tb Plot Reference Grids 0 
mm optimize 
optimize region 0 
optimize region 0 
optimize region 0 
mm delRegion 
delete region 0 
Create Unstructured Region 
select outer 0 
select outer 1 
Done 
Done 
use cutout 0 
use cutout 1 
generate 
mm optimize 
optimize region 1 
optimize region 1 
optimize region 1 
optimize region 1 
optimize region 1 
optimize region 1 
optimize region 1 
optimize region 1 
mm delRegion 
delete region 1 
Create Unstructured Region 
select outer 0 
select outer 1 
Done 
Done 
dx,dy :  0.01,0.01
generate
