*
* cgcns example: test a shock that moves through a deforming grid
*
* Usage:
*   
*  cgcns [-noplot] deformShock -g=<name> -tz=<poly/trig/none> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*        -bc=<a|d|r> -model=<Jameson/Godunov> -debug=<num> -bg=<backGround> -uInflow=<> -mu=<> ...
*        -motion=[oscillate|scale|translate] -go=[run/halt/og]
* 
*  -go : run, halt, og=open graphics
*  -uInflow : inflow velocity
*  -dg : the name of the grid to deform or -dg="share=<num>" to choose all grids with a given share value 
*  -df, -da : deformation frequency and amplitude. 
* 
* Examples: [note: shock speed is 2]
* 
*  cgcns deformShock -g=square10 -tf=2. -tp=.05 -motion=translate -go=halt 
*  cgcns deformShock -g=square128 -tf=.5 -tp=.05 -motion=oscillate -go=halt 
*  cgcns deformShock -g=square64 -tf=.5 -tp=.05 -motion=translate -go=halt 
* 
*  cgcns deformShock -g=square5  -tf=.5 -tp=.001 -motion=scale -go=halt -debug=15
*  cgcns deformShock -g=square64  -tf=.5 -tp=.1 -motion=scale -go=halt 
*
*  cgcns deformShock -g=rsis2e -bg="outer-square" -dg="inner-square" -xStep="x=-.5" -sx=1. -sy=.5 -tf=.5 -tp=.1 -motion=scale -go=halt 
*  cgcns deformShock -g=rsis8e -bg="outer-square" -dg="inner-square" -xStep="x=-.5" -sx=1. -sy=.5 -tf=.5 -tp=.1 -motion=scale -go=halt 
* 
*  cgcns deformShock -g=solidDiskDeformi2 -bg="outerSquare" -dg="outerInterface" -xStep="x=-1." -sx=.25 -sy=.5  -tf=1.2 -tp=.2 -motion=scale -go=halt 
*  cgcns deformShock -g=solidDiskDeformi4 -bg="outerSquare" -dg="outerInterface" -xStep="x=-1." -sx=.25 -sy=.5  -tf=1.2 -tp=.2 -motion=scale -go=halt 
*  cgcns deformShock -g=solidDiskDeformi8 -bg="outerSquare" -dg="outerInterface" -xStep="x=-1." -sx=.25 -sy=.5  -tf=1.2 -tp=.2 -motion=scale -go=halt 
* 
* --- TZ
*  cgcns deformShock -g=square10 -tf=2. -tp=.05 -tz=poly -motion=translate -go=halt 
*  cgcns deformShock -g=solidDiskDeformi2 -bg="outerSquare" -dg="outerInterface" -xStep="x=-1." -sx=.25 -sy=.5  -tf=1. -tp=.1 -motion=scale -tz=poly -go=halt
* 
* --- set default values for parameters ---
* 
$model="Godunov"; $motion="oscillate"; 
$grid="halfCylinder.hdf"; $backGround="square"; $bcn="slipWall"; $uInflow=.1; $xStep="x=.25"; 
$deformingGrid="square"; $deformFrequency=.5; $deformAmplitude=1.; $deformationType="ice deform"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $mu=.0; $Prandtl=.72; $thermalExpansivity=.1; 
$noplot=""; $debug = 0;  
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$sx=1.; $sy=0.; $sz=0.;  # scale factors in each direction, 0=no scale  (scale: (1+scaleFactor*t)*x )
$ad2=0; $ad22=2.; $deformFrequency=1.; $deformAmplitude=1.;
$bc="a"; 
* 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"bcn=s"=>\$bcn,"xStep=s"=>\$xStep,\
 "dtMax=f"=>\$dtMax,"motion=s"=>\$motion,"sx=f"=>\$sx,"sy=f"=>\$sy,\
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
* For Jeff's
**  compressible Navier Stokes (multi-component)
*
    define real parameter gamma1   1.4
    define real parameter cv1      1.
    define real parameter pi1      0.0
    define real parameter gamma2   1.4
    define real parameter cv2      1.
    define real parameter pi2      0.0
    define integer parameter slope 1
    define integer parameter fix   1
    define integer parameter useDon 0
    define real parameter gammai   1.4
    define real parameter gammar   1.4
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
if( $motion eq "oscillate" ){ $moveCmds ="oscillate\n 1. 0. 0\n  1.\n .25\n  0."; }
if( $motion eq "translate" ){ $moveCmds ="translate\n 1. 0. 0\n  1."; }
if( $motion eq "scale" ){ $moveCmds ="scale\n $sx $sy $sz"; }
if( $motion eq "ellipse" ){ $moveCmds ="deforming body\n  user defined deforming body\n" .\
   "$deformationType\n deformation frequency\n $deformFrequency\n deformation amplitude\n $deformAmplitude\n done"; }
* 
  turn on moving grids
  specify grids to move
    $moveCmds
    $deformingGrid
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
  reduce interpolation width
    2
* 
  boundary conditions
    all=slipWall 
**    $backGround(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
    $backGround(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,T=1.205331,s=0)
**   $backGround(0,0)=superSonicInflow uniform(r=1.,e=1.786)
*   $backGround(0,0)=superSonicOutflow
    $te=1.205331; # check 
*jeff    $backGround(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,T=$te)
    $backGround(1,0)=superSonicOutflow
    $backGround(0,1)=slipWall
    $backGround(1,1)=slipWall
    if( $tz ne "turn off twilight zone" ){ $bcCmds="all=dirichletBoundaryCondition"; }else{ $bcCmds="*"; }
    $bcCmds
  done
* 
$icCmds="*";
if( $tz eq "turn off twilight zone" ){ $icCmds="initial conditions\n step function\n $xStep\n r=2.6667 u=1.25 T=1.205331\n r=1. T=.7144\n continue";} 
 $icCmds
#   initial conditions
# *    smooth step function
#      step function
#       $xStep
# *        5.
# *     r=2.6667 u=1.25 e=10.119 
# *     r=1. e=1.786 
#      r=2.6667 u=1.25 T=1.205331
#      r=1. T=.7144
#   continue
* 
  debug $debug
  exit
  $go
