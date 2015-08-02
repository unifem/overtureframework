#
#  plotStuff plotBackStepGrid.cmd -grid=backStepGride4.order4.ml1.hdf -name=backStepGrid
#  plotStuff plotBackStepGrid.cmd -grid=backStepAndBodyGride4.order4.hdf -name=backStepAndBodyGrid
#
#  plotStuff plotBackStepGrid.cmd -grid=backStepRefineGride2.order4.hdf -name=backStepRefineGrid2
#  plotStuff plotBackStepGrid.cmd -grid=backStepRefineGride4.order4.ml1.hdf -name=backStepRefineGrid
#
$grid="backStepGride4.order4.ml1.hdf"; $name="bic2";
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "grid=s"=>\$grid, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax );
#
$grid
DISPLAY AXES:0 0
DISPLAY SQUARES:0 0
bigger:0
set view:0 0.00369942 -0.00462428 0 1.27056 1 0 0 0 1 0 0 0 1
line width scale factor:0 3
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
#
colour boundaries by grid number
plot interpolation points 1
# 
$plotName = $name . ".ps";
hardcopy file name:0 $plotName
hardcopy save:0
pause
# zoom
$zoom=" "; 
if( $name =~ /^backStepGrid/ ){ $zoom="set view:0 0.441786 0.101137 0 7.6833 1 0 0 0 1 0 0 0 1"; }
if( $name =~ /^backStepRefineGrid/ ){ $zoom="set view:0 0.565907 0.0810701 0 7.2543 1 0 0 0 1 0 0 0 1"; }
if( $name =~ /^backStepAndBody/ ){ $zoom="set view:0 0.565907 0.0810701 0 7.2543 1 0 0 0 1 0 0 0 1"; }
$zoom
# 
$plotName = $name . "Zoom.ps";
hardcopy file name:0 $plotName
hardcopy save:0

# 
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
DISPLAY LABELS:0 0
DISPLAY SQUARES:0 0
set view:0 0.00483384 -0.00725076 0 1.27308 1 0 0 0 1 0 0 0 1
line width scale factor:0 3
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
  colour interpolation points 0
  point size 2 pixels
exit this menu
solution: 1
$plotName = $name . "Gridt0p0.ps";
hardcopy file name:0 $plotName
hardcopy save:0
pause
solution: 3
$plotName = $name . "Gridt0p2.ps";
hardcopy file name:0 $plotName
hardcopy save:0
pause
solution: 5 
$plotName = $name . "Gridt0p4.ps";
hardcopy file name:0 $plotName
hardcopy save:0
pause 
solution: 11
$plotName = $name . "Gridt1p0.ps";
hardcopy file name:0 $plotName
hardcopy save:0