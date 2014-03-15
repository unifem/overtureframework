Create Curves 
new point 0 0 
new point .5 0 
new point .5 1 
new point 0 1 
Mouse Mode Interpolate Curve 
point for interpolation 0 
point for interpolation 1 
stop picking 
point for interpolation 1 
point for interpolation 2 
stop picking 
point for interpolation 2 
point for interpolation 3 
stop picking 
point for interpolation 3 
point for interpolation 0 
stop picking 
exit 
DISPLAY AXES:0 0 
mm nCurvePts 
points on curve 3  21 
points on curve 0  41 
points on curve 1  21 
points on curve 2  5 
mm strCurve 
stretch points on curve 3
Stretch r1:exp
STP:stretch r1 exp: min dx .0025
stretch grid
STP:stretch r1 exp: cluster at r=1
stretch grid
exit
stretch points on curve 1
Stretch r1:exp
STP:stretch r1 exp: min dx .005
stretch grid
exit
Create Unstructured Region
select outer 3
select outer 0
select outer 1
select outer 2
Done
Done
use cutout 0
generate
optimize region 0
