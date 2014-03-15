  Cylinder
    orientation
      1 2 0
    bounds on the axial variable
      -1. 1.
    bounds on the radial variable
      .5 .75
    boundary conditions
      -1 -1 0 0 3 0
    mappingName
      main-cylinder
    lines
      31 21 6
    exit
*
Sphere
  mappingName
    sphere
  inner radius
    .7
  exit
*
  Cylinder
    mappingName
      top-cylinder
    orientation
      2 0 1
    bounds on the axial variable
      .25 1.
    bounds on the radial variable
      .25 .425
    boundary conditions
      -1 -1 0 0 3 4
    lines
      25 15 5
    exit
  join
   choose curves
     top-cylinder
     main-cylinder (side=0,axis=2)
   compute join
   lines
    31 15 7

