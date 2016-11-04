#
#  plotStuff plotShockCylGrids.cmd
#
# $show="shockCyl.show";
$show="shockCyl2.show";
# $show="shockCyl4.show";
# $show="shockCyl8.show";
# $show="shockCyl16.show";
#
$show
#
DISPLAY COLOUR BAR:0 0
DISPLAY LABELS:0 0
DISPLAY SQUARES:0 0
set view:0 0.0154482 -0.0143123 0 2.07134 1 0 0 0 1 0 0 0 1
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
line width scale factor:0 4
# 
frame series:outerDomain
grid
  colour grid lines from chosen name
  grid colour 0 BLUERED
  grid colour 1 GREEN
exit
# 
frame series:innerDomain
displacement
  colour grid lines from chosen name
  grid colour 0 RED
  grid colour 1 ORCHID
exit
# 
hardcopy file name:0 shockCylGrid0p0.ps
hardcopy save:0
pause
previous
hardcopy file name:0 shockCylGrid0p8.ps
hardcopy save:0
pause
# 
frame series:outerDomain
apply commands to all frame series 0
erase
contour
wire frame (toggle)
  exit
hardcopy file name:0 shockRhoDisp0p8.ps
hardcopy save:0


contour
  plot:rho
  vertical scale factor 0.
  plot contour lines (toggle)
#  min max 1. 3.
  exit
frame series:innerDomain
# 
 derived types
  speed
 specify velocity components
  4 5 6
exit
contour
  adjust grid for displacement 1
 # plot:vorz
  plot:speed
  vertical scale factor 0.
  plot contour lines (toggle)
exit
