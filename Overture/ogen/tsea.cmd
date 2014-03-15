*
*  Make a grid for cylinder
*
*
* ----- here is the core of the main cylinder -----
*
Box
specify
-.75 -.75 -.5   .75 .75  1.
lines
19 19 5
boundary
0 0  0 0  3 2 
* share: 2=top
share
0 0  0 0  3 2
mappingName
unrotated-cylinder-core
exit
* now rotate
rotate/scale/shift
transform which mapping?
unrotated-cylinder-core
rotate
-90. 0.
0. 0. 0.
mappingName
cylinder-core
show parameters
exit
*
*  Here is the main cylinder (theta,axial,r)
*
Cylinder
bounds on the radial variable
.65 1.
bounds on the axial variable
-.5 1.
lines
49 5 9
boundary conditions
-1 -1  3 2  0 1
periodicity
2 0 0
* share: top=2 outside=1
share
0 0  0 2  0 1 
mappingName
unrotated-cylinder
* initial-cylinder
exit
* now rotate so cylinder-axis is along the y-axis
rotate/scale/shift
transform which mapping?
unrotated-cylinder
* initial-cylinder
mappingName
cylinder
rotate
-90. 0.
0. 0. 0.
show parameters
exit
