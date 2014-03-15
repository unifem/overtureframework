*
*************************************************************************
*   Make a building with a round cross-section
*   Input: uses the getGridPoints perl subroutine
*************************************************************************
* 
  smoothedPolygon 
    vertices 
    3 
    .25  .0 
    .25  1. 
    .125 1. 
    n-dist 
    fixed normal distance 
    -.15
    corners 
    specify positions of corners 
    .25 0. 
    .125 1. 
    .40 0. 
    .125 1.15 
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
      * 57 9 57
      getGridPoints(57,9,57);
      $nx $ny $nz
    boundary conditions 
      1 0 1 0 -1 -1 
    share 
      2 0 1 0 0 0 
    mappingName
     roundedCylinderGrid
    exit
*
Box
  set corners
   -.12 .12  1. 1.125  -.12 .12 
  lines
    * 13 13 13
    getGridPoints(13,13,13);
    $nx $ny $nz
  boundary conditions
    0 0 1 0 0 0 
  share
    0 0 1 0 0 0
  mappingName
     roundedCylinderTop-unstretched
  exit
*
  stretch coordinates
    Stretch r2:itanh
    $dx = .005*13/$nx; printf("dx=$dx\n");
    STP:stretch r2 itanh: position and min dx 0 $dx
    stretch grid
    STRT:name roundedCylinderTop
    exit
*
********************************************************************