# ------------------------------------------------------------------------------------------------------
#
# Grid for a cylinder settling to a wall
#
#  Usage:
#     ogen [-noplot] settlingCylGrid.cmd -factor=<i> -interp=[i|e] -ml=<i> -blf=<> -cx=<> -cy=<> -radius=<f>
#
#  -radius : radius of the cylinder 
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx, -cy : center for the annulus
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#
#  Examples:
#    ogen -noplot settlingCylGrid.cmd -interp=i -factor=2 
#    ogen -noplot settlingCylGrid.cmd -interp=i -factor=4
#
#    ogen -noplot settlingCylGrid.cmd -interp=e -factor=2 
#
# ------------------------------------------------------------------------------------------------------
$prefix="settlingCylGrid";  $rgd="var";
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-1.; $yb=2.; 
$radius=.5; # radius of the cylinder
$cx=0.; $cy=0.;  # center for the cylinder 
$height=.25; # height of stretch grid at bottom wall
$blf=5;  # grid lines are this much finer near the boundary and bottom wall
$deltaRadius0=.1; # radius for rgd fixed
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf,"blfc=f"=> \$blfc,\
            "prefix=s"=> \$prefix,\
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
# Background grid:
#
rectangle
  set corners
    $yac=$ya+$height-$ng*$ds; 
    $xa $xb $yac $yb 
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$yac)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 0 4 
  share
    1 2 0 0 
  mappingName
   backGround
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
   unstretchedDrop
exit
#
#
# Stretch the grid lines next to the cylinder
# 
 stretch coordinates 
  transform which mapping? 
    unstretchedDrop
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
# Stretched grid near bottom wall:
rectangle
  set corners
    $ybw=$ya+$height+$ng*$ds; 
    $xa $xb $ya $ybw 
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($ybw-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 0 
  share
    1 2 0 0 
  mappingName
   unstretchedBottomWallGrid
exit
#
# Stretch bottom wall grid
# 
 stretch coordinates 
  transform which mapping? 
    unstretchedBottomWallGrid
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  stretch resolution factor  $stretchResolution
  # exponential to linear stretching: 
   Stretch r2:exp to linear
   STP:stretch r2 expl: position 0
   $dxMin = $ds/$blf; 
   STP:stretch r2 expl: min dx, max dx $dxMin $ds
  STRT:name bottomWallGrid
 exit
#
#
exit
generate an overlapping grid
    backGround
    bottomWallGrid
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

