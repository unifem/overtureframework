****************************************************
** ogen command file to build some 3D buildings 
***************************************************
*
create mappings 
* 
*   open a data-base 
*   /home/henshaw/Overture/mapping/roundedBuilding.hdf
*     open an old file read-only 
*     get all mappings from the data-base 
*   close the data-base
*
*   open a data-base 
*   /home/henshaw/Overture/mapping/building2.hdf
*     open an old file read-only 
*     get all mappings from the data-base 
*   close the data-base
*
*
  smoothedPolygon 
    vertices 
    5 
    .25  .0 
    .75  .0 
    .75  1. 
    .25  1. 
    .25  .0 
    periodicity 
    2 0 
    n-dist
    fixed normal distance
    -.2
    lines
      75 7
    mappingName 
      roundedGrid2d
    exit
*
  sweep
    extrude
      .075 1
    choose reference mapping
    roundedGrid2d
    boundary conditions
    -1 -1 1 0 0 2 
    lines
      75 7 25
    mappingName
      roundedGrid
    exit
*
  smoothedPolygon 
    vertices 
    5 
    .25  .0 
    .75  .0 
    .75  1. 
    .25  1. 
    .25  .0 
    periodicity 
    2 0 
    curve or area (toggle) 
    mappingName 
    roundedCurve 
    exit 
*
  nurbs (curve)
    interpolate from a mapping
    roundedCurve
    mappingName
     roundedNurbs
* pause
   exit
*
  plane or rhombus
    mappingName
    plane
    specify plane or rhombus by three points
    0 0 0 1 0 0 0 1 0
    exit
*
  trimmed mapping
  specify mappings
    plane
    roundedNurbs
  done
  mappingName
    roundedPlane
  exit
*
  sweep
    extrude
    0 1
    choose reference mapping
     roundedNurbs
    mappingName
      roundedSurface
    exit
*
  reparameterize
    transform which mapping?
    roundedSurface
   set corners
    0 .5 0. 1.
    mappingName
    roundedSurface1
    exit
*
  reparameterize
    transform which mapping?
    roundedSurface
   set corners
    .5 1. 0. 1.
    mappingName
    roundedSurface2
    exit
*
  composite surface
    CSUP:add a mapping roundedSurface1
    CSUP:add a mapping roundedSurface2
    CSUP:add a mapping roundedPlane
* pause
    CSUP:determine topology
*    debug 7
*    deltaS 0.05
*     improve triangulation 1
    build edge curves
    merge edge curves
    triangulate
* pause
    exit
    exit
  builder
    target grid spacing .05 .05
    create surface grid...
      # Choose a starting curve for the surface grid near the top edge of the builing
      choose boundary curve 2
      # old way: 
      # choose edge curve 4 7.239714e-01 2.351254e-01 0.000000e+00 
      # choose edge curve 5 2.240427e-01 7.128884e-01 0.000000e+00 
      done
      # edit initial curve
      #  periodicity
      #  2
      #  exit
      forward and backward
      target grid spacing 0.01, 0.015 (tang,normal, <0 : use default)
      lines to march 9 9 (forward,backward)  
      generate
      #  pause
      name roundedTopSurface
      exit
*
    create volume grid...
      target grid spacing -1, 0.01 (tang,normal, <0 : use default)
      forward
      spacing: geometric
      generate
      name roundedTop
*  pause
      exit
  x+r 150
  y+r 20
    build a box grid
      * x bounds: 0.275 0.725
      x bounds: 0.295 0.685
      y bounds: 0.05, 0.9
      z bounds: -0.25, 0.
      lines: 11 17 5
* pause
     exit
    assign BC and share values
      shared boundary flag: 0
      boundary condition: 0
      set BC and share 2 0 2 0 0
      plot lines on non-physical boundaries 0
      set BC and share 2 1 0 0 0
      set BC and share 2 0 1 0 0
      set BC and share 2 1 1 0 0
      set BC and share 2 0 0 0 0
      boundary condition: 1
      shared boundary flag: 1
      set BC and share 2 1 2 1 1
      set BC and share 1 0 2 1 1
      set BC and share 0 0 1 1 1
      boundary condition: 2
      shared boundary flag: 2
      set BC and share 0 1 2 2 2
