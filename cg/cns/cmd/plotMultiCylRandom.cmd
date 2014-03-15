#
# plotStuff plotMultiCylRandom -show=<name> -sMin=<val> -sMax=<val>
#  -sMin, -sMax : min max for schlieren (make sMin larger to darken shocks)
# 
# Examples:
#   plotStuff plotMultiCylRandom -show=multiCylRandom2.show
#   plotStuff plotMultiCylRandom -show=multiCylRandom2a.show
#   plotStuff plotMultiCylRandom -show=multiCylRandom4.show
#
# Make hard-copy:
#   plotStuff plotMultiCylRandom -show=multiCylRandom4.show -name=multiCylRandom4 -solution=31 -time=1p5
#   plotStuff plotMultiCylRandom -show=multiCylRandom4.show -name=multiCylRandom4 -solution=21 -time=1p0
#   plotStuff plotMultiCylRandom -show=multiCylRandom4.show -name=multiCylRandom4 -solution=11 -time=0p5
#
$show="multiCylRandom2.show"; $sMin=.2; $sMax=1.; $name=""; $solution=21; $time="10p0"; 
GetOptions( "show=s"=>\$show,"name=s"=>\$name,"sMin=s"=>\$sMin,"sMax=s"=>\$sMax,"solution=i"=>\$solution,"time=s"=>\$time  );
#
$show
#
previous
# 
derived types
schlieren
exit
DISPLAY SQUARES:0 0
contour
  plot:schlieren
  plot contour lines (toggle)
  # gray scale colour table:
  gray
  # rainbow
  vertical scale factor 0.
  min max $sMin $sMax
  exit
grid
  plot grid lines 0
  plot non-physical boundaries 1
  colour boundaries by refinement level number
exit this menu
# Stop here unless hard-copies are needed:
if( $name eq "" ){ $cmd = " "; }else{ $cmd= "pause"; } 
$cmd
#
# -- hard copies:
#
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
bigger
# -- pressure + AMR
#
solution: $solution
contour
  plot:p
  rainbow
exit
$plotName = $name . "PressureAndAMR" . $time . ".ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
# -- schlieren, no AMR
erase
contour
  plot:schlieren
  gray
exit
$plotName = $name . "Schlieren" . $time . ".ps"; 
hardcopy file name:0 $plotName
hardcopy save:0


