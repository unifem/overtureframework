#
# plotStuff plotDrops -show=<name> -var=[p|u|v|vorticity] -vMin=<f> -vMax=<f>
# 
# Examples:
#   plotStuff plotDrops -show=drops4.show 
#   plotStuff plotDrops -show=drops4p.show
# G16: 
#   plotStuff plotDrops -show=drops16.show -var=vorticity -vMin=-150. -vMax=150.
#   plotStuff plotDrops -show=drops16b.show -var=vorticity -vMin=-150. -vMax=150.
#   plotStuff plotDrops -show=drops16c.show -var=vorticity -vMin=-150. -vMax=150.
#   plotStuff plotDrops -show=drops16d.show -var=vorticity -vMin=-150. -vMax=150.
#   plotStuff plotDrops -show=drops16e.show -var=vorticity -vMin=-150. -vMax=150.
#   plotStuff plotDrops -show=drops16f.show -var=vorticity -vMin=-150. -vMax=150.
#
$show="drops2.show"; $vMin=1.; $vMax=-1.; $name=""; $solution=21; $time="10p0"; 
$var="v"; 
GetOptions( "show=s"=>\$show,"name=s"=>\$name,"vMin=f"=>\$vMin,"vMax=f"=>\$vMax,"solution=i"=>\$solution,\
            "time=s"=>\$time,"var=s"=>\$var  );
#
$show
#
previous
# 
if( $var eq "vorticity" ){ $cmd="derived types\n vorticity\n exit\n"; }else{ $cmd="#"; }
$cmd
DISPLAY SQUARES:0 0
contour
  plot:$var
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


