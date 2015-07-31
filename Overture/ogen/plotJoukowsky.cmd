# plotStuff plotJoukowsky.cmd
#
$show="joukowsky2de8.order4.ml3.hdf";
#
$show
#
  DISPLAY AXES:0 0
  DISPLAY SQUARES:0 0
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
  line width scale factor:0 5
  coarsening factor 8
  bigger:0
  hardcopy file name:0 joukowsky2de8Order4Grid.ps
  hardcopy save:0
pause
  # zoom: 
  coarsening factor 1
  plot interpolation points 1
  set view:0 0.470319 -0.00104499 0 15.804 1 0 0 0 1 0 0 0 1
  hardcopy file name:0 joukowsky2de8Order4GridZoom.ps
  hardcopy save:0
