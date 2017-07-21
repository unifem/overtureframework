#
#
#  plotStuff plotOne.cmd -show=*.show
# 
$show="oneStirMovingi1.show";
$vorMin=-50; $vorMax=25.; $option=""; $name="oneStir"; $matlab="oneStir"; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "option=s"=>\$option, "name=s"=>\$name, "matlab=s"=>\$matlab );
#
$show
# 
#plot:p
#contour
#exit
# 
plot sequence:rigid body 0
  w3
  pause
  add wt3
  add g3
  add x2
  add x1 
  pause
  if( $matlab ne "" ){ $m=$matlab . "Body1";  $cmd = "save results to a matlab file\n $m.m"; }else{ $cmd="#"; }
  $cmd
exit
#
derived types
speed
exit
#
plot:speed
contour
exit

# ---- MOVIE ---
previous
plot:p
contour
  plot contour lines (toggle)
  # min max -.15 .15
  min max -.05 .25
  vertical scale factor 0.
exit
# contour plots
DISPLAY AXES:0 0
DISPLAY COLOUR BAR:0 0
DISPLAY LABELS:0 0
set view:0 0.000762813 -0.000762813 0 1.58326 1 0 0 0 1 0 0 0 1
movie file name: twoFallingBodiesG4
solution: 1
save movie files 1


# 
pause
plot:p
contour
exit






