#
#  plotStuff plotFreeSurface3d.cmd -show=fs3d4.show
#
#
$show="fs3d4.show";
$vorMin=-20; $vorMax=20.; $numStreamLines=100; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "numStreamLines=i"=>\$numStreamLines );
#
$show
# 
x-r 90
set home
next
contour
  plot:w
  plot contour lines (toggle)
  # shift contour planes by eps to avoid plotting artifacts
  contour shift 1.e-5
  +shift contour planes
  plot contours on grid boundaries
  (1,0,2) = (freeSurface,side,axis) (off)
  exit
  pick to delete contour planes
  delete contour plane 2

exit
# 


# -- movie
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
set view:0 -0.00504559 0.0647128 0 1.45693 1 0 0 0 1 0 0 0 1
movie file name: flowPastDeformingCylinder

save movie files 1

show movie


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