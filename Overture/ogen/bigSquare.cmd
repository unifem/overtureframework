*
* Big square
*
*
* usage: ogen [noplot] bigSquare -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot bigSquare -factor=1 
*     ogen noplot bigSquare -factor=2
*     ogen noplot bigSquare -factor=4
* 
*     ogen noplot bigSquare -factor=1 -xa=-1. -xb=1. -ya=-1. -yb=1. -name="bigSquareSize1f1.hdf"
*     ogen noplot bigSquare -factor=2 -xa=-1. -xb=1. -ya=-1. -yb=1. -name="bigSquareSize1f2.hdf"
*     ogen noplot bigSquare -factor=4 -xa=-1. -xb=1. -ya=-1. -yb=1. -name="bigSquareSize1f4.hdf"
*     ogen noplot bigSquare -factor=8 -xa=-1. -xb=1. -ya=-1. -yb=1. -name="bigSquareSize1f8.hdf"
*     ogen noplot bigSquare -factor=16 -xa=-1. -xb=1. -ya=-1. -yb=1. -name="bigSquareSize1f16.hdf"
*     ogen noplot bigSquare -factor=32 -xa=-1. -xb=1. -ya=-1. -yb=1. -name="bigSquareSize1f32.hdf"
*
# For cgmx: 
*     ogen noplot bigSquare -order=4 -factor=8 -xa=-2. -xb=2. -ya=-2. -yb=2. -name="bigSquareX2Y2f8.order4.hdf"
*
* For superseismic initial conditions:
*     ogen noplot bigSquare -xa=-1.75 -xb=1.0 -ya=-.5 -yb=1.25  -factor=8 -name="obliqueShockGridf8.hdf"
*     ogen noplot bigSquare -xa=-1.75 -xb=1.0 -ya=-.5 -yb=1.25  -factor=16 -name="obliqueShockGridf16.hdf"
* 
* For superseismic (solid runs)
*     ogen noplot bigSquare -xa=-1.75 -xb=1.0 -ya=-1.25 -yb=0. -factor=8 -name="superSeismicSolidGridf8.hdf"
*     ogen noplot bigSquare -xa=-1.75 -xb=1.0 -ya=-1.25 -yb=0. -factor=16 -name="superSeismicSolidGridf16.hdf"
*     ogen noplot bigSquare -xa=-1.75 -xb=1.0 -ya=-1.25 -yb=0. -factor=32 -name="superSeismicSolidGridf32.hdf"
* For Rayleigh waves (cg/sm/rayleigh.cmd)
*     ogen noplot bigSquare -xa=-1. -xb=1.0 -ya=-1. -yb=0. -bc="-1 -1 1 2" -factor=8 -name="rayleighGridf8.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=1.0 -ya=-1. -yb=0. -bc="-1 -1 1 2" -factor=16 -name="rayleighGridf16.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=1.0 -ya=-1. -yb=0. -bc="-1 -1 1 2" -factor=32 -name="rayleighGridf32.hdf"
* For elastic piston
*     ogen noplot bigSquare -xa=-1. -xb=.0 -ya=-.2 -yb=0. -bc="1 2 -1 -1" -factor=8 -name="pistonSolidGridf8.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=.0 -ya=-.2 -yb=0. -bc="1 2 -1 -1" -factor=16 -name="pistonSolidGridf16.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=.0 -ya=-.2 -yb=0. -bc="1 2 -1 -1"  -factor=32 -name="pistonSolidGridf32.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=.0 -ya=-.2 -yb=0. -bc="1 2 -1 -1" -factor=64 -name="pistonSolidGridf64.hdf"
*    
*     ogen noplot bigSquare -xa=-1. -xb=.0 -ya=-.2 -yb=0. -bc="1 2 3 4" -factor=8  -name="pistonSolidGridfa8.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=.0 -ya=-.2 -yb=0. -bc="1 2 3 4" -factor=16 -name="pistonSolidGridfa16.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=.0 -ya=-.2 -yb=0. -bc="1 2 3 4" -factor=32 -name="pistonSolidGridfa32.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=.0 -ya=-.2 -yb=0. -bc="1 2 3 4" -factor=64 -name="pistonSolidGridfa64.hdf"
* For corner tests (sm/cmd/corner)
*     ogen noplot bigSquare -xa=-1. -xb=1.0 -ya=-1. -yb=0. -bc="1 2 3 4" -factor=8 -name="cornerGridf8.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=1.0 -ya=-1. -yb=0. -bc="1 2 3 4" -factor=16 -name="cornerGridf16.hdf"
*     ogen noplot bigSquare -xa=-1. -xb=1.0 -ya=-1. -yb=0. -bc="1 2 3 4" -factor=32 -name="cornerGridf32.hdf"
*    
* For probe bounding box restart:
*   ogen noplot bigSquare -xa=2.5 -xb=7.5 -ya=-2.5 -yb=2.5 -bc="1 2 3 4" -factor=2 -name="probeSquaref2.hdf"
*   ogen noplot bigSquare -xa=2.5 -xb=7.5 -ya=-2.5 -yb=2.5 -bc="1 2 3 4" -factor=4 -name="probeSquaref4.hdf"
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "explicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
$bc = "1 2 3 4"; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "bc=s"=> \$bc,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "bigSquare" . "$factor" . $suffix . ".hdf";}
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
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    $bc
  mappingName
  square
exit
*
exit
generate an overlapping grid
    square
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
bigSquare
exit
