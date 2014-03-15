#
# plotStuff plotTerrainGrid.cmd -show=site300Gride1.order2 -name=site300Gride1
# plotStuff plotTerrainGrid.cmd -show=site300Gride2.order2.ml2.hdf -name=site300Gride2
#
$show="site300Gride1e1.order2.hdf"; $name="site300Gride1"; 
GetOptions( "show=s"=>\$show,"name=s"=>\$name );
#
$show
  x-r 90
  set home
#
  DISPLAY AXES:0 0
  DISPLAY SQUARES:0 0
#
  # toggle grid 0 0
  # toggle grid 1 0
  #               s a g off
  toggle boundary 0 0 0 0
  toggle boundary 0 1 0 0 
  toggle grid lines on boundary 0 0 0 0
  toggle grid lines on boundary 0 1 0 0
  #
  toggle boundary 1 2 0 0
  toggle grid lines on boundary 1 2 0 0
  #               s a g 
  toggle boundary 0 0 1 0
  toggle boundary 0 1 1 0
  toggle grid lines on boundary 0 0 1 0
  toggle grid lines on boundary 0 1 1 0
#
  set view:0 -0.0493744 0.0045503 0 1.24423 0.939693 0.116978 -0.321394 -0.34202 0.321394 -0.883023 2.09427e-17 0.939693 0.34202
#
  ## line width scale factor:0 4
  ## plot
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048
  $plotName="$name.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0

  $plotName="$name.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0




  set view:0 -0.0091163 0.0262507 0 0.925308 0.939693 0.0593912 -0.336824 -0.34202 0.163176 -0.925417 2.09427e-17 0.984808 0.173648
  bigger 1.1
  bigger
 #bigger



  set view:0 -0.160121 0.265861 0 2.01829 0.939693 0.0593912 -0.336824 -0.34202 0.163176 -0.925417 2.09427e-17 0.984808 0.173648
  bigger:0


exit this menu


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