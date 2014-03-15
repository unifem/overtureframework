*
* Two half cylinders in a channel (taking arguments)
*
*
* usage: ogen [noplot] twoBumpArg -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot twoBumpArg -factor=1 -order=2
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
$name = "twoBump" . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.1/$factor;
*
*
create mappings
*
rectangle
  set corners
    $xa=-1.5; $xb=4.; $ya=0.; $yb=3.; 
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds + 1.5 );
    $ny = int( ($yb-$ya)/$ds + 1.5 );
    $nx $ny
  boundary conditions
    1 1 1 1
  mappingName
   channel
exit
*
Annulus
  mappingName
    bottomAnnulus
  lines
    * fix the number of lines in the radial direction: (add more for higher-order)
    $nr = 5+$ng;
    $innerRad=.5; $outerRad = $innerRad + ($nr-1)*$ds;
    $nTheta = int( 3.1415*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  inner and outer radii
    $innerRad $outerRad
  start and end angles
    0. .5
  centre for annulus
    .5 0.
  boundary conditions
    1 1 1 0
exit
Annulus
  mappingName
    topAnnulus
  lines
    $nTheta $nr
  inner and outer radii
    $innerRad $outerRad
  start and end angles
    .5 1. 
  centre for annulus
    1. 3.
  boundary conditions
    1 1 1 0
exit
*
exit
generate an overlapping grid
    channel
    bottomAnnulus
    topAnnulus
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
twoBump
exit

