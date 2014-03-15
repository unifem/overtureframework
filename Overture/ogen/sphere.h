# ---------------------------------------------------------------------
# Make a sphere : this file is included in other ogen cmd files.
#    Construct a grid for a sphere with 3-patches
# Input:
#  $sphereRadius    : radius of the sphere
#  $sphereRadialWidth : optionally set the radial width of the sphere. If this variable is not set
#                      then the sphere will have a fixed number of points in the radial direction.
#  $numberOfSpheres : set to zero at start (will be incremented)
# $sphereBC : use this bc value for the sphere surface (default=1)
#  $sphereShare     : share value for this sphere (will be incremented)
#  ($xSphere,$ySphere,$zSphere) : center of the sphere
#  $nr              : number of lines in the radial direction
#  $ds              : target grid spacing 
#  $sphereStretchb  : stretching exponent
# Notes:
#  This file assumes the perl function intmg is defined (for MG levels)
# ----------------------------------------------------------------------
if( $sphereOuterBoundaryCondition eq "" ){ $sphereOuterBoundaryCondition=0; }  #
if( $sphereBC eq "" ){ $sphereBC=1; } #
Sphere
  $innerRad=$sphereRadius; 
  if( $sphereRadialWidth eq "" ){ $outerRad=$innerRad+($nr-$nrExtra-1)*$ds; }else{ $outerRad=$innerRad+$sphereRadialWidth; }  #
  inner and outer radii
    $innerRad $outerRad
  # set centre below after rotating
  # centre for sphere
  #  $xSphere $ySphere $zSphere
  lines
    $nTheta=intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds +1.5 );    
    $nPhi = intmg( $pi*($innerRad+$outerRad)*.5/$ds +1.5 );  
    $nPhi $nTheta $nr
  boundary conditions
    0 0 -1 -1 $sphereBC $sphereOuterBoundaryCondition 
  share
    0 0 0 0 $sphereShare $sphereOuterBoundaryCondition 
  mappingName
    $numberOfSpheres++;
    $sphereName="unrotatedSphere$numberOfSpheres";
    $sphereName
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
    $sa = .55 + ($order-2)*$ds*.5; $sb=$sa; 
    specify sa,sb
      $sa $sb
  exit
  lines
    # $nTheta=intmg( .7*$sa*$pi*($innerRad+$outerRad)*.5/$ds +1.5 );    
    $nTheta=intmg( .5*$sa*$pi*($innerRad+$outerRad)*.5/$ds +1.5 );    
    $nTheta $nTheta $nr
  mappingName
    $northPoleName="unRotatedNorthPole$numberOfSpheres";
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
  mappingName
    $southPoleName="unRotatedSouthPole$numberOfSpheres";
    $southPoleName
exit
#
#  -- stretch mappings ---
# 
  stretch coordinates
    transform which mapping?
      $sphereName
    Stretch r3:itanh
    STP:stretch r3 itanh: layer 0 1 $sphereStretchb 0 (id>=0,weight,exponent,position)
    stretch grid
    mappingName
      $sphereName="stretchedSphere$numberOfSpheres";
      $sphereName
    exit
# 
  stretch coordinates
    transform which mapping?
      $northPoleName
    Stretch r3:itanh
    STP:stretch r3 itanh: layer 0 1 $sphereStretchb 0 (id>=0,weight,exponent,position)
    stretch grid
    mappingName
      $northPoleName="stretchedNorthPole$numberOfSpheres";
      $northPoleName
    exit
# 
  stretch coordinates
    transform which mapping?
      $southPoleName
    Stretch r3:itanh
    STP:stretch r3 itanh: layer 0 1 $sphereStretchb 0 (id>=0,weight,exponent,position)
    stretch grid
    mappingName
      $southPoleName="stretchedSouthPole$numberOfSpheres";
      $southPoleName
    exit
#
#  -- remove the singular ends of the sphere 
  reparameterize
    transform which mapping?
      $sphereName
    restrict parameter space
      set corners
      $phia=.125; $phib=.875;
      # $phia=.05; $phib=.95;
        $phia $phib  0. 1.  0. 1.
      exit
    lines
      $nTheta=intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds +1.5 );    
      $nPhi = intmg( ($phib-$phia)*$pi*($innerRad+$outerRad)*.5/$ds +1.5 );  
      $nPhi $nTheta $nr
#     boundary conditions
#       0 0 2 2 1 0 
#     share
#       0 0 3 2 1 0 
    mappingName
      $sphereName="sphereWithoutPoles$numberOfSpheres";
      $sphereName
    exit
#
# -- convert to nurbs for faster evaluation AND rotate mappings to align with the x-axis
# 
  nurbs 
    interpolate from mapping with options
      $sphereName
      # keep stretching: 
      parameterize by index (uniform)
    done
    rotate
      -90 1
      0 0 0
    shift 
      $xSphere $ySphere $zSphere
    mappingName
      $sphereName="sphere$numberOfSpheres";
      $sphereName
    exit
# 
  nurbs 
    interpolate from mapping with options
      $northPoleName
      # keep stretching: 
      parameterize by index (uniform)
    done
    rotate
      -90 1
      0 0 0
    shift 
      $xSphere $ySphere $zSphere
    mappingName
      $northPoleName="northPole$numberOfSpheres";
      $northPoleName
    exit
# 
  nurbs 
    interpolate from mapping with options
      $southPoleName
      # keep stretching: 
      parameterize by index (uniform)
    done
    rotate
      -90 1
      0 0 0
    shift 
      $xSphere $ySphere $zSphere
    mappingName
      $southPoleName="southPole$numberOfSpheres";
      $southPoleName
    exit
# 
    $gridNames.="\n $sphereName\n $northPoleName\n $southPoleName";
    $sphereShare++;
