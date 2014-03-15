*
*   mx cmd/cic.planeWaveBC
*
*   Assign the plane wave forcing on the boundary
*
**** NOTE: when using the scattered field as an initial condition and BC use the options
*           (1) planeWaveBoundaryForcing
*           (2) planeWaveScatteredFieldInitialCondition
*           (3) square=dirichlet 
*           (4) slow start interval -1.
*
* cic.hdf
$grid="cici1.order4.hdf";
*
$grid
*
NFDTD
*
*  Initial condition:
* zeroInitialCondition
* planeWaveInitialCondition
planeWaveBoundaryForcing
* ====
planeWaveScatteredFieldInitialCondition
scatteringFromADiskKnownSolution
* ====
*
kx,ky,kz 1 0 0
bc: square=dirichlet
bc: Annulus=perfectElectricalConductor
tFinal .2
tPlot  .1
dissipation  .1 
*
* plot scattered field 1
plot errors 1
check errors 1
*
continue
movie mode
finish
