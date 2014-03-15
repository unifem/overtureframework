#
#  Create the hub and join for the wind turbine
#
$hubRadius=.6;                # hub radius 
$hubFront=-1.25; $hubBack=.75;  # hub front and back positions (in x)
$hubShare=1;  # share flag for hub
$wingShare=2; # share flag for blade (first blade)
#
$nr=9; 
$rDist = $ds*8;  # radial distance for grids 
#
  smoothedPolygon
    vertices
    11
    $x0=$hubFront; 
    $r=.40; $x1=$hubFront+$r*($hubBack-$hubFront);
    $r=.6;  $x2=$hubFront+$r*($hubBack-$hubFront);
    $r=.80; $x3=$hubFront+$r*($hubBack-$hubFront);
    $r=1. ; $x4=$hubFront+$r*($hubBack-$hubFront);
    $x0    0
    $x1   $hubRadius
    $x2   $hubRadius
    $x3   $hubRadius
    $x4   $hubRadius
    $x4   0.
    $x4  -$hubRadius
    $x3  -$hubRadius
    $x2  -$hubRadius
    $x1  -$hubRadius
    $x0   0
#
#     -1.5   0
#     -.75  $hubRadius
#     0.    $hubRadius
#      .5   $hubRadius
#     1.5   $hubRadius
#     1.5   0.
#     1.5  -$hubRadius
#      .5  -$hubRadius
#     0.   -$hubRadius
#    -.75  -$hubRadius
#    -1.5   0
#
    sharpness
    20
    20
    40
    40
    40
    40
    40
    40
    20
    20
    20
# 
    n-dist
    fixed normal distance
     $nDist = $rDist;
     $nDist
    n-stretch
    1 4 0
    t-stretch
    .0  20
    .0  20
    .0  20
    .0  20
    .15 20
    .0  20
    .15 20
    .0  20
    .0  20
    .0  20
    .0  20
# 
    boundary conditions
    -1 -1 1 0
    lines
       81 7
  exit
#   Now choose half the hub so we can revolve
  reparameterize
    set corners
      0 .5 0. 1.
    mappingName
      halfHub
    exit
#  Form a body of revolution
  #  Form a body of revolution 
  body of revolution 
    revolve which mapping? 
    halfHub 
    tangent of line to revolve about 
      1 0 0 
    choose a point on the line to revolve about 
      0 0 0 
    force polar singularity
      0 0
    force polar singularity
      1 0
    lines 
     $nTheta= int( 2.*$pi*$hubRadius/$ds  + 1.5);
     $ns = int( ($hubBack-$hubFront+2.*$hubRadius)/$ds + 1.5 );
     $ns $nTheta $nr
    boundary conditions
      0 0 -1 -1 1 0
    mappingName
      hubSingular
    exit
#  build the cap on the front of the hub
  reparameterize
    transform which mapping?
      hubSingular
    orthographic
      specify sa,sb
        .4 .4
      exit
    mappingName
    hubFrontCap
    lines
      $width = 1.2*$hubRadius;
      $nx = int( $width/$ds + 1.5 );
      $nx $nx $nr
    share
      0 0 0 0 $hubShare 0 
    exit
#  build the cap on the back of the hub
  reparameterize
    transform which mapping?
      hubSingular
    orthographic
      choose north or south pole
      -1
      specify sa,sb
        .3 .3
      exit
    lines
      $width = 1.2*$hubRadius;
      $nx = int( $width/$ds + 1.5 );
      $nx $nx $nr
    share
      0 0 0 0 $hubShare 0
    mappingName
      hubBackCap
    exit
# removes the singular ends of the hub
  reparameterize
    transform which mapping?
      hubSingular
    set corners
      .1 .925 0. 1.
    share
      0 0 0 0 $hubShare 0
    mappingName
      hubBody
    exit
#
# Build a mapping that will join the hub to the turbine blade
#
# First create a cylinder with radius equal to the end of the turbine blade and
# that intersects the hub
#
  cylinder
    $bladeBaseRadius=.3; # radius of cylindrical end of the blade
    $outerRad = $bladeBaseRadius + $rDist; 
    if( $outerRad > .5 ){ $outerRad = .5; }
    bounds on the radial variable
      $bladeBaseRadius $outerRad
    # the cylinder needs to start inside the hub and extend outside: 
    bounds on the axial variable
      # $joinLength=1.25; 
      $joinCylStart=$hubRadius*.1; 
      $joinCylEnd  =$hubRadius + $hubRadius*.6; 
      $joinCylStart $joinCylEnd
    boundary conditions
      -1 -1 1 0 1 0
    share
      0 0 $hubShare 0 2 0
    mappingName
      hubWingCylinder
    exit
# form the join between the hub and the wing blade
  join
    choose curves
    hubWingCylinder
    hubSingular (side=0,axis=2)
    compute join
    boundary conditions
      -1 -1 5 0 2 0
    share
      0 0 $hubShare 0 $wingShare 0
    mappingName
      hubWingJoinMap
    lines
     $nTheta= int( 2.*$pi*($bladeBaseRadius+$rDist)/$ds  + 1.5);
     $ns = int( ($joinCylEnd-$hubRadius)/$ds + 1.5 );
     $nTheta $ns  $nr
     # 51 21 9 
    exit
# convert the join to a Nurbs for faster evaluation
  nurbs (surface)
    interpolate from a mapping
      hubWingJoinMap
    mappingName
      hubWingJoin
  exit
