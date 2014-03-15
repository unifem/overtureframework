*************************************************************************
*   Make the poly-building - using a smoothedPolygon as the cross-section
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
*
smoothedPolygon 
  vertices 
       5
     -.125 -.125 
      .125 -.125 
      .125  .125
     -.125  .125
     -.125 -.125
  sharpness
    60 
    60
    60
    60
    60
*
    curve or area (toggle)
    make 3d (toggle)
      .0  * z-level
    exit
*
  rotate/scale/shift
    rotate
      90 0
      0 0 .0 
    mappingName
      sweepCurve
    exit
*
* create the 3d building by sweeping the cross-section
*
  sweep
   *  use center of sweep curve
    specify center
    .1 0 0
    lines
*      37 7 73 
      getGridPoints(37,7,73);
      $nx $ny $nz
    mappingName
      polyBuilding-unstretched
    boundary conditions
      1 0 1 0 -1 -1 
    share
      2 0 3 0 0 0    * 2=base 3=building
  exit
* 
*   concrentrate more grid lines near the vertical corners of the building
*
  stretch coordinates
    Stretch r3:itanh
    STP:stretch r3 itanh: layer 0 1 12 0 (id>=0,weight,exponent,position)
    STP:stretch r3 itanh: layer 1 1 12 .25 (id>=0,weight,exponent,position)
    STP:stretch r3 itanh: layer 2 1 12 0.5 (id>=0,weight,exponent,position)
    STP:stretch r3 itanh: layer 3 1 12 0.75 (id>=0,weight,exponent,position)
    stretch grid
    STRT:name polyBuilding
    exit
  * 
  box
    set corners
      -.15 .15  1. 1.15 -.15 .15
    lines
      * 21 9 21 
      getGridPoints(21,9,21);
      $nx $ny $nz
    exit
  stretch coordinates
    Stretch r2:itanh
    STP:stretch r2 itanh: position and min dx 0 0.01
    stretch grid
    STRT:name polyTopBox
    boundary conditions
      0 0 1 0 0 0
    share
      0 0 3 0 0 0
    exit
*
**************************************************************************