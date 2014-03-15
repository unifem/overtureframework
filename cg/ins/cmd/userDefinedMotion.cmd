*
* cgins: user defined motion of a annulus in a box
*
$tFinal=2.; $tPlot=.025; $show = " "; $nu=.1; 
$grid="cic.hdf"; $show = "stir.show"; 
*
$grid
* 
  incompressible Navier Stokes
  exit
  show file options
    * compressed
    open
     $show
    frequency to flush
      5
    exit
  turn off twilight zone
  project initial conditions
*
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  * no plotting
* 
  turn on moving grids
* 
  specify grids to move
   user defined
     sinusoidal motion
      * (x,y,z)(t) = (x0,x1,x2) + (d0,d1,d2){ [ 1-cos( (t-ta)*(omega *2*pi) ) ]^beta
      * x0,x1,x2, d0,d1,d2, ta, omega, beta
     $omega=2.; $ta=0.;
     0. 0. 0. .5 .5 .0 $ta $omega 1.
    done
*    rotate
*      0. 0. 0.
*    specify rate and rampInterval (rampInterval=0. => impulsive start, .5=slow start)
*      .5 .0
    Annulus
    done
  done
  * use implicit time stepping
  implicit
  choose grids for implicit
     all=explicit
     Annulus=implicit
    done
* 
  pde parameters
    nu $nu
  done
  boundary conditions
    all=noSlipWall
    done
  initial conditions
    uniform flow
     p=1.
  exit
* 
  continue
