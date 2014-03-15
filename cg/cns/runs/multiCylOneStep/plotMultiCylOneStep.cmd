#
# Plot results from multiCylOneStep.cmd
#
#  plotStuff plotMultiCylOneStep.cmd -show=multiCylOneStep2.show
#  plotStuff plotMultiCylOneStep.cmd -show=multiCylOneStep4l2r4.show
#  plotStuff plotMultiCylOneStep.cmd -show=multiCylOneStep4l2r4a.show -sMin=.6 -sMax=.95 -name=multiCylOneStep4l2r4 -solution-56 -time=1p0
#
#  ppm2mpeg multiCylOneStepSchieren 0 200
# mplayer -fps 15 -x 1024 -y 1200 -loop 10 multiCylOneStepSchieren.mpg 
# mplayer -fps 10 -x 1024 -y 1200 -loop 10 multiCylOneStepSchierenBlue.mpg 
# 
$count=1; $delta=2; $sMin=.2; 
# 
$show="multiCylOneStep4-l2.show"; $plotName="multiCylOneStep4l2"; $sMin=.6; 
# $show="multiCylOneStep4.show"; $plotName="multiCylOneStep4";  $count=2; $delta=4; # This file has twice as many entries
# multiCylOneStep4.show
# multiCylOneStep2.show
# multiCylOneStep2rho1.show
# 
$show="multiCylRandom2.show"; $sMin=.2; $sMax=1.; $name=""; $solution=21; $time="10p0"; 
GetOptions( "show=s"=>\$show,"name=s"=>\$name,"sMin=s"=>\$sMin,"sMax=s"=>\$sMax,"solution=i"=>\$solution,"time=s"=>\$time  );
#
$show
#
previous
# 
derived types
schlieren
exit
DISPLAY SQUARES:0 0
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
contour
  plot:schlieren
  plot contour lines (toggle)
  # gray scale colour table:
  gray
  # rainbow
  vertical scale factor 0.
  min max $sMin $sMax
  exit
grid
  plot grid lines 0
  plot non-physical boundaries 1
  colour boundaries by refinement level number
exit this menu
##
# time t=.5
solution: 6
set view:0 -0.0465595 0.00309366 0 2.00772 1 0 0 0 1 0 0 0 1

# closeup for t=1.
solution: 56
set view:0 -0.276388 -0.000534649 0 3.12917 1 0 0 0 1 0 0 0 1



DISPLAY LABELS:0 0


# Background colour:0 black
# Foreground colour:0 black
# 
# Background colour:0 mediumgoldenrod
# Foreground colour:0 mediumgoldenrod
# 
Background colour:0 darkturquoise
Foreground colour:0 darkturquoise
#
# -----  BLUE -----------------
#* line width scale factor:0 5
derived types
schlieren
 schlieren parameters
 # flip sclieren scale 
   -1. 15. 
exit
plot:schlieren
erase
#  solution5
# solution 196 = t=.975 ( tplot=.005)
solution: 201
# 
previous
contour
#*  gray
  plot contour lines (toggle)
  plot:rho
  plot:schlieren
 # make the lighter lines appear darker by adjusting the min contour value
 #* min max $sMin 1
 # min max $sMin 1.25
 #  min max .75 1.07  (for t=.615)
  vertical scale factor 0.
  compute coarsening factor 0
  coarsening factor 1 (<0 : adaptive)
#
  exit
set view:0 0.0156245 -0.0224113 0 1.51384 1 0 0 0 1 0 0 0 1
x-:0
x-:0


# 
# hardcopy vertical resolution:0 512 
# hardcopy horizontal resolution:0 512
hardcopy vertical resolution:0 8192 
hardcopy horizontal resolution:0 8192
#* hardcopy colour:0 Gray
# save as eps, then we can convert to other formats (otherwise the size appears as 540x...)
# trouble with this: 
hardcopy format:0 EPS
hardcopy colour:0 24bit
# $name = "multiCylOneStepColourSchlieren1p0BlueBlack.ps";
# $name = "multiCylOneStepColourSchlieren1p0BlueGold.ps";
$name = "multiCylOneStepColourSchlieren1p0BlueTurq.ps";
hardcopy file name:0 $name
hardcopy save:0



# -----  BLUE MOVIE-----------------
derived types
schlieren
 schlieren parameters
 # flip sclieren scale 
   -1. 15. 
exit
plot:schlieren
erase
# 
contour
#*  gray
  plot contour lines (toggle)
  plot:rho
  plot:schlieren
 # make the lighter lines appear darker by adjusting the min contour value
 #* min max $sMin 1
 # min max $sMin 1.25
 #  min max .75 1.07  (for t=.615)
  vertical scale factor 0.
  compute coarsening factor 0
  coarsening factor 1 (<0 : adaptive)
#
  exit
set view:0 0.0060423 0.00962935 0 1.26882 1 0 0 0 1 0 0 0 1
# 
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
movie file name:multiCylOneStepSchierenBlue
save movie files 1


# ----- Black and White MOVIE -----------------
# DISPLAY LABELS:0 0
derived types
schlieren
exit
plot:schlieren
erase
# 
contour
  gray
  plot contour lines (toggle)
  plot:rho
  plot:schlieren
 # make the lighter lines appear darker by adjusting the min contour value
  min max $sMin 1
 #* min max $sMin 1.25
 #  min max .75 1.07  (for t=.615)
  vertical scale factor 0.
  compute coarsening factor 0
  coarsening factor 1 (<0 : adaptive)
