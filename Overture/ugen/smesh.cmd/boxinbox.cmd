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
