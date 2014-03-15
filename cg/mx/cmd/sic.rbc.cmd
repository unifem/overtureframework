*
*   cgmx sic.rbc
*
*  Test the radiation BC for a circular boundary
*
$tFinal=10.; $tPrint=.1;
* $gridName = "sic2e.hdf"; 
* $gridName = "sic3.hdf"; 
** $gridName = "sic5.hdf"; 
$gridName = "sice5.order4.hdf";
*
$gridName
*
gaussianSource
NFDTD
*
*
bc: all=rbcNonLocal
* bc: all=perfectElectricalConductor
*
$freq=10.; 
Gaussian source: 100. 5. .0 .0 0. 
*
tFinal $tFinal
tPlot  $tPrint
*  .1 seems to work with diss-order==4  (.05 almost enough)
*  .05 ok with diss-order==8 (.1 requires cfl=.9)
dissipation  .1  0. 1.  0.5
*
*
* plot scattered field 1
plot errors 0
check errors 0
*
continue


continue

*
plot:Ey

movie mode
finish
