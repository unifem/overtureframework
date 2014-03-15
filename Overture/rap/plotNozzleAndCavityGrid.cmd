#
# plotStuff plotNozzleAndCavityGrid.cmd
#
$grid="../ogen/nozzleAndCavity1order2.hdf";
#
$grid
  set view:0 0.00687642 -0.019042 0 1.17608 0.766044 0.321394 -0.55667 0 0.866025 0.5 0.642788 -0.383022 0.663414
  DISPLAY AXES:0 0
  DISPLAY SQUARES:0 0
  plot block boundaries 0
  colour boundaries by grid number
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
# for mac:
  hardcopy rendering:0 frameBuffer
pause
  hardcopy file name:0 nozzleAndCavityGrid.ps
  hardcopy save:0