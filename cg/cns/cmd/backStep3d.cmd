*
* cgcns command file for a 3d shock past a backward facing step
*
* Usage:
*    cgcns [-noplot] backStep3d.cmd -g=<grid>  -amr=[0|1] -l=<levels> -r=<ratio> -tf=<final time> ...
*          -tp=<tPlot> -xs=<xstep> -show=<show file> 
*
* Examples:
*     cgcns backStep3d.cmd -g=backStepSmooth3de1.order2 -amr=0 -tf=1. -tp=.1 -xStep="x=-1.5"
*     cgcns backStep3d.cmd -g=backStepSmooth3de1.order2 -l=2 -r=2 -tf=1. -tp=.1 -xStep="x=-1.5" 
*     cgcns backStep3d.cmd -g=backStepSmooth3de2.order2 -l=2 -r=2 -tf=1. -tp=.1 -xStep="x=-1.5"
*     cgcns backStep3d.cmd -g=backStepSmooth3de1.order2 -l=2 -r=2 -tf=1. -tp=.1 -xStep="x=-1.5" -show="backStep3d.show"
*     
*
$tFinal=1.; $tPlot=.1; $show="";
$grid="backStepSmooth3de1.order2.hdf"; $show = " "; $cnsVariation="godunov"; 
$amr=1; $numberOfLevels=2;  $refinementRatio=4;  $regrid=2; 
* --- set default values for parameters ---
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tFinal=1.; $tPlot=.1; $cfl=1.; $debug=1; $tol=.2; $x0=.5; $dtMax=1.e10; $nbz=2; 
$xStep="x=-1.5"; $go="halt"; 
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal,"debug=i"=> \$debug, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "cnsVariation=s"=>\$cnsVariation );
* -------------------------------------------------------------------------------------------------
if( $amr eq "0" ){ $amr="turn off adaptive grids"; }else{ $amr="turn on adaptive grids"; }
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
* 
  turn off twilight
  final time $tFinal
  times to plot $tPlot
  plot and always wait
*  no plotting
  show file options
    uncompressed
      open
        $show
      frequency to flush
        2
      exit
*
  pde parameters
    mu
      0.
    kThermal
      0.
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
  reduce interpolation width
    2
  boundary conditions
    all=slipWall
    bcNumber2=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
    bcNumber3=superSonicOutflow
    done
* 
  cfl $cfl
* 
  $amr
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
      r=2.6667 u=1.25 e=10.119 
      r=1. e=1.786 s=0.
  continue
continue
*
$go

