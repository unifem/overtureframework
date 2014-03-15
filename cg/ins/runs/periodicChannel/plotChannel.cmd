#
#  plotStuff plotChannel -show="channel20.show"
#  plotStuff plotChannel -show="channelLong10.show"
#  plotStuff plotChannel -show="channelL12f10.show"
#  plotStuff plotChannel -show="channelWithBump5.show"
#  plotStuff plotChannel -show="channelWithBump10.show" -vorMin=-30. -vorMax=30.
#  plotStuff plotChannel -show="channelWithBump10mg.show" -vorMin=-30. -vorMax=30.
#  plotStuff plotChannel -show="channelWithBump20.show" -vorMin=-30. -vorMax=30.
#  plotStuff plotChannel -show="channelWithBump40.show" -vorMin=-40. -vorMax=40.
#  plotStuff plotChannel -show="channelWithBump40b.show" -vorMin=-40. -vorMax=40.
# 
#  plotStuff plotChannel -show="bump5.show" -vorMin=-40. -vorMax=40.
#  plotStuff plotChannel -show="bump10.show" -vorMin=-60. -vorMax=60.
#  plotStuff plotChannel -show="bump20.show" -vorMin=-60. -vorMax=60.
#  plotStuff plotChannel -show="bump20a.show" -vorMin=-60. -vorMax=60.
# 
#  plotStuff plotChannel -show="pChannel2da.show" -vorMin=-5. -vorMax=5. -name="pChannel128"
#  plotStuff plotChannel -show="pChannel2db.show" -vorMin=-5. -vorMax=5. -name="pChannel256"
#  plotStuff plotChannel -show="pChannel2dc.show" -vorMin=-5. -vorMax=5. -name="pChannel512"
#  plotStuff plotChannel -show="pChannel2dd.show" -vorMin=-5. -vorMax=5. -name="pChannel1024"
# -- movie:
#  plotStuff plotChannel -show="pChannel2dMovie.show" -vorMin=-5. -vorMax=5. -name="movie"
#
$show="channel10.show"; $name="pChannel128"; 
GetOptions( "show=s"=>\$show,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,"name=s"=>\$name );
#
$show
#
derived types
 vorticity
exit
previous
contour
  plot:vorticity
  min max $vorMin $vorMax
  plot contour lines (toggle)
  vertical scale factor 0.
  compute coarsening factor 0
  coarsening factor 1 (<0 : adaptive)
  plot boundaries (toggle)
exit
#


# -- hardcopies ---
reset:0
bigger:0
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
# 
$plotName = $name . "Vorticity.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
pause
erase 
# 
line width scale factor:0 4
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
stream lines
  streamline density 100
  arrow size 3.000000e-02
  $plotName = $name . "StreamLines.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0


# --- movie:
reset:0
bigger:0
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
next
movie file name: periodicChannel
save movie files 1
show movie



set view:0 -0.0838874 0.0406987 0 1.17507 1 0 0 0 1 0 0 0 1

# -- hardcopies ---
reset:0
bigger:0
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
# 
$plotName = $name . "Vorticity.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
pause
erase 
# 
line width scale factor:0 4
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
stream lines
  streamline density 100
  arrow size 3.000000e-02
  $plotName = $name . "StreamLines.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0

