*
* Rotated box in a box
*
*
* usage: ogen [noplot] rbib -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot rbib -factor=1 -order=2 -interp=i
*     ogen noplot rbib -factor=2 -order=2 -interp=e
*     ogen noplot rbib -factor=2 -order=4 -interp=e
*     ogen noplot rbib -factor=4 -order=4 -interp=e
*     ogen noplot rbib -factor=8 -order=4 -interp=e
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
$name = "rbib" . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.1/$factor;
* 
create mappings
  Box
    specify corners
      -1. -1. -1. 1. 1. 1.
    lines
      $nx = int( 2./$ds + 1.5 );
      $nx $nx $nx
    mappingName
      outer-box
  exit
  Box
    specify corners
      $xa=-.4; $xb=.4; $ya=-.4; $yb=.4; $za=-.4; $zb=.4; 
      $xa $xb $ya $yb $za $zb
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nz = int( ($zb-$za)/$ds + 1.5 );
      $nx $ny $nz
    mappingName
      inner-box
    boundary conditions
      0 0 0 0 0 0
  exit
*  rotate 45 about x-axis, followed by 45 about y-axis, followed by 45 about z axis
  rotate/scale/shift
    transform
      inner-box
    rotate
      45 0
      0. 0. 0.
    rotate
      45 1 
      0. 0. 0.
    rotate
      45 2
      0. 0. 0.
    mappingName
    inner-rotated-box
  exit
exit
*
generate an overlapping grid
    outer-box
    inner-rotated-box
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
*
save a grid (compressed)
$name
rbib
exit
