#
# Plot results from the shock hitting a beam
#
#   plotStuff plotBeamInAChannel -show=beam32.show
#
$show="beam4.show"; $name="beam4"; $root=""; $vMin=0.; $vMax=-1.;  $res=1024; 
# $root = "dE_M1p5"; $root = "dELin_M1p5";
$sc="stressNorm"; $fc="p"; # component names
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show,"root=s"=>\$root,"solution=i"=>\$solution,"vMin=f"=>\$vMin,"vMax=f"=>\$vMax,"res=i"=>\$res );
#
$show
#
derived types
schlieren
exit
plot:schlieren
# plot:p
contour
#  plot contour lines (toggle)
  gray
  vertical scale factor 0.
  coarsening factor 1 (<0 : adaptive)
  # schlieren: 
  plot contour lines (toggle)
  min max .4 1
  # p: 
  # min max .5 1.0001 
 exit
# 
bigger
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
#
# -- MOVIE --
#
DISPLAY LABELS:0 0
solution: 1
save movie files 1
movie file name:beam32
pause
show movie