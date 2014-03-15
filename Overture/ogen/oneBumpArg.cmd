*
* One half cylinder in a channel (taking arguments)
*
*
* usage: ogen [noplot] oneBumpArg -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot oneBumpArg -factor=1 -order=2
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
$name = "oneBump" . "$factor$interp" . $suffix . ".hdf";
* 
$ds=.1/$factor;
*
*
create mappings
*
rectangle
  set corners
    $xa=-2.5; $xb=2.5; $ya=0.; $yb=2.5;
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
    $innerRad=1; $outerRad = $innerRad + 6.*$ds;
    $nr = int( ($outerRad-$innerRad)/$ds + 1.5 );
    $nTheta = int( 3.1415*$innerRad/$ds + 1.5 );
    $nTheta $nr
  inner and outer radii
    $innerRad $outerRad
  start and end angles
    0. .5
  centre for annulus
    0. $ya
  boundary conditions
    1 1 1 0
exit
*
exit
generate an overlapping grid
    channel
    bottomAnnulus
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
oneBump
exit

