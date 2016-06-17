#
#  plotStuff plotRotatingDisk.cmd -show=did2bd1.show
#
# rhos=0.001:
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p001amp.show -matlab=did2rotatebd0p001amp  [ G2
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p001ampAD.show -matlab=did2rotatebd0p001ampAD  [ G2 -- include all added damping terms
#
# rhos=0.01:
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p01.show -matlab=did2rotatebd0p01  [ G2
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p01amp.show -matlab=did2rotatebd0p01amp  [ G2
#  plotStuff plotRotatingDisk.cmd -show=didnudt.show
#
# rhos=0.1:
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1.show -matlab=did2rotatebd0p1  [ G2 TP
#
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1amp.show -matlab=did2rotatebd0p1amp  [ G2
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampAD.show -matlab=did2rotatebd0p1ampAD   [ G2 -- include all added damping terms
#
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampTSpc.show -matlab=did2rotatebd0p1ampTSpc  [ G2, explicit TS beta=4
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampTSpca.show -matlab=did2rotatebd0p1ampTSpca  [ G2, expl-TS beta=1.1
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampTSpcb.show -matlab=did2rotatebd0p1ampTSpcb  [ G2, expl-TS beta=2 dt=FIXED
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampTSpcc.show -matlab=did2rotatebd0p1ampTSpcc  [ G2, expl-TS beta=2 RigidBody: Use AB2 predictor
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampTSpcd.show -matlab=did2rotatebd0p1ampTSpcd  [ G2, expl-TS beta=2 RigidBody: Use AB2 predictor, CFL=.5 
#
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampAD4.show 
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampAD8.show 
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampAD4nc.show 
#
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p01ampAD8.show  [ long time run
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p001ampAD8.show  [ long time run
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p001ampAD4.show  [ long time run
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p001ampAD16.show  [ long time run
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p001ampAD16a.show  [ long time run, smaller amp
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p001ampAD16b.show  [ long time run, smaller amp
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p001ampAD100.show  [ long time run
#
#  -- project velocity
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p01ampAD8pv.show
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p01ampAD1pv.show
#  plotStuff plotRotatingDisk.cmd -show=did4rotatebd0p01ampAD1pv.show
#  plotStuff plotRotatingDisk.cmd -show=did4rotatebd0p01ampAD4pv.show
#  plotStuff plotRotatingDisk.cmd -show=did4rotatebd0p01ampAD8pv.show
# 
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd0p1ampIMdt0p0001.show [ small dt
#
# rhos=1:
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd1p0.show -matlab=did2rotatebd1p0  [ G2
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd1p0amp.show -matlab=did2rotatebd1p0amp  [ G2
#
# rhos=10:
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd10p0.show -matlab=did2rotatebd10p0  [ G2
#
# rhos=100:
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd100p0.show -matlab=did2rotatebd100p0  [ G2
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd100p0pc.show [ G2, ts=pc
#  plotStuff plotRotatingDisk.cmd -show=did2rotatebd100p0cdv.show [ G2, smaller cdv
# 
# rhos=1000
#  plotStuff plotDisk.cmd -show=did2rotatebd1000p0.show [ G2 
#  plotStuff plotDisk.cmd -show=did4rotatebd1000p0.show [ G4
# 
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
  w3
  add wt3
  add g3
pause
  add x1 
  add x2
  add v1 
  add v2
  add a1
  add a2  
pause
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

