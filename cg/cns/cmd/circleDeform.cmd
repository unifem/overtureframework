***************************************************************
*  Example showing a deforming circle or cylinder
*  You may have to make the grid:  
*    (1) 2D:  circleDeform.hdf used below
*    (1) 3D:  cylDeformi2.hdf
**************************************************************
*
$tFinal=.5; $tPlot=.02; $show = " "; $deformFrequency=2.; 
*
* $grid="circleDeform.hdf";  $deformingGrid="ice"; 
$grid="circleDeform2.hdf";  $deformingGrid="ice"; 
* 
* $grid="cylDeformi1.hdf";  $deformingGrid="deformVolume"; $tPlot=.001; $deformFrequency=50.; 
* $grid="cylDeformi1.hdf";  $deformingGrid="deformVolume"; $tFinal=.25; $tPlot=.05; $deformFrequency=2.; $show="cylDeform1.show";
* $grid="cylDeformi2.hdf";  $deformingGrid="deformVolume"; $tFinal=.25; $tPlot=.05; $deformFrequency=2.; $show="cylDeform2.show";
* $grid="cylDeformi2.hdf";  $deformingGrid="deformVolume"; $tPlot=.05; $deformFrequency=2.; $show="cylDeform.show"; 
* $grid="cylDeformi4.hdf";  $deformingGrid="deformVolume"; $tPlot=.05; $deformFrequency=1.; $show="cylDeform.show"; 
*
$grid
*
  compressible Navier Stokes (Jameson)
*   compressible Navier Stokes (Godunov)
*   one step
  exit
  turn off twilight
*
  final time $tFinal
*
  times to plot $tPlot
*
  show file options
    compressed
    open
      $show
    frequency to flush
      2
    exit
  * no plotting
*****************************
  turn on moving grids
  specify grids to move
      deforming body
        user defined deforming body
        deformation frequency
          $deformFrequency
        done
        $deformingGrid
     done
  done
***************************
  reduce interpolation width
    2
 maximum number of iterations for implicit interpolation
    10
  boundary conditions
    $rho=1.; $u=10.; $T=300.;
    all=slipWall
    backGround(0,0)=subSonicInflow uniform(r=$rho,u=$u,T=$T)
    backGround(1,0)=subSonicOutflow mixedDerivative(1.*t+1.*t.n=$T)
    done
*
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
*  debug
*    1
*
  initial conditions
    uniform flow
      r=$rho u=$u T=$T
  exit
continue
*

movie mode
finish



grid
exit


erase
streamlines
exit

