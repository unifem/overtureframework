#
#  Ogen: Two spheres in a channel 
#
# usage: ogen [-noplot] twoSpheresInAChannel -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nrExtra=<> -ml=<> -ns=<>
#
#  nrExtra: extra lines to add in the radial direction on the sphere grids 
#  ml = number of (extra) multigrid levels to support
#  ns : number of spheres, 1 or 2 
#  xa, xb, ya, yb, za, zb : bounds on the channel
# 
# examples:
#     ogen -noplot twoSpheresInAChannel -factor=1 -order=2
#     ogen -noplot twoSpheresInAChannel -factor=1 -order=4
#     ogen -noplot twoSpheresInAChannel -factor=2 -order=4 -interp=e -nrExtra=8    (for cgmx : add extra grid lines in the normal direction)
# 
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -factor=1 
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -factor=2 
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -factor=4 -xb=3.
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -factor=8 
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -factor=16
# -- multigrid:
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -factor=1 -ml=1 -xa=-2.5 -xb=2.5 -ya=-2.5 -yb=2.5 -za=-2.5 -zb=2.5
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -factor=2 -ml=3
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -factor=2 -ml=3 -ns=1  (oneSphere)
#
#     ogen -noplot twoSpheresInAChannel -order=4 -interp=e -factor=2 -ml=2
#     ogen -noplot twoSpheresInAChannel -order=4 -interp=e -factor=2 -ml=3
#     ogen -noplot twoSpheresInAChannel -order=4 -interp=e -factor=4 -ml=3
#     ogen -noplot twoSpheresInAChannel -order=4 -interp=e -factor=4 -ml=4
#     ogen -noplot twoSpheresInAChannel -order=4 -interp=e -factor=2 -ml=3 -ns=1  (oneSphere)
#
# -- bigger domain
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -factor=4 -xa=-2.5 -xb=7.5 -ya=-2.5 -yb=2.5 -za=-2.5 -zb=2.5 
# -- one sphere
#     ogen -noplot twoSpheresInAChannel -order=2 -interp=e -ns=1 -x1=0. -y1=0. -z1=0. -factor=1 -ml=1 
#
# -- for AFS runs -- bigger domain
#  TROUBLE: -factor=2 a bit coarse for -order=4?
#   ogen -noplot twoSpheresInAChannel -order=4 -interp=e -factor=2 -ml=2 -xbc=5.
#   ogen -noplot twoSpheresInAChannel -order=4 -interp=e -factor=2 -ml=2 -xa=-2. -xb=7.5 -ya=-2. -yb=2. -za=-2. -zb=2.
#  OK: 
#   ogen -noplot twoSpheresInAChannel -order=4 -interp=e -factor=3 -ml=2 -xbc=4. -xbc=7. [OK 3M pts
#   ogen -noplot twoSpheresInAChannel -order=4 -interp=e -factor=4 -ml=3 -xbc=4. -xbc=7. [
#   srun -N2 -n16 -ppdebug $ogenp -noplot twoSpheresInAChannel -order=4 -interp=e -factor=4 -ml=3 -xbc=4. -xbc=7. -numberOfParallelGhost=4 [6.6M pts
#   srun -N2 -n16 -ppdebug $ogenp -noplot twoSpheresInAChannel -order=4 -interp=e -factor=8 -ml=4 -xbc=4. -xbc=7. -numberOfParallelGhost=4 [49M pts
#   srun -N2 -n16 -ppdebug $ogenp -noplot twoSpheresInAChannel -order=4 -interp=e -factor=10 -ml=4 -xbc=4. -xbc=7. -numberOfParallelGhost=4 [ 89M pts, 170s to compute
#   srun -N2 -n16 -ppdebug $ogenp -noplot twoSpheresInAChannel -order=4 -interp=e -factor=12 -ml=4 -xbc=4. -xbc=7. -numberOfParallelGhost=4 [ 155M pts, 262s to compute
#   srun -N2 -n16 -ppdebug $ogenp -noplot twoSpheresInAChannel -order=4 -interp=e -factor=16 -ml=5 -xbc=4. -xbc=7. -numberOfParallelGhost=4 [ 384M pts, 817s to compute
#   srun -N4 -n32 -ppdebug $ogenp -noplot twoSpheresInAChannel -order=4 -interp=e -factor=20 -ml=5 -xbc=4. -xbc=7. -numberOfParallelGhost=4 [ 707Mpts, 911s to compute
#   srun -N8 -n64 -ppdebug $ogenp -noplot twoSpheresInAChannel -order=4 -interp=e -factor=24 -ml=5 -xbc=4. -xbc=7. -numberOfParallelGhost=4 [ 1.2B pts, 972s to compute  NOTE: -N4 not enough memory
#
# Background: 
$xac=-2.5; $xbc=4.; $yac=-2.5; $ybc=2.5; $zac=-2.5; $zbc=2.5; 
#
# Refinement patch:
$refinementBox=1; # 1=add refinement box 
# old: $xa=-1.75; $xb=2.5; $ya=-1.75; $yb=1.75; $za=-1.75; $zb=1.75; 
# new longer and slightly narrower refinement
$xa=-1.55; $xb=4.0; $ya=-1.55; $yb=1.55; $za=-1.55; $zb=1.55; 
#
$nrExtra=3; $loadBalance=0; $ml=0; $ns=2; 
#
# -- Centers of the spheres:
# $x1=-.6; $y1=-.6; $z1=-.6; # center of sphere 1
# $x2=+.6; $y2=+.6; $z2=+.6; # center of sphere 2
$x1=-.5; $y1=-.5; $z1=+.5; # center of sphere 1
$x2=+.5; $y2=+.5; $z2=-.5; # center of sphere 2
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "za=f"=>\$za,"zb=f"=>\$zb,"ml=i"=>\$ml,"ns=i"=>\$ns,"x1=f"=>\$x1,"y1=f"=>\$y1,"z1=f"=>\$z1,\
            "x2=f"=>\$x2,"y2=f"=>\$y2,"z2=f"=>\$z2,"refinementBox=i"=>\$refinementBox,\
            "xac=f"=>\$xac,"xbc=f"=>\$xbc,"yac=f"=>\$yac,"ybc=f"=>\$ybc,"zac=f"=>\$zac,"zbc=f"=>\$zbc );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$prefix = "twoSpheresInAChannel"; 
