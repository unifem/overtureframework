*
* Create a intake port and valve stem that intersects it
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
*
* Here is the valve seat
*
  SmoothedPolygon
    vertices
      4
      1.4  0.
      1.2  0.
      .8   0.4 
      .8   0.6
    n-dist
    fixed normal distance
       .25
    mappingName
      valveSeatCrossSection
    boundary conditions
      0 0 3 0
    share
      0 0 3 0
    exit
*
  body of revolution
    choose a point on the line to revolve about
      0 0 0
    mappingName
      revolvedValveSeat
    lines
      31 7 31
    exit
*
  rotate/scale/shift
    transform which mapping
      revolvedValveSeat
    rotate
      -90 0
       0. 0. 0.
    mappingName
      valveSeat
    exit
*
*
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
*
  Cylinder
    mappingName
      valveStem
    bounds on the axial variable
      -3.0 0.6
    bounds on the radial variable
      .15 .4
    lines
      25 25 7
    boundary conditions
      -1 -1 0 0 1 2
    share
       0 0  1 0 4 0
    exit
*
  join
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
      25 11 7
    mappingName
      valveStemPortIntersection
  exit
*
  change a mapping
    valveStem
    boundary conditions
      -1 -1 0 1 4 0
    share
       0  0 0 1 4 0
    exit
*
   DataPointMapping
     build from a mapping
       valveStemPortIntersection
     mappingName
      valveStemPortIntersection-dp
  exit
*
  SmoothedPolygon
    vertices
      4
      .2 -1.
      1.2 -1.
      .8 -.6
      .15 -.6
    n-dist
    fixed normal distance
      -.25
    mappingName
      valveCrossSection
    boundary conditions
      0 1 2 0
    share
      0 4 1 0
    exit
  body of revolution
    choose a point on the line to revolve about
      0 0 0
    mappingName
      revolvedValve
    lines
      51 7 31
    exit
  rotate/scale/shift
    transform which mapping
      revolvedValve
    rotate
      -90 0
       0. 0. 0.
    mappingName
      valve
    exit
*
*
  Box
    mappingName
      valveBottom
    specify corners
      -.5 -.5 1. .5 .5 1.5
    lines
      10 10 10
    boundary conditions
      0 0 0 0 1 0
    share
      0 0 0 0 1 0
    exit
  pause
  exit
*
  generate an overlapping grid
      box
      cylinder
      core
      port
      valveStem
      valveStemPortIntersection-dp
      valve
      valveBottom
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
      ghost points
        all
       2 2 2 2 2 2
    exit
    * pause
    compute overlap
    * pause
  exit
*
save an overlapping grid
valvePort.hdf
valvePort
exit

