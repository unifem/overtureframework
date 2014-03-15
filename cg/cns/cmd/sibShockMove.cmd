***************************************************************
*  sibShockMove.cmd -- cgcns example --
* 
*     Shocking hitting a moving sphere
* 
*     You may have to make the grid used below (sampleGrids/sibArg.cmd)
**************************************************************
*
$tFinal=2.; $tPlot=.1; $show=" ";
$ratio=2; $levels=2; $bufferZones=2; $tol=.01; 
$amrOff = "turn off adaptive grids";
$amrOn = "turn on adaptive grids";
$amr=$amrOff;
$hybrid = "use hybrid grid for surface integrals";
* $hybrid = "do not use hybrid grid for surface integrals";
* 
$grid="sibi1.order2.hdf"; $amr=$amrOn; $tPlot=.05;
* $grid="sibi2.order2.hdf"; $amr=$amrOn; $tPlot=.05;
* $grid="sibi4.order2.hdf"; $amr=$amrOff; $tPlot=.05;
* $grid="sibi8.order2.hdf"; $amr=$amrOff; $tPlot=.1;
* 
*
$grid
*
*  Either Jameson or Godunov should work
*   compressible Navier Stokes (Jameson)
  compressible Navier Stokes (Godunov)
*   one step
  exit
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time $tFinal
  times to plot $tPlot
*plot and always wait
*
  show file options
    compressed
    open
      $show
    frequency to flush
      1
    exit
  * no plotting
****
* turn on user defined output
****
  turn on moving grids
  $pi =3.141592653;
  * $radius=.35;
  $radius=.25;
  $density = 1.;  $volume = 4./3.*$pi*$radius**3; $mass=$volume*$density;
  specify grids to move
      $hybrid
      rigid body
        density
          $density
        mass
           $mass
         moments of inertia
           $momentOfInertia=2./5.*$mass*$radius**2;
           $momentOfInertia $momentOfInertia $momentOfInertia
         initial centre of mass
            0. 0. 0. 
         * initial velocity
         done
 	north-pole
	south-pole
      done
* ------ start ugen ---
        * use advancing front
        Advancing Front...
        plot all faces 1
        plot all edges 1
        exit

        enlarge hole
        edge growth factor 1
        continue generation
        pause
        exit
pause
* ----------- end ugen -----
  done
**********
  reduce interpolation width
    2
*****
  $amr
  order of AMR interpolation
      2
  error threshold
     $tol
  regrid frequency
    $regrid=$ratio*$bufferZones;
    $regrid
  change error estimator parameters
    set scale factors
      1 1 1 1 1 1 
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
      $bufferZones
    grid efficiency
      .7
  exit
*****
  boundary conditions
    all=slipWall
    box(0,0)=superSonicInflow uniform(r=2.6069,T=.943011,u=0.694444,v=0.,s=0.0)
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
***************
*  cfl
*   .5 
*    .95
*   OBPDE:exact Riemann solver
* OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
*   OBPDE:Godunov order of accuracy 2
******************
*  debug
*    1
*
  initial conditions
    step function
    x=-1.
    T=.943011, u=.694444, v=0., r=2.6069
    T=.714286, u=0., v=0., r=1.4
  continue
continue
*
movie mode
finish
