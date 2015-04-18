*
* Cgins example: flow past a deforming circle 
*
* Usage:
*   
*  cgins [-noplot] deform -g=<name> -tz=<poly/trig/none> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*        -bc=<a|d|r>  -solver=<yale/best> -order=<2/4> -model=<ins/boussinesq> -ts=<implicit> -debug=<num> ..,
*        -ad2=<0|1> -bg=<backGround> -project=<0/1> -iv=[viscous/adv/full] -imp=<val> -rf=<val> ...
*        -constantVolume=[0|1] -volumePenalty=<val> -bc1=[inflow|pInflow] -sbc=[d|s] -pIn=<f> -go=[run/halt/og]
* 
*  -iv : implicit variation : viscous=viscous terms implicit, adv=viscous + advection, full=full linearized version
*  -imp : .5=CN, 1.=BE, 0.=FE
*  -rf : refactor frequency
*  -go : run, halt, og=open graphics
*  -ad2 : turn on or off the 2nd order artificial dissipation 
*  -dg : the name of thee grid to deform or -dg="share=<num>" to choose all grids with a given share value 
*  -df, -da : deformation frequency and amplitude. 
* 
* Generate the grid:
*     ogen -noplot circleDeformGrid -interp=e -factor=2
* Run:
*    cgins deform -g=circleDeformGride2.order2.hdf -dg=ice -nu=.02 -tf=2. -tp=.1 -model=ins -go=halt
* 
* --- set default values for parameters ---
* 
$grid="halfCylinder.hdf"; $backGround="backGround"; $bcn="noSlipWall"; 
$deformingGrid="ice"; $deformFrequency=2.; $deformAmplitude=1.; $deformationType="ice deform"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $nu=.05; $Prandtl=.72; $thermalExpansivity=.1; 
$gravity = "1. 0. 0.";   # NOTE: gravity must be in the x-direction for axisymmetric
* $gravity = "0. 0. 0."; 
$model="ins"; $ts="adams PC"; $noplot=""; $implicitVariation="viscous"; $refactorFrequency=100; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$solver="yale"; $rtol=1.e-4; $atol=1.e-6; $ogesDebug=0; $project=1; $cdv=1.; $ad2=0; $ad22=1.; 
$bc="a"; 
$rhoe=10.; $te=1.; $ke=1.; $be=0.1; $ad2e=5.; # ellastic shell parameters
$constantVolume=0; $volumePenalty=.5; 
* 
$bc1="inflow"; $pInflow=0.; 
$outflowOption="neumann";
$sbcl="d"; $sbcr="d"; # BC for surface, d=dirichlet, s=slide
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"bcn=s"=>\$bcn,\
 "iv=s"=>\$implicitVariation,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
  "bc=s"=>\$bc,"dg=s"=>\$deformingGrid,"dt=s"=>\$deformationType,"da=f"=>\$deformAmplitude,"df=f"=>\$deformFrequency,\
  "rhoe=f"=>\$rhoe,"te=f"=>\$te,"ke=f"=>\$ke,"be=f"=>\$be,"ad2e=f"=>\$ad2e, "constantVolume=i"=>\$constantVolume,\
  "volumePenalty=f"=>\$volumePenalty,"bc1=s"=>\$bc1,"pInflow=f"=>\$pInflow, "sbcl=s"=>\$sbcl, "sbcr=s"=>\$sbcr,\
  "deformFrequency=f"=>\$deformFrequency );
* -------------------------------------------------------------------------------------------------
$kThermal=$nu/$Prandtl; 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
* 
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
# 
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;}
# 
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* specify the overlapping grid to use:
$grid
* Specify the equations we solve:
  $model
  define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  done
* 
  show file options
    compressed
    open
      $show
    frequency to flush
      5 
    exit
* -- twilightzone options:
  $tz
  degree in space $degreex
  degree in time $degreet
  frequencies (x,y,z,t)   $fx $fy $fz $ft
* 
* choose the time stepping:
  $ts
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
          change hype parameters 0
          # -- elastic shell parameters
          # elastic shell parameters
          #   # te : surface tension, ke=spring restoring force, be=velocity damping, ad2e=art-diss
          #   $rhoe $te $ke $be $ad2e
          #
          elastic shell density: $rhoe
          elastic shell tension: $te
          elastic shell stiffness: $ke
          elastic shell damping: $be
          elastic shell dissipation: $ad2e
          volume penalty parameter: $volumePenalty
          constant volume $constantVolume
          # note: Dirichlet BC will be over-ridden if periodic
          if( $sbcl eq "d" ){ $sbcl="Dirichlet"; }else{  $sbcl="slide"; }
          if( $sbcr eq "d" ){ $sbcr="Dirichlet"; }else{  $sbcr="slide"; }
          BC left: $sbcl
          BC right: $sbcr
          BC bottom: $sbcl
          BC top: $sbcr
        #
        done
        if( $deformingGrid =~ /^share=/ ){ $deformingGrid =~ s/^share=//; \
                   $deformingGrid="choose grids by share flag\n $deformingGrid"; };
        $deformingGrid
        * north-pole
        * south-pole
        * choose grids by share flag
        *   100
     done
  done
***************************
  useNewImplicitMethod
  $implicitVariation
  refactor frequency $refactorFrequency
  choose grids for implicit
    all=implicit
*     square=explicit
    done
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  dtMax $dtMax
* 
* 
  plot and always wait
  * no plotting
  pde parameters
    nu $nu
    kThermal $kThermal
    gravity
      $gravity
* 
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22  $ad22, $ad22
    OBPDE:divergence damping  $cdv 
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
  done
  pressure solver options
     $solver
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
  implicit time step solver options
     $solver
     relative tolerance
       $rtol
     absolute tolerance
       $atol 
    exit
* 
  boundary conditions
    $u=1.; $T=1.; 
    * all=slipWall
    * annulus(0,1)=noSlipWall, uniform(T=$T)
    all=$bcn, uniform(T=$T)
*     all=slipWall, uniform(T=$T)
    $backGround=slipWall
    # $backGround(0,0)=inflowWithVelocityGiven, uniform(u=$u,T=0.)
    # $backGround(1,0)=outflow, pressure(1.*p+.1*p.n=0.)
    if( $bc1 eq "inflow" ){ $cmd="bcNumber1=inflowWithVelocityGiven, uniform(u=$u,T=0.)"; }
    if( $bc1 eq "pInflow" ){ $cmd="bcNumber1=inflowWithPressureAndTangentialVelocityGiven, uniform(p=$pInflow))"; }
    $cmd
    #
    bcNumber2=outflow, pressure(1.*p+.1*p.n=0.)
  done
* 
  initial conditions
   if( $tz eq "turn off twilight zone" ){ $ic = "uniform flow\n p=1., u=$u, T=0. \n done";}else{ $ic = "done"; }
   $ic 
  done
  debug $debug
  $project
*
*-    output options...
*-    frequency to save probes 2
*-    create a probe
*-      file name probeFile.dat
*-      nearest grid point to -.5 .1 0.
*-      exit
*-    close output options
*
  exit
  $go


* 
      erase
      grid
        bigger:0
        exit this menu
