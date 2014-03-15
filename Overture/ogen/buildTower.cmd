*
*************************************************************************
*   Make a tower building
*   Input: uses the getGridPoints perl subroutine
*************************************************************************
* 
  if( $factor == "0" ){ $factor = 1; }
*
*
*
  smoothedPolygon 
    vertices 
     5
    .15  .0 
    .15  .1
    .1   .4 
    .075 .6
    .075 1.5
    n-dist 
    fixed normal distance 
    -.15
    t-stretch
     0 10
     0 10
     0 10
     0 10
     0 10
    corners 
    specify positions of corners 
     .15  0. 
     .075 1.5
     .30  0. 
     .225 1.5
    lines 
      55 7 
    boundary conditions 
      2 0 1 0 
    share 
      2 0 1 0 
    mappingName 
      towerCrossSection 
* pause
*
    exit
  body of revolution
    revolve which mapping?
     towerCrossSection
    choose a point on the line to revolve about
      0 0 0
    lines
      * 57 9 57
      getGridPoints(57,9,57);
      $nx $ny $nz
    boundary conditions 
      1 0 1 0 -1 -1 
    share 
      2 0 3 0 0 0  * 3=match to pod
    mappingName
     tower
* pause
    exit
********************************
* pod
********************************
  smoothedPolygon 
    vertices 
     9
    .075  1.5
    .2    1.5
    .325  1.54
    .37   1.56
    .4    1.65
    .37   1.74
    .325  1.76
    .2    1.8
    .125  1.8 
    n-dist 
    fixed normal distance 
    -.15
    t-stretch
     0 10
     0 10
     .125 15
     .125 15
     .125 10
     .125 15
     .125 15
     0 10
     0 10
*pause
    corners 
    specify positions of corners 
     .075 1.5
     .125 1.8 
     .075 1.35 
     .125 1.95
    lines 
      55 7 
    boundary conditions 
      1 0 1 0 
    share 
      3 0 4 0 
    mappingName 
      towerPodCrossSection
* pause
*
    exit
  body of revolution
    revolve which mapping?
     towerPodCrossSection
    choose a point on the line to revolve about
      0 0 0
    lines
      * 57 9 57
      getGridPoints(57,9,57);
      $nx $ny $nz
    boundary conditions 
      1 0 1 0 -1 -1 
    share 
      3 0 4 0 0 0 
    mappingName
     towerPod
* pause
    exit
*
********************************
* spike
********************************
  smoothedPolygon 
    vertices 
     5
    .05    1.8
    .05    2.0
    .025   2.4 
    .0125  2.45
    0.     2.45
    n-dist 
    fixed normal distance 
    -.125
    t-stretch
     0 10
     0 10
     .25 15
     .25 10
     0 10
* pause
    corners 
    specify positions of corners 
     .05  1.8
     .0   2.45
     .175 1.8 
     .0   2.575
    lines 
      21 7 
    boundary conditions 
      4 0 1 0 
    share 
      4 0 0 0 
    mappingName 
      towerSpikeCrossSection
* pause
*
    exit
  body of revolution
    revolve which mapping?
     towerSpikeCrossSection
    choose a point on the line to revolve about
      0 0 0
    lines
      * 31 21 7 
      getGridPoints(31,21,7);
      $nx $ny $nz
* pause
    boundary conditions 
      1 0 -1 -1 1 0 
    share 
      4 0 0 0 5 0 
    mappingName
     towerSpikeSingular
* pause
    exit
*   
*   top of the spike
  reparameterize
    transform which mapping?
      towerSpikeSingular
    orthographic
      choose north or south pole
      -1
      exit
    orthographic
      specify sa,sb
      .75 .75
      exit
    lines
      * 11 11 9
      getGridPoints(11,11,9);
      $nx $ny $nz
    mappingName
     towerSpikeCap
    exit
*
  reparameterize
    transform which mapping?
      towerSpikeSingular
    restrict parameter space
      set corners
      0 .95 0 1 0 1
      exit
    mappingName
     towerSpike
    exit
*
* view mappings
* towerSpike
* towerSpikeCap
* towerPod
* pause
*
Box
  set corners
   -.135 .135 1.8 1.95 -.135 .135  
  lines
    * 9  9 9  
    getGridPoints(9,9,9);
    $nx $ny $nz
  boundary conditions
    0 0 1 0 0 0 
  share
    0 0 3 0 0 0
  mappingName
    towerPodTop-unstretched
  exit
*
  stretch coordinates
    Stretch r2:itanh
    STP:stretch r2 itanh: position and min dx 0 0.075
    stretch grid
    STRT:name towerPodTop
    exit
*
* =================== end buildTower ============================================

