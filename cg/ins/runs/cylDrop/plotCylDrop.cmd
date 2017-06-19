#
#  plotStuff plotCylDrop.cmd -show=cic.show
#
# rhos=10:
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd10p0.show -matlab=cylDrop2bd10p0
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd10p0TP.show  -matlab cylDrop2bd10p0TP
# 
# rhos=1:
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd1p0.show -matlab=cylDrop2bd1p0
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd1p0TP.show  -matlab cylDrop2bd1p0TP
# 
# rhos=.001
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd0p001.show -matlab=cylDrop2bd0p001
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd0p001TP.show -matlab=cylDrop2bd0p001TP
#
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd0p001a.show -matlab=cylDrop2bd0p001a  -- decrease nu
#
# rhos=.01
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd0p1.show  -matlab=cylDrop2bd0p1
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd0p1TP.show  -matlab=cylDrop2bd0p1TP
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd0p1TPa.show  -matlab=cylDrop2bd0p1TPa
# 
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd0p01.show  -matlab=cylDrop2bd0p01
#  plotStuff plotCylDrop.cmd -show=cylDrop2bd0p01TP.show  -matlab=cylDrop2bd0p01TP
#
#-- bigger domain
#  plotStuff plotCylDrop.cmd -show=cylDrop2b.show  
#  plotStuff plotCylDrop.cmd -show=cylDrop2r1b.show    [ density=1
#
# -- self convergence plot
#  plotStuff plotCylDrop.cmd -show=fallingDropG8.show -name=fallingDropG8NonMoving
#
#
$show="cic.show";
$vorMin=-50; $vorMax=25.; $option=""; $name="bic"; $solution=-1; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "option=s"=>\$option, "name=s"=>\$name, "matlab=s"=>\$matlab );
#
$show
# 
if( $option eq "SL" ){ $cmd="previous\n stream lines\n pause"; }else{ $cmd="#";}
$cmd
#
plot sequence:rigid body 0
  x2
  add v2
  add a2
  add f2
  if( $matlab ne "" ){ $cmd = "save results to a matlab file\n $matlab.m"; }else{ $cmd="#"; }
  $cmd
pause
exit
# 
contour
exit
solution: $solution




# contour plots
DISPLAY AXES:0 0
set view:0 -0.0969789 -0.00302115 0 1.02795 1 0 0 0 1 0 0 0 1
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
solution: 46
plot:p
$plotName = $matlab . "pressure.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
#
plot:v
solution: 51
$plotName = $matlab . "t2p5v.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
plot:u
$plotName = $matlab . "t2p5u.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0

