*
* use the depth mapping to define a cylindrical region
*
create mappings
*
* first built a TFI mapping to define the depth
*
  quadratic (surface)
    parameters
      -1. 2. -1. 2. 0. 0. 0. 0. .4 .4 
    mappingName
      bottom
    exit
*
  quadratic (surface)
    parameters
      -1. 2. -1. 2. 1.5 0. 0. 0. -.4 -.4 
    mappingName
      top
    exit
*
   tfi
     choose back curve 
       bottom
     choose front curve
       top
     exit
*  
  annulus
    reset
    inner and outer radii
      .5 .9
    centre for annulus
      0. 0. 
    exit
*
  rectangle
   set corners
    -.5 .5 -.5 .5 
   lines
     5 5
   mappingName
    square
  exit
*
*  give depth to the annulus
*
  depth mapping
    extend depth from which mapping?
    Annulus
*    the scaling function is x=-1 + 2*r, y=-1+2*s
    depth function parameters
      -1. 2. -1. 2.
    depth function
    TFIMapping
    mappingName
     cylinder
    lines
      21 7 7
    boundary conditions
      -1 -1 0 1 2 3
    share
       0 0 0 0 2 3
  exit
*
*
*  give depth to the core
*
  depth mapping
    extend depth from which mapping?
    square
*    the scaling function is x=-1 + 2*r, y=-1+2*s
    depth function parameters
      -1. 2. -1. 2.
    depth function
    TFIMapping
    mappingName
     core
    lines
      11 11 7
    boundary conditions
      0 0 0 0 2 3
    share
       0 0 0 0 2 3
  exit
*
exit
generate an overlapping grid
    core
    cylinder
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
  exit
*
save an overlapping grid
depth.hdf
depth
exit
