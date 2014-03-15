*
* Create an overlapping grid for a 2D valve
*
create mappings
*
* First make a back-ground grid
*
rectangle
mappingName
backGround
specify corners
0 0 2. 1.
lines
65 33
exit
*
* Now make the valve
*
SmoothedPolygon
mappingName
valve
vertices
4
1.4 0.
1.4 .75
1.65 .5
1.65 0.
n-dist
fixed normal distance
.175
lines
65 17
boundary conditions
1 1 1 0
share
1 61 1 0
sharpness
30
30
30
30
reset
exit
*
* Here is the part of the boundary that the valve closes against
*
SmoothedPolygon
vertices
4
2. .5
1.75 .5
1.5 .75
1.5 1.
n-dist
fixed normal distance
.175
lines
65 17
boundary conditions
1 1 1 0
share
1 1 1 0
mappingName
stopper
exit
* pause here
pause
exit
*
* Make the overlapping grid
*
make an overlapping grid
3
backGround
stopper
valve
EXIT
*
save an overlapping grid
valve.hdf
valve
exit
