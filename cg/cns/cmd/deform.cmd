*
* cgcns example: test deforming grids (e.g. flow past a deforming disk)
*
* Usage:
*   
*  cgcns [-noplot] deform -g=<name> -tz=<poly/trig/none> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*        -bc=<a|d|r> -model=<Jameson/Godunov> -debug=<num> -bg=<backGround> -uInflow=<> -mu=<> -go=[run/halt/og]
* 
*  -go : run, halt, og=open graphics
*  -uInflow : inflow velocity
*  -dg : the name of the grid to deform or -dg="share=<num>" to choose all grids with a given share value 
*  -df, -da : deformation frequency and amplitude. 
* 
* Examples: (see ogen scripts: circleDeform.cmd)
* 
*  cgcns deform -g=circleDeform -dg=ice -tf=2. -tp=.05 -go=halt 
* 
* -- deforming ellipse: 
*  cgcns deform -g=diskDeformoutere1.hdf -bg=outerSquare -dg=outerInterface -dt="ellipse deform" -da=2. -tf=2. -tp=.02 -go=halt 
*   -- shock hitting a deforming ellipse with AMR on 
*  cgcns deform -g=diskDeformOuterBige1 -bg=outerSquare -dg=outerInterface -dt="ellipse deform" -da=2. -tf=2. -tp=.05 -go=halt 
* 
* -- deforming circle with 2 grids on each side of the interface
* cgcns deform -g=diskDeformSplitoutere2 -bg=outerSquare -dg="outerInterface1\n outerInterface2" -dt="ellipse deform" -da=2. -tf=2. -tp=.02 -go=halt
* 
* -- deforming SPHERE
* cgcns deform -g=sibDeforme2.order2 -bg=box -dg="share=100" -dt="sphere deform" -da=1. -tf=2. -tp=.1 -go=halt
* 
* cgcns noplot deform -g=sibDeforme4.order2 -bg=box -dg="share=100" -dt="sphere deform" -da=1. -tf=6. -tp=.02 -df=.5 -go=go -show=sibDeform.show
* 
* -- advect
*  cgcns deform -g=diskDeformoutere1 -bcn=slipWall -bg=outerSquare -dg=outerInterface -dt="advect body" -tf=2. -tp=.02 -go=halt
* 
* -- elasticShell -- simple spring model 
*  cgcns deform -g=diskDeformoutere1 -bg=outerSquare -dg=outerInterface -dt="elastic shell" -tf=20. -tp=.1 -go=halt
* 
* -- userDeformingSurface 
*    (make the grid using Overture/sampleGrids/diskDeform.cmd, ogen noplot diskDeform -case=outer -factor=1 -interp=e)
*  cgcns deform -g=diskDeformoutere1 -bcn=slipWall -bg=outerSquare -dg=outerInterface -dt="user defined deforming surface" -tf=5. -tp=.2 -go=halt
* 
* -- moving plane interface --
* cgcns deform -g=planeInterfacenp2.hdf -bg=none -dg=interface -dt="advect body" -tf=2. -tp=.001 -go=halt 
*
* --- set default values for parameters ---
* 
$model="Godunov"; 
$grid="halfCylinder.hdf"; $backGround="backGround"; $bcn="slipWall"; $uInflow=.1; 
$deformingGrid="ice"; $deformFrequency=.5; $deformAmplitude=1.; $deformationType="ice deform";
$tFinal=1.; $tPlot=.1; $cfl=.9; $mu=.0; $Prandtl=.72; $thermalExpansivity=.1; 
$noplot=""; $debug = 0;  
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$ad2=0; $ad22=2.; 
$bc="a"; 
* 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"bcn=s"=>\$bcn,\
  "bc=s"=>\$bc,"dg=s"=>\$deformingGrid,"dt=s"=>\$deformationType,"da=f"=>\$deformAmplitude,"df=f"=>\$deformFrequency );
* -------------------------------------------------------------------------------------------------
$kThermal=$mu/$Prandtl;   # check this 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "Godunov" ){ $model = "compressible Navier Stokes (Godunov)"; }else\
                     { $model = "compressible Navier Stokes (Jameson)"; }
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* specify the overlapping grid to use:
$grid
  $model
  done
* 
  show file options
    compressed
    open
      $show
    frequency to flush
      10
    exit
* -- twilightzone options:
  $tz
  degree in space $degreex
  degree in time $degreet
  frequencies (x,y,z,t)   $fx $fy $fz $ft
* 
*****************************
  turn on moving grids
  specify grids to move
      deforming body
        user defined deforming body
          $deformationType
          deformation frequency
            $deformFrequency
          deformation amplitude
            $deformAmplitude
          elastic shell parameters
            $rhoe=10.; $te=1.; $ke=5.; $be=0.1; $ad2=1.; 
            $rhoe=10.; $te=1.; $ke=3.; $be=0.1; $ad2=5.; 
            $rhoe $te $ke $be $ad2
        * test grid history:
        *th  provide past history
        done
        if( $deformingGrid =~ /^share=/ ){ $deformingGrid =~ s/^share=//; \
                   $deformingGrid="choose grids by share flag\n $deformingGrid"; };
        $deformingGrid
        * north-pole
        * south-pole
        * choose grids by share flag
        *   100
     done
     * test: provide past time grids
     *th deforming grid history
     *th  diskDeformoutere1.hdf
     *th  -.05
     *th  done
     *th done
  done
***************************
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
* 
* 
  plot and always wait
  * no plotting
*
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
* 
  boundary conditions
    $rho=1.; $T=300.;
    all=slipWall
    $backGround(0,0)=subSonicInflow uniform(r=$rho,u=$uInflow,T=$T)
    $backGround(1,0)=subSonicOutflow mixedDerivative(1.*t+1.*t.n=$T)
  done
* 
  initial conditions
    uniform flow
      r=$rho u=$uInflow T=$T
  exit
  debug $debug
  exit
  $go
* 
      erase
      grid
        bigger:0
        exit this menu
