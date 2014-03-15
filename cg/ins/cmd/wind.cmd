*
* cgins : flow past wind turbines
* 
*   nohup $ins/bin/cgins noplot wind -g=windFarm1Towersi1.order2 -go=go -show="wind.show" >! wind.out
*   nohup $ins/bin/cgins noplot wind -g=windFarm1Towersi2.order2 -go=go -show="wind2.show" >! wind2.out &
* 
*   nohup $ins/bin/cgins noplot wind -g=windFarm2Towersi2.order2.hdf -go=go -show="wind22.show" >! wind22.out &
*
* 
$maxPressureIterations=20; 
* 
$grid="windFarm1Towersi1.order2.hdf";  $show="wind.show"; 
* --- set default values for parameters ---
* 
$model="ins"; $ts="steady state RK-line"; $noplot=""; $backGround="square"; 
$debug = 0;  $tFinal=1.; $tPlot=.1; $maxIterations=100; $rtol=1.e-4; $atol=1.e-5; $dtMax=.5; 
$show=" ";  $solver="best";   
$ogesDebug=0; $its=500; $pits=50; $cfl=1.; $nu=.001; $ad2=5.; $Prandtl=.72; $thermalExpansivity=.1;
$gravity = "0. 0. 0."; $T0=1.; $go="halt"; 
$implicitVariation="full"; $refactorFrequency=100;  $implicitFactor=1.;
* 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
* $ksp="gmres"; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"its=i"=>\$its,"pits=i"=>\$pits,"model=s"=>\$model,"dtMax=f"=>\$dtMax,\
 "mpits=i"=>\$maxPressureIterations,\
 "tp=f"=>\$tPlot,"tf=f"=>\$tFinal, "solver=s"=>\$solver, "show=s"=>\$show,"debug=i"=>\$debug,"go=s"=>\$go, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "gravity=s"=>\$gravity, "noplot=s"=>\$noplot,\
 "rf=i"=> \$refactorFrequency, "iv=s"=>\$implicitVariation,"imp=f"=>\$implicitFactor,"ad2=f"=>\$ad2 );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
$grid 
*
  incompressible Navier Stokes
  exit
* 
  show file options
    compressed
   open
     $show
   * Name to use for a restart:
   *   wind2.show
    frequency to flush
      1
    exit
  turn off twilight zone
*
*    ==== choose the steady state solver here ====
    steady state RK-line
    dtMax $dtMax
*
***
  max iterations $its
  plot iterations $pits
***
  plot and always wait
* 
 pde parameters
    nu
      $nu
    done
* 
  OBPDE:second-order artificial diffusion 1
  OBPDE:ad21,ad22  $ad2 $ad2
********
*  OBPDE:fourth-order artificial diffusion
*  OBPDE:use implicit fourth-order artificial diffusion 1
*  OBPDE:ad41,ad42 1,1
********
*
  maximum number of iterations for implicit interpolation
    10
*
*
  * use an iterative solver for the pressure equation
   pressure solver options
     * PETSc
     * SLAP
     choose best iterative solver
     maximum number of iterations
      $maxPressureIterations
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
****
  initial conditions
    uniform flow
     v=1., p=1.
  exit
****
****** for a restart uncomment the next lines and comment out the previous lines
****** NB: Be sure to use a new name for the show file for the restarted solution
******     or else it will already have been over-written by now
*  initial conditions
*    read from a show file
*     wind.show
*      -1
*  exit
*****
  boundary conditions
*   The boundary conditions have been assigned numbers 1,2,3,4 when the grid was
*   generated. Here we convert these numbers into boundary conditions:
    all=noSlipWall
    bcNumber1=slipWall
    bcNumber2=slipWall
    bcNumber3=inflowWithVelocityGiven, uniform(p=1.,v=1.)
    bcNumber4=outflow
    bcNumber6=slipWall
    * bcNumber1=outflow
    * bcNumber2=outflow
    * bcNumber5=outflow
    * bcNumber6=outflow
   done
* 
  project initial conditions
  debug 3 
  exit
$go

