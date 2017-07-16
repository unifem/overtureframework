#
#  plotStuff plotRadial.cmd -show=radialG2.show -name=radialG2 -solution=6 -vMax=.015
# 
#  plotStuff plotRadial.cmd -show=repG4scf1.show -name=radialG4 -solution=10 -vMax=.25
#
$show="radialG2.hdf"; $vMax=""; $solution=1; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show,"name=s"=>\$name,"vMax=f"=>\$vMax,"solution=i"=>\$solution  );
#
$show
#
solution: $solution
frame series:fluidDomain
* 
derived types
  speed
exit
contour
  plot:speed
  vertical scale factor 0.
  plot contour lines (toggle)
  if( $vMax ne "" ){ $cmd="min max 0 $vMax"; }else{ $cmd="#"; }
  $cmd
  exit
frame series:solidDomain
* 
derived types
  speed
 specify velocity components
  0 1 2
exit
contour
  adjust grid for displacement 1
  * plot:vorz
  plot:speed
  vertical scale factor 0.
  if( $vMax ne "" ){ $cmd="min max 0 $vMax"; }else{ $cmd="#"; }
  $cmd
  plot contour lines (toggle)
exit
#
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
#
$plotName = $name . "SLt0p5.ps"; 
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
