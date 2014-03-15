* 
*  Spherical shell grid
*
*   ogen noplot sphericalShellGrid -interp=e -factor=2
* 
*   ogen noplot sphericalShellGrid -interp=e -factor=3
*   ogen noplot sphericalShellGrid -interp=e -factor=4
* 
* 
$interpType="implicit for all grids";
$innerRad=.9;  # inner radius of the sphere
$outerRad=1.; 
$order=2; $orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$factor=1.;   
*
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "sphericalShellGrid" . "$interp$factor" . $suffix . ".hdf";}
* 
* 
# $ds=1./50./$factor;
$ds=1./10./$factor;  #target grid spacing
$pi=4.*atan2(1.,1.);
*
create mappings 
  * 
  sphere
    inner radius
      $innerRad
    outer radius
      $outerRad
    $rad=.5*( $innerRad + $outerRad );
    $deltaRad = $outerRad - $innerRad;
    lines
      $nPhi = int( $pi*$rad/$ds+1.5 );
      $nTheta = int( 2.*$pi*$rad/$ds+1.5 );
      $radialFactor=1.5; 
      $nr = int( $radialFactor*$deltaRad/$ds + 1.5 );
      $nPhi $nTheta $nr
* 
    boundary condition
      0 0 -1 -1 1 2 
    share
      0 0 0 0 1 2
    mappingName
      sphereStart
    exit
* 
*  rotate/scale/shift
*    rotate
*      -90 1
*      0 0 0
*    mappingName
*      sphere-rotated
* pause
*    exit
* 
  reparameterize
    transform which mapping?
      sphereStart
    orthographic
      choose north or south pole
      1
      specify sa,sb
        $sa=.6; $sb=$sa; 
        $sa $sb
      exit
    lines 
      $ns= int( 2.*$pi*$rad/5./$ds + 1.5 );
      $ns $ns $nr
    boundary conditions
      0 0 0 0 1 2 
    share
      0 0 0 0 1 2
    mappingName
      northPole
    exit
* 
  reparameterize
    transform which mapping?
      sphereStart
    orthographic
      choose north or south pole
        -1
      specify sa,sb
        $sa $sb
      exit
    lines 
      $ns $ns $nr 
    mappingName
    southPole
    boundary conditions
      0 0 0 0 1 2 
    share
      0 0 0 0 1 2
    exit
*
*
  reparameterize
    transform which mapping?
      sphereStart
    restrict parameter space
      set corners
       # .15 .85   0. 1.  0. 1.
        .125 .875   0. 1.  0. 1.
      exit
    mappingName
      sphere
    boundary conditions
      0 0 -1 -1  1 2 
    share
      0 0 0 0 1 2 
    exit
*
exit this menu
*
  generate an overlapping grid
    northPole
    southPole
    sphere
    done
    change parameters
     order of accuracy 
      $orderOfAccuracy
     ghost points
        all
        2 2 2 2 2 2
    interpolation type
      $interpType
    exit
* 
* 
    compute overlap
    exit
*
* save an overlapping grid
save a grid (compressed)
  $name
  sphericalShell
exit

