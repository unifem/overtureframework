#
# plotStuff plotDrops -show=<name> 
# 
# Examples:
#   plotStuff plotDrops -show=drops4.show -vMin=-1. -vMax=1.8
#
$show="drops2.show"; $vMin=1.; $vMax=-1.; $name=""; $solution=21; $time="10p0"; 
GetOptions( "show=s"=>\$show,"name=s"=>\$name,"vMin=f"=>\$vMin,"vMax=f"=>\$vMax,"solution=i"=>\$solution,"time=s"=>\$time  );
#
$show
#
previous
# 
DISPLAY SQUARES:0 0
contour
  plot:v
  plot contour lines (toggle)
  vertical scale factor 0
  if( $vMax > $vMin ){ $cmd = "min max $vMin $vMax"; }else{ $cmd="#"; }
  $cmd
exit


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


