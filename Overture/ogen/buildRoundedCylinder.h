#
#########################################################################
#   Make a building with a round cross-section
#   Input: uses the getGridPoints perl subroutine
#########################################################################
# 
  smoothedPolygon 
    $heightrc=1.; # height of the cylinder
    $radrc=.25; # radius of cyliner
    $radInner=.5*$radrc-$dsExtra;
    vertices 
      3 
      $radrc   .0 
      $radrc    $heightrc 
      $radInner $heightrc 
    n-dist 
      fixed normal distance 
      $nDist = ($nr-3)*$ds;
      -$nDist     
      # -.15
    corners 
    specify positions of corners 
      $xc2=$radrc+$nDist;
      $yc3=$heightrc+$nDist;
      $radrc 0. 
      $radInner $heightrc 
      $xc2 0. 
      $radInner $yc3
    lines 
      55 7 
    boundary conditions 
      2 0 1 0 
    share 
      2 0 1 0 
    mappingName 
      crossSection 
   # pause
#
    exit
  body of revolution
    revolve which mapping?
    crossSection
    choose a point on the line to revolve about
      0 0 0
    lines
      # 57 9 57
      ## getGridPoints(57,9,57);
      $axialLengthrc=1.5; # approximate height + top, include extra for stretching
      $ns = intmg( $axialLengthrc/$ds +1.5 ); 
      $nTheta = intmg( 2.*$pi*$radrc/$ds +1.5 ); 
      $ns $nr $nTheta
    # open graphics
    # pause
    boundary conditions 
      1 0 1 0 -1 -1 
    share 
      2 0 1 0 0 0 
    mappingName
     roundedCylinderGrid
    exit
#
#  -- here is the top cap ---
#
Box
  set corners
   $xbrc=$radInner+$dsExtra+$ds; $xarc=-$xbrc; $yarc=$heightrc; $ybrc=$heightrc+$nDist; $zarc=$xarc; $zbrc=$xbrc; #
   $xarc $xbrc $yarc $ybrc $zarc $zbrc
    #  -.12 .12  1. 1.125  -.12 .12 
  lines
    # 13 13 13
    #getGridPoints(13,13,13);
    $topFactor=1.25;  # make a grid a bit finer on the top
    $nx = intmg( $topFactor*($xbrc-$xarc)/$ds +1.5 ); 
    $ny = intmg( $topFactor*($ybrc-$yarc)/$ds +1.5 );
    $nz = intmg( $topFactor*($zbrc-$zarc)/$ds +1.5 );
    $nx $ny $nz
  boundary conditions
    0 0 1 0 0 0 
  share
    0 0 1 0 0 0
  mappingName
     roundedCylinderTop-unstretched
  exit
#
  stretch coordinates
    STRT:multigrid levels $ml
    Stretch r2:exp to linear
    STP:stretch r2 expl: position 0
    STP:stretch r2 expl: min dx, max dx $dsBL $ds
    stretch grid
    STRT:name roundedCylinderTop
    exit
#
####################################################################
