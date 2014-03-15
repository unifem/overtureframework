*
* CGCNS file: shock hitting a flexible beam
* 
* Usage:
*    cgcns [-noplot] beam_in_channel_dynamic_with_beam.cmd -g=<grid> -tf=<final time> ...
*          -tp=<tPlot>  
*
* Examples:
*     cgcns beam_in_channel__dynamic_with_beam_run.cmd -g="beam_in_channel_dynamic_with_beam_gride4.hdf" -tf=1. -tp=.1
*
*
* --- set default values for parameters ---
$grid="../grid/beam_in_channel_statice2.hdf"; $show = " "; $backGround="rectangle4"; $cnsVariation="godunov"; 
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tFinal=3.; $tPlot=.1; $cfl=1.; $debug=1; $tol=.2; $x0=.5; $dtMax=1.e10; $nbz=2; 
$xStep="x=0.03"; $go="halt"; 
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
  turn on moving grids
  specify grids to move
      deforming body
        user defined deforming body
          elastic beam
          elastic beam parameters 
            # Enter I,E,rho,L,t,pnorm,x0,y0,dec
            8.333e-11 2.42e11 7.6e3 0.04 1e-3 1.0 0.05 0.0 90.0
            # Enter nelem, bcl, bcr, exact (default=(%d,%d,%d,%d) (0=cantilevered, 1=pinned, 2=free)"
            5 0 2 0
           boundary parameterization
             1
           sub iteration convergence tolerance
            1e-2
          BC left: Dirichlet
          BC right: Dirichlet
          BC bottom: Dirichlet
          BC top: Dirichlet
        #
        done
        choose grids by share flag
          1
     done
  done
*
*  do not use iterative implicit interpolation
*
  final time $tFinal
  times to plot $tPlot
* no plotting
  plot and always wait
*
  show file options
    open
     $show
    frequency to flush
      20
    exit
# * -- specify which variables will appear in the show file:
#      showfile options...
#      OBPSF:show variable: rho 1
#      OBPSF:show variable: u 0
#      OBPSF:show variable: v 0
# *     OBPSF:show variable: w 0
# *     OBPSF:show variable: T 0
#      OBPSF:show variable: Mach Number 0
#      OBPSF:show variable: p 1
#      close show file options
* 
  reduce interpolation width
    2
  boundary conditions
    * all=noSlipWall uniform(T=.3572)
    all=slipWall 
    $backGround(0,0)=superSonicOutflow uniform(r=1.6158,u=0.3468,T=0.9540)
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
*  turn on adaptive grids
*   save error function to the show file
**  show amr error function 1
*  order of AMR interpolation
*      2
*  error threshold
*     $tol 
*  regrid frequency
*    $regrid=$nbz*$ratio;
*    $regrid
*  change error estimator parameters
*    default number of smooths
*      1
*    set scale factors     
*      2 1 1 1 1 
*    done
*    exit
*  change adaptive grid parameters
*    refinement ratio
*      $ratio
*    default number of refinement levels
*      $nrl
*    number of buffer zones
*      $nbz
*    width of proper nesting 
*      1
*    grid efficiency
*      .7 .5 
*  exit
*
  initial conditions
*    smooth step function
     step function
      $xStep
*        5.
      r=1.6158 u=109.2 e=3.86e5
      r=1.189 e=2.5e5 s=0
  continue
continue
*
$go