if( $ns eq 1 ){ $prefix = "oneSphereInAChannel"; }
$name = $prefix . "$interp$factor" . $suffix . ".hdf";
# 
$ds=.1/$factor;
$pi=4.*atan2(1.,1.);
# 
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
#
create mappings
# 
# add extra points for stretching 
$nr=3+ $nrExtra +$order; if( $interp eq "e" ){ $nr=$nr+$order; } 
$nr = intmg( $nr );
#
$gridNames="*"; 
$numberOfSpheres=0;  $sphereShare=1; $sphereStretchb=10.; 
$sphereRadius=.5; 
$sphereBC=7; 
#
# sphere 1: 
# 
# $xSphere=-.5; $ySphere=-.4; $zSphere=-.4; 
# $xSphere=-.6; $ySphere=-.6; $zSphere=-.6; 
$xSphere=$x1; $ySphere=$y1; $zSphere=$z1; 
include sphere.h
#
# sphere 2: 
# $xSphere= .5; $ySphere= .4; $zSphere= .4; 
# $xSphere= .6; $ySphere= .6; $zSphere= .6; 
$xSphere=$x2; $ySphere=$y2; $zSphere=$z2; 
if( $ns eq 2 ){ $cmd = "include sphere.h"; }else{ $cmd="#"; }
$cmd
#include sphere.h
#
# Here is the refinement box 
#
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5);
    $ny = intmg( ($yb-$ya)/$ds +1.5);
    $nz = intmg( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    0 0 0 0 0 0 
  mappingName
    refinementPatch
  exit
#
#  Coarser Background
#
Box
  set corners
    $xac $xbc $yac $ybc $zac $zbc
  lines
    if( $refinementBox eq 1 ){ $dsc=$ds*2.; }else{ $dsc=$ds; }
    $nx = intmg( ($xbc-$xac)/$dsc +1.5 ); 
    $ny = intmg( ($ybc-$yac)/$dsc +1.5 ); 
    $nz = intmg( ($zbc-$zac)/$dsc +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    0 0 0 0 0 0 
  mappingName
    backGround
  exit
#
exit
#
generate an overlapping grid
  backGround
  if( $refinementBox eq 1 ){ $cmd="refinementPatch"; }else{ $cmd="#"; }
  $cmd
  $gridNames
#  sphere1
#  northPole1
#  southPole1
  done
  change parameters
    * improve quality of interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
# change the plot
# open graphics
 compute overlap
exit
* save an overlapping grid
save a grid (compressed)
$name
twoSpheresInAChannel
exit



