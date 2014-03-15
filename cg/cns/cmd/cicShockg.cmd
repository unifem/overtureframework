*
*  cgcns command file: shock hitting a cylinder
*     Solve the Euler equations with Godunov's method and AMR
* 
* Usage:
*    cgcns [-noplot] cicShockg.cmd -g=<grid> -l=<levels> -r=<ratio> -tf=<final time> ...
*          -tp=<tPlot> -xs=<xstep> -show=<show file> 
*
* Examples:
*     cgcns cicShockg.cmd -g=cice2.order2.hdf -l=2 -r=2 -tf=1. -tp=.1 -xStep="x=-1.5" -show="cicShockg.show"
*     cgcns cicShockg.cmd -g=cice2.order2.hdf -l=3 -r=2 -tf=1. -tp=.1 -xStep="x=-1.5"
*     cgcns cicShockg.cmd -g=cice3.order2.hdf -l=2 -r=2 -tf=1. -tp=.1 -xStep="x=-1.5"
* 
* -- Jameson:
*     cgcns cicShockg.cmd -g=cice2.hdf -l=2 -r=2 -tf=1. -tp=.1 -xStep="x=-1.5" -cnsVariation="jameson"
*    
* -- parallel: 
*     mpirun -np 2 $cgcnsp cicShockg.cmd -g=cice2.order2 -l=2 -r=2 -tf=.2 -tp=.1 -xStep="x=-1.5" -show="cicShockg.show"
*     mpirun -np 2 $cgcnsp cicShockg.cmd -g=cice2.order2 -l=1 -r=2 -tf=.2 -tp=.1 -xStep="x=-1.5"
* 
*   mpirun -np 2 $cgcnsp noplot cicShockg.cmd -g=cicmp.hdf -l=3 -r=2 -tf=.02 -tp=.01 -xStep="x=0." -bg=channel
*
*   srun -N1 -n2 -ppdebug $cgcnsp noplot cicShockg.cmd -g=cice2.hdf -l=2 -r=2 -tf=.2 -tp=.05 -xStep="x=-1.5" -show="cicShockg.show"
*
* --- set default values for parameters ---
$grid="cice2.order2.hdf"; $show = " "; $backGround="square"; $cnsVariation="godunov"; 
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tFinal=1.; $tPlot=.1; $cfl=1.; $debug=1; $tol=.2; $x0=.5; $dtMax=1.e10; $nbz=2; 
$xStep="x=-1.5"; $go="halt"; 
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal,"debug=i"=> \$debug, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "cnsVariation=s"=>\$cnsVariation );
* -------------------------------------------------------------------------------------------------
if( $cnsVariation eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $cnsVariation eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $cnsVariation eq "nonconservative" ){ $pdeVariation="compressible Navier Stokes (non-conservative)";}  #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* Here is the overlapping grid to use:
$grid 
*
$pdeVariation
**   compressible Navier Stokes (Godunov)  
**  compressible Navier Stokes (Jameson)
*
*   one step
*   add extra variables
*     1
  exit
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time $tFinal
  times to plot $tPlot
* no plotting
  plot and always wait
*
  show file options
    compressed
    * specify the max number of parallel hdf sub-files: 
    OBPSF:maximum number of parallel sub-files 2
    open
     $show
    frequency to flush
      20
    exit
* -- specify which variables will appear in the show file:
*     showfile options...
*     OBPSF:show variable: rho 1
*     OBPSF:show variable: u 0
*     OBPSF:show variable: v 0
*     OBPSF:show variable: w 0
*     OBPSF:show variable: T 0
*     OBPSF:show variable: Mach Number 0
*     OBPSF:show variable: p 1
*     close show file options
* 
  * no plotting
  reduce interpolation width
    2
  boundary conditions
    * all=noSlipWall uniform(T=.3572)
    all=slipWall 
    $backGround(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
    $backGround(1,0)=superSonicOutflow
    $backGround(0,1)=slipWall
    $backGround(1,1)=slipWall
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
*  .7 ok, .8 not ok 
*  cfl .8 .7  .85  .75
***
  * debug = 1 + 2 + 4 + 8
  debug $debug
****
*
  turn on adaptive grids
*   save error function to the show file
**  show amr error function 1
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
*    width of proper nesting 
*      1
    grid efficiency
      .7 .5 
  exit
*
  initial conditions
*    smooth step function
     step function
      $xStep
*        5.
      r=2.6667 u=1.25 e=10.119 
      r=1. e=1.786 s=0.
  continue
continue
*
$go
