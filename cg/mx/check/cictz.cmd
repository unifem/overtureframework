* mx -bc=dirichlet cmd/cic4
*
$tFinal=.1; $tPlot=.05; $cfl=.9; $diss=.1; $degreeX=4; $degreeT=4; 
$tx="polynomial"; 
$tz="trigonometric"; 
* 
* 
$grid="cici1.order4.hdf"; 
*
$grid
* 
NFDTD
***
tFinal $tFinal
tPlot  $tPlot
* 
twilightZone
* 
$tz
*
degreeSpace, degreeTime $degreeX $degreeT
* 
TZ omega: 1. 1. 1. 1. 
*
modifiedEquationTimeStepping
*
*
bc: all=perfectElectricalConductor
*
dissipation $diss
*
****************
continue
* 
movie mode
finish
