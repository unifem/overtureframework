*
*  make a hyperbolic surface grid for a valve
* 
open a data-base
catValve.hdf
open an old file read-only
get from the data-base
  valve
Circle or ellipse (3D)
specify centre
40.5 50 95 
specify radius of the circle
5.
exit
*
rotate/scale/shift
rotate
90 0
40.5 50 95 
exit
*
*
*
*   13,85   


hyperbolic surface
set debug
3
number of lines in marching direction
35
far field distance
90.
grow surface grid in opposite direction
pause
generate the hyperbolic surface grid
