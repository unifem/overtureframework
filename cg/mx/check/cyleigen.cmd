************************************************
* ***  compare to the actual eigenfunctions ****
************************************************
* run this with 
*
*    mx cyleigen
*    mx -noplot cmd/cyleigen
*
$tFinal=.1; $tPlot=.05; $cfl=.95; $diss=.5; $show=" ";
$n=1; $m=1; $k=1;   # n=Jn, m=m*theta, k=k*Pi*z
* 
$grid="tube1.order4.hdf"; 
*
$grid
*
modifiedEquationTimeStepping
*
NFDTD
***
solve for magnetic field 0
***
bc: all=perfectElectricalConductor
annulusEigenfunctionInitialCondition
 $n $m $k    * n=Jn, m=m*theta, k=k*Pi*z
***
* 
use conservative divergence 1
*
*
tFinal $tFinal
tPlot  $tPlot
dissipation $diss
cfl $cfl
****
*
debug 0
*
continue
movie mode
finish


