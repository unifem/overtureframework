# TEST for explicit hole cutting 
#
# usage: ogen [noplot] cylInBoxRefinedGrid -factor=<num> -order=[2/4/6/8]
#
# Examples:
#  ogen -noplot cylInBoxRefinedGrid 
#
# -- save current values of parameters so this script can be called by CG scripts
$orderCylinder=$order; $orderOfAccuracyCylinder=$orderOfAccuracy; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$suffix = ".order$order"; 
$name = "cylInBoxRefinedGrid" . "$factor" . $suffix . ".hdf";
#
$innerRad=.5; $outerRad=1.; $za=-.5; $zb=.5; 
$bc="-1 -1 1 2 3 4"; 
#
#
$ds = .1/$factor;
#
create mappings
#
#  Define the cylinder 
#
  cylinder
    lines
     $nTheta=int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5);
     $nr = int( ($outerRad-$innerRad)/$ds + 1.5 );
     $nz = int(($zb-$za)/$ds + 1.5);
     $nTheta $nz $nr 
    bounds on the radial variable
     $innerRad $outerRad=1.
    bounds on the axial variable
      $za $zb
    boundary conditions
      -1 -1 5 6 7 0 
    share
       0  0 5 6 0 0 
    mappingName
     cylinder
    exit
#
# Here is the back ground grid 
#
Box
  set corners
    $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
    $xa $xb $ya $yb $za $zb
  lines
    $dsc=$ds*2.; # coarsen 
    $nx = int( ($xb-$xa)/$dsc +1.5);
    $ny = int( ($yb-$ya)/$dsc +1.5);
    $nz = int( ($zb-$za)/$dsc +1.5);
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    0 0 0 0 5 6
  mappingName
    backGround
  exit
#
# Here is the refinement grid 
#
Box
  set corners
    $xa=-1.5; $xb=1.5; $ya=-1.5; $yb=1.5; 
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    0 0 0 0 5 6 
  share
    0 0 0 0 5 6
  mappingName
    refinement
  exit
#
# Define an explicit hole cutter 
#
  cylinder
    lines
     $outerRad=$innerRad-$ds*.1; 
     $innerRad=0.;
     $nTheta=int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5);
     $nr = int( ($outerRad-$innerRad)/$ds + 1.5 );
     $nz = int(($zb-$za)/$ds + 1.5);
     $nTheta $nz $nr 
    bounds on the radial variable
     $innerRad $outerRad
    bounds on the axial variable
      $za $zb
    boundary conditions
      -1 -1 1 1 1 1
    mappingName
     cylinderHoleCutter 
    exit
#
exit
#
  generate an overlapping grid
    backGround
    refinement
    cylinder
  done
  change parameters
    ghost points
      all
      $ng $ng $ng $ng $ng $ng
    order of accuracy
     $orderOfAccuracy  
    create explicit hole cutter
      Hole cutter:cylinderHoleCutter
      name: cylinderHoleCutter
    exit
  exit
# open graphics
  compute overlap
# 
  exit
#
save an overlapping grid
$name
cylinder
exit
