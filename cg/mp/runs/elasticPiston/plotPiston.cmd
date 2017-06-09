#
#  plotStuff plotPiston.cmd -show=pistonG4.show -name=pistonG4 -solution=8 
#
$show="radialG2.hdf"; $vMax=""; $solution=1; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show,"name=s"=>\$name,"vMax=f"=>\$vMax,"solution=i"=>\$solution  );
#
$show
#
solution: $solution
frame series:fluidDomain
# 
contour
  plot:p
  vertical scale factor 0.
  plot contour lines (toggle)
  if( $vMax ne "" ){ $cmd="min max 0 $vMax"; }else{ $cmd="#"; }
  $cmd
  exit
frame series:solidDomain
#
# derived types
#   speed
#  specify velocity components
#   0 1 2
# exit
contour
  adjust grid for displacement 1
  * plot:vorz
  plot:s22
  vertical scale factor 0.
  if( $vMax ne "" ){ $cmd="min max 0 $vMax"; }else{ $cmd="#"; }
  $cmd
  plot contour lines (toggle)
exit
#
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
#
$plotName = $name . "stress0p7.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
next
$plotName = $name . "stress0p8.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0



  # bigger
  DISPLAY SQUARES:0 0
#
 colour boundaries by grid number
if( $name eq "elasticPistonGrid" ){ \
  $cmd="colour grid lines from chosen name\n grid colour 0 RED\n grid colour 1 BLUE\n grid colour 2 GREEN"; }else{ $cmd="#"; }
  $cmd
#
  line width scale factor:0 3
  plot interpolation points 1
  # colour interpolation points 1
#
  hardcopy file name:0 $name.ps
  hardcopy save:0
  hardcopy save:0
