#
# Two adjacent non-boxes
#
# usage: ogen [noplot] twoNonBoxes -factor=<num> -order=[2/4/6/8] -interp=[e/i]
# 
# examples:
#     ogen -noplot twoNonBoxes -order=2 -interp=i -factor=1 
#     ogen -noplot twoNonBoxes -order=2 -interp=i -factor=2 
#     ogen -noplot twoNonBoxes -order=2 -interp=i -factor=4 
# 					       				       
#     ogen -noplot twoNonBoxes -order=2 -interp=e -factor=1
#     ogen -noplot twoNonBoxes -order=2 -interp=e -factor=2 
#     ogen -noplot twoNonBoxes -order=4 -interp=e -factor=2 
#
$xa=-1.; $xb=1.; $ya=-1.; $yb=1.; $za=-.5; $zb=.5; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$prefix="twoNonBoxes";
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=3; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=5; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
$name = $prefix . "$interp$factor" . $suffix . ".hdf";
# 
$ds=.1/$factor;
$width = ($order-2)/2;
if( $interp eq "e" ){ $width=$width+1.; }
$overlap = $ds*$width + $ds*.125;
# 
create mappings
  Box
    set corners
     $xas=$xa; $xbs=$overlap; $yas=$ya; $ybs=$yb
     $xas $xbs $ya $yb $za $zb
    lines
      $nx=int( ($xbs-$xas)/$ds+1.5 );
      $ny=int( ($ybs-$yas)/$ds+1.5 );
      $nz=int( ($zb -$za )/$ds+1.5 );
      $nx $ny $nz
    boundary conditions
      1 0 3 4 5 6 
    share
      0 0 3 4 5 6 
    mappingName
      leftBox-rectangular
    exit
# 
  rotate/scale/shift
    mappingName
      leftBox
  exit
# 
  Box
    set corners
     $xas=-$overlap; $xbs=$xb; $yas=$ya; $ybs=$yb
     $xas $xbs $ya $yb $za $zb
    lines
      $ds2 = $ds*1.12345;
      $nx=int( ($xbs-$xas)/$ds2+1.5 );
      $ny=int( ($ybs-$yas)/$ds2+1.5 );
      $nz=int( ($zb -$za )/$ds+1.5 );
      $nx $ny $nz
    boundary conditions
      0 2 3 4 5 6 
    share
      0 0 3 4 5 6 
    mappingName
      rightBox-rectangular
    exit
# 
  rotate/scale/shift
    mappingName
      rightBox
  exit
#
  exit
#
generate an overlapping grid
  leftBox
  rightBox
  done
  change parameters
    ghost points
      all
       $ng $ng $ng $ng $ng $ng 
    order of accuracy
      $orderOfAccuracy
    interpolation type
      $interpType
  exit
  compute overlap
exit
save a grid (compressed)
$name
sis
exit
