*
* Sphere in a Box (taking arguments) ***USING DataPointMappings ***
*
* usage: ogen [noplot] sibArg -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot sibArg -factor=1 -order=2
*     ogen noplot sibArg -factor=1 -order=4
*     ogen noplot sibArg -factor=2 -order=4
* 
*     ogen noplot sibArg -factor=1 -order=2 -interp=e
*     ogen noplot sibArg -factor=2 -order=2 -interp=e
*     ogen noplot sibArg -factor=4 -order=2 -interp=e
*     ogen noplot sibArg -factor=8 -order=2 -interp=e
*     ogen noplot sibArg -factor=16 -order=2 -interp=e
*
$xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
* 
$suffix = ".order$order"; 
$name = "sib" . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.2/$factor;
* 
create mappings
* first make a sphere
Sphere
  $nr=3+$order;
  $innerRad=.5; $outerRad=$innerRad+$nr*$ds;
  inner and outer radii
    $innerRad $outerRad
exit
*
* now make a mapping for the north pole
*
reparameterize
  orthographic
    * sa=2 --> patches just match (not including ghost points)
    $sa = 2. + $order*$dse*$ds + ($order-2)*$ds*.5; $sb=$sa; 
    specify sa,sb
      $sa $sb
  exit
  lines
    $nTheta=int( 3.2*($innerRad+$outerRad)*.5/$ds +1.5 );    
    $nTheta $nTheta $nr
*    15 15 5
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 1 0
  mappingName
    north-pole
exit
*
* now make a mapping for the south pole
*
reparameterize
  orthographic
    choose north or south pole
      -1
    specify sa,sb
      $sa $sb
  exit
  lines
    $nTheta $nTheta $nr
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 1 0
  mappingName
    south-pole
exit
*
* Here is the box
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
  dataPointMapping
    build from a mapping
     north-pole
   mappingName
    north-pole-dp
  exit
* 
  dataPointMapping
    build from a mapping
     south-pole
   mappingName
    south-pole-dp
  exit
exit
*
generate an overlapping grid
  box
  north-pole-dp
  south-pole-dp
  done
  change parameters
    * improve quality of interpolation
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
  compute overlap
exit
* save an overlapping grid
save a grid (compressed)
$name
sib
exit
