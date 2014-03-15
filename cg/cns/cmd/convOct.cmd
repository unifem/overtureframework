*
*  Circular converging shock hitting Eight circular obstacles.
* 
$ratio=4; $levels=2; $errTol=.05; 
* $grid="convOct.hdf"; $show="convOctl2r4.show";
* This uses a base grid that is twice as fine:
$grid="convOct2.hdf"; $show="convOct2l2r4.show";
*
* Specify the grid to use
$grid
*
   compressible Navier Stokes (Godunov)
* one step
   exit
   recompute dt interval
   2
   turn off twilight
   final time 28.
   times to plot 2.
   plot and always wait
*  no plotting
*
   show file options
     compressed
     open
       $show
     frequency to flush
       2
     exit
*
   pde options
   OBPDE:mu 0.
   OBPDE:kThermal 0.
   OBPDE:gamma 1.4
   OBPDE:artificial viscosity 1.
   close pde options
*
   initial conditions
     OBIC:user defined
     converging shock
     exit
   exit
*
* OBPDE:exact Riemann solver
   OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
   OBPDE:Godunov order of accuracy 2
*
*  turn on axisymmetric flow
*
   boundary conditions
     all=slipWall
     perimeter(1,1)=superSonicOutflow
    done
*
   reduce interpolation width
   2
*
* turn on user defined output
*
   turn on adaptive grids
   order of AMR interpolation
       2
   error threshold
      $errTol
   regrid frequency
     $regrid=$ratio*2;
     $regrid
   change error estimator parameters
     set scale factors
       1 10000 10000 10000 10000
     done
     weight for first difference
       1. 
     weight for second difference
       1.   
     exit
     truncation error coefficient
       1.
     show amr error function
   change adaptive grid parameters
     refinement ratio
       $ratio
     default number of refinement levels
       $levels
     number of buffer zones
       2
     grid efficiency
       .7
   exit
continue

movie mode
finish


