* hyperbolic surface grid generation on cyl4
open a data-base
cyl4.hdf
open an old file read-only
get from the data-base
compositeSurface
*
*
Circle or ellipse (3D)
specify radius
2.25
specify centre
0 0 8.
exit
*
hyperbolic surface
x+r 30.
y+r 20.
grow surface grid in opposite direction
number of lines in marching direction (KMAX)
9
* dist=4. and kmax=9
far field distance
4.
* set debug
* 3



