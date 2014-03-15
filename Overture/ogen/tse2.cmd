read command file
tse.cmd
*
*  Make a grid for a two-stroke engine
*
*
* ----- here is the core of the main cylinder -----
*
Box
specify
-.75 -.75 -.5   .75 .75  1.
lines
19 19 17
* 19 19 5
boundary
0 0  0 0  0 2 
* 0 0  0 0  3 2 
* share: 2=top
share
0 0  0 0  0 2
* 0 0  0 0  3 2
mappingName
cylinder-core
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
49 17 9
* 49 5 9
boundary conditions
-1 -1  0 2  0 1
* -1 -1  3 2  0 1
periodicity
2 0 0
* share: top=2 outside=1
share
0 0  3 2  0 1 
* 0 0  0 2  0 1 
mappingName
unrotated-cylinder
show parameters
exit
*
* ----- here is the core of piston -----
*
Box
specify
-.75 -.75 -1. .75 .75 -.25 
lines
19 19 21
boundary
0 0  0 0  1 0
* bottom=2
share
0 0  0 0  2 0
mappingName
piston-core
exit
*
*  Here is the piston (theta,axial,r)
*
Cylinder
bounds on the radial variable
.65 1.
bounds on the axial variable
-1. -.25
lines
71 21 9
boundary conditions
-1 -1  1 0 0 1 
periodicity
2 0 0
* share 2=bottom, 1=outside
share
0 0  2 0  0 1
mappingName
unrotated-piston
exit
rotate/scale/shift
transform which mapping?
CylinderMapping : unrotated-cylinder
rotate
90. 0
0 0 0
rotate
-90. 0
0 0 0
rotate
-90  0
0 0 0
mappingName
cylinder
exit
rotate/scale/shift
transform which mapping?
CylinderMapping : unrotated-piston
rotate
-90 0
0 0 0
mappingName
piston
exit
savve
save command file
