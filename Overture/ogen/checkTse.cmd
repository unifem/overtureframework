*
*  Make a grid for a two-stroke engine
* 
*  time to make: 11570s (ultra) =192min = 3.21hours (OLD)
*                46s (ultra, Overture.g) (NEW)   
create mappings
*
* ----- here is the core of the main cylinder -----
*
  Box
    specify
    -.75 -.5 -.75    .75 1. .75 
    lines
    19 17 19 
    * 19 5 19 
    boundary
    0 0  0 3  0 0 
    * share: 2=top
    share
    0 0  0 2  0 0
    mappingName
    cylinder-core
    * pause
  exit
  *
  *  Here is a cylinder
  *
  Cylinder
    bounds on the radial variable
    .65 1.
    bounds on the axial variable
    -.5 1.
    lines
    49 17 9
    * 49 5 9
    boundary conditions
    -1 -1  0 2  0 1
    periodicity
    2 0 0
    * share: top=2 outside=1
    share
    0 0  0 2  0 1 
    mappingName
    unrotated-cylinder
    * pause
  exit
  * now rotate so cylinder-axis is along the y-axis
  rotate/scale/shift
    transform which mapping?
    unrotated-cylinder
    rotate
    -90.  0
    0 0 0
    mappingName
    cylinder
    * pause
  exit
  delete
    unrotated-cylinder
  *
  * ----- here is the core of piston -----
  *
  Box
    specify
    -.75 -1. -.75  .75 -.25 .75 
    lines
    19 21 19 
    boundary
    0 0  1 0  0 0
    * bottom=2
    share
    0 0  2 0  0 0
    mappingName
    piston-core
    * pause
  exit
  *
  *  Here is the piston (theta,axial,r)
  *
  Cylinder
    bounds on the radial variable
    .65 1.
    bounds on the axial variable
    -1. -.25
    lines
    71 21 9
    boundary conditions
    -1 -1  1 0 0 1 
    periodicity
    2 0 0
    * share 2=bottom, 1=outside
    share
    0 0  2 0  0 1
    mappingName
    unrotated-piston
    * pause
  exit
  * now rotate so cylinder-axis is along the z-axis
  rotate/scale/shift
    transform which mapping?
    unrotated-piston
    rotate
    -90. 0
    0 0 0
    mappingName
    piston
  exit
  delete
    unrotated-piston
  *
  * Here is a port for the two-stroke-engine
  *
  SmoothedPolygon
    vertices
    4
    -1. -2.
    -1.2 -2.
    -1.2 -1.
    -1. -1.
    n-dist
    variable normal distance
    .35 .3 5.
    .3 .25 5.
    .25 .25 5.
    sharpness
    20.
    20.
    20.
    20.
    n-stretch
    1. 1. 0.
    t-stretch
    1. 0
    1. 8.
    1. 4.
    1. 0.
    lines
    31 9
    mappingName
    2d-port
    * pause
  exit
  *
  * make a 3d port
  *
  body of revolution
    revolve which mapping?
    2d-port
    tangent of line to revolve about
      0 1 0
    choose a point on the line to revolve about
      0 0 0
    start/end angle
      -10. 10.
    boundary conditions
      1 1 2 2 3 3
    lines
      31 7 7
    mappingName
      3d-port
    * pause
  exit
  *
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
    3d-port
    stretch
      specify stretching along axis=0
      layers
        1
        1. 2. 1.
      exit
    exit
    mappingName
    stretched-3d-port
    * pause
    exit
  *
  * shift to the right spot
  *
  rotate/scale/shift
    transform which mapping?
    stretched-3d-port
    shift
    .025 -.05 0.
    mappingName
    port-1
  exit
  *
  * Here is port 2
  *
  rotate/scale/shift
    transform which mapping?
    stretched-3d-port
    shift
    .025 -.05 0.
    * rotate -75 degress about the y-axis
    rotate
    -75. 1
    0. 0. 0.
    mappingName
    port-2
  exit
  *
  * Here is port 3
  *
  rotate/scale/shift
    transform which mapping?
    stretched-3d-port
    shift
    .025 -.05 0.
    * rotate -75 degress about the y-axis
    rotate
    -105. 1
    0. 0. 0.
    mappingName
    port-3
  exit
  *
  * delete un-used mappings or else cmpgrd has trouble
  *
  delete
    stretched-3d-port
  delete
    3d-port
  delete 
    2d-port
  *
  * Here is the cross-section of the exhaust port
  *
  rectangle
    specify corners
    1. -1. 2. -.5
    mappingName
    2d-exhaust
  exit
  *
  * make a 3d exhaust port
  *
  body of revolution
    revolve which mapping?
    2d-exhaust
    start/end angle
    -30. 30.
    boundary conditions
    1 1 2 2 3 3
    lines
    19 13 11
    tangent of line to revolve about
    0 1 0
    choose a point on the line to revolve about
    0 0 0
    choose a point on the line to revolve about
    0 0 0
    mappingName
    3d-exhaust
  exit
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
    3d-exhaust
    mappingName
    stretched-3d-exhaust
    stretch
      specify stretching along axis=0
      layers
      1
      1. 5. 0.
      exit
    exit
  exit
  *
  * shift to the right spot
  *
  rotate/scale/shift
    transform which mapping?
    stretched-3d-exhaust
    shift
    -.025 -.05 0.
    mappingName
    exhaust
  exit
  delete
    2d-exhaust
  delete 
    3d-exhaust
  delete
    stretched-3d-exhaust
  * pause
  exit
*
check overlap
    cylinder-core
    cylinder
    piston-core
    piston
    port-1
    port-2
    port-3
    exhaust
  done
  change parameters
    prevent hole cutting
      * first turn off all hole cutting
      all
      all
      * allow cylinder to cut 
      cylinder
      none
      done
    allow hole cutting
      * allow piston to cut in the piston core
      piston
      piston-core
      done
    exit
