#
#
#  plotStuff plotFallingBody.cmd -show=fallingBody2.show
#  plotStuff plotFallingBody.cmd -show=fallingBody4.show
#
#  plotStuff plotFallingBody.cmd -show=fallingBodyG4d2p0.show
#
# -- rhos=2
#   plotStuff plotFallingBody.cmd -show=fallingBodyG4d2p0.show -matlab=fallingBodyG4d2p0
#   plotStuff plotFallingBody.cmd -show=fallingBodyG4d2p0amp.show -matlab=fallingBodyG4d2p0amp
#   plotStuff plotFallingBody.cmd -show=fallingBodyG4d2p0ampAD1.show -matlab=fallingBodyG4d2p0ampAD1 [AD=1
#
#   plotStuff plotFallingBody.cmd -show=fallingBodyG4d2p0ampVp.show -matlab=fallingBodyG4d2p0ampVp [velocity-proj
#   plotStuff plotFallingBody.cmd -show=fallingBodyG4d2p0ampVpi.show -matlab=fallingBodyG4d2p0ampVpi [iterative-solve
#   -- G8:
#   plotStuff plotFallingBody.cmd -show=fallingBodyG8d2p0amp.show -matlab=fallingBodyG8d2p0amp  [ dt=.02
#   plotStuff plotFallingBody.cmd -show=fallingBodyG8d2p0ampII.show -matlab=fallingBodyG8d2p0ampII [ dt=.01
#   plotStuff plotFallingBody.cmd -show=fallingBodyG8d2p0ampVp.show -matlab=fallingBodyG8d2p0ampVp
# 
# --- rhos=.5
#   plotStuff plotFallingBody.cmd -show=fallingBodyG4d0p5amp.show -matlab=fallingBodyG4d0p5amp
#
# --- rhos = 0.1
#  plotStuff plotFallingBody.cmd -show=fallingBodyG4d0p1.show -matlab=fallingBodyG4d0p1 [G4 TP-SI 
#  plotStuff plotFallingBody.cmd -show=fallingBodyG4d0p1amp.show -matlab=fallingBodyG4d0p1amp [G4
#  plotStuff plotFallingBody.cmd -show=fallingBodyG4d0p1ampAD2.show -matlab=fallingBodyG4d0p1ampAD2 [ AD=2
#  plotStuff plotFallingBody.cmd -show=fallingBodyG4d0p1ampAD4.show -matlab=fallingBodyG4d0p1ampAD4 [ AD=4 
#  plotStuff plotFallingBody.cmd -show=fallingBodyG4d0p1ampBE.show -matlab=fallingBodyG4d0p1ampBE [ RB-backward-Euler
#
#  --- G8
#  plotStuff plotFallingBody.cmd -show=fallingBodyG8d0p1amp.show -matlab=fallingBodyG8d0p1amp [dt=.04
#  plotStuff plotFallingBody.cmd -show=fallingBodyG8d0p1ampA.show -matlab=fallingBodyG8d0p1ampA [dt=.02
# 
# Light: rhob=0.1 
#  plotStuff plotFallingBody.cmd -show=fallingBodyG4d0p1ampAD0p1.show -matlab=fallingBodyG4d0p1ampAD0p1 [ AD=0.1
#  plotStuff plotFallingBody.cmd -show=fallingBodyG4d0p1ampAD1.show -matlab=fallingBodyG4d0p1ampAD1 [ AD=1
#  plotStuff plotFallingBody.cmd -show=fallingBodyG8d0p1ampAD1.show -matlab=fallingBodyG8d0p1ampAD1 [ AD=1
#  plotStuff plotFallingBody.cmd -show=fallingBodyG16d0p1ampAD1.show -matlab=fallingBodyG16d0p1ampAD1 [ AD=1
#  ts=PC: 
#  plotStuff plotFallingBody.cmd -show=fallingBodyG4d0p1ampAD1pc.show
# 
# -- Paper:rising:
#  plotStuff plotFallingBody.cmd -show=risingBodyG2a.show -matlab=risingBodyG2a
#  plotStuff plotFallingBody.cmd -show=risingBodyG4a.show -matlab=risingBodyG4a
#  plotStuff plotFallingBody.cmd -show=risingBodyG8a.show -matlab=risingBodyG8a
#  plotStuff plotFallingBody.cmd -show=risingBodyG16a.show -matlab=risingBodyG16a
#  plotStuff plotFallingBody.cmd -show=risingBodyG32a.show -matlab=risingBodyG32a
#
# -- Paper:rising: OLD
#  plotStuff plotFallingBody.cmd -show=risingBodyG2.show -matlab=risingBodyG2
#  plotStuff plotFallingBody.cmd -show=risingBodyG4.show -matlab=risingBodyG4
#  plotStuff plotFallingBody.cmd -show=risingBodyG8.show -matlab=risingBodyG8
#  plotStuff plotFallingBody.cmd -show=risingBodyG16.show -matlab=risingBodyG16
#  plotStuff plotFallingBody.cmd -show=risingBodyG32.show -matlab=risingBodyG32
# 
#  plotStuff plotFallingBody.cmd -show=risingBodyG4tp.show -matlab=risingBodyG4tp [TP-SI 
#  plotStuff plotFallingBody.cmd -show=risingBodyG4u.show -matlab=risingBodyG4u [ unstable case
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
  a2 
  add x2
  add v2
  add wt3
