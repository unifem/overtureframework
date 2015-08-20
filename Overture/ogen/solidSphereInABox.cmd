#
#  Solid sphere in a box (two-domain problem, sphere covered by 3 patches)
#
# usage: ogen [noplot] solidSphereInABox -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nrExtra=<> -rgd=[fixed|var]
#
#  nrExtra: extra lines to add in the radial direction on the sphere grids 
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
# 
# examples:
#     ogen noplot solidSphereInABox -order=2 -factor=1 
# --implicit:
#     ogen noplot solidSphereInABox -order=4 -factor=1 
#     ogen noplot solidSphereInABox -order=4 -factor=2 
#     ogen noplot solidSphereInABox -order=4 -factor=4 
#     ogen noplot solidSphereInABox -order=4 -factor=8 -nrMin=11 
#     ogen noplot solidSphereInABox -order=4 -factor=16
# 
#     ogen noplot solidSphereInABox -order=2 -interp=e -factor=1 
#     ogen noplot solidSphereInABox -order=2 -interp=e -factor=2 
#     ogen noplot solidSphereInABox -order=2 -interp=e -factor=4 
#     ogen noplot solidSphereInABox -order=2 -interp=e -factor=8 
# 
#     ogen noplot solidSphereInABox -order=2 -interp=e -factor=4 -nrMin=15 -name="solidSphereInABoxe4nrMin15.order2.hdf"
# 
#     ogen noplot solidSphereInABox -order=4 -interp=e -factor=2 
#     ogen noplot solidSphereInABox -order=4 -interp=e -factor=4 
#     ogen noplot solidSphereInABox -order=4 -interp=e -factor=8
#     ogen noplot solidSphereInABox -order=4 -interp=e -factor=16
# 
#     ogen noplot solidSphereInABox -order=4 -interp=e -factor=2 -nrMin=11 -name="solidSphereInABoxe2nrMin11.order4.hdf"
#     ogen noplot solidSphereInABox -order=4 -interp=e -factor=4 -nrMin=15 -name="solidSphereInABoxe4nrMin15.order4.hdf"
#     ogen noplot solidSphereInABox -order=4 -interp=e -factor=8 -nrMin=15 -name="solidSphereInABoxe8nrMin15.order4.hdf"
#     ogen noplot solidSphereInABox -order=4 -interp=e -factor=8 -nrMin=11 -name="solidSphereInABoxe8nrMin11.order4.hdf"
# 
$xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; $nrMin=3; $nrExtra=0; $rgd="var"; $name=""; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$deltaRadius0=.3; # do not make larger than .3 or troubles with cgmx
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=>\$nrExtra,"nrMin=i"=>\$nrMin,\
            "interp=s"=> \$interp,"rgd=s"=> \$rgd,"deltaRadius0=f"=>\$deltaRadius0,"name=s"=>\$name,"numGhost=i"=>\$numGhost );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$prefix="solidSphereInABox";
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $name eq "" ){ $name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
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
# fixed radial distance
if( $rgd eq "fixed" ){ $nr = $nr*($factor-1); }
#
$sphereName="outerSphere"; 
$northPoleName="outerNorthPole";
$southPoleName="outerSouthPole"; 
$sphereShare=100; 
$sphereRadius=1.; $radiusDir=1; 
$xSphere=0.; $ySphere=0.; $zSphere=0.; 
$gridNames="*"; 
# 
include solidSphereInABox.h
#
# Here is the box
#
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  mappingName
    outerBox
  exit
#
#   ******** inner-sphere ********
$sphereName="innerSphere"; 
$northPoleName="innerNorthPole";
$southPoleName="innerSouthPole"; 
$sphereShare=100;   # reset this so the inner sphere has the same corresponding share values
$radiusDir=-1;
# 
include solidSphereInABox.h
#
# Here is the box
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
    innerBox
  exit
#**********************************
exit
#
generate an overlapping grid
  outerBox
  outerSphere
  outerSouthPole
  outerNorthPole
# 
  innerBox
  innerSphere
  innerSouthPole
  innerNorthPole
  done
# 
  change parameters
    specify a domain
 # domain name:
      outerDomain 
 # grids in the domain:
       outerBox
       outerSphere
       outerSouthPole
       outerNorthPole
      done
    specify a domain
 # domain name:
      innerDomain 
 # grids in the domain:
       innerBox
       innerSphere
       innerSouthPole
       innerNorthPole
      done
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
solidSphereInABox
exit
