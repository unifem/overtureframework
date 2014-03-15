*
*   mx cmd/cic4.planeWaveBC
*   mx noplot cmd/cic4.planeWaveBC
*
*   Assign the plane wave forcing on the boundary
*
$tFinal=2.; $tPrint=.5; $diss=.1; 
*
* $gridName="radiation0.order4.hdf"; 
* $gridName="radiation1.order4.hdf";
$gridName="radiation2.order4.hdf";
*
*
$gridName
*
NFDTD
*
gaussianIntegralInitialCondition
*
bc: square=dirichlet
bc: square(0,0)=rbcNonLocal
*
** bc: square=abcEM2
*bc: square(1,0)=planeWaveBoundaryCondition
* 
tFinal $tFinal
tPlot  $tPrint
*  .1 seems to work with diss-order==4  (.05 almost enough)
*  .05 ok with diss-order==8 (.1 requires cfl=.9)
dissipation  $diss
************************
cfl .9  .75 .95  .75 .7 .8 1.
*
****
plot errors 1
check errors 1
*
* use conservative difference 0
debug 0
continue
*
plot:Ey

movie mode
finish

