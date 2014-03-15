#
# plotStuff plotCylBeam -show=cylBeamTH3f4.show -vorMax=100.
# 
# -movie: 
# plotStuff plotCylBeam -show=cylBeam2c.show -vorMax=100.
#
$show="cylBeamTH3f4.show"; $vorMax=10.; 
GetOptions( "show=s"=>\$show,"vorMax=s"=>\$vorMax );
#
$show
#
derived types
vorticity
exit
set view:0 0.337835 0 0 2.51746 1 0 0 0 1 0 0 0 1
contour
  plot:vorticity
  vertical scale factor 0.
  min max -$vorMax $vorMax
  plot contour lines (toggle)
  exit
# 
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
# ---
DISPLAY LABELS:0 0

hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
hardcopy colour:0 24bit
solution: 591
hardcopy file name:0 cylBeamVort5p90.ps
hardcopy save:0
solution: 593
hardcopy file name:0 cylBeamVort5p92.ps
hardcopy save:0
solution: 595
hardcopy file name:0 cylBeamVort5p94.ps
hardcopy save:0
solution: 597
hardcopy file name:0 cylBeamVort5p96.ps
hardcopy save:0
solution: 599
hardcopy file name:0 cylBeamVort5p98.ps
hardcopy save:0
solution: 601
hardcopy file name:0 cylBeamVort6p00.ps
hardcopy save:0



save movie files 1
movie file name: cylBeam

show movie


# -- 
stride: 4
$cmd="#"; $num=30; 
for( $i=0; $i<$num; $i++ ){ $cmd .= "\n hardcopy file name:0 cylBeamVor$i" ."p0.ps\n hardcopy save:0\n next"; }
$cmd
