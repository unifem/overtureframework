#
# Order=2: 
# plotStuff plotFlatPlate.cmd -show=flatPlateOrder2Res4.show -name=flatPlateOrder2Res4
#
# Order=4: 
# plotStuff plotFlatPlate.cmd -show=flatPlateOrder4Res4.show -name=flatPlateOrder4Res4
#
#
$show="flatPlateOrder4Res4.show"; $name="flatPlateOrder4Res4"; 
GetOptions( "show=s"=>\$show,"name=s"=>\$name );
#
$show
previous
#
derived types
 vorticity
exit
#
plot:u
# 
  contour
    # Set min max: 
    # min max $Tmin $Tmax
  exit
# 
x-:0
# hardcopy rendering:0 frameBuffer
#
$plotName = $name . "_u.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
plot:v
$plotName = $name . "_v.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
plot:p
$plotName = $name . "_p.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
plot:p_err
$plotName = $name . "_pErr.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
plot:u_err
$plotName = $name . "_uErr.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
plot:v_err
$plotName = $name . "_vErr.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
plot:vorticity
$plotName = $name . "_vor.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
contour
  line plots
    specify lines
    1 101
      3. 0. 3. 1.
      u
      add v
      add vorticity
      add u_true
      add v_true
      add u_err
      add v_err
      add p_err
      add x0
      add x1      
      save results to a matlab file
       $name.m
