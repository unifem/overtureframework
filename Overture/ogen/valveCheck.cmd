*
* Create an overlapping grid for a 2D valve
*
*  time to make: old:27s (ultra) new: 4.4s
*                .34 (tux50)
*                .27 tux50
*                .12 tux231
*
create mappings
  *
  * Here is the part of the boundary that 
  * the valve closes against  
  *
  SmoothedPolygon
    mappingName
      stopper
    vertices
      4
      1. .5
      0.75 .5
      0.5 .75
      0.5 1.
      n-dist
        fixed normal distance
        * .1
        .05
      lines
        * 61 9
        * 61 9
        65 9
      t-stretch
        1. 0. 
        1. 5.
        1. 5.
        1. 0.
      n-stretch
        1. 4. 0.
      boundary conditions
        1 1 1 0
      share
        2 4 0 0
*
    check inverse
    enter a point
    .47 .35 0
    enter a point
    .470 .352 0.


    enter multiple points
    .47 .35 0
    .47 .6 0
    done
    enter multiple points
    4.70e-01,3.52e-01,0
    4.73e-01,6.10e-01,0
    done


  exit
exit
*
* Make the overlapping grid
*
generate an overlapping grid
    backGround
    stopper
    valve
  done
  change parameters
    interpolation type
     explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
*  debug
*    7
*  display intermediate results
***  compute overlap
  debug 
    15
  compute overlap
  continue
  continue
  continue
  continue
  continue
  continue
  continue
  continue
  continue
  continue
  continue
  continue
  continue
  continue
  continue
  continue
*  pause
  exit
*
* save an overlapping grid
save a grid (compressed)
valve.hdf
valve
exit
















*
* Create an overlapping grid for a 2D valve
*
*  time to make: old:27s (ultra) new: 4.4s
*                .34 (tux50)
*                .27 tux50
*                .12 tux231
*
create mappings
  *
  * First make a back-ground grid  
  *
  rectangle
    mappingName
      backGround
    set corners
      0 1.  0 1.
    lines
      * 41 41
      * 51 51
      49 49 
    share
      1 2 3 4
  exit
  *
  * Now make the valve  
  *
  SmoothedPolygon
    mappingName
      valve
    vertices
    * .4 .4 .65 .65  ok
    * .45 .45 .7 .7  ok
    * .47  .47  .72  .72  ok
    * .475 .475 .725 .725 no
    * .47  .47  .72  .72  last used, ok
     4
     0.47  0.
     0.47  .75
     0.72  .5
     0.72  0.
    n-dist
      fixed normal distance
      * .1
      .05
    lines
      * 65 9
      * 75 9
      73 9 
    boundary conditions
      1 1 1 0
    share
      3 3 0 0 
    sharpness
      15
      15
      15
      15
    t-stretch
      1. 0. 
      1. 6.
      1. 4.
      1. 0.
    n-stretch
      1. 4. 0.
  exit
  *
  * Here is the part of the boundary that 
  * the valve closes against  
  *
  SmoothedPolygon
    mappingName
      stopper
    vertices
      4
      1. .5
      0.75 .5
      0.5 .75
      0.5 1.
      n-dist
        fixed normal distance
        * .1
        .05
      lines
        * 61 9
        * 61 9
        65 9
      t-stretch
        1. 0. 
        1. 5.
        1. 5.
        1. 0.
      n-stretch
        1. 4. 0.
      boundary conditions
        1 1 1 0
      share
        2 4 0 0
  exit
exit
*
* Make the overlapping grid
*
generate an overlapping grid
    backGround
    stopper
    valve
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  debug
    7
*  display intermediate results
  compute overlap
  continue
  continue
  continue
  continue



*  pause
  exit
*
* save an overlapping grid
save a grid (compressed)
valve.hdf
valve
exit

