#
#  plotStuff plotShockMultiDiskGrids.cmd
#
$show="shockMultiDisk2i.show";
$show="shockMultiDisk2m.show";
#
$show
#
DISPLAY COLOUR BAR:0 0
DISPLAY LABELS:0 0
DISPLAY SQUARES:0 0
DISPLAY AXES:0 1
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
line width scale factor:0 4
# 
frame series:outerDomain
grid
  colour grid lines from chosen name
  grid colour 0 BLUE
  grid colour 1 GREEN
  grid colour 2 GREEN
exit
# 
frame series:innerDomain1
derived types
specify displacement components
6 7 8
exit
displacement
  colour grid lines from chosen name
  grid colour 0 RED
  grid colour 1 ORCHID
exit
# 
# 
frame series:innerDomain2
derived types
specify displacement components
6 7 8
exit
displacement
  colour grid lines from chosen name
  grid colour 0 RED
  grid colour 1 ORCHID
exit
# movie:
set view:0 -0.0780665 0.00413293 0 1.58646 1 0 0 0 1 0 0 0 1
save movie files 1
movie file name: shockMultiDiskGrid


# -----------------------------------------------
# figures for flunsi paper:
set view:0 -0.111782 -0.0543807 0 2.90351 1 0 0 0 1 0 0 0 1
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
line width scale factor:0 4
solution: 11
hardcopy file name:0 shockMultiCylGrid1p0.ps
hardcopy save:0
pause
solution: 16
hardcopy file name:0 shockMultiCylGrid1p5.ps
hardcopy save:0
solution: 21
hardcopy file name:0 shockMultiCylGrid2p0.ps
hardcopy save:0

pause
previous
hardcopy file name:0 shockCylGrid0p8.ps
hardcopy save:0
pause
# 
frame series:outerDomain
apply commands to all frame series 0
erase
contour
wire frame (toggle)
  exit
hardcopy file name:0 shockRhoDisp0p8.ps
hardcopy save:0