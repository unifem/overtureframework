#
#  plotStuff plotBeam.cmd -show=beam_ar25.show -name=beam_ar25
#
$show="beam_ar20.show"; $name="beam_ar25";
GetOptions( "show=s"=>\$show, "name=s"=>\$name  );
#
$show
#
contour
  adjust grid for displacement 1
  exit
contour
  DISPLAY AXES:0 0
  x-:0
  x-:0
  set view:0 -0.1 0 0 1.02477 1 0 0 0 1 0 0 0 1
  plot contour lines (toggle)
exit
# 
contour
  wire frame (toggle)
exit
#
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
line width scale factor:0 2
#
plot:u
solution: 0
  $plotName = $name . "_t0_u_wireFrame.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0
#
solution: 44
  $plotName = $name . "_t4p3_u_wireFrame.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0
#
solution: 88
  $plotName = $name . "_t8p7_u_wireFrame.ps"; 
  hardcopy file name:0 $plotName
  hardcopy save:0
