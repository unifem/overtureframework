*
* Create a intake port and cylinder
*
create mappings
*
  Box
    mappingName
      box
    specify corners
      -1.5  -1.5  -.1  1.5 1.5 3.9
    lines
      24 24 32 
    boundary conditions
      0 0 0 0 1 1
    share
      0 0 0 0 6 7
   exit
  Cylinder
    mappingName
      cylinder
    bounds on the axial variable
      -.1 3.9
    bounds on the radial variable
      1.25 2.5
    lines
      35 25 9
    boundary conditions
      -1 -1 1 1 0 1
    share
       0 0  6 7 0 0
    exit
*
  Annulus
    mappingName
      annulus
    inner
      .5
    exit
*
  body of revolution
    revolve which mapping?
      annulus
    mappingName
      port
    choose a point on the line to revolve about
      2 0 0
    start/end angle
      0 90
    lines
      41 9 31  31 9 21 
    boundary conditions
      -1 -1 0 3 0 2
    share
       0  0 0 3 0 2
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
      0 0 0 0 0 2
    share
       0  0 0 0 0 2
    lines
      15 15 21
    exit
exit
*
  generate an overlapping grid
      box
      cylinder
      core
      port
    done choosing mappings
    change parameters
      prevent hole cutting
        all
        all
        done
      ghost points
        all
       2 2 2 2 2 2
    exit
    pause
    compute overlap
    * pause
  exit
*
save an overlapping grid
portCylinder.hdf
portCylinder
exit

