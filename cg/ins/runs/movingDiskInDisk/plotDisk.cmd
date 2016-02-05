#
#  plotStuff plotDisk.cmd -show=did2bd1.show
#
# rhos=100:
#  plotStuff plotDisk.cmd -show=did4bd100p0a.show -matlab=did4bd100p0a  [ G4
# 
# rhos=10:
#  plotStuff plotDisk.cmd -show=did2bd10p0.show -matlab=did2bd10p0
#  plotStuff plotDisk.cmd -show=did2bd10p0TP.show -matlab=did2bd10p0TP
# 
# rhos=1:
#  plotStuff plotDisk.cmd -show=did2bd1p0a.show -matlab=did2bd1p0a
#
#  plotStuff plotDisk.cmd -show=did4bd1p0a.show -matlab=did4bd1p0a   G4 
#  plotStuff plotDisk.cmd -show=did2bd1p0c.show -matlab=did2bd1p0c  amp=1.e-4
#  plotStuff plotDisk.cmd -show=did4bd1p0Amp100p0.show -matlab=did4bd1p0Amp100p0      [ G4, amp=100
#  plotStuff plotDisk.cmd -show=did4bd1p0Amp100p0pc.show -matlab=did4bd1p0Amp100p0pc  [ G4, amp=100 ts=PC
#
#  plotStuff plotDisk.cmd -show=did2bd1p0d.show -matlab=did2bd1p0d  nu=.01
#  plotStuff plotDisk.cmd -show=did4bd1p0d.show -matlab=did4bd1p0d  nu=.01
#
#  plotStuff plotDisk.cmd -show=did2bd1p0e.show -matlab=did2bd1p0e   nu=1
# rhos=100
#  plotStuff plotDisk.cmd -show=did2bd100p0a.show                   
# 
#  plotStuff plotDisk.cmd -show=did2bd1p0d.show -matlab=did2bd1p0d  amp=1.e-6
# 
#  plotStuff plotDisk.cmd -show=did2bd1p0.show -matlab=did2bd1p0b
#  plotStuff plotDisk.cmd -show=did2bd1p0TP.show -matlab=did2bd1p0TP
# 
# rhos=0.1:
#  plotStuff plotDisk.cmd -show=did2bd0p1.show -matlab=did2bd0p1
#  plotStuff plotDisk.cmd -show=did2bd0p1TP.show -matlab=did2bd0p1TP
# 
# rhos=0.01:
#  plotStuff plotDisk.cmd -show=did2bd0p01a.show -matlab=did2bd0p01a
#  plotStuff plotDisk.cmd -show=did4bd0p01a.show -matlab=did4bd0p01a  [ G4
#  plotStuff plotDisk.cmd -show=did4bd0p01nu0p01.show -matlab=did4bd0p01nu0p01  [ G4, nu=.01
#
#  plotStuff plotDisk.cmd -show=did4bd0p01Amp0p1.show -matlab=did4bd0p01Amp0p1  [ G4, amp=.1
#  plotStuff plotDisk.cmd -show=did4bd0p01Amp10p0.show -matlab=did4bd0p01Amp10p0  [ G4, amp=10
#  plotStuff plotDisk.cmd -show=did4bd0p01Amp50p0a.show -matlab=did4bd0p01Amp50p0a  [ G4, amp=50 dtmax=.01
#  plotStuff plotDisk.cmd -show=did4bd0p01Amp50p0b.show -matlab=did4bd0p01Amp50p0b  [ G4, amp=50 dtmax=.0025
#
#  plotStuff plotDisk.cmd -show=did4bd0p01Amp50p0.show -matlab=did4bd0p01Amp50p0  [ G4, amp=50
#  plotStuff plotDisk.cmd -show=did4bd0p01Amp50p0a.show -matlab=did4bd0p01Amp50p0a  [ G4, amp=50 cfl=.25
#  plotStuff plotDisk.cmd -show=did2bd0p01Amp100p0.show -matlab=did2bd0p01Amp100p0  [ G2, amp=100
#  plotStuff plotDisk.cmd -show=did4bd0p01Amp100p0.show -matlab=did4bd0p01Amp100p0  [ G4, amp=100
#  plotStuff plotDisk.cmd -show=did8bd0p01Amp100p0.show -matlab=did8bd0p01Amp100p0  [ G8, amp=100
#  plotStuff plotDisk.cmd -show=did2bd0p01Amp100p0nu0p01.show -matlab=did2bd0p01Amp100p0nu0p01  [ G2, amp=100, nu=.01
# 
#  plotStuff plotDisk.cmd -show=did2bd0p01.show -matlab=did2bd0p01
#  plotStuff plotDisk.cmd -show=did2bd0p01TP.show -matlab=did2bd0p01TP
# 
#  plotStuff plotDisk.cmd -show=did2bd0p01a.show -matlab=did2bd0p01a   [ lower viscosity 
#  plotStuff plotDisk.cmd -show=did2bd0p01b.show -matlab=did2bd0p01b   [ lower freq
#  plotStuff plotDisk.cmd -show=did2bd0p01Rad3.show -matlab=did2bd0p01Rad3
#
#  plotStuff plotDisk.cmd -show=did4bd0p01.show -matlab=did4bd0p01   [ G4 nu=.01
#
# -- ROTATING: -- see plotRotatingDisk.cmd
# 
$show="cic.show";
$vorMin=-50; $vorMax=25.; $option=""; $name="bic"; $matlab="did"; 
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "name=s"=>\$name,"solution=i"=>\$solution,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,\
            "option=s"=>\$option, "name=s"=>\$name, "matlab=s"=>\$matlab );
#
$show
# 
plot sequence:rigid body 0
  x1
  add v1
  add a1
#   add f1
  if( $matlab ne "" ){ $cmd = "save results to a matlab file\n $matlab.m"; }else{ $cmd="#"; }
  $cmd
pause
exit
# 
contour
exit
# contour plots
DISPLAY AXES:0 0
set view:0 -0.0969789 -0.00302115 0 1.02795 1 0 0 0 1 0 0 0 1
hardcopy vertical resolution:0 1024
hardcopy horizontal resolution:0 1024
solution: 46
plot:p
$plotName = $matlab . "t2p25pressure.ps"; 
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

