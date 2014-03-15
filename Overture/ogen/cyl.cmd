*
* Two intersecting cylinders, test for ray tracing
*
create mappings
* valve stem -- extends outside region
  Cylinder
    mappingName
      valveStem
    * orient the cylinder so y-axis uis axial direction
    orientation
      2 0 1
    bounds on the radial variable
      .2 .6
    bounds on the axial variable
      -.6 -.2
    lines
      21 13 9
    boundary conditions
      -1 -1 0 2 2 1 
    share
       0  0 0 3 5 2 
    exit
* outlet valve stem  
  Cylinder
    mappingName
      outletStem
    * orient the cylinder so y-axis uis axial direction
    orientation
      2 0 1
    bounds on the radial variable
      .2 .6
    bounds on the axial variable
      -.5 -.3
    lines
      21 7 9
    boundary conditions
      -1 -1 3 0 2 1 
    share
      0   0 4 0 5 2 
    exit
  view mappings
    outletStem
    valveStem
    * pause
  exit
exit
* 
generate an overlapping grid
    outletStem
    valveStem
  done 
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  pause
  compute overlap
exit
save an overlapping grid
cyl.hdf
cyl
exit
