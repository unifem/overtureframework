#
# cmd file for tbm : test beam models
# 
$nElem=11; $cfl=.5; $Em=1.; $tension=0.; 
GetOptions( "nElem=i"=>\$nElem,"cfl=f"=>\$cfl,"Em=f"=>\$Em,"tension=f"=>\$tension );
#
#Longfei 20160116: new options added: FEM or FD:
Finite Element
linear beam model
# 
tFinal: 0.5
tPlot: 0.05
#cfl: $cfl
# 
change beam parameters
#
use exact solution 0
#
bc left:pinned
bc right:pinned
#
name: beam
number of elements: $nElem
# momOfIntertia=1., E=1., rho=100., beamLength=1., thickness=.1, pnorm=10.,  x0=0., y0=0.;
area moment of inertia: 1.
elastic modulus: $Em
tension: $tension
density: 100.
thickness: 0.1
length: 1
debug: 1
cfl: $cfl
#
initial conditions...
  standing wave
  amplitude: 0.1
  wave number: 1
  exit
#
exit
solve


