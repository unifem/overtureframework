#
#  Define a box shaped nacelle and tower for a wind turbine
#
#
$nacelleLength=2.5; 
# the $towerOuterRadius should be a bit less than half the nacelleWidth
# $nacelleWidth=1.; $towerRadius=.3; $towerOuterRadius=.45; $towerJoinOuterRadius=.45; 
$nacelleWidth=1.5; $towerRadius=.55; $towerOuterRadius=.7; $towerJoinOuterRadius=$towerOuterRadius; 
# 
$towerBase=-8.; 
$xTower=1.75; $zTower=0.;   # tower is centered here 
$xNacelle=$xTower-1.; # shift nacelle this much 
#
# The nacelle is a rounded "block"
  lofted surface 
    blade length: $nacelleLength
    chord: $nacelleWidth
    # exponent for transition from smoothed polygon to circles on ends
    section exponent: 20
    smoothed polygon sections
    flat double tip profile
    # we can change the profile to make the corners sharper (default=40)
    edit profile
      sharpness
      40 
      40
      40
      40
      40
      40
      exit
    # we can change the section to make the corners sharper (default=40)
    edit section
      sharpness
      40
      40
      40
      40
      40
      40
      exit
    mappingName
      nacelleSurface
# pause
    exit
#  build a cap on the end of the nacelle
  reparameterize
    orthographic
      choose north or south pole
      1
      specify sa,sb
      .225 .225
      exit
    lines
      21 21
    mappingName
      nacelleCapNorthPole
# pause
    exit
#  build a cap on the end of the nacelle
  reparameterize
    orthographic
      choose north or south pole
      -1
      specify sa,sb
      .225 .225
      exit
    lines
      21 21
    mappingName
      nacelleCapSouthPole
    exit
  reparameterize
    restrict parameter space
      set corners
      .06 .94 0 1
      exit
    mappingName
    nacelleSurfaceNoCaps
    exit
#
  cylinder
    orientation
      2 0 1
    centre for cylinder
      0 0 1
    bounds on the radial variable
      $towerRadius $towerJoinOuterRadius
    bounds on the axial variable
      $towerJoinTop=-.5; $towerJoinBase=-1.; 
      $towerJoinRealTop=-.25; 
      $towerJoinBase $towerJoinRealTop
    lines
      41 11 7
    boundary conditions
      -1 -1 0 0 1 0
    mappingName
      cylToNacelle
    exit
# -- make a new surface for the nacelle with the branch cut on the top for the join mapping which
#    does not like to have the branch cut in the join region
  reparameterize
    transform which mapping?
      nacelleSurface
    set corners
      0 1 -.5 .5
    mappingName
      nacelleSurfaceWithBranchCutOnTop
    exit
#
  join
    choose curves
    cylToNacelle
    nacelleSurfaceWithBranchCutOnTop
    # specify which end of the cylinder we want to keep:
    end of join
      0.
    compute join
    lines
      $ns = int( ($towerJoinTop-$towerJoinBase)/$ds + 1.5 );
      $nTheta = int( 2.*$pi*.5*($towerRadius+$towerJoinOuterRadius)/$ds + 1.5 );
      $nr = int( ($towerJoinOuterRadius-$towerRadius)/$ds + 2.5 );
      $nTheta $ns  $nr 
      # 41 11 7
$nacelleShare=10;
$towerShare=11; 
$groundShare=20; 
    boundary conditions
     # 10=share for nacelle
     # 11=share for tower
     # 20 = ground (temporary)
      -1 -1 $nacelleShare 0 $towerShare 0
    share
      0 0 $nacelleShare 0 $towerShare 0
    mappingName
      towerNacelleJoin
    exit
#
  mapping from normals
    extend normals from which mapping?
    nacelleSurfaceNoCaps
    normal distance
      $nDist=.2; 
      -$nDist
    lines
      $ns = int( ($nacelleLength+$nacelleWidth)/$ds + 1.5 );
      $nTheta = int( 4.*$nacelleWidth/$ds + 1.5 );
      $nr = int( $nDist/$ds + 2.5 );
      $ns $nTheta $nr 
      # 61 51 11
    boundary conditions
      0 0 -1 -1 $nacelleShare 0
    share
      0 0 0 0 $nacelleShare 0
    mappingName
      nacelle
    exit
