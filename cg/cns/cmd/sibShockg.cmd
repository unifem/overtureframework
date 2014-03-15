*
*  cgcns command file: shock hitting a sphere (3d)
*     Solve the Euler equations with Godunov's method and AMR
* 
* Usage:
*    cgcns [-noplot] sibShockg.cmd -g=<grid> -l=<levels> -r=<ratio> -tf=<final time> ...
*          -tp=<tPlot> -xs=<xstep> -show=<show file> 
*
* Examples:
*     cgcns sibShockg.cmd -g=sib.hdf -l=2 -r=2 -tf=1. -tp=.1 -xs="x=-1."
*     cgcns sibShockg.cmd -g=sibe2.order2.hdf -l=3 -r=2 -tf=1. -tp=.1 -xs="x=-.75"
*     srun -N4 -n32 -ppdebug $cgcnsp -noplot sibShockg -g=sibe2.order2.hdf -l=2 -r=2 -tf=1. -tp=.1 -xs="x=-.75" >! sib2l2r2.N4n32.out & 
*
* --- set default values for parameters ---
$show = " "; 
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tFinal=1.; $tPlot=.1; $cfl=1.; $debug=1; $tol=.2; $x0=.5; $dtMax=1.e10; $nbz=2; $noplot=""; 
$xStep="x=-1.5";
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal, \
            "tp=f"=>\$tPlot, "xs=s"=>\$xStep, "show=s"=>\$show, "noplot=s"=>\$noplot );
* -------------------------------------------------------------------------------------------------
*
* Here is the overlapping grid to use:
$grid
*
  compressible Navier Stokes (Godunov)  
*    compressible Navier Stokes (Jameson)
*   one step
  exit
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  no plotting
  show file options
    compressed
    open
     $show
    frequency to flush
      10
    exit
  reduce interpolation width
    2
  boundary conditions
    * all=noSlipWall uniform(T=.3572)
    all=slipWall 
    box(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
    box(1,0)=superSonicOutflow
    box(0,1)=slipWall
    box(1,1)=slipWall
    box(0,2)=slipWall
    box(1,2)=slipWall
    done
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
  cfl $cfl
  debug
   $debug
*
  turn on adaptive grids
*   save error function to the show file
  show amr error function
  order of AMR interpolation
      2
  error threshold
     $tol
  regrid frequency
    $regrid=$nbz*$ratio;
    $regrid
  change error estimator parameters
    default number of smooths
      1
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $nrl
    number of buffer zones
      $nbz
    grid efficiency
      .7 .5 
  exit
*
******
   initial conditions
      step function
       $xStep
       r=2.6667 u=1.25 e=10.119 
       r=1. e=1.786 s=0.
   continue
****** for a restart
*  initial conditions
*    read from a show file
*     sibShockg.show
*      -1
*  exit
*****
continue
* 
movie mode
finish
















  continue
  grid
    raise the grid by this amount (2D) 1.75
    plot non-physical boundaries (toggle) 1
    colour boundaries black
    exit this menu


*
  times to plot 0.01
  final time 1.7
  hardcopy resolution:0 2048
  contour
*   rho: .1 to 5.2
    set min and max
    .1 5.2
    plot contour lines (toggle)
    exit this menu
  grid
    plot non-physical boundaries (toggle) 1
    plot grid lines (toggle) 0
    raise the grid by this amount (2D) 6.
    colour boundaries by refinement level number
    exit this menu
*

  movie and save
  amrShock



*contour
*wire frame (toggle)
*exit


continue
continue
continue
continue
continue


