#
#  plotStuff plotDrop3d.cmd -show=cic.show
#
# rhos=2:
#  plotStuff plotDrop3d.cmd -show=drop3d.show
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
if( $option eq "SL" ){ $cmd="previous\n stream lines\n pause"; }else{ $cmd="#"; }
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
derived types
speed
exit
plot:speed
contour
exit
solution: $solution
set view:0 0 0 0 1 0.939684 -0.0461891 0.338911 0.000205567 0.990916 0.134479 -0.342044 -0.126298 0.931158
grid
  toggle grid 0 0
  plot block boundaries 0
  plot grid lines 0
  grid colour 1 GRAY40
  grid colour 2 GRAY40
  exit this menu


# contour plots
DISPLAY AXES:0 0
set view:0 -0.0969789 -0.00302115 0 1.02795 1 0 0 0 1 0 0 0 1
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
#solution: 46
plot:v

