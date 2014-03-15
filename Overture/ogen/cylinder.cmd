#
# Usage: ogen [-noplot] cylinder -factor=<num> -order=[2/4/6/8] -periodic=[np|p] -axial=[x|y|z]
#  -periodic=p : make periodic in axial direction
#  -axial : x,y or z indicates axial axis (default is "z")
#
# Examples:
#  ogen -noplot cylinder -factor=1
#  ogen -noplot cylinder -factor=2
#  ogen -noplot cylinder -factor=4
#  ogen -noplot cylinder -factor=8
#  ogen -noplot cylinder -factor=16
#
#  ogen -noplot cylinder -order=4 -factor=1
#  ogen -noplot cylinder -order=4 -factor=2
#  ogen -noplot cylinder -order=4 -factor=4
#  ogen -noplot cylinder -order=4 -factor=8
#  ogen -noplot cylinder -order=4 -factor=16
#
# periodic:
#  ogen -noplot cylinder -order=4 -periodic=p -factor=1
#  ogen -noplot cylinder -order=4 -periodic=p -factor=2
#  
# periodic along x-axis
#  ogen -noplot cylinder -order=4 -periodic=p -axial=x -factor=1
#  ogen -noplot cylinder -order=4 -periodic=p -axial=x -factor=2
#  ogen -noplot cylinder -order=4 -periodic=p -axial=x -factor=4
#
# periodic along y-axis
#  ogen -noplot cylinder -order=4 -periodic=p -axial=y -factor=1
#  ogen -noplot cylinder -order=4 -periodic=p -axial=y -factor=2
#  ogen -noplot cylinder -order=4 -periodic=p -axial=y -factor=4
#
# -- save current values of parameters so this script can be called by CG scripts
$axial="z"; # axial axis
$orderCylinder=$order; $orderOfAccuracyCylinder=$orderOfAccuracy; 
$order=2; $factor=1; $interp="i"; # default values
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
$suffix = ".order$order"; 
if( $periodic eq "p" ){ $suffix .= "p"; }
$prefix = "cylinder"; 
if( $axial ne "z" ){ $prefix .= $axial; }
$name = $prefix . "$factor" . $suffix . ".hdf";
#
$bc="-1 -1 1 2 3 4"; 
if( $periodic eq "p" ){ $bc="-1 -1 -1 -1 3 4"; }
#
# $grid = "cylinder.hdf"; $bc="-1 -1 -1 -1 1 1"; 
# $factor=1; $name = "cylinder$factor.hdf"; 
# $factor=2; $name = "cylinder$factor.hdf"; 
# $factor=4; $name = "cylinder$factor.hdf"; 
#
$ds = .1/$factor;
#
create mappings
  cylinder
    orientation
    #  orientation: 0,1,2 : axial direction is 2, cylinder is along the z-axis
    #               1,2,0 : axial direction is 0, cylinder is along the x-axis.
    #               2,0,1 : axial direction is 1, cylinder is along the y-axis.
    if( $axial eq "z" ){ $cmd="0,1,2"; }elsif( $axial eq "x" ){ $cmd="1 2 0"; }else{ $cmd="2 0 1"; }  
    $cmd
    #
    lines
     $nTheta=int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5);
     $nr = int( ($outerRad-$innerRad)/$ds + 1.5 );
     $nz = int(($zb-$za)/$ds + 1.5);
     $nTheta $nz $nr 
    bounds on the radial variable
     $innerRad $outerRad
    bounds on the axial variable
      $za $zb
    boundary conditions
      $bc
    mappingName
     cylinder
#    periodicity
#      2 1 0
    exit
exit
#
  generate an overlapping grid
    cylinder
  change parameters
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
cylinder
$order=$orderCylinder; $orderOfAccuracy=$orderOfAccuracyCylinder;  # reset 
exit
