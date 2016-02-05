#
#  plotStuff plotTwoSpheresInAChannel.cmd
#
$show="twoSpheresInAChannele3.order4.ml2.hdf";
#
$show
#
  toggle grid 0 0
  DISPLAY AXES:0 0
  DISPLAY SQUARES:0 0
  line width scale factor:0 5
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
# 
  colour boundaries by grid number
#
  set view:0 0.00417537 -0.00584551 0 1.26452 0.866025 0.17101 -0.469846 0 0.939693 0.34202 0.5 -0.296198 0.813798
  hardcopy file name:0 twoSpheresInAChannelGrid.ps
  hardcopy save:0
pause
#
  set view:0 0.292446 0.0592248 0 3.92872 0.866025 0.17101 -0.469846 0 0.939693 0.34202 0.5 -0.296198 0.813798
  hardcopy file name:0 twoSpheresInAChannelGridZoom.ps
  hardcopy save:0
