*
* Big box
*
*
* usage: ogen [noplot] bigBox -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot bigBox -factor=1 
*     ogen noplot bigBox -factor=2
*     ogen noplot bigBox -factor=4
*     ogen noplot bigBox -factor=8
*     ogen noplot bigBox -factor=16
*     ogen noplot bigBox -factor=32
* 
*     ogen noplot bigBox -factor=1 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -name="bigBoxSize1f1.hdf"
*     ogen noplot bigBox -factor=2 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -name="bigBoxSize1f2.hdf"
*     ogen noplot bigBox -factor=4 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -name="bigBoxSize1f4.hdf"
*     ogen noplot bigBox -factor=8 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -name="bigBoxSize1f8.hdf"
*     ogen noplot bigBox -factor=16 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -name="bigBoxSize1f16.hdf"
*     ogen noplot bigBox -factor=32 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -name="bigBoxSize1f32.hdf"
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "explicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "za=f"=> \$za,"zb=f"=> \$zb,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "bigBox" . "$factor" . $suffix . ".hdf";}
* 
$ds=.1/$factor;
* 
$dw = $order+1; $iw=$order+1; 
* parallel ghost lines: for ogen we need at least:
*       .5*( iw -1 )   : implicit interpolation 
*       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
create mappings
*
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  mappingName
    box
  exit
*
exit
generate an overlapping grid
    box
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
*  display intermediate results
  compute overlap
**  display computed geometry
  exit
*
* save an overlapping grid
save a grid (compressed)
$name
bigBox
exit




*
* box in a box
*
create mappings
  Box
    specify corners
      -1. -1. -1. 1. 1. 1.
    lines
      21 21 21 41 41 41 11 11 11
    * periodicity
    *  0 0 1
    mappingName
      box
  exit
exit
*
generate an overlapping grid
  box
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  pause
  compute overlap
exit
save an overlapping grid
bigBox.hdf
bigBox
exit

