#
#************************************************************************
#   Make a tower building
#   Input: uses the getGridPoints perl subroutine
#************************************************************************
# 
  if( $factor == "0" ){ $factor = 1; }
#
#
#
  smoothedPolygon 
$baseWidth=.15; $topWidth=.075; $height=1.5; $towerBaseHeight=$height-5.*$ds;
    vertices 
     5
    $baseWidth  .0 
    $baseWidth  .1
        .1   .4 
    $topWidth .6
    $topWidth $towerBaseHeight
    n-dist 
      fixed normal distance 
      $nDist = ($nr-3)*$ds;
      # $nDist=.15;
      -$nDist 
    t-stretch
     0 10
     0 10
     0 10
     0 10
     0 10
    corners 
    specify positions of corners 
     $x3=$baseWidth+$nDist;
     $x4=$topWidth+$nDist;
     $baseWidth  0. 
     $topWidth $towerBaseHeight
     $x3  0. 
     $x4 $towerBaseHeight
    lines 
      55 7 
    boundary conditions 
      2 0 1 0 
    share 
      2 0 1 0 
    mappingName 
      towerCrossSection 
  # open graphics
  #  pause
#
    exit
  body of revolution
    revolve which mapping?
     towerCrossSection
    choose a point on the line to revolve about
      0 0 0
    lines
      # 57 9 57
      # getGridPoints(57,9,57);
      # $nx $ny $nz
      $axialLengthTower=$height + ($topWidth-$baseWidth); # approximate axial dist.
      $ns = intmg( $axialLengthTower/$ds +1.5 ); 
      $nTheta = intmg( 2.*$pi*$baseWidth/$ds +1.5 ); 
      $ns $nr $nTheta
    boundary conditions 
      1 0 1 0 -1 -1 
    share 
      2 0 3 0 0 0  * 3=match to pod
    mappingName
     tower
    ##open graphics
    ## pause
    exit
#*******************************
# pod
#*******************************
  smoothedPolygon 
    vertices 
       9
     $xp1 = .075;     # "radius" of pod grid at the bottom
     $xp9 = .125-$ds; # "radius" of pod grid at the top
     $xp1  1.5
     .2    1.5
     .325  1.54
     .37   1.56
     .4    1.65
     .37   1.74
     .325  1.76
     .2    1.8
     $xp9  1.8 
    n-dist 
      fixed normal distance 
      -$nDist
      # -.15
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
    corners 
    specify positions of corners 
     $y3=$height-$nDist;
     $y4=1.8+$nDist;
     #
     $xp1 1.5
     $xp9 1.8 
     $xp1 $y3
     $xp9 $y4
    lines 
      71 9 
    boundary conditions 
      1 0 1 0 
    share 
      3 0 4 0 
    mappingName 
      towerPodCrossSection
# pause
#
    exit
  body of revolution
    revolve which mapping?
     towerPodCrossSection
    choose a point on the line to revolve about
      0 0 0
#
    lines
      # 57 9 57
      # getGridPoints(57,9,57);
      # $nx $ny $nz
      $axialLengthPod= ( 2.*.4 + .3 );
      $ns = intmg( $axialLengthPod/$ds +1.5 ); 
      $podRadius=.35;  # approx.
      $nTheta = intmg( 2.*$pi*$podRadius/$ds +1.5 ); 
      $ns $nr $nTheta
    boundary conditions 
      1 0 1 0 -1 -1 
    share 
      3 0 4 0 0 0 
    mappingName
     towerPod
# pause
    exit
#
#*******************************
# spike
#*******************************
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
      # -.125
      $nDistSpike=1.*$nDist;
      -$nDistSpike 
    t-stretch
      0 10
      0 10
      .125 10
      .25 15
      .25 10
# pause
    corners 
    specify positions of corners 
      $x3=.05+ $nDistSpike;
      $y4=2.45+ $nDistSpike;
     .05  1.8
     .0   2.45
     $x3  1.8 
     .0   $y4
    lines 
      41 9 
    boundary conditions 
      4 0 1 0 
    share 
      4 0 0 0 
    mappingName 
      towerSpikeCrossSection
#
# pause
#
    exit
  body of revolution
    revolve which mapping?
     towerSpikeCrossSection
    choose a point on the line to revolve about
      0 0 0
#
    lines
      # 31 21 7 
      # getGridPoints(31,21,7);
      # $nx $ny $nz
      $axialLengthSpike= ( 2.45 - 1.8 )*1.5;
      $ns = intmg( $axialLengthSpike/$ds +1.5 ); 
      $spikeRadius=.05+$nDistSpike;
      $nTheta = intmg( 2.*$pi*$spikeRadius/$ds +1.5 ); 
      $ns $nTheta $nr 
# pause
    boundary conditions 
      1 0 -1 -1 1 0 
    share 
      4 0 0 0 5 0 
    mappingName
     towerSpikeSingular
# pause
    exit
#   
#   cap on the top of the spike
#
  reparameterize
    transform which mapping?
      towerSpikeSingular
    orthographic
      choose north or south pole
      -1
      exit
    orthographic
      specify sa,sb
        # .75 .75
        $saSpike=.7;
        $saSpike $saSpike
      exit
    lines
      # 11 11 9
      # getGridPoints(11,11,9);
      # $nx $ny $nz
      $spikeCapWidth=.075;
      $spikeCapRefinementFactor=3.;
      $nx = intmg( $spikeCapRefinementFactor*$spikeCapWidth/$ds + 3.5 ); # add more points on tip
      $nx $nx $nr 
    mappingName
     towerSpikeCap
    exit
#
#  -- remove the singular end from the main spike grid
  reparameterize
    transform which mapping?
      towerSpikeSingular
    restrict parameter space
      set corners
       # 0 .95 0 1 0 1
       0. .925 0. 1. 0. 1.
      exit
    lines
      $ns $nTheta $nr 
    mappingName
     towerSpike
    exit
#
# view mappings
# towerSpike
# towerSpikeCap
# towerPod
# pause
#
Box
  set corners
    $yatp=1.8; $ybtp=$yatp+$nDist;
     -.135 .135 $yatp $ybtp -.135 .135  
  lines
    # 9  9 9  
    # getGridPoints(9,9,9);
    $podTopWidth=2.*.135;
    $nx = intmg( $podTopWidth/$ds + 1.5 );
    $ny = intmg( $podTopWidth/$ds + 1.5 );
    $nx $ny $nz
  boundary conditions
    0 0 1 0 0 0 
  share
    0 0 3 0 0 0
  mappingName
    towerPodTop-unstretched
  exit
#
  stretch coordinates
    STRT:multigrid levels $ml
    Stretch r2:exp to linear
    STP:stretch r2 expl: position 0
    STP:stretch r2 expl: min dx, max dx $dsBL $ds
    stretch grid
    STRT:name towerPodTop
    exit
#
# =================== end buildTower ============================================

