*
* 3D cylindrical tube 
*
*
* usage: ogen [noplot] tubeArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -name= -xa= -xb= -outerRad=
* 
*  factor : grid resolution factor 
* 
* examples:
*     ogen noplot tubeArg -factor=2 -order=2 -interp=e ( creates tubee2.order2.hdf)
*     ogen noplot tubeArg -factor=4 -order=2 -interp=e ( creates tubee4.order2.hdf)
*     ogen noplot tubeArg -factor=4 -order=2 -interp=e -xa=-1. -xb=1. 
* 
* -- set default parameter values:
$outerRad=.5; 
$xa=-.5; $xb=.5;
$order=2; $factor=1; $interp="i"; $name="";
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
* 
* get command line arguments
GetOptions("order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"outerRad=f"=> \$outerRad,"xa=f"=> \$xa,"xb=f"=> \$xb,"name=s"=>\$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){ $name = "tube" . "$interp$factor" . $suffix . ".hdf"; }
* 
$ds=.05/$factor;
*
*
* Make a cylinder in a box
*
create mappings
*
  Cylinder
    mappingName
      cylinder
    bounds on the radial variable
      * cylinder is a fixed number of lines in the radial direction: 
      $nr=6; 
      $deltaRad=($nr-1)*$ds; 
      $innerRad = $outerRad - $deltaRad; 
      $innerRad $outerRad
    bounds on the axial variable
      $xa $xb
    orientation
     * make the x-axis the axial direction
      1 2 0 
    lines
      $nt = int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5 );
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $nt $nx $nr 
    boundary conditions
     * theta axial radial
     -1 -1   2 3   0 1 
    share
      0 0 2 3 0 0 
  exit
* 
  Box
    mappingName
      box
  set corners
  lines
    $ya=-$innerRad-($ng-1)*$ds; $yb=-$ya; $za=$ya; $zb=$yb;
    set corners
      $xa $xb $ya $yb $za $zb
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nz = int( ($zb-$za)/$ds + 1.5 );
      $nx $ny $nz
    boundary conditions
      2 3 0 0 0 0 
    share
      2 3 0 0 0 0 
  exit
exit
*
*
generate an overlapping grid
    box
    cylinder
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
*  display intermediate results
* pause
* 
  compute overlap
  exit
*
save an overlapping grid
$name
tube
exit


