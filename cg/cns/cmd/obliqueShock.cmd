*
*  cgcns command file: oblique shock -- exact solution
*     Solve the Euler equations with Godunov's method and AMR
* 
* Usage:
*    cgcns [-noplot] obliqueShock.cmd -g=<grid> -l=<levels> -r=<ratio> -tf=<final time> ...
*          -tp=<tPlot> -xs=<xstep> -show=<show file> 
*
* Examples:
*   cgcns obliqueShock.cmd -g=square20.hdf -l=2 -r=2 -tf=1. -tp=.1 
*   cgcns obliqueShock.cmd -g=square256.hdf -l=2 -r=2 -tf=.5 -tp=.1
*
* --- set default values for parameters ---
$grid="square20.hdf"; $show = " "; $backGround="square"; $cnsVariation="godunov"; 
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tFinal=1.; $tPlot=.1; $cfl=.9; $debug=1; $tol=.2; $x0=.5; $dtMax=1.e10; $nbz=2; 
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
  * no plotting
  reduce interpolation width
    2
  boundary conditions
    all=dirichletBoundaryCondition
    # * all=noSlipWall uniform(T=.3572)
    # all=slipWall 
    # $backGround(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
    # $backGround(1,0)=superSonicOutflow
    # $backGround(0,1)=slipWall
    # $backGround(1,1)=slipWall
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
* 
  cfl $cfl
* 
  OBTZ:user defined known solution
    oblique shock flow
#  --- steady shock ---
#*  # state 1 : ahead of the shock: r,v1,v2,v3,T
#*  1.114827e-01 -1.000000e+00 0.000000e+00 0.000000e+00 9.375000e-02
#*  # state 2 : behind the shock: r,v1,v2,v3,T
#*  4.032762e-01 -2.790852e-01 -4.364727e-02 0.000000e+00 2.252081e-01
#*  # The normal to the shock [n1,n2,n3]
#*  9.981722e-01 -6.043363e-02 0.000000e+00
#*  # A point on the shock
#*  .5 .5 0.
#*  # Shock speed
#*  0.000000e+00
#   --- moving shock ---
# xi=6.047047e-02, theta=1.551374e-01, beta=1.510326e+00, normalOption=1 (0=deformed, 1=un-deformed)
# state 1 : ahead of the shock: r,v1,v2,v3,T
1.114827e-01 0.000000e+00 0.000000e+00 0.000000e+00 9.375000e-02
# state 2 : behind the shock: r,v1,v2,v3,T
4.032762e-01 7.209148e-01 -4.364727e-02 0.000000e+00 2.252081e-01
# The normal to the shock [n1,n2,n3]
9.981722e-01 -6.043363e-02 0.000000e+00
# A point on the shock
.25 .5 0.
# Shock speed
9.981722e-01
* 
**    1. 0. 0. 0. 2.
**    2. .5 .6 0. 3.
**    1. -.5 0
**    .5 .5 0.
**    .5
  done
  debug $debug
****
*
  turn on adaptive grids
**  turn off adaptive grids
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
#  initial conditions
# *    smooth step function
#      step function
#       $xStep
# *        5.
# #       r=2.6667 u=1.25 e=10.119 
#       r=1. e=1.786 s=0.
#   continue
continue
*
$go
