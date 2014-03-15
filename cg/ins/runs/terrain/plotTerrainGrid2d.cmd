#
# plotStuff plotTerrainGrid2d.cmd -show=site3002de4.order2.ml3.hdf -name=site300Grid2dO2G4
#
#
$show="terrainGride2.order2.hdf";
GetOptions( "show=s"=>\$show,"name=s"=>\$name );
#
line width scale factor:0 4
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
#
$show
# 
  bigger:0
  DISPLAY AXES:0 0
  DISPLAY SQUARES:0 0
  $plotName = $name . ".ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0
#
  set view:0 -0.0543807 0.247734 0 6.24528 1 0 0 0 1 0 0 0 1
  $plotName = $name . "Zoom.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0