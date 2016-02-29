#
# plotStuff plotEmbedded.cmd -show=ebG4Order4Angle60.show -name=ebG4Order4Angle60  [ G4
# plotStuff plotEmbedded.cmd -show=ebG8Order4Angle60.show -name=ebG8Order4Angle60  [ G8
#
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
contour
  plot contour lines (toggle)
  vertical scale factor 0.
pause
exit
#
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
