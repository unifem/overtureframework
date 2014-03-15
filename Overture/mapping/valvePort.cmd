*
* Create a intake port and valve stem that intersects it
*
  Annulus
    mappingName
      annulus
    exit
*
  body of revolution
    mappingName
      port
    revolve which mapping?
    annulus
    choose a point on the line to revolve about
      2 0 0
    start/end angle
      0 90
    lines
      31 21 9
    boundary conditions
      -1 -1 0 3 1 2
    share
       0  0 0 3 1 2
    exit
*
  rectangle
    specify corners
      -.6 -.6 .6 .6
    mappingName
      coreEnd
    exit
*
  body of revolution
    mappingName
      core
    choose a point on the line to revolve about
      2 0 0
    start/end angle
      0 90
    boundary conditions
      0 0 0 0 1 2
    share
       0  0 0 0 1 2
    lines
      31 21 9
    exit
*
  Cylinder
    bounds on the axial variable
      -3.0 0.
    bounds on the radial variable
      .15 .4
    mappingName
      valveStem
    boundary conditions
      -1 -1 0 0 1 2
    share
       0 0  1 0 4 0
    exit
*
  join
    mappingName
      valveStemPortIntersection
    choose curves
    valveStem
    port (side=1,axis=1)
    end of join
      .5
    compute join
    boundary conditions
      -1 -1 1 0 2 0
    share
       0  0 3 0 4 0
    lines
      25 7 11
  exit
*
  change a mapping
    valveStem
    boundary conditions
      -1 -1 0 1 4 0
    share
       0  0 0 1 4 0
    exit
