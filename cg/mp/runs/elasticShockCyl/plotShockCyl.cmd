*
*  plotStuff plotShockCyl.cmd
*
* $show="shockCyl.show";
$show="shockCyl2.show";
* $show="shockCyl4.show";
* $show="shockCyl8.show";
* $show="shockCyl16.show";
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


*    --- for the movie ---
DISPLAY COLOUR BAR:0 0
DISPLAY LABELS:0 0
DISPLAY AXES:0 0
set view:0 0.0060423 -0.00302113 0 1.27799 1 0 0 0 1 0 0 0 1
*
$res = 1204; 
$res=800; 
hardcopy vertical resolution:0 $res
hardcopy horizontal resolution:0 $res
save movie files 1
movie file name: shockCyl
