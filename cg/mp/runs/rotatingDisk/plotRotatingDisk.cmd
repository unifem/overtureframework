#
# Plot results from rotatingDisk.cmd
#   plotStuff plotRotatingDisk -show=rotatingDisk4.show
#   plotStuff plotRotatingDisk -show=rotatingDisk4_scf10.show
#   plotStuff plotRotatingDisk -show=rotatingDisk8long.show
# 
#   plotStuff plotRotatingDisk -show=rotatingDisk8Narrow.show
#
# -- revisit : June 16, 2017: 
#   plotStuff plotRotatingDisk -show=rotatingDisk4a.show
#
$show="rotatingDisk4.show"; $name="rotatingDisk4"; 
$sc="stressNorm"; $fc="p"; # component names
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution );
#
$show
#
# -- set displacement components for Godunov 
frame series:innerDomain
derived types
specify displacement components
  6 7 8
stressNorm
exit
# 
# previous
plot:stressNorm
contour
  adjust grid for displacement 1
#  plot contour lines (toggle)
  vertical scale factor 0.
  # stress-Norm: 
  # min max 0 .54
  exit
frame series:outerDomain
plot:p
contour
#  plot contour lines (toggle)
  vertical scale factor 0.
  # p: 
  # min max .5 1.0001 
 exit
# 


DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
set view:0 -0.00598171 -0.0691154 0 1.10155 1 0 0 0 1 0 0 0 1
#
solution: 6
# ---------print min and max --------------------
frame series:innerDomain
contour
  print solution info
exit
frame series:outerDomain
contour
  print solution info
exit
# -----------------------------
pause
hardcopy file name:0 rotatingDisk4_sNorm_p_t0p5.ps
hardcopy save:0
pause
#
solution: 11
# ---------print min and max --------------------
frame series:innerDomain
contour
  print solution info
exit
frame series:outerDomain
contour
  print solution info
exit
# -----------------------------
hardcopy file name:0 rotatingDisk4_sNorm_p_t1p0.ps
hardcopy save:0
pause
#
# -- now plot horizontal velocity 
frame series:innerDomain
plot:v1
frame series:outerDomain
plot:u
#
solution: 6
hardcopy file name:0 rotatingDisk4_v1_u_t0p5.ps
hardcopy save:0
pause
#
solution: 11
hardcopy file name:0 rotatingDisk4_v1_u_t1p0.ps
hardcopy save:0


frame series:solidDomain
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
bigger:0
y-:0
y-:0
hardcopy file name:0 dd16_p_sNorm.ps
hardcopy save:0




# 
pause
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
set view:0 0.0054308 -0.0528746 0 1.22673 1 0 0 0 1 0 0 0 1

plot:v2
hardcopy file name:0 ss64nsv2rt1p0.ps

$plotName = $name . "v1rt0p5.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0