#
# plotStuff plotTcilc.cmd
#
$show="tcilce2.order2.hdf"; $name="tcilc2Grid.ps"; 
$show="tcilce4.order2.hdf"; $name="tcilc4Grid.ps"; 
$show
# 
  DISPLAY AXES:0 0
  DISPLAY LABELS:0 0
  DISPLAY SQUARES:0 0
  bigger:0
  plot interpolation points 1
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
  line width scale factor:0 4
  plot
  hardcopy file name:0 $name
  hardcopy save:0
