create mappings
  rectangle
    set corners
      0. 10. 0 3.
    lines
      101 31
    boundary conditions
      0 1 1 1   
    share
      0 0 3 0 
    mappingName
      mainChannel
    exit
  rectangle
    set corners
      -3. 0. 1. 3.
    lines
      31 21
    boundary conditions
      1 0 1 1   
    share
      2 0 1 0
    mappingName
      inlet
    exit
  * 
  smoothedPolygon
    vertices
    3
    -3. 1.
    0 1
    0 0
    lines
      51 8  
    n-dist
    fixed normal distance
      .3
*
    sharpness
      30.
      30.
      30.
*
    n-stretch
     1. 5. 0 
*
    correct corners
*
    boundary conditions
      1 1 1 0
    share
      2 3 1 0
    mappingName
      corner
    exit
  exit this menu
*
  generate an overlapping grid
    mainChannel
    inlet
    corner
    done
    change parameters
    ghost points
      all
      2 2 2 2 2 2
*    interpolation type
*      explicit for all grids
  exit
  * display intermediate results
  compute overlap
  * pause
  exit
*
save an overlapping grid
backStepSmooth.hdf
backStep
exit

