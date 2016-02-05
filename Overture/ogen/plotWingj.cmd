#
# plotStuff plotWingj.cmd
#
$show="wingji1.order2.hdf";
$show
# 
  toggle grid 0 0
  DISPLAY SQUARES:0 0
  DISPLAY AXES:0 0
  plot block boundaries 0
  colour boundaries by grid number
  set view:0 0.218934 -0.0782695 0 1.67375 0.940151 0.0886356 -0.329029 0.0301537 0.940151 0.339422 0.339422 -0.329029 0.881211
  hardcopy file name:0 wingjGrid.ps
  coarsening factor 2
  hardcopy save:0