# --------------------------------------------------------------------
# ---- Define a subroutine to convert a Mapping to a Nurbs Mapping ---
# --------------------------------------------------------------------
sub convertToNurbs\
{ local($old,$new,$angle,$xShift,$yShift,$zShift)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from a mapping\n" . "$old\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "shift\n" . "$xShift $yShift $zShift\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
#
# -- we need to convert the orthographic caps to nurbs as the spherical coordinate derivatives 
#    are not correct for the Loft mapping
$angle=0; $xShift=0.;  $yShift=0.; $zShift=0.;
convertToNurbs(nacelleCapNorthPole,nacelleCapNorthPoleNurbs,$angle,$xShift,$yShift,$zShift);
$commands
convertToNurbs(nacelleCapSouthPole,nacelleCapSouthPoleNurbs,$angle,$xShift,$yShift,$zShift);
$commands
# 
#   nurbs (surface)
#     interpolate from a mapping
#       nacelleCapNorthPole
#     mappingName
#       nacelleCapNorthPoleNurbs
#     exit
# 
#   nurbs (surface)
#     interpolate from a mapping
#       nacelleCapSouthPole
#     mappingName
#       nacelleCapSouthPoleNurbs
#     exit
#
  mapping from normals
    extend normals from which mapping?
    nacelleCapNorthPoleNurbs
    normal distance
      -$nDist
    lines
      $nx = int( .8*$nacelleWidth/$ds + 1.5 );
      $nr = int( $nDist/$ds + 2.5 );
      $nx $nx $nr 
      # 15 15 11
    boundary conditions
      0 0 0 0 10 0
    share
      0 0 0 0 10 0
    mappingName
      nacelleCapBack
# pause
    exit
#
  mapping from normals
    extend normals from which mapping?
    nacelleCapSouthPoleNurbs
    normal distance
      -$nDist
    lines
      $nx $nx $nr 
      # 15 15 11
    boundary conditions
      0 0 0 0 10 0
    share
      0 0 0 0 10 0
    mappingName
      nacelleCapFront
# pause
    exit
#
# convert to nurbs so we can rotate/shift and for faster processing 
$angleNacelle=90.; # rotate nacelle this amount around y-axis
$angle=$angleNacelle; $xShift=$xNacelle;  $yShift=0.; $zShift=$zTower;
#
convertToNurbs(nacelle,nacelleNurbs,$angle,$xShift,$yShift,$zShift);
$commands
convertToNurbs(nacelleCapBack,nacelleCapBackNurbs,$angle,$xShift,$yShift,$zShift);
$commands
convertToNurbs(nacelleCapFront,nacelleCapFrontNurbs,$angle,$xShift,$yShift,$zShift);
$commands
$xShift=$xTower-1.; 
convertToNurbs(towerNacelleJoin,towerNacelleJoinNurbs,$angle,$xShift,$yShift,$zShift);
$commands
#
# Here is the tower
#
  cylinder
    orientation
      2 0 1
    centre for cylinder
      $xTower 0. $zTower
    bounds on the radial variable
      $towerRadius $towerOuterRadius
    bounds on the axial variable
      $towerTop=-.9; 
      $towerBase $towerTop
    lines
      $ns = int( ($towerTop-$towerBase)/$ds + 1.5 );
      $nTheta = int( 2.*$pi*.5*($towerRadius+$towerOuterRadius)/$ds + 1.5 );
      $nr = int( ($towerOuterRadius-$towerRadius)/$ds + 2.5 );
      $nTheta $ns $nr 
      # 41 11 7
    boundary conditions
      -1 -1 $groundShare 0 1 0
    share
       0  0 $groundShare 0 $towerShare 0 
    mappingName
      tower
    exit
