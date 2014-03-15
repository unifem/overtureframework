* ---------------------------------------------------------------------------------
* half cylinder in a channel for axisymmetric computations of flow past a sphere 
* Usage:
*     ogen [noplot] halfCylinder -name=<name> -factor=<> -xa=<> -xb=<> -ya=<> -yb=<> 
*
* Examples:
*    ogen noplot halfCylinder -name="halfCylinder"
*    ogen noplot halfCylinder -name="halfCylinder2" -factor=2
*    ogen noplot halfCylinder -name="halfCylinder4" -factor=4
*    ogen noplot halfCylinder -name="shortHalfCylinder" -xa=-2.5 -xb=2.5 -factor=.5 -stretch=2.
*    ogen noplot halfCylinder -name="shortHalfCylinder1" -xa=-2.5 -xb=2.5 -factor=1. -stretch=2.
*    ogen noplot halfCylinder -name="shortHalfCylinder2" -xa=-2.5 -xb=2.5 -factor=2. -stretch=2.
* ---------------------------------------------------------------------------------
*
* -- assign default values for parameters --
$name="halfCylinder"; 
$xa=-2.5; $xb=7.5; $ya=0.; $yb=2.5; $factor=1; $stretch=8.; 
$bca = 13;   # BC value for the axis of symmetry (y=0)
*
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "stretch=f"=> \$stretch,"interp=s"=> \$interp,"name=s"=> \$name);
* 
* $name = "halfCylinder.hdf"; $xa=-2.5; $xb=7.5; $ya=0.; $yb=2.5; $factor=1; $stretch=8.; 
* $name = "shortHalfCylinder.hdf"; $xa=-2.5; $xb=2.5; $ya=0.; $yb=2.5; $factor=.5; $stretch=2.; 
* $name = "shortHalfCylinder1.hdf"; $xa=-2.5; $xb=2.5; $ya=0.; $yb=2.5; $factor=1; $stretch=2.; 
* $name = "shortHalfCylinder2.hdf"; $xa=-2.5; $xb=2.5; $ya=0.; $yb=2.5; $factor=2; $stretch=2.; 
* $name = "shortHalfCylinder4.hdf"; $xa=-2.5; $xb=2.5; $ya=0.; $yb=2.5; $factor=4; $stretch=2.; 
* 
$ds = .1/$factor;
$pi = 4.*atan2(1.,1.);
create mappings
  rectangle
    set corners
     $xa $xb $ya $yb 
     * -2.5  7.5  0. 2.5
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nx $ny 
      * 101 26
    boundary conditions
      1 1 $bca 1
    share
     0 0 1 3
    mappingName
      square
    exit
*
  Annulus
    start and end angles
      0 .5
    $nr=11; 
    $innerRad=.5; $outerRad=$innerRad + ($nr-5)*$ds; 
    inner radius
      $innerRad
    outer radius
      $outerRad
    boundary conditions
      $bca $bca 1 0
    share
      1 1 0 0
    lines
      $nTheta = int( $pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
      $nTheta $nr 
      * 43 17
    exit
*
* stretch the annulus
*
  stretch coordinates
    transform which mapping?
    Annulus
    stretch
      specify stretching along axis=1
        layers
        1
        1. $stretch 0.
        exit
      exit
    mappingName
      annulus
    exit
  *
*
exit this menu
*
  generate an overlapping grid
    square
    annulus
    done
    change parameters
    interpolation type
      * implicit for all grids
      explicit for all grids
      ghost points
        all
        2 2 2 2 2 2
    exit
    compute overlap
    exit
*
save an overlapping grid
  $name.hdf
  halfCylinder
exit
