* -- plot figures for paper ---
*
*  plotStuff plotShockCylFigs.cmd
*
* $show="shockCyl.show";
* $show="shockCyl4.show";
* $show="shockCyl8.show";
$show="shockCyl16.show";
*
$show
*
frame series:outerDomain
* 
contour
  plot:rho
  vertical scale factor 0.
  plot contour lines (toggle)
*  min max 1. 3.
  exit
frame series:innerDomain
* 
 derived types
  speed
 specify velocity components
  4 5 6
exit
contour
  adjust grid for displacement 1
  * plot:vorz
  plot:speed
  vertical scale factor 0.
  plot contour lines (toggle)
exit
DISPLAY LABELS:0 0
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
bigger:0
*
solution: 31
hardcopy file name:0 shockCylRhoSpeed0p3.ps
hardcopy save:0
*
solution: 51
hardcopy file name:0 shockCylRhoSpeed0p5.ps
hardcopy save:0
*
solution: 71
hardcopy file name:0 shockCylRhoSpeed0p7.ps
hardcopy save:0
*
solution: 101
hardcopy file name:0 shockCylRhoSpeed1p0.ps
hardcopy save:0
*
solution: 121
hardcopy file name:0 shockCylRhoSpeed1p2.ps
hardcopy save:0
*
solution: 151
hardcopy file name:0 shockCylRhoSpeed1p5.ps
hardcopy save:0
*
solution: 201
hardcopy file name:0 shockCylRhoSpeed2p0.ps
hardcopy save:0
*
solution: 251
hardcopy file name:0 shockCylRhoSpeed2p5.ps
hardcopy save:0
*
exit


