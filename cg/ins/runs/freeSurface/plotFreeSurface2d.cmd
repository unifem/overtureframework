#
# -- surface waves
#  plotStuff plotFreeSurface2d.cmd -show=surfaceWavesG8.show -name=surfaceWavesG8
# --- oscillating drop:
#  plotStuff plotFreeSurface2d.cmd -show=dropDeformG2.show 
#
#
$show="dropDeformG2.show"; $name="dropDeform"; 
$vorMin=-20; $vorMax=20.; $numStreamLines=100; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "numStreamLines=i"=>\$numStreamLines );
#
$show
# 
#derived types
#vorticity
#exit
#
contour
  plot:v
#  coarsening factor 1 (<0 : adaptive)
#  vertical scale factor 0.
#  plot contour lines (toggle)
exit
DISPLAY AXES:0 0
pause
#
# plot contours and streamlines at different times:
#
#  plotContours(num,timeLabel)
sub plotContours\
{ local($num,$label)=@_; \
  $plotName = $name . "t$label" . "v.ps"; \
  $cmds = "erase\n contour\n exit\n solution: $num \n" . \
   "plot:v\n" . \
   "hardcopy file name:0 $plotName\n" . \
   "hardcopy save:0\n"; \
  $plotName = $name . "t$label" . "sl.ps"; \
  $cmds .= "erase\n stream lines\n streamline density 60\n arrow size .025\n exit\n" .  \
   "hardcopy file name:0 $plotName\n" . \
   "hardcopy save:0\n"; \
}
#
plotContours(2,"0p1"); 
$cmds
pause
#
plotContours(6,"0p5"); 
$cmds
#
plotContours(8,"0p7"); 
$cmds
#
plotContours(9,"0p8"); 
$cmds
#
plotContours(11,"1p0"); 
$cmds
#
plotContours(16,"1p5"); 
$cmds

# 
# -- movie
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
set view:0 0.00294913 -0.0107739 0 1.30058 1 0 0 0 1 0 0 0 1
bigger:0
smaller:0
save movie files 1
movie file name: submergedCylinder



stream lines
  streamline density $numStreamLines
 exit





derived types
vorticity
exit
#
contour
  plot:vorticity
  coarsening factor 1 (<0 : adaptive)
  min max $vorMin $vorMax
  vertical scale factor 0.
  plot contour lines (toggle)
  exit
# 


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