pause
  add w3
  add g3
  add x1 
  add v1 
  add a1
  if( $matlab ne "" ){ $cmd = "save results to a matlab file\n $matlab.m"; }else{ $cmd="#"; }
  $cmd
exit
# 
plot:p
contour
exit
# contour plots
DISPLAY AXES:0 0
set view:0 -0.0969789 -0.00302115 0 1.02795 1 0 0 0 1 0 0 0 1
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
# Stop here if no matlab file is specified
if( $matlab eq "" ){ $cmd=" "; }else{ $cmd="#"; }
$cmd
#
# plot contours at different times:
#
#  plotContours(num,timeLabel)
sub plotContours\
{ local($num,$label)=@_; \
  $plotName = $matlab . "t$label" . "pressure.ps"; \
  $cmds = "erase\n contour\n exit\n solution: $num \n" . \
   "plot:p\n" . \
   "hardcopy file name:0 $plotName\n" . \
   "hardcopy save:0\n"; \
  $plotName = $matlab . "t$label" . "u.ps"; \
  $cmds .=  \
   "plot:u\n" . \
   "hardcopy file name:0 $plotName\n" . \
   "hardcopy save:0\n"; \
  $plotName = $matlab . "t$label" . "v.ps"; \
  $cmds .=  \
   "plot:v\n" . \
   "hardcopy file name:0 $plotName\n" . \
   "hardcopy save:0\n"; \
  $plotName = $matlab . "t$label" . "sl.ps"; \
  $cmds .= "erase\n stream lines\n exit\n" .  \
   "hardcopy file name:0 $plotName\n" . \
   "hardcopy save:0\n"; \
}
#
plotContours(3,"2p0"); 
$cmds
pause
#
plotContours(4,"3p0"); 
$cmds
pause
#
plotContours(5,"4p0"); 
$cmds
pause
#
plotContours(11,"10p0"); 
$cmds
pause
# 
#
plotContours(13,"12p0"); 
$cmds

# 
# ------------------- plot grids ----------
sub plotGrid\
{ local($num,$label)=@_; \
  $plotName = $matlab . "t$label" . "Grid.ps"; \
  $cmds = "erase\n solution: $num \n"; \
  $cmds .= "grid\n" . \
   "DISPLAY SQUARES:0 0\n" . \
   "line width scale factor:0 4\n" . \
   "hardcopy vertical resolution:0 2048\n" . \
   "hardcopy horizontal resolution:0 2048\n" . \
   "hardcopy file name:0 $plotName\n" . \
   "plot interpolation points 1\n" . \
   "hardcopy save:0\n" . \
   "exit this menu\n"; \
}
#
plotGrid(3,"2p0"); 
$cmds
pause
#
plotGrid(4,"3p0"); 
$cmds
pause
#
plotGrid(5,"4p0"); 
$cmds
pause
#
plotGrid(11,"10p0"); 
$cmds


#
plotContours(21,"2p0"); 
$cmds
pause
#
plotContours(51,"5p0"); 
$cmds





