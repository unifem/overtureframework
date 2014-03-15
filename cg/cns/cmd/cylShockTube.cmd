*
*  Shock tube with a cylindrical test section.
* 
$tFinal=300.; $tPrint=5.; $xStep="x=-50.";
$ratio=4; $levels=2; $errTol=.05; 
$amrOn="turn on adaptive grids"; $amrOff="turn off adaptive grids";
$amr=$amrOff; 
* 
* $grid="cylShockTube1.hdf"; $show="cylShockTube1.show"; $tPrint=1.;
$grid="cylShockTube2.hdf"; $amr=$amrOn; $show="cylShockTube2l2r4.show"; 
* $grid="cylShockTube4.hdf"; $show="cylShockTube.show"; 
*
* Specify the grid to use
$grid
*
   compressible Navier Stokes (Godunov)
   exit
* 
   turn off twilight
   final time $tFinal
   times to plot $tPrint
   plot and always wait
*  no plotting
*
   show file options
     compressed
     open
       $show
     frequency to flush
       5
     exit
*
   pde options
   OBPDE:mu 0.
   OBPDE:kThermal 0.
   OBPDE:gamma 1.4
   OBPDE:artificial viscosity 1.
   close pde options
*
*
  initial conditions
*    smooth step function
     step function
      $xStep
      r=2.6667 u=1.25 e=10.119 
      r=1. e=1.786 s=0.
  continue
*
* OBPDE:exact Riemann solver
   OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
   OBPDE:Godunov order of accuracy 2
*
 turn on axisymmetric flow
*
   boundary conditions
     all=slipWall
     shockTube(0,0)=superSonicOutflow
     * boundaries with bc=5 are the axis: 
     bcNumber5=axisymmetric
    done
*
   reduce interpolation width
   2
*
* turn on user defined output
*
   $amr
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
* 

movie mode
finish


