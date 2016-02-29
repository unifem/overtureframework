#
# plotStuff plotGrid.cmd -show=embeddedBodyGride4.order2.hdf -name=embeddedBodyGride4
#
#
$show="embeddedBodyGride8.order2.hdf";
GetOptions( "show=s"=>\$show,"name=s"=>\$name );
#
#line width scale factor:0 4
#hardcopy vertical resolution:0 2048
#hardcopy horizontal resolution:0 2048
#
$show
# 
  bigger:0
  DISPLAY AXES:0 0
  DISPLAY SQUARES:0 0
pause
  $plotName = $name . ".ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0
#
  set view:0 -0.00362675 0.035195 0 8.01236 1 0 0 0 1 0 0 0 1
  $plotName = $name . "Zoom.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0