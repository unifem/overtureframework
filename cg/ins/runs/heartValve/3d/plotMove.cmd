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
  w3
  add wt3
  pause
  add v1
  add v2
  add v3
  add w1
  add w2
  pause
  if( $matlab ne "" ){ $m=$matlab . "Body1";  $cmd = "save results to a matlab file\n $m.m"; }else{ $cmd="#"; }
  $cmd
exit
#
plot sequence:rigid body 1
  w3
  add wt3
  pause
  add v1
  add v2
  add v3
  add w1
  add w2
  pause
  if( $matlab ne "" ){ $m=$matlab . "Body2";  $cmd = "save results to a matlab file\n $m.m"; }else{ $cmd="#"; }
exit
# 
derived types
speed
exit
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
plot:speed
#plot:p
contour
exit
grid
  toggle grid 1
  toggle grid 0
  plot block boundaries 0
  plot grid lines 0
  grid colour 2 GRAY40
  grid colour 3 GRAY40
  grid colour 4 GRAY40 #BRASS
  grid colour 5 GRAY40 #BRASS
  grid colour 6 GRAY40 #BRASS
  grid colour 7 GRAY40 #BRASS
  #set view:0 0.010181 0.00226244 0 0.959826 0.508022 -0.262003 0.820529 -0.0593912 0.939693 0.336824 -0.859294 -0.219846 0.461824
  #set view:0 -0.01251 0.000152542 0 1.09579 0.966822 -0.105335 0.232722 0.0244387 0.944987 0.326194 -0.254279 -0.309684 0.916208
  #set view:0 -0.00504559 0.0647128 0 1.45693 1 0 0 0 1 0 0 0 1
  exit
solution: $solution
reset:0
y-r:0
y-r:0
y-r:0
x+r:0

#for movie
DISPLAY AXES:0 0
DISPLAY LABELS:0 0
DISPLAY COLOUR BAR:0 0
bigger:0

# contour plots
DISPLAY AXES:0 0
set view:0 -0.0969789 -0.00302115 0 1.02795 1 0 0 0 1 0 0 0 1
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
#solution: 46
plot:v

