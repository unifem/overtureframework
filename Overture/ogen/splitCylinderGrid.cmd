# Grid for a 3D cylinder consisting of an inner and outer cylinder
#
# Usage: ogen [-noplot] splitCylinderGrid -factor=<num> -interp=[i|e] -order=[2/4/6/8] -periodic=[np|p] -axial=[x|y|z]
#  -periodic=p : make periodic in axial direction
#  -axial : x,y or z indicates axial axis (default is "z")
#
# Examples:
#  ogen -noplot splitCylinderGrid -factor=1
#  ogen -noplot splitCylinderGrid -factor=2
#  ogen -noplot splitCylinderGrid -factor=4
#  ogen -noplot splitCylinderGrid -factor=8
#  ogen -noplot splitCylinderGrid -factor=16
#
#  ogen -noplot splitCylinderGrid -order=4 -factor=1
#  ogen -noplot splitCylinderGrid -order=4 -factor=2
#  ogen -noplot splitCylinderGrid -order=4 -factor=4
#  ogen -noplot splitCylinderGrid -order=4 -factor=8
#  ogen -noplot splitCylinderGrid -order=4 -factor=16
#
# periodic:
#  ogen -noplot splitCylinderGrid -order=2 -periodic=p -factor=1
# 
#  ogen -noplot splitCylinderGrid -order=4 -periodic=p -factor=2
#  ogen -noplot splitCylinderGrid -order=4 -periodic=p -factor=4
#  
# periodic along x-axis
#  ogen -noplot splitCylinderGrid -order=4 -periodic=p -axial=x -factor=2
#  ogen -noplot splitCylinderGrid -order=4 -periodic=p -axial=x -factor=4
#
# periodic along y-axis
#  ogen -noplot splitCylinderGrid -order=4 -periodic=p -axial=y -factor=2
#  ogen -noplot splitCylinderGrid -order=4 -periodic=p -axial=y -factor=4
#
# -- save current values of parameters so this script can be called by CG scripts
$axial="z"; # axial axis
$orderSplitCylinderGrid=$order; $orderOfAccuracySplitCylinderGrid=$orderOfAccuracy; 
$order=2; $factor=1; $interp="e"; # default values
$orderOfAccuracy = "second order"; $ng=2; $periodic="np"; 
$innerRad=.5; $outerRad=1.; 
$za=-.5; $zb=.5; 
# 
# get command line arguments
Getopt::Long::Configure("prefix_pattern=(--grid_|--|-)");
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"periodic=s"=> \$periodic,"axial=s"=>\$axial);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
#
if( $interp eq "i" ){ $interpType = "implicit for all grids"; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $periodic eq "p" ){ $suffix .= "p"; }
$prefix = "splitCylinderGrid"; 
if( $axial ne "z" ){ $prefix .= $axial; }
$name = $prefix . "$interp$factor" . $suffix . ".hdf";
#
#
# $grid = "splitCylinderGrid.hdf"; $bc="-1 -1 -1 -1 1 1"; 
# $factor=1; $name = "splitCylinderGrid$factor.hdf"; 
# $factor=2; $name = "splitCylinderGrid$factor.hdf"; 
# $factor=4; $name = "splitCylinderGrid$factor.hdf"; 
#
$ds = .1/$factor;
#
$midRad = .5*( $innerRad+$outerRad);
# 
create mappings
#
#  --- inner cylinder ---
#
  cylinder
    orientation
    #  orientation: 0,1,2 : axial direction is 2, splitCylinderGrid is along the z-axis
    #               1,2,0 : axial direction is 0, splitCylinderGrid is along the x-axis.
    #               2,0,1 : axial direction is 1, splitCylinderGrid is along the y-axis.
    if( $axial eq "z" ){ $cmd="0,1,2"; }elsif( $axial eq "x" ){ $cmd="1 2 0"; }else{ $cmd="2 0 1"; }  
    $cmd
    #
    $ra=$innerRad=.5; $rb = $midRad + $ds*($order-2);
    lines
     $nTheta=int( 2.*3.1415*($ra+$rb)*.5/$ds + 1.5);
     $nr = int( ($rb-$ra)/$ds + 1.5 );
     $nz = int(($zb-$za)/$ds + 1.5);
     $nTheta $nz $nr 
    bounds on the radial variable
     $ra $rb
    bounds on the axial variable
      $za $zb
    boundary conditions
      $bc="-1 -1 1 2 3 0"; 
      if( $periodic eq "p" ){ $bc="-1 -1 -1 -1 3 0"; }
      $bc
    share 
      0 0 1 2 0 0 
    mappingName
     innerCylinder
    exit
#
#  --- outer cylinder ---
#
  cylinder
    orientation
    #  orientation: 0,1,2 : axial direction is 2, splitCylinderGrid is along the z-axis
    #               1,2,0 : axial direction is 0, splitCylinderGrid is along the x-axis.
    #               2,0,1 : axial direction is 1, splitCylinderGrid is along the y-axis.
    if( $axial eq "z" ){ $cmd="0,1,2"; }elsif( $axial eq "x" ){ $cmd="1 2 0"; }else{ $cmd="2 0 1"; }  
    $cmd
    #
    $ra=$midRad - $ds*($order-2); $rb = $outerRad;
    lines
     $nTheta=int( 2.*3.1415*($ra+$rb)*.5/$ds + 1.5);
     $nr = int( ($rb-$ra)/$ds + 1.5 );
     $nz = int(($zb-$za)/$ds + 1.5);
     $nTheta $nz $nr 
    bounds on the radial variable
     $ra $rb
    bounds on the axial variable
      $za $zb
    boundary conditions
      $bc="-1 -1 1 2 0 4"; 
      if( $periodic eq "p" ){ $bc="-1 -1 -1 -1 0 4"; }
      $bc
    share 
      0 0 1 2 0 0 
    mappingName
     outerCylinder
    exit
exit
#
  generate an overlapping grid
    innerCylinder     
    outerCylinder     
  change parameters
    interpolation type
      $interpType
    ghost points
      all
      if( $periodic eq "p" ){ $ngp = $ng+1; }else{ $ngp=$ng; } #extra ghost for singular problems 
      $ng $ng $ng $ng $ng $ngp
    order of accuracy
     $orderOfAccuracy  
  exit
  compute overlap
# 
  exit
#
save an overlapping grid
$name
splitCylinderGrid
$order=$orderSplitCylinderGrid; $orderOfAccuracy=$orderOfAccuracySplitCylinderGrid;  # reset 
exit
