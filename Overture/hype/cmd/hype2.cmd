*
* make a hyperbolic grid
*
DataPointMapping
read file
fuelTank.plot3d
lines
21 9
mappingName
fuelTank
exit
*
hyperbolic
boundary conditions
1 1 2 2 3 0
hypgen
stretching in normal
2
number of regions in normal direction
3
distance to march
.159
.475
.5
lines in normal direction
5
5 
5  
spacing
1.e-2 0
0 0
0 0
boundary condition
20 20 12 12
dissipation
* 1=variable coeff
1 1.0
axis bc parameters
1 0. .3

* 2, 3				IZSTRT(-1/1/2),NZREG
* 36, 0.159, 5e-05, 0			NPZREG(),ZREG(),DZ0(),DZ1()
* 10, 0.475, 0, 0			NPZREG(),ZREG(),DZ0(),DZ1()
* 16, 9.1, 0, 0			NPZREG(),ZREG(),DZ0(),DZ1()
* 20, 20, 12, 12			IBCJA,IBCJB,IBCKA,IBCKB
* 1, 0.000,  40			IVSPEC(1/2),EPSSS,ITSVOL
* 1, 1.00				IMETH(0/1/2/3),SMU2
* 0.00, 0.00			TIMJ,TIMK
* 1, 0.00, 0.30			IAXIS(1/2),EXAXIS,VOLRES
