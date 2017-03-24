#
# plotStuff plotEmbedded.cmd -show=ebG4Order4Angle60.show -name=ebG4Order4Angle60  [ G4
# plotStuff plotEmbedded.cmd -show=ebG8Order4Angle60.show -name=ebG8Order4Angle60  [ G8
#
# -- annular far field:
#  plotStuff plotEmbedded.cmd -show=ebG16k40.show
#  plotStuff plotEmbedded.cmd -show=ebG32k40.show
#
# -- plot difference show file:
#  plotStuff plotEmbedded.cmd -show=ebG8Theta45diff.show
#  plotStuff plotEmbedded.cmd -show=ebG4Theta45diff.show
# 
#  plotStuff plotEmbedded.cmd -show=ebG4pTheta60diff.show
#
$show="ebG4Order4Angle60.hdf";
GetOptions( "show=s"=>\$show,"name=s"=>\$name );
#
#line width scale factor:0 4
#hardcopy vertical resolution:0 2048
#hardcopy horizontal resolution:0 2048
#
$show
# 
x-:0
previous
derived types
 E field norm
exit
contour
  plot:eFieldNorm
  min max 0 0.5
  plot contour lines (toggle)
  vertical scale factor 0.
  coarsening factor 1 (<0 : adaptive)
  # pause
exit
DISPLAY COLOUR BAR:0 0
DISPLAY LABELS:0 0
set view:0 -0.0317347 0.0601882 0 2.65834 1 0 0 0 1 0 0 0 1


#
# -- movie of Ex
solution: 1
plot:Ex
DISPLAY AXES:0 0
contour 
  min max -0.35 0.25
exit
movie file name: embeddedBodyG4Angle45ScatteredEx
save movie files 1
pause
show movie
#  ---- plot Ey -----
solution: 1
plot:Ey
contour
  min max -0.2 0.15
exit
movie file name: embeddedBodyG4Angle45ScatteredEy
save movie files 1
pause
show movie


solution: 21
plot:Ex
$plotName = $name . "Ext10p0.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
plot:Ey
$plotName = $name . "Eyt10p0.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
#
solution: 41
plot:Ex
$plotName = $name . "Ext20p0.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
plot:Ey
$plotName = $name . "Eyt20p0.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
