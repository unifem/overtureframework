#
#  plotStuff plotPiston.cmd -show=piston4bd1.show -matlab=piston4bd1
#  plotStuff plotPiston.cmd -show=piston4bd0p1.show -matlab=piston4bd0p1
#
#  plotStuff plotPiston.cmd -show=piston4bd0p1i.show -matlab=piston4bd0p1i  [ iterative solvers 
#  plotStuff plotPiston.cmd -show=piston4bd0p1i.show -matlab=piston4bd0p1ipv  [ iterative solvers, project velocity
#
#  plotStuff plotPiston.cmd -show=piston4bd0p01.show -matlab=piston4bd0p01
#  plotStuff plotPiston.cmd -show=piston4bd0p001.show -matlab=piston4bd0p001
#
#  -- New scaling for added-damping
#   plotStuff plotPiston.cmd -show=slider64bd01Ad1pv
#
$show="piston.show"; $matlab=""; 
$vorMin=-50; $vorMax=25.; $option=""; $name="bic"; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "option=s"=>\$option, "matlab=s"=>\$matlab );
#
$show
# 
plot sequence:rigid body 0
  x1
  add v1
  add a1
  if( $matlab ne "" ){ $cmd = "save results to a matlab file\n $matlab.m"; }else{ $cmd="#"; }
  $cmd
pause
exit
contour
plot:p
#remove contour planes
    0
    done
exit
set view:0 -0.0330148 0.0174064 0 1 0.766044 0.321394 -0.55667 0 0.866025 0.5 0.642788 -0.383022 0.663414

pause
DISPLAY AXES:0 0
#save solutions
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
hardcopy file name:0 pressureG4T0p8.ps
pause
grid
  plot interpolation points 1
  plot interpolation points 0
toggle grid lines on boundary 1 2 1 0
toggle grid lines on boundary 1 1 1 0
toggle grid lines on boundary 1 2 0 0
toggle grid lines on boundary 1 1 0 0
  interior boundary points 0
  interior boundary points 1
  plot block boundaries 0
  plot shaded surfaces (3D) 0
  exit this menu

hardcopy save:0

derived types
vorticity
exit
#
previous
contour
  plot:vorticity
  coarsening factor 1 (<0 : adaptive)
  min max $vorMin $vorMax
  vertical scale factor 0.
  plot contour lines (toggle)
  exit
# 
if( $option eq "sl" ){ $cmd="erase"; }else{ $cmd="#"; }
$cmd
# 
forcing regions
  body force grid lines 1
  line width: 2
  # forcing region colour (bf,colour): 0 RED
exit
#
# -- MOVIE: (commands end with a blank to STOP)
if( $option eq "movie" ){ $cmd="DISPLAY COLOUR BAR:0 0\n DISPLAY AXES:0 0\n set view:0 -0.00504559 0.0647128 0 1.45693 1 0 0 0 1 0 0 0 1\n movie file name: $name\n solution: 1\n "; }else{ $cmd="#"; }
$cmd
#
if( $option eq "sl" ){ $cmd="#"; }else{ $cmd=" "; }
$cmd
# -- STREAM-LINES
DISPLAY AXES:0 0
DISPLAY LABELS:0 1
DISPLAY COLOUR BAR:0 0
#
# set view:0 0.278265 0.0060423 0 2.32062 1 0 0 0 1 0 0 0 1
reset
bigger 1.3
line width scale factor:0 4
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
stream lines
  streamline density 100
  arrow size 0.02
exit
$plotName = $name . "_SL.ps"; 
hardcopy file name:0 $plotName
hardcopy save:
pause
# 
# Zoom:
# set view:0 0.464345 0.00742516 0 5.38919 1 0 0 0 1 0 0 0 1
# set view:0 0.492117 0.00652821 0 6.94635 1 0 0 0 1 0 0 0 1
set view:0 0.267774 0.0112432 0 5.18392 1 0 0 0 1 0 0 0 1
$plotName = $name . "_SL_ZOOM.ps"; 
hardcopy file name:0 $plotName
hardcopy save:



# -- movie
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
set view:0 -0.00504559 0.0647128 0 1.45693 1 0 0 0 1 0 0 0 1
movie file name: twoBeamsInAChannel
save movie files 1
show movie


# -- Hardcopy:
set view:0 0.103601 0.038383 0 1.94067 1 0 0 0 1 0 0 0 1
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
previous
  hardcopy file name:0 twoBeamsInAChannelVor_t10.ps
  hardcopy save:0
