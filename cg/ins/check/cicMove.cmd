************************************************************
* translation example
************************************************************
*
$show = " "; $tPlot=.1; $nu=.1; 
$grid="cic2.hdf"; 
*  $grid="cic3.hdf";
*  $grid="cic4.hdf";
*
$grid
* 
  incompressible Navier Stokes
  exit
*
  show file options
   open
     $show
    frequency to flush
      1 
  exit  
*
  turn off twilight zone
* 
  final time .3
  times to plot $tPlot
  plot and always wait
*
  project initial conditions
  turn on moving grids
*  detect collisions
***********
  specify grids to move
      translate
      1. 0. 0.
      -.5
        Annulus
      done
  done
***************
  * use full implicit system 1
  * use implicit time stepping
  implicit
  choose grids for implicit
*     all=implicit
    all=explicit
    Annulus=implicit
    done
  pde parameters
    nu $nu
   done
*
  boundary conditions
     all=noSlipWall
     square(0,0)=inflowWithVelocityGiven, uniform(u=1.)
     square(1,0)=outflow
     square(0,1)=slipWall
     square(1,1)=slipWall
   done
*
  initial conditions
    uniform flow
     u=1., v=0., p=1.
  exit
*
  continue
*
movie mode
finish


