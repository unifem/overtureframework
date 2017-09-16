#
#   plotStuff plotGrid.cmd -show=ellipticalDiskGride8.order2.hdf -name=ellipticalDiskGrid8
# 
#   plotStuff plotGrid.cmd -show=rodGrid2de8.order2 -name=rodGridG8
#
#   plotStuff plotGrid.cmd -show=diskInBoxYpe4.order2.hdf -name=diskInBoxG4
# 
#   plotStuff plotGrid.cmd -show=dielectricSphereGride4.order2 -name=sphereG4
# 
#   plotStuff plotGrid.cmd -show=solidPillBoxGride4.order2 -name=pillBoxG4
#
#   plotStuff plotGrid.cmd -show=crossGride16.order2 -name=crossGridG16
# 
#   plotStuff plotGrid.cmd -show=dielectricBlockGrid2de2.order4 -name=dieBlockG2
#
#   plotStuff plotGrid.cmd -show=dielectricMultiBlockGrid2d2Blockse2.order4 -name=dieTwoBlockG2
# 
#   plotStuff plotGrid.cmd -show=diskInAChannelGride2.order2 -name=diskInAChannelG2
#
$show="ellipticalDiskGride8.order2.hdf";
# get command line arguments
GetOptions( "show=s"=>\$show, "name=s"=>\$name );
#
$show
#
  hardcopy vertical resolution:0 2048
  hardcopy horizontal resolution:0 2048  
  line width scale factor:0 3
  DISPLAY SQUARES:0 0
  plot
  $plotName = $name . ".ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0

  set view:0 0.233167 -0.00650583 0 11.4097 1 0 0 0 1 0 0 0 1
  bigger:0
  smaller:0
  DISPLAY SQUARES:0 0
  $plotName = $name . "Zoom.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0


  line width scale factor:0 3
  toggle grid 0 0
  set view:0 -0.00302115 -0.00302115 0 1.0216 0.866025 0.17101 -0.469846 0 0.939693 0.34202 0.5 -0.296198 0.813798
  $plotName = $name . ".ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0

#
  set view:0 -0.0097415 -0.0247327 0 3.95008 0.866025 0.17101 -0.469846 0 0.939693 0.34202 0.5 -0.296198 0.813798
  grid colour 3 ORANGE
  grid colour 1 GOLDENROD
  colour block boundaries black
  $plotName = $name . "Zoom.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0


  colour boundaries by grid number
  set view:0 0 -0.00188324 0 1.00378 0.957702 0.072741 -0.278415 -0.00813269 0.973978 0.226495 0.287646 -0.21465 0.933373
  line width scale factor:0 3
  plot
  toggle grid lines on boundary 1 0 0 0
  toggle grid lines on boundary 0 0 0 0
  $plotName = $name . ".ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0

# 
#   -- zoom --
#
  set view:0 0.112813 -0.0859939 0 9.88103 1 0 0 0 1 0 0 0 1
  hardcopy file name:0 diskInBoxG4Zoom.ps
  hardcopy save:0


# 
#   -- zoom --
#
  set view:0 0.000912518 -0.195583 0 9.69759 1 0 0 0 1 0 0 0 1
  hardcopy file name:0 rodGridG8Zoom.ps
  hardcopy save:0

# 
#   -- zoom --
#
  set view:0 -0.235479 0.00977062 0 6.87693 1 0 0 0 1 0 0 0 1
  $plotName = $name . "Zoom.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0