#
  exit
set view:0 0.0060423 0.00962935 0 1.26882 1 0 0 0 1 0 0 0 1
# 
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
movie file name:multiCylOneStepSchieren
save movie files 1



# ----- Black and White -----------------
line width scale factor:0 3
derived types
schlieren
exit
plot:schlieren
erase
#  solution5
# solution 196 = t=.975 ( tplot=.005)
solution: 201
# 
previous
contour
  gray
  plot contour lines (toggle)
  plot:rho
  plot:schlieren
 # make the lighter lines appear darker by adjusting the min contour value
  min max $sMin 1
 #* min max $sMin 1.25
 #  min max .75 1.07  (for t=.615)
  vertical scale factor 0.
  compute coarsening factor 0
  coarsening factor 1 (<0 : adaptive)
#
  exit
set view:0 0.0156245 -0.0224113 0 1.51384 1 0 0 0 1 0 0 0 1
x-:0
x-:0
# 
hardcopy vertical resolution:0 8192 
hardcopy horizontal resolution:0 8192
hardcopy colour:0 Gray
$name = "multiCylOneStepSchlieren1p0BW.ps";
hardcopy file name:0 $name
hardcopy save:0



# -----  BLUE -----------------
line width scale factor:0 3
derived types
schlieren
 schlieren parameters
 # flip sclieren scale 
   -1. 15. 
exit
plot:schlieren
erase
#  solution5
# solution 196 = t=.975 ( tplot=.005)
solution: 201
# 
previous
contour
#*  gray
  plot contour lines (toggle)
  plot:rho
  plot:schlieren
 # make the lighter lines appear darker by adjusting the min contour value
 #* min max $sMin 1
 # min max $sMin 1.25
 #  min max .75 1.07  (for t=.615)
  vertical scale factor 0.
  compute coarsening factor 0
  coarsening factor 1 (<0 : adaptive)
#
  exit
set view:0 0.0156245 -0.0224113 0 1.51384 1 0 0 0 1 0 0 0 1
x-:0
x-:0
# 
hardcopy vertical resolution:0 8192 
hardcopy horizontal resolution:0 8192
#* hardcopy colour:0 Gray
# hardcopy colour:0 24bit
$name = "multiCylOneStepColourSchlieren1p0Blue.ps";
hardcopy file name:0 $name
hardcopy save:0



# ---   RED -----------------
derived types
schlieren
exit
plot:schlieren
erase
#  solution5
# solution 196 = t=.975 ( tplot=.005)
solution: 201
# 
previous
contour
#*  gray
  plot contour lines (toggle)
  plot:rho
  plot:schlieren
 # make the lighter lines appear darker by adjusting the min contour value
 #* min max $sMin 1
 # min max $sMin 1.25
  min max .75 1.07  (for t=.615)
  vertical scale factor 0.
  compute coarsening factor 0
  coarsening factor 1 (<0 : adaptive)
#
  exit
set view:0 0.0156245 -0.0224113 0 1.51384 1 0 0 0 1 0 0 0 1
x-:0
x-:0
# 
hardcopy vertical resolution:0 4096
hardcopy horizontal resolution:0 4096
#* hardcopy colour:0 Gray
$name = "multiCylOneStepColourSchlieren1p0Red.ps";
#* hardcopy colour:0 24bit
hardcopy file name:0 $name
hardcopy save:0













# ---------------- gray scale schlieren ------------------
contour
  gray
  plot contour lines (toggle)
  plot:rho
  plot:schlieren
 # make the lighter lines appear darker by adjusting the min contour value
  min max $sMin 1
 # min max $sMin 1.25
 # min max .75 1.07  (for t=.615)
  vertical scale factor 0.
  compute coarsening factor 0
  coarsening factor 1 (<0 : adaptive)
#
  exit
set view:0 0.0156245 -0.0224113 0 1.51384 1 0 0 0 1 0 0 0 1
x-:0
x-:0
hardcopy vertical resolution:0 4096
hardcopy horizontal resolution:0 4096
hardcopy colour:0 Gray
$name = "multiCylOneStepSchlieren0p975.ps";
hardcopy file name:0 $name
hardcopy save:0




# 
$solution="solution" . "$count";   $count=$count+$delta;
$solution
$name=$plotName . "Schlieren0p1.ps";
hardcopy file name:0 $name
hardcopy save:0
#
$solution="solution" . "$count";   $count=$count+$delta;
$solution
$name=$plotName . "Schlieren0p3.ps";
hardcopy file name:0 $name
hardcopy save:0
#
$solution="solution" . "$count";   $count=$count+$delta;
$solution
$name=$plotName . "Schlieren0p5.ps";
hardcopy file name:0 $name
hardcopy save:0
#
$solution="solution" . "$count";   $count=$count+$delta;
$solution
$name=$plotName . "Schlieren0p7.ps";
hardcopy file name:0 $name
hardcopy save:0
#
$solution="solution" . "$count";   $count=$count+$delta;
$solution
$name=$plotName . "Schlieren0p9.ps";
hardcopy file name:0 $name
hardcopy save:0
#

