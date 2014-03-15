#
#  Purdue's VAV room : Grid for a 3d room including "diffusers" for use with immersed boundary clouds
#
# usage: ogen [noplot] vavWithDiffusersGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> -clouds=[0|1]
# 
#
# NOTE: watch out at inlet/outlet : the background grid may retain a short section of wall where it shouldn't
#       if there is not enough overlap
#
# examples:
# 
#  ogen -noplot vavWithDiffusersGrid -interp=e -factor=1 -ml=1 
#  ogen -noplot vavWithDiffusersGrid -interp=e -factor=2 -ml=2  [
#  ogen -noplot vavWithDiffusersGrid -interp=e -factor=3 -ml=2  [3.4M
#  ogen -noplot vavWithDiffusersGrid -interp=e -factor=4 -ml=3  [8.2M
#  ogen -noplot vavWithDiffusersGrid -interp=e -factor=6 -ml=3  [
#  ogen -noplot vavWithDiffusersGrid -interp=e -factor=8 -ml=4  [64.6 M
#
# 
#
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; $xa=-1.; $xb=1.; $ya=-1.; $yb=1.;
$clouds=0; 
# 
$f2m = .3048; # feet to meters conversion (exact)
# $f2m = 1.; # work in units of feet
#
# -- Room dimensions:
# 
$xaRoom=0.; $xbRoom=32.*$f2m; $yaRoom=0.; $ybRoom=32.*$f2m; $zaRoom=0.; $zbRoom=14.5*$f2m; $dse=0.;
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"clouds=i"=>\$clouds);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=.5; }else{ $interpType = "implicit for all grids"; }
# 
$prefix ="vavWithDiffusersGrid";
if( $clouds eq 1 ){ $prefix .= "WithClouds"; }
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds0=.5*$f2m;
$ds=$ds0/$factor;
$pi=4.*atan2(1.,1.);
# 
$dw = $order+1; $iw=$order+1; 
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$Pi = 4.*atan2(1.,1.);
#
create mappings
#
$bcOutflow=12;   # outflow BC
#
#  Main room grid:
#
Box
  $xad=$xaRoom; $xbd=$xbRoom; $yad=$yaRoom; $ybd=$ybRoom; $zad=$zaRoom; $zbd=$zbRoom;
  set corners
    $xad $xbd $yad $ybd $zad $zbd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nz = intmg( ($zbd-$zad)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    1 2 3 4 5 6 
  mappingName
    roomBackGround
  exit
#
#  Outlet grid (poor man's)
#
Box
  $nyOutlet = intmg( 9 ); # outlet grid extends this far into the room
  # Note: make wider by $ds in each direction to account for poor man's 
  $xao=12.5*$f2m-$ds; $xbo=16.*$f2m+$ds; $zao=11.*$f2m-$ds; $zbo=12.5*$f2m+$ds; $yao=$ybRoom-$nyOutlet*$ds; $ybo=$ybRoom; 
  set corners
    $xao $xbo $yao $ybo $zao $zbo
  lines
    $nx = intmg( ($xbo-$xao)/$ds +1.5 ); 
    $ny = intmg( ($ybo-$yao)/$ds +1.5 ); 
    $nz = intmg( ($zbo-$zao)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    0 0 0 $bcOutflow 0 0 
  share
    0 0 0 4 0 0 
  mappingName
    outletUpperNorth
  exit
#
#
# Here is the diffuser -- use a sphere
  $xWidthd=2.*$f2m;  $yWidthd=2.*$f2m;  # width of diffusers
  $nr = intmg( 7 );
#   ******** Create a grid for a sphere with 3 patches ********
# Note -- make sphere radius larger since south pole is the diffuser
$diffuserFactor=1.2; # **check me**
$sphereRadius=$diffuserFactor*$xWidthd*.5; $radiusDir=1; 
$xSphere=0.; $ySphere=0.; $zSphere=0.; 
$sphereName="sphere"; 
$northPoleName="northPole";
$southPoleName="southPole"; 
$sphereBC=20; 
$sphereShare=20; 
$phiStart=.15; $phiEnd=1. - $phiStart;  # note phiStart=.2 was too big for explicit interp (with moving sphere)
# 
# include $ENV{Overture}/sampleGrids/sphereThreePatch.h
$sphereFactor=.7 + .4/$factor; # we decrease grid lines on sphere as mesh gets finer to overcome clustering
include $ENV{OvertureCheckout}/ogen/sphereThreePatch.h
# open graphics
#
#
# -- convert grids to Nurbs and perform rotation and shift: 
#
  $angle= 0.; $rotationAxis=1; 
  $xShift=0.; $yShift=0.; $zShift=0.; 
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
   " boundary conditions\n" . \
   "  $bcLine\n" . \
   "  share\n" . \
   "    0 0 0 0 $sphereShare 0 \n" . \
   "mappingName\n" . \
   " $new\n" . \
   "exit"; \
}
#
# -- DIFFUSERS ----
#  We model the diffusers are spheres. Inflow is created on the lower half of the spheres
#
$xa=7.75*$f2m; $xb=$xa+$xWidthd;  $ya=6.*$f2m; $yb=$ya+$xWidthd; $za=10.*$f2m; $zb=10.5*$f2m; 
## $za=$za + $sphereRadius*.3; # raise the sphere up a bit since "diffuser" is the south pole patch
$delta = .0*$f2m;  # shift diffuser away from the end of the cloud a bit 
#
#    -- locations of the diffusers (inlets)
    $ix[$n]= 7.75; $iy[$n]=6.0;   $n=$n+1;
    $ix[$n]= 7.75; $iy[$n]=12.25; $n=$n+1;
    $ix[$n]= 7.75; $iy[$n]=17.75; $n=$n+1;
    $ix[$n]= 7.75; $iy[$n]=24.0;  $n=$n+1;
#
    $ix[$n]= 22.25; $iy[$n]=6.0;   $n=$n+1;
    $ix[$n]= 22.25; $iy[$n]=12.25; $n=$n+1;
    $ix[$n]= 22.25; $iy[$n]=17.75; $n=$n+1;
    $ix[$n]= 22.25; $iy[$n]=24.0;  $n=$n+1;
#
#   -- NOTE: set a different BC on the sphere and southPole so we can easily set the bottom as inflow
    $diffuserNames = "#";
    $cmd="#"; 
    for( $m=0; $m<$n; $m++ ){ \
      $xa=$ix[$m]*$f2m; $xb=$xa+$xWidthd; $ya=$iy[$m]*$f2m; $yb=$ya+$yWidthd; \
      $xShift = .5*($xa+$xb) - $delta; $yShift = .5*($ya+$yb); $zShift=$za;\
      $sphereBC=20+$m; $sphereShare=10+$m; $mp1=$m+1; \
      $bcLine="0 0 -1 -1 $sphereBC 0"; \
      $dName = "diffuserSphere$mp1"; \
      convertToNurbs(sphere,$dName,$angle,$rotationAxis,$xShift,$yShift,$zShift);\
      $cmd .= "\n" . $cmds ; \
      $diffuserNames .= "\n" . $dName; \
      $dName = "diffuserNorth$mp1"; \
      $sphereBC=50+$m; \
      $bcLine="0 0 0 0 $sphereBC 0"; \
      convertToNurbs(northPole,$dName,$angle,$rotationAxis,$xShift,$yShift,$zShift);\
      $cmd .= "\n" . $cmds ; \
      $diffuserNames .= "\n" . $dName; \
      $sphereBC=20+$m; $dName = "diffuserSouth$mp1"; \
      $bcLine="0 0 0 0 $sphereBC 0"; \
      convertToNurbs(southPole,$dName,$angle,$rotationAxis,$xShift,$yShift,$zShift);\
      $cmd .= "\n" . $cmds ; \
      $diffuserNames .= "\n" . $dName; \
    }
    $cmd
#
# $xShift = .5*($xa+$xb) - $delta; $yShift = .5*($ya+$yb); $zShift=$za;
# convertToNurbs(northPole,diffuserNorth1,$angle,$rotationAxis,$xShift,$yShift,$zShift);
# $cmds
# convertToNurbs(southPole,diffuserSouth1,$angle,$rotationAxis,$xShift,$yShift,$zShift);
# $cmds
# #
# # -- diffuser 2
# #
# $xShift = 22.25*$f2m + .5*$xWidthd + $delta;
# convertToNurbs(northPole,diffuserNorth2,$angle,$rotationAxis,$xShift,$yShift,$zShift);
# $cmds
# convertToNurbs(southPole,diffuserSouth2,$angle,$rotationAxis,$xShift,$yShift,$zShift);
# $cmds
#
exit
# display intermediate results
#-   change the plot
#-     toggle grid 0 0
#-     x-r 90
#-     set home
#-     plot block boundaries 1
#-  open graphics
#- 
#- 
#-
#
#
# Make the overlapping grid
#
generate an overlapping grid
  roomBackGround
  outletUpperNorth
  $diffuserNames
#
  done
  change parameters
 # choose implicit or explicit interpolation
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
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
vavWithDiffusersGrid
exit

