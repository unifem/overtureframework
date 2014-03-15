# -------------------------------------------------------------------
# plotStuff plotLens.cmd
# -------------------------------------------------------------------
# $show="lens.show";
# $show="lens4.show";
$show="lens8.show";
#
$show
# 
frame series:fluidDomain
contour
  min max 1 1.2
  vertical scale factor 0
  exit
# 
frame series:solidDomain 
derived types
speed
exit
contour 
  adjust grid for displacement 1 
*   plot:div 
   plot:speed
  # min max 0 .09 
  exit 
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
bigger:0
solution 1
hardcopy file name:0 lens0p0.ps
hardcopy save:0
solution: 81
hardcopy file name:0 lens0p8.ps
hardcopy save:0
solution: 91
hardcopy file name:0 lens0p9.ps
hardcopy save:0
solution: 101
hardcopy file name:0 lens1p0.ps
hardcopy save:0
solution: 111
hardcopy file name:0 lens1p1.ps
hardcopy save:0
solution: 121
hardcopy file name:0 lens1p2.ps
hardcopy save:0
# 
solution: 131
hardcopy file name:0 lens1p3.ps
hardcopy save:0
# 
solution: 141
hardcopy file name:0 lens1p4.ps
hardcopy save:0
# 
solution: 151
hardcopy file name:0 lens1p5.ps
hardcopy save:0
# 
solution: 161
hardcopy file name:0 lens1p6.ps
hardcopy save:0
# 
solution: 171
hardcopy file name:0 lens1p7.ps
hardcopy save:0
# 
solution: 181
hardcopy file name:0 lens1p8.ps
hardcopy save:0
# 
solution: 191
hardcopy file name:0 lens1p9.ps
hardcopy save:0
# 
solution: 201
hardcopy file name:0 lens2p0.ps
hardcopy save:0


* movie: 
DISPLAY AXES:0 0
DISPLAY LABELS:0 0
DISPLAY COLOUR BAR:0 0
bigger:0


stride: 2 


frame series:solidDomain
contour
  adjust grid for displacement 1
  plot:div
  exit
stride: 2
