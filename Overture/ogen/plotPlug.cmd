*
* For mog paper 
*    plotStuff plotPlug.cmd
*
  noPlug2.hdf
*  noPlug4.hdf
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
  line width scale factor:0 6
  set view:0 0.0610519 0.00408992 0 1.1634 1 0 0 0 1 0 0 0 1
  DISPLAY AXES:0 0
  boundary line width
    8
  pick to colour grids
  colour boundaries by chosen name
  colour grid lines from chosen name
  PIC:green
  grid colour 0 GREEN
*
  plot
  hardcopy file name:0 noPlug2-highRes.ps
  hardcopy save:0
pause
exit
erase
open a new file
  plug2.hdf
  plot interpolation points 1
  colour boundaries by grid number
  colour grid lines by grid number
  set view:0 -0.253873 0.00795172 0 1.79868 1 0 0 0 1 0 0 0 1
  colour interpolation points 1
  point size   20 pixels
  plot
  hardcopy file name:0 plug2-highRes.ps
  hardcopy save:0





