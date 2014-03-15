#
#  Grid for the region inside a sphere (using 3 patches) 
#
# usage: ogen [noplot] sphere -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nrExtra=<> -rgd=[fixed|var]
#
#  nrExtra: extra lines to add in the radial direction on the sphere grids 
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
# 
# examples:
#     ogen noplot sphere -order=2 -factor=1 
#     ogen noplot sphere -order=2 -factor=2
#     ogen noplot sphere -order=2 -factor=4
#
#     ogen noplot sphere -order=4 -factor=1 
#     ogen noplot sphere -order=4 -factor=2 
#     ogen noplot sphere -order=4 -factor=4 
#     ogen noplot sphere -order=4 -factor=8 -nrMin=11 
#     ogen noplot sphere -order=4 -factor=16
# 
#     ogen noplot sphere -order=2 -interp=e -factor=1 
#     ogen noplot sphere -order=2 -interp=e -factor=2 
#     ogen noplot sphere -order=2 -interp=e -factor=4 
#     ogen noplot sphere -order=2 -interp=e -factor=8 
#     ogen noplot sphere -order=2 -interp=e -factor=16   ( 17M pts)
#     ogen noplot sphere -order=2 -interp=e -factor=20 
# 
#     -- fixed radial distance: 
#     ogen noplot sphere -order=2 -interp=e -rgd=fixed -factor=1 
#     ogen noplot sphere -order=2 -interp=e -rgd=fixed -factor=2
#     ogen noplot sphere -order=2 -interp=e -rgd=fixed -factor=4
#     ogen noplot sphere -order=2 -interp=e -rgd=fixed -factor=8
# 
#     ogen noplot sphere -order=2 -interp=e -factor=4 -nrMin=15 -name="spheree4nrMin15.order2.hdf"
# 
#     ogen noplot sphere -order=4 -interp=e -factor=2 
#     ogen noplot sphere -order=4 -interp=e -factor=4 
#     ogen noplot sphere -order=4 -interp=e -factor=8
#     ogen noplot sphere -order=4 -interp=e -factor=16
# 
#     ogen noplot sphere -order=4 -interp=e -factor=2 -nrMin=11 -name="spheree2nrMin11.order4.hdf"
#     ogen noplot sphere -order=4 -interp=e -factor=4 -nrMin=15 -name="spheree4nrMin15.order4.hdf"
#     ogen noplot sphere -order=4 -interp=e -factor=8 -nrMin=15 -name="spheree8nrMin15.order4.hdf"
#     ogen noplot sphere -order=4 -interp=e -factor=8 -nrMin=11 -name="spheree8nrMin11.order4.hdf"
# 
$xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; $nrMin=3; $nrExtra=0; $rgd="var"; $name=""; $ml=0; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$deltaRadius0=.25; # do not make larger than .3 or troubles with cgmx
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=>\$nrExtra,"nrMin=i"=>\$nrMin,"ml=i"=>\$ml,\
            "interp=s"=> \$interp,"rgd=s"=> \$rgd,"deltaRadius0=f"=>\$deltaRadius0,"name=s"=>\$name );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$prefix="sphere";
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; $sphereWidth=$deltaRadius0; }else{ $sphereWidth=-1.; }
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
$ds=.1/$factor;
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
# Here is the radial width of the spherical grids -- this will be fixed if rgd=fixed
# matching interface grids should be given distinct share values for now for cgmx -- fix me -- cgmp is different
# ---------------------------------------
# turn off graphics
# ---------------------------------------
$dw = $order+1;
$iw = $dw;
$parallelGhost=($dw+1)/2;
if( $interp eq "e" ){ $parallelGhost=($dw+$iw-2)/2; }
minimum number of distributed ghost lines
  $parallelGhost
#
create mappings
$pi=4.*atan2(1.,1.);
# number of points to use in the radial direction : $nrExtra is used for stretching 
$nr=$nrMin + $order; if( $interp eq "e" ){ $nr=$nr+$order+$nrExtra; } 
#
$gridNames="*"; 
# 
#
#   ******** Sphere ********
$sphereRadius=1.; $radiusDir=1; 
$xSphere=0.; $ySphere=0.; $zSphere=0.; 
$sphereBC=1; 
$sphereName="sphere"; 
$northPoleName="northPole";
$southPoleName="southPole"; 
$sphereShare=100;   # reset this so the inner sphere has the same corresponding share values
$radiusDir=-1; $phiStart=.2; $phiEnd=1. - $phiStart;
# 
include $ENV{Overture}/sampleGrids/sphereThreePatch.h
#
# Here is the inner box
#
Box
  set corners
 # $xa = -( $innerRad + ($order-2)*$ds + $order*$dse );
    $xa = -( $innerRad + ($order-2 + $dse*1.5 )*$ds );
    $xb=-$xa;  $ya=$xa; $yb=$xb; $za=$xa; $zb=$xb; 
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    0 0 0 0 0 0
  mappingName
    box
  exit
#**********************************
exit
#
generate an overlapping grid
  box
  sphere
  southPole
  northPole
  done
# 
  change parameters
# 
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#
  compute overlap
#
exit
# save an overlapping grid
save a grid (compressed)
$name
sphere
exit
