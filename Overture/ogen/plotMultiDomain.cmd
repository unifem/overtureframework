* 
* --- plot the multiDomain "cartoons" for mult-domain mult-physics ----
* 
* plotStuff plotMultiDomain.cmd
*
$res=2048; $lineWidth=6;
* $lineWidth=1; 
*  multiDomaini1.order2.hdf
  multiDomaini2.order2.hdf
* 
  line width scale factor:0 $lineWidth
  DISPLAY AXES:0 0
  DISPLAY SQUARES:0 0
  bigger:0
  hardcopy vertical resolution:0 $res
  hardcopy horizontal resolution:0 $res
*  colour boundaries by grid number
* 
  plot interpolation points 0
  * GREEN SPRINGGREEN CADETBLUE NAVYBLUE  CYAN VIOLET RED  BLUEVIOLET SANDYBROWN DARKTURQUOISE MEDIUMTURQUOISE
  colour boundaries by chosen name
  colour grid lines from chosen name
  grid colour 0 BLUE
  grid colour 5 BLUE
  grid colour 11 BLUE
  grid colour 1 DARKTURQUOISE
  grid colour 2 DARKTURQUOISE
  grid colour 3 MEDIUMTURQUOISE
*  grid colour 4 CADETBLUE
  grid colour 4 GREEN
  grid colour 9 RED
  grid colour 10 RED
  grid colour 6 RED
  grid colour 7 RED
* 
  hardcopy file name:0 multiDomainGrid.ps
  hardcopy save:0
pause
    plot grid lines 0
* 
    hardcopy file name:0 multiDomainCartoon.ps
    hardcopy save:0
