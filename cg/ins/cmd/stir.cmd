* =============================================================================================
* cgins: stirring stick example (rotating stick)
* 
* Usage:
*   
*  cgins [-noplot] stir -g=<name> -tf=<tFinal> -tp=<tPlot> -nu=<> -cfl=<> -rate=<> -ramp=<> -ts=<> -go=[run/halt/og]
* 
*  -rate : rotate rate, rotations per second
*  -ramp : slow start ramp time 
*  -ts : "adams PC" or "implicit" 
*  -go : run, halt, og=open graphics
* 
* Examples: 
* 
*  cgins stir -g=stir.hdf -nu=.05 -tf=1. -tp=.025 -go=halt 
*  cgins stir -g=stir.hdf -nu=.05 -tf=1. -tp=.025 -go=halt -ts="adams PC"
*  cgins stir -g=stir.hdf -nu=.05 -tf=1. -tp=.004 -rate=8. -go=halt 
*  cgins stir -g=stir2.hdf -nu=.01 -tf=1. -tp=.002 -rate=8. -go=halt 
#
# -- parallel:
#  mpirun -np 2 $cginsp stir -g=stire.hdf -nu=.01 -tf=1. -tp=.025 -go=halt -freqFullUpdate=1
*
* =============================================================================================
* 
* --- set default values for parameters ---
$tFinal=.5; $tPlot=.025; $show = " "; 
$rate=.5; $ramp=0.; $nu=.05; $cfl=.9; $dtMax=.1; $ts="implicit"; $newts=0;
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,\
 "iv=s"=>\$implicitVariation,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
  "bc=s"=>\$bc,"rate=f"=>\$rate,"ramp=f"=>\$ramp,"newts=i"=>\$newts,"freqFullUpdate=i"=>\$freqFullUpdate );
* -------------------------------------------------------------------------------------------------
*
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
*
* 
$grid
* 
  incompressible Navier Stokes
  * simulate grid motion only 1
  exit
  show file options
    * compressed
    open
     $show
    frequency to flush
      5
    exit
  turn off twilight zone
  project initial conditions
*
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  dtMax $dtMax
  plot and always wait
  * no plotting
* 
  turn on moving grids
* 
  specify grids to move
    rotate
      0. 0. 0.
*    specify rate and rampInterval (rampInterval=0. => impulsive start, .5=slow start)
      $rate $ramp
    stir
    done
  done
# 
  frequency for full grid gen update $freqFullUpdate
#
* -- specify the time stepping method
  $ts 
  $newts
 compact finite difference
  * for implicit time stepping
  choose grids for implicit
     all=explicit
#     all=implicit
     stir=implicit
    done
* 
  pde parameters
    nu $nu
  done
  boundary conditions
    all=noSlipWall
    done
  initial conditions
    uniform flow
     p=1.
  exit
* 
  continue
