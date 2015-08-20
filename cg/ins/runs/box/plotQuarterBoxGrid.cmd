#
#  plotStuff plotQuarterBoxGrid.cmd 
#
$grid="loftedQuarterBoxGride4.order4.ml1.hdf";
#
$grid
#
  DISPLAY AXES:0 0
  DISPLAY SQUARES:0 0
  line width scale factor:0 3
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
  coarsening factor 2
#
  colour boundaries by grid number
  pick to toggle boundaries
  pick closest 1
  toggle boundary 1 2 0 0
  toggle boundary 1 1 0 0
  pick to toggle grid lines
  toggle grid lines on boundary 1 2 0 0
  set view:0 0.421787 0.0178659 0 2.03938 0.866025 -0.17101 0.469846 0 0.939693 0.34202 -0.5 -0.296198 0.813798
  hardcopy file name:0 loftedQuarterBoxGrid4.ps
  hardcopy save:0

