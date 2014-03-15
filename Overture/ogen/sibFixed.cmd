*
* Sphere in a Box (taking arguments) === this version keeps the grid bounds fixed ****
*
* usage: ogen [noplot] sibFixed -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot sibFixed -factor=1 -order=2
*     ogen noplot sibFixed -factor=1 -order=4
*     ogen noplot sibFixed -factor=2 -order=4
* 
*  -- smaller outer box: 
*     ogen noplot sibFixed -order=2 -xa=-1.2 -xb=1.2 -ya=-1.2 -yb=1.2 -za=-1.2 -zb=1.2 -factor=1 
*     ogen noplot sibFixed -order=2 -xa=-1.2 -xb=1.2 -ya=-1.2 -yb=1.2 -za=-1.2 -zb=1.2 -factor=2 
*     ogen noplot sibFixed -order=2 -xa=-1.2 -xb=1.2 -ya=-1.2 -yb=1.2 -za=-1.2 -zb=1.2 -factor=4 
*     ogen noplot sibFixed -order=2 -xa=-1.2 -xb=1.2 -ya=-1.2 -yb=1.2 -za=-1.2 -zb=1.2 -factor=8 
*     ogen noplot sibFixed -order=2 -xa=-1.2 -xb=1.2 -ya=-1.2 -yb=1.2 -za=-1.2 -zb=1.2 -factor=16
* 
*     ogen noplot sibFixed -order=2 -xa=-1.2 -xb=1.2 -ya=-1.2 -yb=1.2 -za=-1.2 -zb=1.2 -name="sibFixedSmall" -interp=e -factor=1 
* 
*     ogen noplot sibFixed -order=2 -interp=e -factor=1 
*     ogen noplot sibFixed -order=2 -interp=e -factor=2 
*     ogen noplot sibFixed -order=2 -interp=e -factor=4 
*     ogen noplot sibFixed -order=2 -interp=e -factor=8 
*     ogen noplot sibFixed -order=2 -interp=e -factor=16
* 
*     ogen noplot sibFixed -order=4 -interp=i -factor=2 
*     ogen noplot sibFixed -order=4 -interp=i -factor=4 
*     ogen noplot sibFixed -order=4 -interp=i -factor=8 
*     ogen noplot sibFixed -order=4 -interp=i -factor=16
*
$xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; 
$order=2; $factor=1; $interp="i"; $name="sibFixed"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$deltaRad=.4; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"name=s"=> \$name,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,"deltaRad=f"=>\$deltaRad);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
* 
$suffix = ".order$order"; 
$name = $name . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.2/$factor;
* 
* ---------------------------------------
* turn off graphics
* ---------------------------------------
*
create mappings
* first make a sphere
Sphere
  $innerRad=.5; $outerRad=$innerRad+$deltaRad;
  $nr = int( ($outerRad-$innerRad)/$ds +1.5 );
  inner and outer radii
    $innerRad $outerRad
exit
*
* now make a mapping for the north pole
*
reparameterize
  orthographic
    $sa = 2.1; $sb=$sa; 
    specify sa,sb
      $sa $sb
  exit
  lines
    $nTheta=int( 3.2*($innerRad+$outerRad)*.5/$ds +1.5 );    
    $nTheta $nTheta $nr
*    15 15 5
  boundary conditions
    0 0 0 0 7 0
  share
    0 0 0 0 7 0
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
    0 0 0 0 7 0
  share
    0 0 0 0 7 0
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
  boundary conditions
   1 2 3 4 5 6
  mappingName
    box
  exit
exit
*
generate an overlapping grid
  box
  north-pole
  south-pole
  done
  change parameters
    * improve quality of interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
* 
  compute overlap
* 
exit
* save an overlapping grid
save a grid (compressed)
$name
sibFixed
exit
