*
* Two co-centric annulii for a two-domain example (e.g. for thermal-hydraulics)
*
*
* usage: ogen [noplot] doubleAnnulusArg -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot doubleAnnulusArg -factor=1 -order=2
*     ogen noplot doubleAnnulusArg -factor=1 -order=4
*     ogen noplot doubleAnnulusArg -factor=2 -order=4
*     ogen noplot doubleAnnulusArg -factor=2 -order=2 -interp=e    ( creates doubleAnnuluse2.order2.hdf)
*     ogen noplot doubleAnnulusArg -factor=3 -order=2 -interp=e    ( creates doubleAnnuluse3.order2.hdf)
*     ogen noplot doubleAnnulusArg -factor=4 -order=2 -interp=e    ( creates doubleAnnuluse4.order2.hdf)
*     ogen noplot doubleAnnulusArg -factor=5 -order=2 -interp=e    ( creates doubleAnnuluse5.order2.hdf)
*     ogen noplot doubleAnnulusArg -factor=2 -order=4 -interp=e    ( creates doubleAnnuluse2.order4.hdf)
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
$name = "doubleAnnulus" . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.1/$factor;
* 
* Here is the radius of the circular boundary:
$innerRad=.5;
$middleRad=1.;
$outerRad=1.5;
* 
create mappings
*
Annulus
  * keep the number of radial points on the annulus fixed:
  inner and outer radii
    $innerRad $middleRad
  lines
    $nTheta = int( 2.*3.1415*($middleRad)/$ds + 1.5 );
    * $nTheta = int( 2.*3.1415*($innerRad+$middleRad)*.5/$ds + 1.5 );
    $nr = int( ($middleRad-$innerRad)/$ds + 2.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 1 100
  share
     0  0 0 100
  mappingName
   innerAnnulus
exit
*
Annulus
  * keep the number of radial points on the annulus fixed:
  inner and outer radii
    $middleRad $outerRad
  lines
    $nr = int( ($outerRad-$middleRad)/$ds + 2.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 100  2
  share
     0  0 100 2 
  mappingName
   outerAnnulus
exit
*
exit
generate an overlapping grid
    innerAnnulus
    outerAnnulus
  done
  change parameters
    * choose implicit or explicit interpolation
    specify a domain
      * domain name:
      outerDomain 
      * grids in the domain:
      outerAnnulus
      done
    specify a domain
      * domain name:
      innerDomain 
      * grids in the domain:
      innerAnnulus
      done
* 
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
* 
  exit
*
save an overlapping grid
$name
doubleAnnulus
exit
