# plotStuff plotDeformingEllipseGrid
$show="deformingEllipseGride4.hdf"; 
#
$show
#
  DISPLAY SQUARES:0 0
  DISPLAY AXES:0 0
  bigger:0
  plot interpolation points 1
#
  grid colour 2 GREEN
  grid colour 1 RED
  colour grid lines from chosen name
  colour boundaries by chosen name
#
  line width scale factor:0 4
  hardcopy vertical resolution:0 4096
  hardcopy horizontal resolution:0 4096
  plot
  hardcopy file name:0 deformingEllipseGride4.ps
  hardcopy save:0
pause
##  set view:0 0.422961 -0.0773414 0 12.9297 1 0 0 0 1 0 0 0 1
  set view:0 0.334932 -0.0999498 0 8.61329 1 0 0 0 1 0 0 0 1
  hardcopy file name:0 deformingEllipseGride4Zoom.ps
  hardcopy save:0


