# ------------------------------------------------------------------------------------------------------
#
# Grid for a disk dropping in a channel
#
#             yb  +---------------------+
#                 |                     |
#                 |                     |
#                 |                     |
#                 |                     |
#                 |           o(cx,cy)  |
#                 |                     |
#                 |                     |
#                 |                     |
#                 |                     |
#                 |                     |
#                 |                     |
#             ym  +---------------------+
#                 |                     |
#                 |                     |
#                 |                     |
#                 |                     |
#                 |                     |
#                 |   coarser outlet    |
#                 |                     |
#                 |                     |
#                 |                     |
#                 |                     |
#             ya  +---------------------+
#                xa                     xb
#
#  Usage:
#     ogen [-noplot] diskDropGrid.cmd -factor=<i> -interp=[i|e] -ml=<i> -blf=<> -cx=<> -cy=<> -radius=<f> ...
#          -xa=<f> -xb=<f> -ya=<f> -yb=<f> -ym=<f>
#
#  -radius : radius of the cylinder 
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx, -cy : center for the annulus
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#
#  Examples:
#    ogen -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=2 
#    ogen -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=4 
#    ogen -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=8
#    ogen -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=16
#    ogen -noplot diskDropGrid.cmd -interp=e -cx=.5 -cy=.5 -factor=32
#
#   -- wider channel,, disk closer to wall:
#    ogen -noplot diskDropGrid.cmd -interp=e -xa=-1.5 -xb=1.5 -cx=.9 -cy=.5 -prefix=risingDiskGrid -factor=2
#    ogen -noplot diskDropGrid.cmd -interp=e -xa=-1.5 -xb=1.5 -cx=.9 -cy=.5 -prefix=risingDiskGrid -factor=4 
#
#   -- shorter channel
#    ogen -noplot diskDropGrid.cmd -interp=e -ym=-2 -ya=-6 -cx=.5 -cy=.5 -prefix=diskDropShortGrid -factor=2
# ------------------------------------------------------------------------------------------------------
$prefix="diskDropGrid";  $rgd="var";
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; 
$xa=-1.25; $xb=1.25; 
$ya=-8.; $ym=-3.; $yb=2.; # bottom, mid top
$radius=.5; # radius of the cylinder
$cx=0.; $cy=0.;  # center for the cylinder 
$blf=3;  # grid lines are this much finer near the boundary
$blfc=3;  # grid lines on Channel-grid are this much finer near the boundary
$wallStretchOption=0; # 0=stretch-near=bottom, 1=stretchNearSides
$deltaRadius0=.1; # radius for rgd fixed
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf,"blfc=f"=> \$blfc,\
            "prefix=s"=> \$prefix,"wallStretchOption=i"=>\$wallStretchOption,"ym=f"=>\$ym,\
            "radius=f"=>\$radius,"cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"numGhost=i"=>\$numGhost );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=3; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=4; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $bcSquare eq "p" ){ $prefix = $prefix . "p"; }
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $blf ne 1 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . "$suffix" . ".hdf";}
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
# 
$dw = $order+1; $iw=$order+1; 
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
create mappings
#
$width=6*$ds; # width of boundary layer grid 
#
# coarse outlet
# 
$dsc=$ds*2; # coarser outlet 
rectangle
  $xac=$xa; $xbc=$xb;  $yac=$ya; $ybc=$ym+$dsc; 
  set corners
    $xac $xbc $yac $ybc 
  lines
    $nx = intmg( ($xbc-$xac)/$dsc +1.5 ); 
    $ny = intmg( ($ybc-$yac)/$dsc +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 0 
  share 
    1 2 3 0 
  mappingName
   outletUnstretched
exit
#
# -- stretch the grid lines on outlet
#
  stretch coordinates
    transform which mapping?
     outletUnstretched
    Stretch r2:exp to linear
    STP:stretch r2 expl: position 1
    STRT:multigrid levels $ml 
    $dsMin = $ds; # grid spacing in the normal direction 
    $farFieldSpacingFactor=8.; # WHAT SHOULD THIS BE ? propto distance ?
    $dsMax = $farFieldSpacingFactor*$ds; # spacing at far-field boundary
    STP:stretch r2 expl: linear weight 2
    STP:stretch r2 expl: min dx, max dx $dsMin $dsMax
    STRT:name outlet
    # open graphics
   exit 
#
# fine backGround 
# 
rectangle
  $xab=$xa+$width-$ds; $xbb=$xb-$width+$ds; 
  $yab=$ym; $ybb=$yb; 
  set corners
    $xab $xbb $yab $ybb 
  lines
    $nx = intmg( ($xbb-$xab)/$ds +1.5 ); 
    $ny = intmg( ($ybb-$yab)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 0 4 
  share 
    0 0 0 4
  mappingName
   backGround
exit
#
# Left wall
# 
rectangle
  $xaw=$xa; $xbw=$xa+$width; 
  $yaw=$ym; $ybw=$yb; 
  set corners
    $xaw $xbw $yaw $ybw 
  lines
    $nx = intmg( ($xbw-$xaw)/$ds +1.5 ); 
    $ny = intmg( ($ybw-$yaw)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 0 0 4 
  share 
    1 0 0 4
  mappingName
   leftWallUnstretched
exit
#
# Right wall
# 
rectangle
  $xaw=$xb-$width; $xbw=$xb;
  $yaw=$ym; $ybw=$yb; 
  set corners
    $xaw $xbw $yaw $ybw 
  lines
    $nx = intmg( ($xbw-$xaw)/$ds +1.5 ); 
    $ny = intmg( ($ybw-$yaw)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 2 0 4 
  share 
    0 2 0 4
  mappingName
   rightWallUnstretched
exit
#
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $innerRad=$radius; $outerRad = $innerRad + ($nr-1)*$ds;
  if( $rgd eq "fixed" ){ $outerRad = $innerRad + $deltaRadius0; $nr=intmg( $deltaRadius0/$ds + 2.5 ); }
  center: $cx $cy
  inner and outer radii
    $innerRad $outerRad
  lines
    if( $blf>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 5 0
  share
     0  0 100 0
  mappingName
   diskUnstretched
exit
#
#
# optionally stretch the grid lines next to the cylinder
# 
 stretch coordinates 
  transform which mapping? 
    diskUnstretched
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  $stretchResolution = 1.2;
  stretch resolution factor  $stretchResolution
  # exponential to linear stretching: 
   Stretch r2:exp to linear
   STP:stretch r2 expl: position 0
   $dxMin = $ds/$blf; 
   STP:stretch r2 expl: min dx, max dx $dxMin $ds
  STRT:name drop
 exit
#
#
# Stretch left wall grid
# 
 stretch coordinates 
  transform which mapping? 
    leftWallUnstretched
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  stretch resolution factor  $stretchResolution
  # exponential to linear stretching: 
   Stretch r1:exp to linear
   STP:stretch r1 expl: position 0
   $dxMin = $ds/$blfc; 
   STP:stretch r1 expl: min dx, max dx $dxMin $ds
  STRT:name leftWall
 exit
#
# Stretch right wall grid
# 
 stretch coordinates 
  transform which mapping? 
    rightWallUnstretched
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  stretch resolution factor  $stretchResolution
  # exponential to linear stretching: 
   Stretch r1:exp to linear
   STP:stretch r1 expl: position 1
   $dxMin = $ds/$blfc; 
   STP:stretch r1 expl: min dx, max dx $dxMin $ds
  STRT:name rightWall
 exit
#
#
exit
generate an overlapping grid
    outlet
    backGround
    leftWall 
    rightWall
    drop
  done
  change parameters
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      # $ngp = $ng+1;
      $ngp = $ng;
      $ng $ng $ng $ngp $ng $ng
  exit
  #  display intermediate results
  # open graphics
# 
  compute overlap
#  pause
  exit
#
save a grid (compressed)
$name
$prefix
exit

