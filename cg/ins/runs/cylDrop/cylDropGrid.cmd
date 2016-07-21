# ------------------------------------------------------------------------------------------------------
#
# Grid for a cylinder dropping in a channel
#
#  Usage:
#     ogen [-noplot] cylDropGrid.cmd -factor=<i> -interp=[i|e] -ml=<i> -blf=<> -cx=<> -cy=<> -radius=<f>
#
#  -radius : radius of the cylinder 
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx, -cy : center for the annulus
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#
#  Examples:
#    ogen -noplot cylDropGrid.cmd -interp=e -factor=2 
#    ogen -noplot cylDropGrid.cmd -interp=e -factor=4
#    ogen -noplot cylDropGrid.cmd -interp=e -factor=4 -ml=2
#    ogen -noplot cylDropGrid.cmd -interp=e -factor=8 -ml=3
#
# -- bigger cylinder, small domain for testing 
#    ogen -noplot cylDropGrid.cmd -interp=e -radius=.25 -yb=2. -cx=1. -cy=1. -prefix=cylGridSmall -factor=2
#    ogen -noplot cylDropGrid.cmd -interp=e -radius=.25 -yb=2. -cx=1. -cy=1. -prefix=cylGridSmall -factor=4
#
# -- bigger cylinder, small domain. stretch backGround
#    ogen -noplot cylDropGrid.cmd -interp=e -radius=.25 -yb=2. -cx=1. -cy=1. -blfc=3. -prefix=cylGridSmallStretched -factor=2
#    ogen -noplot cylDropGrid.cmd -interp=e -radius=.25 -yb=2. -cx=1. -cy=1. -blfc=3. -prefix=cylGridSmalllStretched -factor=4
#
#  --- offset cylinder in a channel
#   ogen -noplot cylDropGrid.cmd -interp=e -radius=.5 -xa=-1.25 -xb=1.25 -ya=-3. -yb=2. -cx=.5 -cy=0. -wallStretchOption=1 -blfc=2 -blf=2 -prefix=offsetCylInChannel -factor=2
#   ogen -noplot cylDropGrid.cmd -interp=e -radius=.5 -xa=-1.25 -xb=1.25 -ya=-6. -yb=3. -cx=.5 -cy=0. -wallStretchOption=1 -blfc=2 -blf=2 -prefix=offsetCylInChannel -factor=4
#   ogen -noplot cylDropGrid.cmd -interp=e -radius=.5 -xa=-1.25 -xb=1.25 -ya=-6. -yb=3. -cx=.5 -cy=0. -wallStretchOption=1 -blfc=2 -blf=2 -prefix=offsetCylInChannel -factor=8
# 
#  -- no stretching on the channel
#   ogen -noplot cylDropGrid.cmd -interp=e -radius=.5 -xa=-1.25 -xb=1.25 -ya=-6. -yb=3. -cx=.5 -cy=0. -wallStretchOption=0 -blf=2 -prefix=offsetCylInChannel -factor=8
#
# ------------------------------------------------------------------------------------------------------
$prefix="cylDropGrid";  $rgd="var";
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=0.; $xb=2.; $ya=0.; $yb=6.; 
$radius=.125; # radius of the cylinder
$cx=1.; $cy=4.;  # center for the cylinder 
$blf=3;  # grid lines are this much finer near the boundary
$blfc=1;  # grid lines on Channel-grid are this much finer near the boundary
$wallStretchOption=0; # 0=stretch-near=bottom, 1=stretchNearSides
$deltaRadius0=.1; # radius for rgd fixed
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf,"blfc=f"=> \$blfc,\
            "prefix=s"=> \$prefix,"wallStretchOption=i"=>\$wallStretchOption,\
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
# Optionally stretch the channel grid
$channelBaseGrid="channel"; $channelStretched="channel-stretched";
if( $blfc ne "1" ){ $channelBaseGrid="channel-unstretched"; $channelStretched="channel"; }
rectangle
  set corners
    $xa $xb $ya $yb 
  lines
   $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 4 
  mappingName
   $channelBaseGrid
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
   drop-unstretched
exit
#
#
# optionally stretch the grid lines next to the cylinder
# 
 stretch coordinates 
  transform which mapping? 
    drop-unstretched
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
# optionally stretch the back-ground grid
# 
 stretch coordinates 
  transform which mapping? 
    $channelBaseGrid
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  stretch resolution factor  $stretchResolution
  # exponential to linear stretching: 
  if( $wallStretchOption eq 0 ){ $dir="r2"; $pos=0.; }
  if( $wallStretchOption eq 1 ){ $dir="r1"; $pos=1.; }
   Stretch $dir:exp to linear
   STP:stretch $dir expl: position $pos
   $dxMin = $ds/$blfc; 
   STP:stretch $dir expl: min dx, max dx $dxMin $ds
  STRT:name $channelStretched
 exit
#
#
exit
generate an overlapping grid
    channel
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

