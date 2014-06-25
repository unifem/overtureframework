#
# cmd file for tbm : test beam models
#         tbm -cmd=tzbeam.cmd -bc=[p|c|per|cf] -tz=[poly|trig|sw|twfsi]
#
#  -tz=sw : standing wave
#  -tz=twfsi : travelinf wavel FSI-INS
# 
$nElem=11; $tf=.5; $tp=.05; $cfl=.5; $Em=1.; $tension=0.; $degreex=2; $degreet=2; $bc="c"; $tz="poly"; $useNewTri=1;
$fx=2.; $ft=2; 
$go="halt";
GetOptions( "nElem=i"=>\$nElem,"cfl=f"=>\$cfl,"Em=f"=>\$Em,"tension=f"=>\$tension,"degreet=i"=>\$degreet,\
            "degreex=i"=>\$degreex, "bc=s"=>\$bc, "tz=s"=>\$tz, "useNewTri=i"=>\$useNewTri,\
            "tf=f"=>\$tf,"tp=f"=>\$tp,"fx=f"=>\$fx,"ft=f"=>\$ft,  "go=s"=>\$go );
# 
if( $go eq "halt" ){ $go = "#"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n exit\n exit"; }
#
linear beam model
# 
tFinal: $tf
tPlot: $tp
cfl: $cfl
# 
change beam parameters
#
#
if( $bc eq "p" ){ $cmd = "bc left:pinned\n bc right:pinned"; }
if( $bc eq "c" ){ $cmd = "bc left:clamped\n bc right:clamped"; }
if( $bc eq "f" ){ $cmd = "bc left:free\n bc right:free"; }
if( $bc eq "cf" ){ $cmd = "bc left:clamped\n bc right:free"; }
if( $bc eq "per" ){ $cmd = "bc left:periodic\n bc right:periodic"; }
$cmd
#
$rhos=100.; $hs=.1; 
name: beam
number of elements: $nElem
# momOfIntertia=1., E=1., rho=100., beamLength=1., thickness=.1, pnorm=10.,  x0=0., y0=0.;
area moment of inertia: 1.
elastic modulus: $Em
tension: $tension
density: $rhos
thickness: $hs
length: 1
debug: 1
#
use new tridiagonal solver $useNewTri
#
if( $tz eq "poly" || $tz eq "trig" ){ $tzToggle=1; }else{ $tzToggle=0; }
twilight-zone $tzToggle
if( $tz eq "poly" ){ $cmd="Twilight-zone: polynomial"; }elsif( $tz eq "trig" ){ $cmd="Twilight-zone: trigonometric"; }else{ $cmd="#"; }
$cmd
# 
degree in space: $degreex
degree in time: $degreet
trig frequencies: $ft, $fx, $fx, $fx (ft,fx,fy,fz)
#
if( $tz eq "twfsi" ){ $cmd="Exact solution:traveling wave FSI"; }elsif( $tz eq "sw" ){ $cmd="Exact solution:standing wave"; }else{ $cmd="#"; }
$cmd
#
initial conditions...
  if( $tz eq "poly" || $tz eq "trig" ){ $cmd="twilight zone initial conditions"; }elsif( $tz eq "sw" ){ $cmd="standing wave\n  amplitude: 0.1\n wave number: 1"; }else{  $cmd="#"; }
  $cmd 
#
  exit
#
exit
solve
$go


  if( $tz eq "twfsi" ){ $cmd="traveling wave FSI-INS"; }
  $cmd 
    height: 1
    length: 1
    kx: 1
    amp, x0, t0: 0.1, 0, 0
    $rhosHs=$rhos*$hs; 
    elastic shell density: $rhosHs
    elastic shell tension: $tension
    elastic shell stiffness: 0
    fluid density: 1
    fluid viscosity: 0.05
    standing wave solution 1
   exit