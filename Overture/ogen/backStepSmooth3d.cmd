*
* Grid for a 3D backward facing step with a rounded corner
*
* usage: ogen [noplot] backStepSmooth3d -factor=<num> -order=[2/4/6/8] -interp=[e/i]
*
* examples:
*     ogen noplot backStepSmooth3d -order=2 -interp=e -factor=1 
*     ogen noplot backStepSmooth3d -order=2 -interp=e -factor=2
*
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
$name = "backStepSmooth3d" . "$interp$factor" . $suffix . ".hdf";
* 
* -- target grid spacing: 
$ds=.1/$factor;
*
create mappings
*
* Here is the main channel
*
Box
  * Note: $xa : shift to the left for explicit interpolation since we need more overlap
  $xa=-$ds*$dse*2.; $xb=5.; $ya=0.; $yb=3.; $za=0.; $zb=1.; 
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
    boundary conditions
      0 3 1 1 4 4 
    share
      0 0 3 0 4 4 
    mappingName
      mainChannel
  exit
*
*  Inlet grid 
*
Box
  $xa=-2.; $xb=0.; $ya=1.; $yb=3.; 
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
    boundary conditions
      2 0 1 1 4 4 
    share
      2 0 1 0 4 4 
    mappingName
      inlet
    exit
* 
* -- smoothed corner : 2D cross-section
  smoothedPolygon
    vertices
    3
    -2. 1.
    0 1
    0 0
    $nDist=$ds*5;  
    n-dist
    fixed normal distance
      $nDist
*
    lines
      $length=3.;
      $ns = int( $length/$ds + 1.5 );
      $nr = int( $nDist/$ds+2.5 );
      $ns $nr 
    sharpness
      20.
      20.
      20.
*
    n-stretch
     1. 3. 0 
* 
    t-stretch
    0 50.
    .15 15.
    0 50
*
    correct corners
*
    boundary conditions
      2 1 1 0
    share
      2 3 1 0
    mappingName
      corner2d
* pause
    exit
*
*  Turn the corner into a 3d grid 
*
  sweep
    extrude
      $za $zb
    choose reference mapping
      corner2d
    lines
      $ns $nr $nz
    boundary conditions
      2 1 1 0 4 4
    share
      2 3 1 0 4 4 
    mappingName
     corner
* pause
   exit
  exit this menu
*
  generate an overlapping grid
    mainChannel
    inlet
    corner
    done
    change parameters
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
  * display intermediate results
  compute overlap
  * pause
  exit
*
save an overlapping grid
$name
backStepSmooth3d
exit

