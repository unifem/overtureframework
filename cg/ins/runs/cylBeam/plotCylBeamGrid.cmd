#
#  plotStuff plotCylBeamGrid.cmd -show=cb2.show
#
#
$show="cb2.show";
$vorMin=-50; $vorMax=25.; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax );
#
$show
# 
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
DISPLAY LABELS:0 0
DISPLAY SQUARES:0 0
set view:0 0.164173 0.0147338 0 2.23685 1 0 0 0 1 0 0 0 1
line width scale factor:0 2
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
# 
forcing regions
  body force grid lines 1
  line width: 2
  # forcing region colour (bf,colour): 0 RED
exit
# 
grid
  plot interpolation points 1
  colour interpolation points 1
exit this menu
#
solution: 1
hardcopy file name:0 cylBeam2Gridt0p0.ps
hardcopy save:0
# pause
solution: 9
hardcopy file name:0 cylBeam2Gridt8p0.ps
hardcopy save:0
# pause
solution: 50
hardcopy file name:0 cylBeam2Gridt49p0.ps
hardcopy save:0
