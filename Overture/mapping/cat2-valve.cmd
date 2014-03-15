*
*  make a hyperbolic surface grid for a valve
* 
open a data-base
cat2-valve.hdf
open an old file read-only
get from the data-base
cat2.igs.compositeSurface
Circle or ellipse (3D)
specify centre
70 30 143
specify radius of the circle
5.
exit
*
rotate/scale/shift
rotate
90 0
70 30 143
exit
*
*
*
*   13,85   
hyperbolic surface
set debug
3
number of lines in marching direction
31
far field distance
70.
grow surface grid in opposite direction
