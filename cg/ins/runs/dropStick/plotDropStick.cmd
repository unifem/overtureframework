#
# plotStuff plotDropStick -show=dropStick2a.show -vorMax=8.
# plotStuff plotDropStick -show=dropStick2b.show -vorMax=8.
# -movie: 
# plotStuff plotDropStick -show=dropStick2c.show -vorMax=8.
#
$show="dropStick2a.show"; $vorMax=10.; 
GetOptions( "show=s"=>\$show,"vorMax=s"=>\$vorMax );
#
$show
#
derived types
vorticity
exit
contour
  plot:vorticity
  vertical scale factor 0.
  min max -$vorMax $vorMax
  plot contour lines (toggle)
  exit
# 
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
bigger:0
y-:0
# ---
save movie files 1
movie file name: dropStick

show movie


# -- 
stride: 4
$cmd="#"; $num=30; 
for( $i=0; $i<$num; $i++ ){ $cmd .= "\n hardcopy file name:0 dropStickVor$i" ."p0.ps\n hardcopy save:0\n next"; }
$cmd
