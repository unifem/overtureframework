* 
* Quarter annulus in a channel (taking arguments)
*
* usage: ogen [noplot] qcicArg -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot qcicArg -factor=1 -order=2
*     ogen noplot qcicArg -factor=1 -order=4
*     ogen noplot qcicArg -factor=2 -order=4
*     ogen noplot qcicArg -factor=2 -order=2 -interp=e    ( creates qcice2.order2.hdf)
*     ogen noplot qcicArg -factor=3 -order=2 -interp=e    ( creates qcice3.order2.hdf)
*     ogen noplot qcicArg -factor=4 -order=2 -interp=e    ( creates qcice4.order2.hdf)
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $interp eq "i" ){ $interpType = "implicit for all grids"; }
* 
$suffix = ".order$order"; 
$name = "qcic" . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.1/$factor;
* 
create mappings
*
rectangle
  set corners
    -2. 0. 0. 2.
  lines
    $nx = int( 2./$ds +1.5 ); $ny=$nx; 
    $nx $ny
  boundary conditions
    1 1 1 1
  share
    0 2 3 4 
  mappingName
  square
exit
*
Annulus
  $nr = 5+$ng;
  $innerRad=.5; $outerRad = $innerRad + ($nr-1)*$ds;
  inner and outer radii
    $innerRad $outerRad
  start and end angles
    .25 .5 
  lines
    $nTheta = int( .5*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
     2 3  1 0
  share 
     2  3 0 0 
exit
*
exit
generate an overlapping grid
    square
    Annulus
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
  display computed geometry
  exit
*
save an overlapping grid
$name
qcic
exit










create mappings
  rectangle
    set corners
    0 1 0 1
    mapping parameters
    mappingName channel
    lines 101 101
    Share Value: bottom  1
    close mapping dialog
    exit
  annulus
    centre for annulus
    0 0
    inner and outer radii
    .3 .35
    start and end angles
    0 .25
    lines
    56 6
    boundary conditions
    1 1 1 0
    share
    1 1 0 0
    mappingName
    cylinder
    exit
  exit this menu
generate an overlapping grid
  channel
  cylinder
  done
  change parameters
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
*   pause
exit
*
save an overlapping grid
  qcic.hdf
  qcic
exit

