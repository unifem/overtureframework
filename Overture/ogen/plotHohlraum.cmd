* plotStuff plotHohlraum.cmd
*
hohlraum.hdf
*
  DISPLAY AXES:0 0
  set view:0 -0.0744879 -0.0111732 0 1.24594 1 0 0 0 1 0 0 0 1
  colour boundaries by grid number
  hardcopy file name:0 hohlraumGrid.ps
pause
  hardcopy save:0
  plot grid lines 0
  colour boundaries black
  line width scale factor:0 4
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
  hardcopy file name:0 hohlraumBoundariesBlack.ps
  erase
  plot
  hardcopy save:0





  colour boundaries by bc number
  colour boundaries by chosen name
  pick colour...
  PIC:brass
  plot grid lines 1
  grid colour 1 BRASS
  PIC:blue
  grid colour 1 BLUE
  colour grid lines from chosen name
  PIC:brass
  grid colour 1 BRASS
  grid colour 0 BRASS
  grid colour 1 BRASS
  grid colour 4 BRASS
  grid colour 10 BRASS
  grid colour 0 BRASS
  grid colour 1 BRASS
  grid colour 4 BRASS
  grid colour 9 BRASS
  grid colour 10 BRASS
  set view:0 -0.206014 -0.301128 0 6.89763 1 0 0 0 1 0 0 0 1
  reset:0
