#
#  plotStuff plotCylBeam.cmd -show=cb2.show
#  plotStuff plotCylBeam.cmd -show=cb2a.show
#
#  plotStuff plotCylBeam.cmd -show=cbRhos10E1.show -vorMin=-10. -vorMax=10.
#  plotStuff plotCylBeam.cmd -show=cbRhos10E2.show -vorMin=-10. -vorMax=10.
#  plotStuff plotCylBeam.cmd -show=cbRhos10E5.show -vorMin=-10. -vorMax=10.
#  plotStuff plotCylBeam.cmd -show=cbRhos10E4.show -vorMin=-10. -vorMax=10.
#  plotStuff plotCylBeam.cmd -show=cbRhos10E3.show -vorMin=-10. -vorMax=10.
#  plotStuff plotCylBeam.cmd -show=cbRhos10E3PIV.show -vorMin=-10. -vorMax=10.
#
#
$show="cylbeam2.show";
$vorMin=-50; $vorMax=25.; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax );
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
# 
forcing regions
  body force grid lines 1
  line width: 2
  # forcing region colour (bf,colour): 0 RED
exit




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