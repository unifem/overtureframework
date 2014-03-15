pause
Create Curve 
Mouse Mode Build Point 
new point 0 0 
new point 1 0 
new point 0 1 
new point 0 .5 
new point .5 0. 
new point .75 0. 
new point 0. .75 
pause
Mouse Mode Interpolate Curve 
Mouse Mode Circular Arc 
arc segment 1 2 
radius of curvature 1 
arc segment 5 6 
radius of curvature .75 
arc segment 4 3 
radius of curvature .5 
pause
Mouse Mode Build Point 
new point 1,1 
DISPLAY AXES:0 0 
Mouse Mode Interpolate Curve 
point for interpolation 0 
point for interpolation 4 
stop picking 
point for interpolation 0 
point for interpolation 3 
stop picking 
stop picking 
point for interpolation 3 
point for interpolation 6 
stop picking 
point for interpolation 6 
point for interpolation 2 
stop picking 
point for interpolation 2 
point for interpolation 7 
stop picking 
point for interpolation 7 
point for interpolation 1 
stop picking 
point for interpolation 1 
point for interpolation 5 
stop picking 
point for interpolation 5 
point for interpolation 4 
stop picking 
exit 
*dims 21,21 
dxdy .05,.05
pause
Create Unstructured Region
select outer 5
select outer 4
select outer 3
Done
Done
generate
pause
tb Plot Reference Grids 0
pause
Create TFI Region
select left 6
Done
select right 10
Done
select bottom 3
Done
select top 2
Done
generate
pause
Create TFI Region
select left 7
Done
select right 9
Done
select bottom 2
Done
select top 1
Done
generate
pause
Create Unstructured Region
select outer 8
select outer 1
select outer 0
Done
Done
generate
pause
Save Mesh
/home/chand/overture/2dmesh/test.msh
