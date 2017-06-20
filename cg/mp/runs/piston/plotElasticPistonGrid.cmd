#
# plotStuff plotElasticPistonGrid.cmd
#
$show="elasticPiston2.show";
#
$show
frame series:rightDomain
grid
  colour boundaries by grid number
  plot interpolation points 1
  exit this menu
frame series:leftDomain
 derived types
 specify displacement components
   6 7 8
exit
displacement
  colour boundaries by grid number
  colour grid lines from chosen name
  grid colour 0 RED
exit this menu
* 
  DISPLAY AXES:0 0
  DISPLAY LABELS:0 0
  DISPLAY SQUARES:0 0
  line width scale factor:0 4
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
set view:0 0.239422 0.00397724 0 0.942199 1 0 0 0 1 0 0 0 1
*
solution: 1
  hardcopy file name:0 elasticPistonGrid0p0.ps
  hardcopy save:0
pause
*
solution: 11
  hardcopy file name:0 elasticPistonGrid1p0.ps
  hardcopy save:0
