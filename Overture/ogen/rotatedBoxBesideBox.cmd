#
# Rotated box beside a box
#
# usage: ogen [noplot] rotatedBoxBesideBox -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nrExtra=<>
#
#  nrExtra: extra lines to add in the radial direction on the sphere grids 
# 
# examples:
#     ogen noplot rotatedBoxBesideBox -factor=1 -order=2
#     ogen noplot rotatedBoxBesideBox -factor=1 -order=4
# 
#     ogen noplot rotatedBoxBesideBox -order=2 -interp=e -factor=1 
#     ogen noplot rotatedBoxBesideBox -order=2 -interp=e -factor=2 
#
$nrExtra=2; $loadBalance=0;
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$suffix = ".order$order"; 
$name = "rotatedBoxBesideBox" . "$interp$factor" . $suffix . ".hdf";
# 
$ds=.2/$factor;
# 
# ---------------------------------------
$loadBalanceCmd = $loadBalance ? "load balance" : "*";
$loadBalanceCmd
# ---------------------------------------
#
create mappings
  Box
    $xa=-1.; $xb=$ds; $ya=-1.; $yb=1.; $za=-1.; $zb=1.; 
    set corners
      $xa $xb $ya $yb $za $zb
    lines
      $nx = int( ($xb-$xa)/$ds +1.5 ); 
      $ny = int( ($yb-$ya)/$ds +1.5 ); 
      $nz = int( ($zb-$za)/$ds +1.5 ); 
      $nx $ny $nz
    boundary conditions
      1 0 1 1 1 1 
    share
      0 0 1 2 3 4 
    mappingName
      unitBox
  exit
# 
  rotate/scale/shift
    rotate
    33. 1
    .5 .5 .5
    exit
  rotate/scale/shift
    rotate
    27 0
    .5 .5 .5
    mappingName 
      left-box
    exit
# 
  Box
    $xa=-1.5*$ds; $xb=1.; $ya=-1.; $yb=1.; $za=-1.; $zb=1.; 
    set corners
      $xa $xb $ya $yb $za $zb
    lines
      $nx = int( ($xb-$xa)/$ds +1.5 ); 
      $ny = int( ($yb-$ya)/$ds +1.5 ); 
      $nz = int( ($zb-$za)/$ds +1.5 ); 
      $nx $ny $nz
    boundary conditions
      0 1 1 1 1 1 
    share
      0 0 1 2 3 4 
    mappingName
      unitRightBox
  exit
# 
  rotate/scale/shift
    rotate
    33. 1
    .5 .5 .5
    exit
  rotate/scale/shift
    rotate
    27 0
    .5 .5 .5
    mappingName 
      right-box
    exit
exit
#
generate an overlapping grid
  left-box
  right-box
  done
  change parameters
 # improve quality of interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
 compute overlap
exit
# save an overlapping grid
save a grid (compressed)
$name
rotatedBoxBesideBox
exit
