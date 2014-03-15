*
* "Square in a circle" (taking arguments)
*
*
* usage: ogen [noplot] sicArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -fixedRadius=<>
* 
* examples:
*     ogen -noplot sicArg -factor=1 -order=2
*     ogen -noplot sicArg -order=2 -interp=e -factor=2    ( creates sice2.order2.hdf)
*     ogen -noplot sicArg -order=2 -interp=e -factor=3    ( creates sice3.order2.hdf)
*     ogen -noplot sicArg -order=2 -interp=e -factor=4    ( creates sice4.order2.hdf)
*     ogen -noplot sicArg -order=2 -interp=e -factor=5    ( creates sice5.order2.hdf)
*
*     ogen -noplot sicArg -order=4 -factor=1 
*     ogen -noplot sicArg -order=4 -factor=2 
*     ogen -noplot sicArg -order=4 -interp=e -factor=2
*     ogen -noplot sicArg -order=4 -interp=e -factor=4
*     ogen -noplot sicArg -order=4 -interp=e -factor=8
* -- fixed radius for annulus:
*     ogen -noplot sicArg -order=2 -fixedRadius=.25 -interp=e -factor=2
*     ogen -noplot sicArg -order=2 -fixedRadius=.25 -interp=e -factor=4
*     ogen -noplot sicArg -order=2 -fixedRadius=.25 -interp=e -factor=8
*     ogen -noplot sicArg -order=2 -fixedRadius=.25 -interp=e -factor=16
*     ogen -noplot sicArg -order=2 -fixedRadius=.25 -interp=e -factor=32
# Extra ghost (for sosup)
#     ogen -noplot sicArg -order=4 -numGhost=3 -interp=e -factor=2
#     ogen -noplot sicArg -order=4 -numGhost=3 -interp=e -factor=4
#   -- order=6:
#     ogen -noplot sicArg -order=6 -numGhost=4 -interp=e -factor=2
#     ogen -noplot sicArg -order=6 -numGhost=4 -interp=e -factor=4
*
$order=2; $factor=1; $interp="i"; $fixedRadius=-1; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$numGhost=-1;  # if this value is set, then use this number of ghost points
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=>\$factor,"interp=s"=>\$interp,\
            "fixedRadius=f"=>\$fixedRadius,"numGhost=i"=> \$numGhost);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
$prefix = "sic"; 
if( $fixedRadius ne -1 ){ $prefix .= "Fixed"; }
$name = $prefix . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.1/$factor;
* 
* Here is the radius of the circular boundary:
$outerRad=1.;
* 
create mappings
*
Annulus
  * keep the number of radial points on the annulus fixed:
  # $nr = 9+$ng;
  $nr = 5+$ng;
  $innerRad= $outerRad - ($nr-1)*$ds;
  if( $fixedRadius ne -1 ){ $innerRad=$outerRad-$fixedRadius; $nr=int( $fixedRadius/$ds+1.5 ); }
  inner and outer radii
    $innerRad $outerRad
  lines
    $nTheta = int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 0 1 
  mappingName
   Annulus
exit
*
rectangle
  set corners
    $xb =  $innerRad+($ng-1)*$ds;
    $xa = -$xb;
    $xa $xb $xa $xb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); $ny=$nx; 
    $nx $ny
  boundary conditions
    0 0 0 0
  mappingName
    square
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
  exit
*
save an overlapping grid
$name
sic
exit
