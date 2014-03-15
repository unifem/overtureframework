*
* Here is a port on a box (for the two-stroke-engine)
*
Box
specify
-1.-1. -1.    1. 1. 1.
lines
11 11 11 
boundary
1 1 2 2 3 3 
mappingName
box
exit
*
* Here is the cross-section of the exhaust port
rectangle
specify corners
1. -1. 2. -.5
mappingName
2d-exhaust
exit
*
* make a 3d exhaust port
*
body of revolution
revolve which mapping?
2d-exhaust
start/end angle
-30. 30.
boundary conditions
1 1 2 2 3 3
lines
19 13 11
tangent of line to revolve about
0 1 0
choose a point on the line to revolve about
0 0 0
choose a point on the line to revolve about
0 0 0
mappingName
3d-exhaust
exit
* Stretch coordinates
stretch coordinates
transform which mapping?
3d-exhaust
mappingName
stretched-3d-exhaust
stretch
specify stretching along axis=0
layers
1
1. 5. 0.
exit
exit
exit
*
* shift to the right spot
*
rotate/scale/shift
transform which mapping?
stretched-3d-exhaust
shift
-.025 -.05 0.
mappingName
exhaust
exit



delete
2d-exhaust
delete 
3d-exhaust
delete
stretched-3d-exhaust



*
*   Here is the large exhaust port
*
grid
 external
  transform
   analytic
    rectangle
     1. -1. 2. -.5
   3
   revolution
    0. 0. 0.
    0. 1. 0.
    -30. 30.
  exit
 stretch
  direct
  1
  expo
   1 0
   1. 5. 0.
 exit
 bc
  1 2 3 1 2 3
 lines
  19 13 11   
 rotscal
  shift
   -.025 -.05 0.
 exit
 name
  exhaust
 pause
exit