* pause
      exit
  exit
*************************************************************************
*   Now make a building with a round cross-section
*************************************************************************
* 
  smoothedPolygon 
    vertices 
    3 
    .25  .0 
    .25  1. 
    .1 1. 
    n-dist 
    fixed normal distance 
    -.2 
    corners 
    specify positions of corners 
    .25 0. 
    .1 1. 
    .45 0. 
    .1 1.2 
    lines 
      55 7 
    boundary conditions 
    2 0 1 0 
    share 
    2 0 1 0 
    mappingName 
      crossSection 
* pause
*
    exit
  body of revolution
    revolve which mapping?
    crossSection
    choose a point on the line to revolve about
    0 0 0
    lines
    55 9 31
    boundary conditions 
    2 0 1 0 -1 -1 
    share 
    2 0 1 0 0 0 
    mappingName
    roundedCylinderGrid
    exit
*
Box
  set corners
   -.125 .125 1. 1.125 -.125 .125 
  lines
    9 7 9 
  boundary conditions
    0 0 1 0 0 0 
  share
    0 0 1 0 0 0
  mappingName
     roundedCylinderTop
  exit
*
*************************************************************************
*
***************************************************************************
*   Now take the basic building and scale/shift it to create new buildings
***************************************************************************
*
  rotate/scale/shift
    transform which mapping?
    roundedCylinderGrid
    shift
    1.25 0. 1.25
    scale
     1. 1. 1.2
   mappingName
    roundedCylinderGrid1
  exit
*
  rotate/scale/shift
    transform which mapping?
    roundedCylinderTop
    shift
    1.25 0. 1.25
    scale
     1. 1. 1.2
   mappingName
    roundedCylinderTop1
  exit
*
  rotate/scale/shift
    transform which mapping?
    roundedGrid
    shift
    0 0 -1
    rotate
    90 0
    0 .0 0
    shift
     -.5 0. .25
   mappingName
    roundedGrid1
  exit
*
  rotate/scale/shift
    transform which mapping?
    roundedTop
    shift
    0 0 -1
    rotate
    90 0
    0 .0 0
    shift
     -.5 0. .25
   mappingName
    roundedTop1
  exit
*
  rotate/scale/shift
    transform which mapping?
    box1
    shift
    0 0 -1
    rotate
    90 0
    0 .0 0
    shift
     -.5 0. .25
   mappingName
    roundedBox1
  exit
*
  rotate/scale/shift
    transform which mapping?
    roundedGrid
    shift
    0 0 -1
    rotate
    90 0
    0 .0 0
    shift
    .5 0. -.25
    scale
    .75 1.5 1.
   mappingName
    roundedGrid2
  exit
*
*
  rotate/scale/shift
    transform which mapping?
    roundedTop
    shift
    0 0 -1
    rotate
    90 0
    0 .0 0
    shift
    .5 0. -.25
    scale
    .75 1.5 1.
   mappingName
    roundedTop2
  exit
*
  rotate/scale/shift
    transform which mapping?
    box1
    shift
    0 0 -1
    rotate
    90 0
    0 .0 0
    shift
    .5 0. -.25
    scale
    .75 1.5 1.
   mappingName
    roundedBox2
  exit
*
*
* Here is the big box
*
Box
  set corners
*     -.5 1.5 0. 2.0 -.5 1.5 
   -1. 2. 0. 2.5 -1. 3.
  lines
    61 51 81
  boundary conditions
    1 1 2 1 1 1
  share
    0 0 2 0 0 0
  mappingName
    backGround
  exit
exit
generate an overlapping grid
  backGround
  roundedBox1
  roundedTop1
  roundedGrid1
  roundedBox2
  roundedTop2
  roundedGrid2
  roundedCylinderGrid1
  roundedCylinderTop1
  done
  change the plot
    toggle grid 0 0
    exit this menu
* display intermediate results
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap 
  exit
save a grid (compressed)
building3.hdf
building3
exit


