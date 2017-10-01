#
#   plotStuff plotField.cmd -show=ellipseG8.show -name=ellipseG8
#   plotStuff plotField.cmd -show=rodG8.show -name=rodG8
#   plotStuff plotField.cmd -show=diskInBoxG8.show -name=diskInBoxG8
#   plotStuff plotField.cmd -show=crossG16.show -name=cross
#
#   plotStuff plotField.cmd -show=starFishG128.show -name=starFish
#   plotStuff plotField.cmd -show=starFishMovie.show -name=starFish
#
#    plotStuff plotField.cmd -show=dieBlockG8.show -solution=101 -name=dieBlock
#    plotStuff plotField.cmd -show=dieBlockG8.show -solution=1  -name=dieBlockt0p0
#    plotStuff plotField.cmd -show=dieBlockG8.show -solution=11 -name=dieBlockt1p0
#    plotStuff plotField.cmd -show=dieBlockG8.show -solution=31 -name=dieBlockt3p0
#    plotStuff plotField.cmd -show=dieBlockG8.show -solution=51 -name=dieBlockt5p0
# 
#    plotStuff plotField.cmd -show=dieBlockG8Eps4.show -solution=101 -name=dieBlockG8Eps4
#
#    plotStuff plotField.cmd -show=pecDiskG4Eps4.show -solution=201 -name=pecDiskG4Eps4
#
#    plotStuff plotField.cmd -show=rod16kx0p5.show -solution=201 -name=rod16kx0p5
#    plotStuff plotField.cmd -show=rod16kx0p25.show -solution=201 -name=rod16kx0p25
# 
#    plotStuff plotField.cmd -show=cross16kx0p5.show -solution=201 -name=cross16kx0p5
#    plotStuff plotField.cmd -show=cross16kx0p25.show -solution=201 -name=cross16kx0p25
#
#    plotStuff plotField.cmd -show=ellipse16kx0p5.show -solution=201 -name=ellipse16kx0p5
#    plotStuff plotField.cmd -show=ellipse8kx0p25.show -solution=201 -name=ellipse8kx0p25
#
$show="ellipseG8.hdf"; $solution="-1"; 
# get command line arguments
GetOptions( "show=s"=>\$show, "name=s"=>\$name, "solution=i"=>\$solution );
#
$show
#
plot:Ey
contour
  plot contour lines (toggle)
  coarsening factor 1 (<0 : adaptive)
  # min max -1 1
 vertical scale factor 0.
exit
solution: $solution
pause
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
#
  line width scale factor:0 3
  plot
  $plotName = $name . "Ey.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0
# 
plot:Ex
  $plotName = $name . "Ex.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0
plot:Hz
  $plotName = $name . "Hz.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0
# 
erase
stream lines
  streamline density 100
  arrow size 0.05
exit
  $plotName = $name . "SL.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0