*
* Create a intake port and valve stem that intersects it
*
create mappings
*
  Box
    mappingName
      box
    specify corners
      -1.5  -1.5  -.1  1.5 1.5 2.9
    lines
      31 31 31 
    boundary conditions
      1 1 1 1 1 1
    share
      0 0 0 0 6 7
   exit
  Cylinder
    mappingName
      cylinder
    bounds on the axial variable
      -.1 2.9  
    bounds on the radial variable
      1.25 2.
    lines
      31 25 9
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
      .45
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
      -.625 -.625 .625 .625
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
      core
      port
*      valveStem
*      valveStemPortIntersection-dp
*      valve
*      valveBottom
    done choosing mappings
    change parameters
      prevent hole cutting
        box
        all
        port
        box
        core
        all
        done
      exit
    pause
    compute overlap
  exit
*
save an overlapping grid
valvePort.hdf
valvePort
exit

