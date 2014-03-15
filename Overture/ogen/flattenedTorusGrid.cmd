# flattenedTorusGrid.cmd : 
# 
# Usage:
#    ogen [-noplot] flattenedTorusGrid.cmd -factor=<> -interp=[e|i] -order=<> -ml=<> -widthX=<> -widthY=<> -widthZ=<> ...
#           -blf=<> -xa=<> -xb=<> -ya=<> -yb=<> -za=<> -zb=<> 
# Options:
#   -xa, -xb, -ya, -yb, -za, -zb : bounding box
#   -blf : boundary layer stretching factor
#
# Examples:
#
#  ogen -noplot flattenedTorusGrid.cmd -factor=2 
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -factor=2 [OK
# 
# -- multigrid:
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -factor=2 -ml=1
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -factor=3 -ml=1
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -factor=4 -ml=1
#
# -- fourth-order
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -order=4 -factor=2 -ml=1 [backup
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -order=4 -factor=3 -ml=1
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -order=4 -factor=4 -ml=1
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -order=4 -factor=4 -ml=2
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -order=4 -factor=8 -ml=2
#  ogen -noplot flattenedTorusGrid.cmd -interp=e -order=4 -factor=8 -ml=3
#
#
$sharpnessLB=40.;                     # corner sharpness
$widthX=1.; $widthY=1.; $widthZ=1.;   # box size 
$blf=5.; # boundary layer stretching factor
$rotateX=0.; $rotateY=0.; $rotateZ=0.; # rotation (degrees) about X followed by Y followed by Z axis
#
# Background grid:
$xa=-1.; $xb=2.5; $ya=-1.25; $yb=1.25; $za=-1.5; $zb=1.5; 
#
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$factorNurbs=1.; # factor for the Nurbs representation 
$name=""; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"blf=f"=>\$blf,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"sharpnessLB=f"=> \$sharpnessLB,\
            "widthX=f"=> \$widthX,"widthY=f"=> \$widthY,"widthZ=f"=> \$widthZ,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,\
            "xac=f"=>\$xac,"xbc=f"=>\$xbc,"yac=f"=>\$yac,"ybc=f"=>\$ybc,"zac=f"=>\$zac,"zbc=f"=>\$zbc );
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "flattenedTorusGrid" . "$interp$factor" . $suffix . ".hdf";}
* 
#
# NOTE: x-bounds and y-bounds should be centered around 0: 
$xalb=-$widthX*.5; $xblb=$widthX*.5; $yalb=-$widthY*.5; $yblb=$widthY*.5; $zalb=-$widthZ*.5; $zblb=$widthZ*.5;   # lofted box bounds 
#
$ds=.1/$factor;
$dsn = .1/$factorNurbs; # build Nurbs representation with this grid spacing 
$dsBL = $ds/$blf; # boundary layer spacing (spacing in the normal direction)
* 
$dw = $order+1; $iw=$order+1; 
*
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$pi = 4.*atan2(1.,1.);
#
# NOTE: do NOT extend the grid too far since the core region needs to background grid in it.
# nr = number of lines in normal directions to boundaries
$nr = max( 5 + $ng + 2*($order-2), 2**($ml) );
$nr = intmg( $nr );
#
$wallBC=7;   # BC for walls of the box 
$wallShare=7; 
# 
$nDist = ($nr-3)*$ds; 
#
create mappings
#
#
SmoothedPolygon
* start on a side so that the polygon is symmetric
  vertices 
    6
    -.1   .00
    -.1   .5
     .1   .5
     .1  -.5
    -.1  -.5
    -.1   .00
  n-stretch
   $nStretch=4.; 
   1. $nStretch 0.
  n-dist
    fixed normal distance
    $nDist
  lines
    # $stretchFactor=1.4; # add more lines in the tangential direction due to stretching at corners
    $stretchFactor=1.0; # add more lines in the tangential direction due to stretching at corners
    $length=2.75; # approx. perimeter length 
    $ns = intmg( $stretchFactor*$length/$ds +1.5 ); 
    $ns $nr
  t-stretch
    $tStretch=8.; 
    0. 1.
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    0. 1.
  boundary conditions
    -1 -1 7 0
  share 
    0 0 0 0
  mappingName
    crossSection
  exit
#
  body of revolution
    revolve which mapping?
    crossSection
    $radius=.5; 
    choose a point on the line to revolve about
      $radius 0. 0.
    tangent of line to revolve about
      0 1 0
    lines
      $nTheta = intmg( 2.*$pi*$radius/$ds +1.5 );
      $ns $nr $nTheta
    boundary conditions
      -1 -1 7 0 -1 -1
    share
      0 0 7 0 0 0
    mappingName
      flattenedTorus
  exit
# **** TEMP: Make the torus right-handed
#   reparameterize
#     reorient domain coordinates
#       0 2 1 
#     mappingName
#       flattenedTorusReoriented
#   exit
#
* Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle,$rotationAxis,$xShift,$yShift,$zShift)=@_; \
  $cmds = "nurbs \n" . \
   "interpolate from mapping with options\n" . \
   " $old \n" . \
   " parameterize by index (uniform)\n" . \
   " number of ghost points to include\n $numGhost\n" . \
   " choose degree\n" . \
   "  3 \n" . \
   " # number of points to interpolate\n" . \
   " #  11 21 5 \n" . \
   "done\n" . \
   "rotate \n" . \
   " $angle $rotationAxis \n" . \
   " 0. 0. 0.\n" . \
   "shift\n" . \
   " $xShift $yShift $zShift\n" . \
   "mappingName\n" . \
   " $new\n" . \
   "exit"; \
}
#
$angle=0.; $rotationAxis=2; $xShift=0.; $yShift=0.; $zShift=0.; 
# convertToNurbs(flattenedTorusReoriented,flattenedTorusNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
convertToNurbs(flattenedTorus,flattenedTorusNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
#
#  Background
#
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nz = intmg( ($zb-$za)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    0 0 0 0 0 0 
  mappingName
    backGround
  exit
exit
#
# Make the overlapping grid
#
generate an overlapping grid
  backGround
  flattenedTorusNurbs
  # flattenedTorus
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
  # open graphics
  compute overlap
**  display computed geometry
  exit
*
* save an overlapping grid
save a grid (compressed)
$name
flattenedTorusGrid
exit
