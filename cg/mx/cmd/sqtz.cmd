* run this with 
*    mx -bc=pec sqtz
*    mx -size=.5 -nx=21 -bc=pec sqtz
*    mx noplot -bc=pec sqtz
*    srun -N1 -n1 -ppdebug $cgmxp -bc=pec sqtz
* -sq
* -rot
square10
* nonSquare10
* nonSquare21mx
* nonSquare100mx
* square100mx
* square40.hdf
* tfiAnnulus20.hdf
* tfiAnnulus40.hdf
* tfiAnnulus80.hdf
* sis3e.hdf
* sise.hdf
* sis2.order4.hdf
*
gaussianSource
NFDTD
***
twilightZone
* degreeSpace, degreeTime 2 2
* degreeSpace, degreeTime 1 1
***
*
tFinal .2
tPlot .1 
dissipation 0.0
bc: all=perfectElectricalConductor
*accuracy in space 2
*accuracy in time 2
*order of dissipation 2
cfl .95 1.
continue
* 

movie mode
finish



erase
contour
  ghost lines 1
exit
plot:Ex error
