*
* Sphere in a Tube for the Visco-plastic solver
*
* usage: ogen [noplot] vpSphereInATube -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nrExtra=<>
*
*  nrExtra: extra lines to add in the radial direction on the sphere grids 
* 
* examples:
*     ogen noplot vpSphereInATube -factor=1 
*     ogen noplot vpSphereInATube -factor=2
*     ogen noplot vpSphereInATube -factor=4 
* 
$xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; $nrExtra=2; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$cylinderRadius=1.5; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
* 
$suffix = ".order$order"; 
$name = "vpSphereInATube" . "$interp$factor" . $suffix . ".hdf";
* 
$ds=.1/$factor;
$pi=4.*atan2(1.,1.);
* 
$bcWall=1; $bcInflow=2; $bcOutflow=3; 
*
create mappings
* 
  sphere
    $rad=.5; $deltaRad=5*$ds; 
    inner radius
      $rad
    outer radius
      $outerRad=$rad+$deltaRad;
      $outerRad
    lines
      $nPhi = int( $pi*$rad/$ds+1.5 );
      $nTheta = int( 2.*$pi*$rad/$ds+1.5 );
      $nr = int( $deltaRad/$ds + 1.5 );
      $nPhi $nTheta $nr
* 
    share
      0 0 0 0 1 0
    mappingName
      sphere-unrotated
    exit
* 
  rotate/scale/shift
    rotate
      -90 1
      0 0 0
    mappingName
      sphere-rotated
* pause
    exit
* 
  reparameterize
    transform which mapping?
      sphere-rotated
    orthographic
      choose north or south pole
      1
      specify sa,sb
        $sa=.6; $sb=$sa; 
        $sa $sb
      exit
     lines 
      $nx = int( .4*$pi*$rad/$ds+1.5 ); $ny=$nx; 
      $nx $ny $nr
    mappingName
    northPole-unstretched
    exit
* 
  reparameterize
    transform which mapping?
      sphere-rotated
    orthographic
      choose north or south pole
        -1
      specify sa,sb
        $sa $sb
      exit
     lines 
      $nx $ny $nr
    mappingName
    southPole-unstretched
    share
      0 0 0 0 1 0
    exit
*
  reparameterize
    transform which mapping?
      sphere-rotated
    restrict parameter space
      set corners
        .15 .85   0. 1.  0. 1.
      exit
    mappingName
      sphere-unstretched
    boundary conditions
      0 0 -1 -1  1 0 
    share
      0 0 0 0  1 0 
    exit
* 
* Stretch coordinates
stretch coordinates
  transform which mapping?
    sphere-unstretched
  stretch
    specify stretching along axis=2
      layers
      1
      1. 7. 0.
      exit
    exit
  mappingName
    sphere
  exit
* 
* Stretch coordinates
stretch coordinates
  transform which mapping?
    northPole-unstretched
  stretch
    specify stretching along axis=2
      layers
      1
      1. 7. 0.
      exit
    exit
  mappingName
    northPole
  exit
* 
* Stretch coordinates
stretch coordinates
  transform which mapping?
    southPole-unstretched
  stretch
    specify stretching along axis=2
      layers
      1
      1. 7. 0.
      exit
    exit
  mappingName
    southPole
  exit
* 
*
  Cylinder
    mappingName
      cylinder
    * make the cylinder axis is parallel to the x-axis
    orientation
     1 2 0 
    bounds on the radial variable
      $deltaRad=4*$ds; 
      $outerRad=$cylinderRadius; $innerRad=$outerRad-$deltaRad; 
      $innerRad $outerRad
    bounds on the axial variable
      $xa $xb
    lines
      $nt = int( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
      $nr = int( $deltaRad/$ds + 2.5 );
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $nt $nx $nr 
    boundary conditions
     -1 -1 2 3 0 1 
    share
      0 0 2 3 0 0
  exit
  Box
    mappingName
      core
    set corners
      $ya=-($innerRad+$ds); $yb=-$ya; $za=$ya; $zb=$yb; 
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
*
exit 
* 
generate an overlapping grid
  core
  cylinder 
  sphere
  northPole
  southPole
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
exit
* save an overlapping grid
save a grid (compressed)
$name
sit
exit




