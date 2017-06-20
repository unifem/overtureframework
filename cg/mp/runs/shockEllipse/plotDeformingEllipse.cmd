# 
# Plot results from deforming ellipse 
#   plotStuff plotDeformingEllipse -show=deformingEllipse2.show 
#   plotStuff plotDeformingEllipse -show=deformingEllipse4.show 
#   plotStuff plotDeformingEllipse -show=deformingEllipse4a.show 
#   plotStuff plotDeformingEllipse -show=deformingEllipse4b.show 
# 
#   plotStuff plotDeformingEllipse -show=deformingEllipse8.show 
#   plotStuff plotDeformingEllipse -show=deformingEllipse8a.show 
#   plotStuff plotDeformingEllipse -show=deformingEllipse8b.show 
#   plotStuff plotDeformingEllipse -show=deformingEllipse8c.show 
# 
#   plotStuff plotDeformingEllipse -show=deformingEllipse16.show 
# 
$show="deformingEllipse4.show"; $name="deformingEllipse4"; 
$sc="stressNorm"; $fc="p"; # component names 
* ----------------------------- get command line arguments --------------------------------------- 
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution ); 
# 
$show 
# 
# -- set displacement components for Godunov 
frame series:innerDomain 
derived types 
speed 
specify displacement components 
6 7 8 
stressNorm 
exit 
# 
previous 
# plot:stressNorm 
plot:speed 
contour 
  adjust grid for displacement 1 
  #  plot contour lines (toggle) 
  vertical scale factor 0. 
  coarsening factor 1 (<0 : adaptive) 
  # stress-Norm: 
  # min max 0 .54 
  exit 
frame series:outerDomain 
derived types 
schlieren 
exit 
plot:schlieren 
# plot:p 
contour 
  #  plot contour lines (toggle) 
  gray 
  vertical scale factor 0. 
  coarsening factor 1 (<0 : adaptive) 
  # schlieren: 
  plot contour lines (toggle) 
  min max .4 1 
  # p: 
  # min max .5 1.0001 
  exit 
# 
bigger 
DISPLAY COLOUR BAR:0 0 
DISPLAY AXES:0 0 
