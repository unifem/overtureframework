*
* cgcns example: test moving grids
*
* Usage:
*   
*  cgcns [-noplot] deform -g=<name> -tz=<poly/trig/none> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*        -bc=<a|d|r> -cnsVariation=<Jameson/Godunov> -debug=<num> -bg=<backGround> -uInflow=<> -mu=<> -go=[run/halt/og]
* 
*  -go : run, halt, og=open graphics
*  -mg : the name of the grid to move 
*  -mt : "move type" : shift, oscillate
* 
* Examples: 
* 
* -- moving plane interface --
* cgcns move -g=planeInterfacenp2.hdf -bg=none -mg=interface -mt=shift -tf=2. -tp=.02 -go=halt 
* 
*  -- note for TZ with moving grids : if ue=X(x)*T(t) and x=G(r,t) then the "real" exact solution is X(G(r,t))*T(t) 
* cgcns move -g=square5 -bg=none -mg=square -mt=shift -tz=poly -degreex=1 -degreet=1 -cnsVariation=nonconservative -tf=2. -tp=.02 -go=halt -debug=1  [exact]
* mpirun -np 1 $cgcnsp move -g=square5 -bg=none -mg=square -mt=shift -tz=poly -degreex=1 -degreet=1 -cnsVariation=nonconservative -tf=2. -tp=.02 -go=halt -debug=1  [exact]
*
* cgcns move -g=planeInterface1 -bg=none -mg=interface -mt=shift -tz=poly -degreex=1 -degreet=1 -cnsVariation=nonconservative -tf=2. -tp=.02 -go=halt -debug=1  [exact]
* cgcns move -g=planeInterface1 -bg=none -mg=interface -mt="advect body" -vg0=1. -tz=poly -degreex=1 -degreet=1 -cnsVariation=nonconservative -tf=2. -tp=.01 -go=halt -debug=1 [exact]
* cgcns move -g=planeInterface0 -bg=none -mg=interface -mt="advect body" -vg0=1. -tz=poly -degreex=1 -degreet=1 -cnsVariation=nonconservative -tf=2. -tp=.01 -go=halt -debug=15 >! junk 
*
* --- set default values for parameters ---
* 
$cnsVariation="Godunov"; $ts="pc"; 
$grid="halfCylinder.hdf"; $backGround="backGround"; $bcn="slipWall"; $uInflow=.1; 
$mg="square"; $mt="shift"; $vg0=0.; $vg1=0.; $vg2=0.;
$tFinal=1.; $tPlot=.1; $cfl=.9; $mu=.0; $Prandtl=.72; $thermalExpansivity=.1; 
$noplot=""; $debug = 0;  
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$ad2=0; $ad22=2.; 
$bc="a"; 
* 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "cnsVariation=s"=>\$cnsVariation,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"bcn=s"=>\$bcn,"vg0=f"=>\$vg0,"vg1=f"=>\$vg1,"vg2=f"=>\$vg2,\
 "dtMax=f"=>\$dtMax,"bc=s"=>\$bc,"mg=s"=>\$mg,"mt=s"=>\$mt,"ts=s"=>\$ts );
* -------------------------------------------------------------------------------------------------
$kThermal=$mu/$Prandtl;   # check this 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $cnsVariation eq "godunov" ){ $cnsVariation="compressible Navier Stokes (Godunov)"; $ts="fe"; }
if( $cnsVariation eq "jameson" ){ $cnsVariation="compressible Navier Stokes (Jameson)"; }  
if( $cnsVariation eq "nonconservative" ){ $cnsVariation="compressible Navier Stokes (non-conservative)"; } 
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* specify the overlapping grid to use:
$grid
  $cnsVariation
  done
* -- time stepping method:
  $ts
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
  * There can be trouble if the grid moves too fast
  turn on moving grids
  specify grids to move
if( $mt eq "shift" ){ $moveCmds="translate\n 1. 0 0\n 1."; }
if( $mt eq "oscillate" ){ $moveCmds="oscillate\n 1. 0 0\n 1.\n .25\n 0."; }
if( $mt eq "advect body" ){ $moveCmds="deforming body\n user defined deforming body\n $mt\n initial velocity\n $vg0 $vg1 $vg2\n done"; }
if( $mg eq "interface" ){ $extraCmds = "specify faces\n 0\n 0\n done"; }else{ $extraCmds ="*"; }
      $moveCmds
      $mg
      $extraCmds
      * square
    done
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
    all=dirichletBoundaryCondition
    $mg(0,1)=slipWall
#    $rho=1.; $T=300.;
#    all=slipWall
#    $backGround(0,0)=subSonicInflow uniform(r=$rho,u=$uInflow,T=$T)
#    $backGround(1,0)=subSonicOutflow mixedDerivative(1.*t+1.*t.n=$T)
  done
* 
if( $tz eq "turn off twilight zone" ){ $icCmds="initial conditions\n uniform flow\n r=$rho u=$uInflow T=$T\n exit";}else{ $icCmds="*"; }
$icCmds
* 
  debug $debug
  exit
  $go
* 


      erase
      grid
        bigger:0
        exit this menu
