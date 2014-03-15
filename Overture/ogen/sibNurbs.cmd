*
* Sphere in a Box (taking arguments)
*
* usage: ogen [noplot] sibNurbs -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nrExtra=<>
*
*  nrExtra: extra lines to add in the radial direction on the sphere grids 
* 
* examples:
*     ogen noplot sibNurbs -factor=1 -order=2
*     ogen noplot sibNurbs -factor=1 -order=4
*     ogen noplot sibNurbs -factor=2 -order=4 -interp=e -nrExtra=8    (for cgmx : add extra grid lines in the normal direction)
* 
*     ogen noplot sibNurbs -order=2 -interp=e -factor=1 
*     ogen noplot sibNurbs -order=2 -interp=e -factor=2 
*     ogen noplot sibNurbs -order=2 -interp=e -factor=4 
*     ogen noplot sibNurbs -order=2 -interp=e -factor=8 
*     ogen noplot sibNurbs -order=2 -interp=e -factor=16
*
$xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; $nrExtra=2; $loadBalance=0;
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp);
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
* ---------------------------------------
turn off graphics
$loadBalanceCmd = $loadBalance ? "load balance" : "*";
$loadBalanceCmd
* ---------------------------------------
*
create mappings
* first make a sphere
Sphere
  $nr=3+$order; if( $interp eq "e" ){ $nr=$nr+$order; } 
  $innerRad=.5; $outerRad=$innerRad+($nr-1)*$ds;
  $nr=$nr + $nrExtra; 
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
    north-pole-start
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
    south-pole-start
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
* Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
*
* -- it is faster to evaluate the Nurbs than the original cap patches --
convertToNurbs("north-pole-start","north-pole",0.);
$commands
convertToNurbs("south-pole-start","south-pole",0.);
$commands
exit
*
generate an overlapping grid
  box
  north-pole
  south-pole
  done
  change parameters
    * do not use local bounding boxes
    * improve quality of interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
 compute overlap
*   debug 
*     15
*   compute overlap
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
*   continue
* 
**  output inverse statistics
exit
* save an overlapping grid
save a grid (compressed)
$name
sib
exit
