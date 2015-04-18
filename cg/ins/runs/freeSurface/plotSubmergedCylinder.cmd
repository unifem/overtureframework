#
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCyl.show -vorMin=-10. -vorMax=10.
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCyla.show -vorMin=-10. -vorMax=10.
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCylb.show -vorMin=-10. -vorMax=10.
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCylc.show -vorMin=-10. -vorMax=10.
#  plotStuff plotSubmergedCylinder.cmd -show=submergedCyld.show -vorMin=-10. -vorMax=10.
#
#
$show="iceDeform4.show";
$vorMin=-20; $vorMax=20.; $numStreamLines=100; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "numStreamLines=i"=>\$numStreamLines );
#
$show
# 
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