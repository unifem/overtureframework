*
*  make a hyperbolic surface grid for a valve
* 
open a data-base
  catValve.hdf
open an old file read-only
get from the data-base
  valve
hyperbolic
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      5 10
    done
    choose an edge
      specify edge curves
        3 7 
      done
    done
  exit
  edit initial curve
    lines
     43 35 31 21
  exit
  * the implicit coeff must be turned on or else we we hit the marching CFL.
  implicit coeff
    .5
*   distance to march 
*       5.
*    lines to march
*       5
*     generate
*  pause
  distance to march
    100.
  lines to march 
    101 81
  * set bc at top to prevent projection of ghost line
  boundary conditions
    -1 -1 1 0
  generate
  pause
  mappingName
    valveSurface
  exit
* Now create the volume grid
hyperbolic
  start from which curve/surface?
    valveSurface
  grow grid in opposite direction
  implicit coeff
   0
  uniform dissipation coefficient
   .01
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      exit
   distance to march 
     5. 6. 5. 4. 4.5 5. 4.
   lines to march 
     13 11 9 7 6 5
  geometric stretching, specified ratio
    1.2
  generate
  pause
  boundary conditions
    -1 -1 0 0 1 0
  share
     0  0 0 0 6 0
  mappingName
    valveVolume
  exit
*
  open a data-base
   valveVolume.hdf
     open a new file
   put to the data-base
     valveVolume
   close the data-base
exit


