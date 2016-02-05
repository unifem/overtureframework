*
* plot a high res figure of a grid 
*    plotStuff plotGrid.cmd
* 
$name = "sic2.order4";
*
$name
* 
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
  line width scale factor:0 4
* 
  bigger:0
  DISPLAY AXES:0 0
* 
  plot interpolation points 1
  colour interpolation points 1
  $plotName="$name.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0
