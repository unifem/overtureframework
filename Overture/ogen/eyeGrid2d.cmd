#
# Grid for a 2D deforming eye,
#
# Usage: 
#     ogen [-noplot] eyeGrid2d -factor=<num> -order=[2/4/6/8] -interp=[e/i]  -radX=<> -radY=<> -ml=<>  
# 
#  -radX, -radY : radii of the ellipse in the x and y directions
#  -ml = number of (extra) multigrid levels to support
# 
# Examples:
# 
#     ogen -noplot eyeGrid2d -interp=e -factor=4
#     ogen -noplot eyeGrid2d -interp=e -factor=8
#     ogen -noplot eyeGrid2d -interp=e -factor=16
#     ogen -noplot eyeGrid2d -interp=e -factor=32
# 
$prefix="eyeGrid2d";  $rgd="var"; $angle=0.; $branch=0; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; 
$xa=-.5; $xb=.5;  # left and right ends
$hTop=.3; $hBot=-.1; 
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"bTop=f"=>\$hTop,"hBot=f"=>\$hBot,"angle=f"=>\$angle,\
            "branch=i"=>\$branch );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $branch ne 0 ){ $prefix = $prefix . "Branch"; }
if( $angle ne 0 ){ $prefix = $prefix . "Angle$angle"; }
$suffix = ".order$order"; 
if( $blf ne 1 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
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
rectangle
  # Make background grid square to allow eye to open to a circle 
  $ya=$hBot; $yb=$hTop;
  set corners
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 0 0 
  share 
    0 0 0 0 
  mappingName
    backGround
exit
# ---
 $nr = intmg( 7 );  # number of points in the radial direction
 $nDist = ($nr-3)*$ds;
# 
 $depth = 2.*$ds; # half-depth of left and right ends.
#
# ----------------------------
# --- make the top eye-lid ---
# ----------------------------
#
$arcLength = ($xb-$xa) + 2.*$hTop;   # approximate -- fix me 
$nTheta = intmg( $arcLength/$ds + 1.5 );
$npts=$nTheta; 
$betaTop=10.;  # factor in Gaussian curve
$pts="$npts"; $arc=0.; 
$offset=$depth-$hTop*exp(-$betaTop*$xa*$xa);
for( $i=0; $i<$npts; $i++ ){ $x=$xa+($xb-$xa)*$i/($npts-1); $y=$offset + $hTop*exp(-$betaTop*$x*$x); $pts .= "\n $x $y"; }
nurbs
  set domain dimension
    1
  set range dimension
    2
  enter points
    $pts
#  open graphics
   mappingName
    eyeLidTopBoundary
exit
# ----- hyperbolic grid -----
  hyperbolic
    distance to march $nDist
    lines to march $nr   
    points on initial curve $nTheta
    spacing: geometric
    geometric stretch factor 1.1
    # increase volume smooths if ellipse is thin to handle sharp ends
    volume smooths 50
    BC: left fix x, float y and z
    BC: right fix x, float y and z
    normal blending 7 7  (lines: left, right)
# 
   generate
   # open graphics
# 
    boundary conditions
     1 2 4 0
    share
     1 2  100 0
    mappingName
      eyeLidTop
  exit
# ----------------------------
# --- make the bottom eye-lid ---
# ----------------------------
#
$arcLength = ($xb-$xa) + 2.*$hBot;   # approximate -- fix me 
$nTheta = intmg( $arcLength/$ds + 1.5 );
$npts=$nTheta; 
$betaBot=10.;  # factor in Gaussian curve
$pts="$npts"; $arc=0.; 
$offset=-$depth-$hBot*exp(-$betaBot*$xa*$xa);
for( $i=0; $i<$npts; $i++ ){ $x=$xa+($xb-$xa)*$i/($npts-1); $y=$offset + $hBot*exp(-$betaBot*$x*$x); $pts .= "\n $x $y"; }
nurbs
  set domain dimension
    1
  set range dimension
    2
  enter points
    $pts
#  open graphics
   mappingName
    eyeLidBottomBoundary
exit
# ----- hyperbolic grid -----
  hyperbolic
    distance to march $nDist
    backward
    lines to march $nr   
    points on initial curve $nTheta
    spacing: geometric
    geometric stretch factor 1.1
    # increase volume smooths if ellipse is thin to handle sharp ends
    volume smooths 50
    BC: left fix x, float y and z
    BC: right fix x, float y and z
    normal blending 7 7  (lines: left, right)
# 
   generate
   # open graphics
# 
    boundary conditions
     1 2 5 0
    share
     1 2 0 0
    mappingName
      eyeLidBottom
  exit
#
exit
generate an overlapping grid
    backGround
    eyeLidBottom
    eyeLidTop
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
  # open graphics
   compute overlap
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
eye2d
exit

