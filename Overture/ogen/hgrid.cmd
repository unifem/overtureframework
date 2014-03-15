create mappings
rectangle
  set corners
*    0.05 2. -.5 .5 -.5 2. -.5 .5
*     0.025 2. -.5 .5 -.5 2. -.5 .5
   -.25  2.25 -.5 .5 -.5 2. -.5 .5
  lines
    51 21
  boundary conditions
    1 1 1 1
  mappingName
  backGround
  exit
*
  spline
    enter spline points
      8
      0.   -1.e-3
      .25  -1.e-3
      .5   -1.e-3
      .75  .1
      1.   .1
      1.25 -1.e-3
      1.5  -1.e-3
      1.75 -1.e-3
    shape preserving (toggle)
    lines
      31 
    exit
*
  hyperbolic
    distance to march
      .25
     grow grid in opposite direction
    generate
    share
      0 0 1 0
    mappingName
     topHalf
    lines
      31 11
   exit
*
  spline
    enter spline points
      8
      0.   1.e-3
      .25  1.e-3
      .5   1.e-3
      .75  -.1
      1.   -.1
      1.25  1.e-3
      1.5   1.e-3
      1.75  1.e-3
    shape preserving (toggle)
    lines 
     31
    exit
*
  hyperbolic
    distance to march
      .25
    generate
    share
      0 0 1 0
    mappingName
     bottomHalf
    lines
      31 11
   exit
exit
  generate an overlapping grid
    backGround
    topHalf
    bottomHalf
    done choosing mappings
    change parameters
      prevent hole cutting
       topHalf 
         bottomHalf 
       bottomHalf
         topHalf 
      done
      mixed boundary
        topHalf
        bottom (side=0,axis=1)
        bottomHalf
         r matching tolerance
           .02
         done
*
        bottomHalf
        bottom (side=0,axis=1)
        topHalf
         r matching tolerance
           .02
         done
        done
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
*  set debug
*    15
  compute overlap
  exit
*
save an overlapping grid
hgrid.hdf
hgrid
exit

   
