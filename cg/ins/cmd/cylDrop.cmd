*
* A dropping cylinder -- compare to the results frm Glowinski, Pan, et.al.dropping cylinders
*
*  cgins cylDrop -g=cylDrop.hdf -tf=1. -tp=.05 -nu=.1 -density=1.25 -dtMax=.75e-3 -go=halt 
* 
*
$nu =.1;
$show = " "; $go="halt"; 
$solver = "choose best iterative solver";
* $solver = "yale";
* $solver = "multigrid";
$tolerance = "1.e-6";
$tFinal=1.;
$sep = 3.;
$cdv = 1.;
$g = -981.; # cm/s^2   -9.81 acceleration due to gravity standard value: 9.80665 m/s^2.
$forceLimit = 30.;
$density=1.25;
$dtMax = .1; # 1.5e-3; 
*
* -- old way:
* $grid = "cylDrop.hdf"; $show="cylDropi2.show"; $tFinal=1.; $tPlot=.05; $nu=.1; $density=1.25; $dtMax=.75e-3; 
* $grid = "cylDrop.hdf"; $show="cylDrope.show"; $tFinal=1.; $tPlot=.05; $nu=.1; $density=1.25;
* $grid = "cylDrop2.hdf"; $show="cylDrop2.show"; $tPlot=.05; $nu=.05; 
* $grid = "cylDrop4.hdf"; $show="cylDrop4.show"; $tPlot=.025; $nu=.01; $forceLimit = 40.;
* $grid = "cylDrop2.hdf"; $show="cylDrop2.show"; $tFinal=.5; $tPlot=.05; $nu=.01; $density=1.5; $forceLimit = 30.;
* $grid = "cylDrop2.hdf"; $show="cylDrop2A.show"; $tFinal=.75; $tPlot=.05; $nu=.05; $density=1.5; $forceLimit = 30.;
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"show=s"=>\$show,"nu=f"=>\$nu, \
            "density=f"=>\$density,"dtMax=f"=>\$dtMax,"go=s"=>\$go );
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
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
      1 2 1 2
  exit  
  turn off twilight zone
  degree in space
    1
**********************************************
  final time $tFinal 
  times to plot $tPlot
  dtMax $dtMax 
*
*
*cfl
*  .25  .75 .1 .5   .75 
*
  recompute dt every 10  * 5 *  1 * 2 * 5 * 10 * 1 
*
  plot and always wait
**  no plotting
*****************************************
  turn on moving grids
**************************
  detect collisions 1
  minimum separation for collisions $sep
*************************
  specify grids to move
      *
*      improve quality of interpolation
     limit forces
       $forceLimit $forceLimit
      *
      rigid body
        * mass
        *   .25
        density
          $density
        moments of inertia
          $pi=3.141592653; 
          $radius=.125; $volume=$pi*$radius**2; $mass=$density*$volume;
          $momentOfInertia=.5*$mass*$radius**2;
          $momentOfInertia
        done
        drop
       done
  done
  * use implicit time stepping
  implicit
  choose grids for implicit
     all=explicit
     drop=implicit
    done
  pde parameters
    nu
     $nu 
*   -- specify the fluid density
   fluid density
     1.
*  turn on gravity
  gravity
     0. $g 0.
    OBPDE:second-order artificial diffusion 1
    OBPDE:ad21,ad22  2,2  * 10,10  * 2,2  ** 10,10
    OBPDE:divergence damping  $cdv 
    done
  boundary conditions
    all=noSlipWall
*     channel(0,1)=inflowWithVelocityGiven,  parabolic(d=.2,p=1,v=$vIn)
*     channel(0,1)=inflowWithVelocityGiven,  uniform(u=0.,v=$vIn)
*     channel(1,1)=outflow , pressure(1.*p+0.0*p.n=0.)
   done
****************************************************
**  project initial conditions -- is this correct in this case ?? 
 initial conditions
   uniform flow
    p=1., 0.
 exit
*  initial conditions
*     read from a show file
*      twoDropNew2.show
*       24 48  -1 35 -1 1 4 -1 10 18  -1
*  exit
*
***************************************************
   pressure solver options
     $solver 
     relative tolerance
       $tolerance
     absolute tolerance
       $tolerance
      multigrid parameters
      *  alternating
      * cycles: 1=V, 2=W
      number of cycles
        2
      residual tolerance
         $tolerance
      error tolerance
         $tolerance
      maximum number of iterations
        50
      maximum number of interpolation iterations
         10
 *      maximum number of levels
 *        5 4 3 2
 *      maximum number of extra levels
 *        4 3 2 1
      debug
        1 3 7 3 1 0 3
      exit
    exit
***************
*  debug
*     63
  continue
*
    plot:v
*
*
  $go



