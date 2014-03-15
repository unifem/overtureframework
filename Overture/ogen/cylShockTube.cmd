*
* Cylindrical shock tube: 
*  A shock tube that bifurcates allowing the formation 
*  of a cylindrically converging shock
*
* Usage:
*     ogen noplot cylShockTube -factor=1
*     ogen noplot cylShockTube -factor=2
*
$factor=1; $ng=2; $interpType="implicit for all grids";
* 
use Getopt::Long; use Getopt::Std;
* get command line arguments
GetOptions( "factor=i"=> \$factor);
*
$name="cylShockTube$factor.hdf"; 
*
* target grid spacing:
$ds=1./$factor;
*
$pi=3.141592653;
$bcAxis=5;  # boundary condition for the axis of symmetry
*
create mappings
*
*  -- grid for shock tube on the left section
  rectangle
    mappingName 
      shockTube
    $xa=-100.; $xb=0.; $ya=0.; $yb=40.; 
    set corners
     $xa $xb $ya $yb 
    $nx=int(($xb-$xa)/$ds+1.5);
    $ny=int(($yb-$ya)/$ds+1.5);
    lines
     $nx $ny
    boundary conditions
     1 0 $bcAxis 1
    share
     0 0 1 2
    exit
*
* --- grid for the left end of the conical insert
*  The tip of the cone is rounded off but gets sharper as the mesh is refined
  $tipRoundness =5./$factor; 
  smoothedPolygon
    mappingName 
       coneRamp
    vertices
    4
    0. 0. 
    0. $tipRoundness
    170. 70.
    180. 70.
    n-dist
      $ny=5;
      $nDist= ($ny-2)*$ds; 
      fixed normal distance
      $nDist
    correct corners
    n-stretch
      1. 1.5 0.
    $bStretch=5.;  # stretching factor in tangential direction 
    t-stretch
      .05 $bStretch
      .10 $bStretch
      .10 $bStretch
      .05 $bStretch
    $sharpness=80.+10.*$factor; # sharpness factor for corners
    sharpness
      $sharpness
      $sharpness
      $sharpness
      $sharpness
    lines
      $len = 5.+10.+sqrt( 170*170 + 70*70 );
      $nx = int($len/$ds+1.5); 
      $nx $ny
    boundary conditions
      $bcAxis 0 1 0
    share
      1 0 3 0
    exit
*
* --- grid for the outer boundary of the expanding shock tube
  smoothedPolygon
    mappingName 
      outerRamp
    vertices
    4
    -10. 40
      0. 40.
    170. 80.
    180. 80.
    n-dist
      $ny=5;
      fixed normal distance
      -$nDist
    correct corners
    n-stretch
      1. 1.5 0.
    t-stretch
      .05 $bStretch
      .10 $bStretch
      .10 $bStretch
      .05 $bStretch
    sharpness
      $sharpness
      $sharpness
      $sharpness
      $sharpness
    lines
      $len = sqrt( 190*190 + 40*40 );
      $nx = int($len/$ds+1.5); 
      $nx $ny
    boundary conditions
      0 0 1 0
    share
      0 0 2 0
    exit
*
* -- back-ground grid for the expansion region
  rectangle
    mappingName 
      expansion
    $xa=0.; $xb=180.; $ya=0.; $yb=80.; 
    set corners
     $xa $xb $ya $yb 
    $nx=int(($xb-$xa)/$ds+1.5);
    $ny=int(($yb-$ya)/$ds+1.5);
    lines
     $nx $ny
    boundary conditions
      0 0 0 0 
    share
     0 0 0 0 
    exit
*
* -- grid for the upper channel
  rectangle
    mappingName 
      upperChannel
    $xa=170.; $xb=525.; $ya=70.; $yb=80.; 
    set corners
     $xa $xb $ya $yb 
    $nx=int(($xb-$xa)/$ds+1.5);
    $ny=int(($yb-$ya)/$ds+1.5);
    lines
     $nx $ny
    boundary conditions
      0 0 1 1
    share
     0 0 3 2 
    exit
*
* --- grid for the inner corner to the test section
  smoothedPolygon
    mappingName 
      innerCorner
    vertices
      3
     515. 70.
     525. 70.
     525  60.
    n-dist
      $ny=5;
      fixed normal distance
      $nDist
    correct corners
    n-stretch
      1. 1.5 0.
    $bStretchc=15.;  # stretching factor in tangential direction 
    t-stretch
      .05 $bStretchc
      .10 $bStretchc
      .05 $bStretchc
    $sharpnessc=10.+5*$factor; # sharpness factor for corners
    sharpness
      $sharpnessc
      $sharpnessc
      $sharpnessc
    lines
      $len = 20.; 
      $nx = int($len/$ds+1.5); 
      $nx $ny
    boundary conditions
      0 0 1 0 
    share
      0 0 3 0
    exit
*
* -- corner grid in upper right
  rectangle
    mappingName 
      cornerToTestSection
    $xa=525.; $xb=530.; $ya=70.; $yb=80.; 
    set corners
     $xa $xb $ya $yb 
    $nx=int(($xb-$xa)/$ds+1.5);
    $ny=int(($yb-$ya)/$ds+1.5);
    lines
     $nx $ny
    boundary conditions
      0 1 0 1 
    share
      0 4 0 2 
    exit
*
* -- test section on the right
  rectangle
    mappingName 
      testSection
    $xa=525.; $xb=530.; $ya=0.; $yb=70.; 
    set corners
     $xa $xb $ya $yb 
    $nx=int(($xb-$xa)/$ds+1.5);
    $ny=int(($yb-$ya)/$ds+1.5);
    lines
     $nx $ny
    boundary conditions
      1 1 $bcAxis 0
    share
      3 4 0 0 
    exit
*
  exit this menu
* 
generate an overlapping grid
  shockTube
  upperChannel
  expansion
  testSection
  cornerToTestSection
  coneRamp
  outerRamp
  innerCorner
  done
*
  change parameters
    * improve quality of interpolation
    interpolation type
      $interpType
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
cylShockTube
exit
