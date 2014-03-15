#
# -- fourth-order:
# plotStuff plotTerrain2d.cmd -show=terrain2dO4G4.show -vorMax=1. -time=300 -name=site3002dO4G4  [AFS24 G4
# plotStuff plotTerrain2d.cmd -show=terrain2da.show -vorMax=1. -time=300  -name=site3002dO4G8 [AFS24 G8
#
# -- second-order
# plotStuff plotTerrain2d.cmd -show=terrain2dG4O2.show -vorMax=1. -time=300 -name=site3002dO2G4
# plotStuff plotTerrain2d.cmd -show=terrain2dG8O2.show -vorMax=1. -time=300 -name=site3002dO2G8
#
$show="terrain2d.show"; $vorMax=1.; $name="terrain"; $time="200"; 
GetOptions( "show=s"=>\$show, "vorMax=f"=>\$vorMax, "name=s"=>\$name,"time=f"=>\$time );
#
$show
#
previous
#
derived types
  vorticity
exit
stream lines
  streamline density 200
exit
# hardcopy 
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
set view:0 -0.0261994 -0.00386707 0 1.60617 1 0 0 0 1 0 0 0 1
$plotName = $name . "slT$time.ps";
printf("Saving plot=%s\n",$plotName);
hardcopy file name:0 $plotName
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
hardcopy save:0
pause
erase
# 
contour
  plot:vorticity
  vertical scale factor 0.
  min max -$vorMax $vorMax
  plot contour lines (toggle)
exit
#
# hardcopy 
$plotName = $name . "vorT$time.ps";
hardcopy file name:0 $plotName
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
hardcopy save:0
