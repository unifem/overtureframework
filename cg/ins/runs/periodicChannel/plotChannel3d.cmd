#
#  plotStuff plotChannel3d -show=pChannel3d.show -vorMin=-5. -vorMax=5.
#  plotStuff plotChannel3d -show=pChannel3dc.show -vorMin=-5. -vorMax=5.
#
#  plotStuff plotChannel3d -show=pChannel3df.show -vorMin=-5. -vorMax=5.
#
$show="pChannel3d.show"; $name="pChannel3d"; 
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

