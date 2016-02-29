#
#
#  plotStuff plotFallingBody.cmd -show=fallingBody2.show
#  plotStuff plotFallingBody.cmd -show=fallingBody4.show
# 
$show="fallingBody2.show";
$vorMin=-50; $vorMax=25.; $option=""; $name="fallingBody"; $matlab="fallingBody"; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "option=s"=>\$option, "name=s"=>\$name, "matlab=s"=>\$matlab );
#
$show
# 
plot sequence:rigid body 0
  x2
  add v2
  add a2  
pause
  add w3
  add wt3
  add g3
  add x1 
  add v1 
  add a1
  if( $matlab ne "" ){ $cmd = "save results to a matlab file\n $matlab.m"; }else{ $cmd="#"; }
  $cmd
exit
# 


derived types
speed
exit
# 
contour
exit
# contour plots
DISPLAY AXES:0 0
set view:0 -0.0969789 -0.00302115 0 1.02795 1 0 0 0 1 0 0 0 1
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
solution: 201
plot:p
$plotName = $matlab . "t20p0pressure.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
#
plot:speed
$plotName = $matlab . "t20p0speed.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
#
plot:u
$plotName = $matlab . "t20p0u.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
#
plot:v
$plotName = $matlab . "t20p0v.ps"; 
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

