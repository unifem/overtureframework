*
* Rectangle (taking arguments)
*
*
* usage: ogen [noplot] rectangleArg -factor=<num> -order=[2/4/6/8] -xa= -xb= -ya= -yb= -name=
* 
* examples:
*     ogen noplot rectangleArg -factor=1 -order=2 -xa=-2. -xb=2. -ya=-2. -yb=2. -name="rect4x4y1.hdf"
*     ogen noplot rectangleArg -factor=2 -order=2 -xa=-2. -xb=2. -ya=-2. -yb=2. -name="rect4x4y2.hdf"
*     ogen noplot rectangleArg -factor=2 -order=2 -xa=-10. -xb=10. -ya=-10. -yb=10. -name="rect20x20y2.hdf"
* 
*     ogen noplot rectangleArg -factor=4 -order=2 -xa=0. -xb=2. -ya=0. -yb=1. -name="rect2x1y4.hdf"
*  -- square with bottom at y=0 for axisymetric problems:
*     ogen noplot rectangleArg -factor=2 -order=2 -xa=-0. -xb=1. -ya=0. -yb=1. -name="axiSquare2.order2.hdf"
*     ogen noplot rectangleArg -factor=2 -order=2 -xa=-0. -xb=1. -ya=.2 -yb=1.2 -name="axiSquare2a.order2.hdf"
*     ogen noplot rectangleArg -factor=4 -order=2 -xa=-0. -xb=1. -ya=.2 -yb=1.2 -name="axiSquare4a.order2.hdf"
*     ogen noplot rectangleArg -factor=8 -order=2 -xa=-0. -xb=1. -ya=.2 -yb=1.2 -name="axiSquare8a.order2.hdf"
*
$xa=-1.; $xb=1. $ya=-1.; $yb=1.;
$order=2; $factor=1; # default values
$orderOfAccuracy = "second order"; $ng=2; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,"name=s"=>\$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
* 
$ds=.1/$factor;
* 
create mappings
*
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 );
    $ny = int( ($yb-$ya)/$ds +1.5 );
    $nx $ny
  boundary conditions
    1 2 3 4 
  mappingName
    rectangle
exit
*
exit
generate an overlapping grid
    rectangle
  done
  change parameters
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
*  display intermediate results
  compute overlap
* 
  display computed geometry
  exit
*
save an overlapping grid
$name
rectangle
exit

