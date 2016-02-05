* 
* plotStuff plotWireWrap.cmd
* 
* $grid="wireWrap3pinsi2.order2.hdf";
* $grid="wireWrap5pinsi1.order2.hdf";
$grid="wireWrap7pinsi2.order2.hdf";
*
$grid
* 
  plot block boundaries 0
  x-r 60
  toggle shaded surfaces 1 0
  coarsening factor 8
  DISPLAY AXES:0 0


  
  plot grid lines 0
  pick colour...
  PIC:brass
  grid colour 4 BRASS
  grid colour 2 BRASS
  grid colour 8 BRASS
  grid colour 0 BRASS
  grid colour 2 BRASS
  grid colour 3 BRASS
  grid colour 4 BRASS
  grid colour 5 BRASS
  grid colour 6 BRASS
  grid colour 7 BRASS
  grid colour 8 BRASS
  grid colour 9 BRASS
  grid colour 10 BRASS
  grid colour 11 BRASS
  grid colour 12 BRASS
  grid colour 13 BRASS
  grid colour 14 BRASS
  grid colour 15 BRASS
  x-r:0
  plot grid lines 1
  coarsening factor 2
  coarsening factor 4
  hardcopy file name:0 assembly7Brass.ps
  hardcopy save:0
  hardcopy close dialog:0
  pick to colour grids
  pick to toggle boundaries
  pick to toggle grid lines
  toggle grid lines on boundary 1 2 1 0
  toggle grid lines on boundary 1 1 12 0
  toggle grid lines on boundary 1 2 1 1
  toggle grid lines on boundary 1 1 12 1
  toggle grid lines on boundary 1 2 1 0
  toggle grid lines on boundary 1 1 13 0
  toggle grid lines on boundary 1 2 1 1
  toggle grid lines on boundary 1 1 13 1
  toggle grid lines on boundary 1 2 5 1
  toggle grid lines on boundary 1 2 5 0
  toggle grid lines on boundary 0 2 5 0
  toggle grid lines on boundary 0 1 5 0
  toggle grid lines on boundary 0 2 6 0
  toggle grid lines on boundary 0 2 8 0
  toggle grid lines on boundary 0 1 8 0
  toggle grid lines on boundary 0 1 7 0
  toggle grid lines on boundary 0 1 12 0
  toggle grid lines on boundary 0 2 12 0
  toggle grid lines on boundary 0 2 13 0
  toggle grid lines on boundary 0 2 9 0
