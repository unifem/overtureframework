# ------------------------------------------------------------------------------------
# Make a sphere : this file is included in other ogen cmd files.
# Input: 
# $sphereName
# $northPoleName
# $southPoleName
# $ds : target grid spacing 
# $sphereBC : use this bc value for the sphere surface (default=1)
# $sphereShare : use this share flag, this value is incremented by 1 on output.
# $sphereRadius=.5; 
# $xSphere=-.6; $ySphere=-.6; $zSphere=-.6; 
# $radiusDir = 1 or -1 
# $phiStart, $phiEnd : bounds on "phi" in [0,1] (i.e. top and bottom of sphere are cut off)
# $sphereWidth : if >0 : width of sphere grids, other-wise use a fix number of points
# $sphereFactor : The grid lines on the sphere cluster near the poles so we need to reduce the number of grid lines
# $nr=3+ $nrExtra +$order;  # for $sphereWidth<=0 
# -----------------------------------------------------------------------------------------
Sphere
  if( $sphereWidth > 0  ){ $sphereRadialDist = $sphereWidth; }else{ $sphereRadialDist = ($nr-$nrExtra-1)*$ds; }
  if( $radiusDir > 0 ){ $innerRad=$sphereRadius; $outerRad=$innerRad+ $sphereRadialDist; } #
  if( $radiusDir < 0 ){ $outerRad=$sphereRadius; $innerRad = $outerRad - $sphereRadialDist; } #
  if( $phiStart eq 0 ){ $phiStart=.125; } #
  if( $phiEnd   eq 0 ){ $phiEnd=1. - .125; } #
  if( $sphereBC eq "" ){ $sphereBC=1; } #
    # The grid lines on the sphere cluster near the poles so we need to reduce the number of
    # grid lines so that the average grid spacing is about $ds (factor of .75, do better here?)
  if( $sphereFactor eq "" ){  $sphereFactor=.7; }
  inner and outer radii
    $innerRad $outerRad
  centre for sphere
    $xSphere $ySphere $zSphere
  lines
    $nTheta=intmg( 2.*$pi*$sphereRadius*$sphereFactor/$ds +1.5 );    
    $nPhi = intmg( $pi*$sphereRadius*.85/$ds +1.5 );  
    $nrSphere = intmg( $sphereRadialDist/$ds + $nrExtra +1.5 );
    $nPhi $nTheta $nrSphere
  boundary conditions
    if( $radiusDir > 0 ){ $bcFlags="0 0 -1 -1 $sphereBC 0"; }else{  $bcFlags="0 0 -1 -1 0 $sphereBC"; } #
    $bcFlags
    if( $radiusDir > 0 ){ $shareFlags="0 0 0 0 $sphereShare 0"; }else{  $shareFlags="0 0 0 0 0 $sphereShare"; } #
  share
    $shareFlags
  mappingName
    $baseSphereName = $sphereName . "Base";
    $baseSphereName
# open graphics
exit
#
# now make a mapping for the north pole
#
reparameterize
  transform which mapping?
    $sphereName
  orthographic
    # sa=2 --> patches just match (not including ghost points)
    # fix me: $sa should depend on $phiStart, $phiEnd
    # $sa = .55 + (2*$order-2)*$ds*.5; $sb=$sa; 
    $sa = .65 + (2*$order-2)*$ds*.5; $sb=$sa; 
    specify sa,sb
      $sa $sb
  exit
  lines
    $nThetaPatch=intmg( .7*$sa*$pi*$sphereRadius*$sphereFactor/$ds +1.5 );    
    $nThetaPatch $nThetaPatch $nrSphere
    if( $radiusDir > 0 ){ $shareFlags="0 0 0 0 $sphereShare 0"; }else{  $shareFlags="0 0 0 0 0 $sphereShare"; } #
  share
    $shareFlags
  mappingName
    $northPoleName
exit
#
# now make a mapping for the south pole
#
reparameterize
  transform which mapping?
    $sphereName
  orthographic
    choose north or south pole
      -1
    specify sa,sb
      $sa $sb
  exit
  lines
    $nThetaPatch $nThetaPatch $nrSphere
    if( $radiusDir > 0 ){ $shareFlags="0 0 0 0 $sphereShare 0"; }else{  $shareFlags="0 0 0 0 0 $sphereShare"; } #
  share
    $shareFlags
  mappingName
    $southPoleName
exit
# 
#  -- remove the singular ends of the sphere 
  reparameterize
    transform which mapping?
      $baseSphereName
    restrict parameter space
      set corners
        # .125 .875  0. 1.  0. 1.
        $phiStart $phiEnd 0. 1.  0. 1.
      exit
    lines
      $nPhi = intmg( ($phiEnd-$phiStart)*$pi*$sphereRadius*.85/$ds +1.5 );  
      $nPhi $nTheta $nrSphere
    mappingName
      $sphereName
    exit
# 
    $sphereShare++;
