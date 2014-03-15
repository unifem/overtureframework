# ------------------------------------------------------------------
# Make a sphere : this file is included in other ogen cmd files.
# Input: 
# $sphereName
# $northPoleName
# $southPoleName
# $ds : target grid spacing 
# $sphereShare=1; $sphereRadius=.5; 
# $xSphere=-.6; $ySphere=-.6; $zSphere=-.6; 
# $nr=3+ $nrExtra +$order;
# $radiusDir = 1 or -1 
# -----------------------------------------------------------------
Sphere
  if( $radiusDir > 0 ){ $innerRad=$sphereRadius; $outerRad=$innerRad+ ($nr-$nrExtra-1)*$ds;} #
  if( $radiusDir < 0 ){ $outerRad=$sphereRadius; $innerRad = $outerRad - ($nr-$nrExtra-1)*$ds;} #
  inner and outer radii
    $innerRad $outerRad
  centre for sphere
    $xSphere $ySphere $zSphere
  lines
    $nTheta=int( 2.*$pi*$sphereRadius*.5/$ds +1.5 );    
    $nPhi = int( $pi*$sphereRadius*.5/$ds +1.5 );  
    $nPhi $nTheta $nr
  boundary conditions
    if( $radiusDir > 0 ){ $bcFlags="0 0 -1 -1 1 0"; }else{  $bcFlags="0 0 -1 -1 0 1"; } #
    $bcFlags
    $sphereShare++;
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
    $sa = .55 + (2*$order-2)*$ds*.5; $sb=$sa; 
    specify sa,sb
      $sa $sb
  exit
  lines
    $nTheta=int( .7*$sa*$pi*$sphereRadius*.5/$ds +1.5 );    
    $nTheta $nTheta $nr
    $sphereShare++;
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
    $nTheta $nTheta $nr
    $sphereShare++;
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
        .125 .875  0. 1.  0. 1.
      exit
    mappingName
      $sphereName
    exit
