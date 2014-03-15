*
* cgcns command file: a 3D shock moving through two intersecting pipes. 
*
*  usage: cgcns [-noplot] twoPipes -g=<name> -l=<levels> -r=[ratio] -tf=<tFinal> -tp=<tPlot> ...
*               -x0=<num> -show=<name> -debug=<num> -go=[run/halt/og]
*
* Examples:
*
*  cgcns -noplot twoPipes -g=twoPipese2.order2.hdf -l=1 -r=2 -tf=1.5 -tp=.1 -y0=1.75 -show="twoPipes.show" -go=og
*  cgcns -noplot twoPipes -g=twoPipese2.order2.hdf -l=2 -r=4 -tf=1.5 -tp=.1 -y0=1.75 -show="twoPipes.show" >! twoPipesl2r4.out &
*
*  mpirun -np 1 $cgcnsp twoPipes -g=twoPipese2.order2.hdf -l=1 -r=2 -tf=1.5 -tp=.1 -y0=1.75 -go=halt
*  srun -N1 -n4 -ppdebug $cgcnsp noplot twoPipes -g=twoPipese2.order2.hdf -l=2 -r=2 -tf=1. -tp=.1 -y0=1.5 -show="twoPipesl2r2.show" >! twoPipes2l2r2.N1n4.out &
* bug : seg fault round t=.3: 
*   mpirun -np 1 $cgcnsp noplot twoPipes -g=twoPipese2.order2.hdf -l=2 -r=2 -tf=1. -tp=.1 -y0=1.5 -show="twoPipes.show" -go=go >! twoPipes.l2r2.np1.out &
* 
* bug ?: 
*   mpirun -np 1 $cgcnsp noplot twoPipes -g=twoPipese2.order2.hdf -l=2 -r=2 -tf=1. -tp=.1 -y0=1. -show="twoPipes.show" -go=go >! twoPipes.l2r2.np1.out &
* 
*  srun -N1 -n4 -ppdebug $cgcnsp noplot twoPipes -g=twoPipese2.order2.hdf -l=1 -r=2 -tf=1.5 -tp=.1 -y0=1.75 -show="twoPipes.show" >! twoPipes.out &
*  totalview srun -a -N1 -n4 -ppdebug $cgcnsp noplot twoPipes -g=twoPipese2.order2.hdf -l=2 -r=2 -tf=1. -tp=.1 -y0=1.75 -show="twoPipes2l2r2.show" >! twoPipes2l2r2.out &
*  srun -N2 -n8 -ppdebug $cgcnsp noplot twoPipes -g=twoPipese4.order2.hdf -l=2 -r=2 -tf=1. -tp=.1 -y0=1.5 -show="twoPipes4l2r2.show" >! twoPipes4l2r2.N2n8out &
*
* srun -N1 -n1 -ppdebug $cgcnsp noplot twoPipes -g=twoPipese2.order2.hdf -l=2 -r=2 -tf=1. -tp=.1 -y0=1.25 -show="twoPipes.show" -go=go >! twoPipes.l2r2.np1.out &
* 
*  cgcns -noplot twoPipes -g=twoPipese2.order2.hdf -l=2 -r=2 -tf=1.5 -tp=.1 -y0=1.25 
* 
* --- set default values for parameters ---
$go="run"; $show = " "; $nplot="";
$nrl=2;  # number of refinement levels
$cfl=1.; $debug=1; $tol=.2; $ratio=2; $x0=.5; $y0=1.75; 
$dtMax=1.e10; $nbz=2;
$tFinal=1.5; $tPlot=.1;
* 
$backGround="channel";
$amrOn="turn on adaptive grids"; $amrOff="turn off adaptive grids";
$amr=$amrOn; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=>\$ratio, "tf=f"=>\$tFinal,"debug=i"=> \$debug, \
            "tp=f"=>\$tPlot, "x0=f"=>\$x0, "y0=f"=>\$y0, "show=s"=>\$show, "go=s"=>\$go,"noplot=i"=>\$noplot );
* -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $nrl eq 1 ){ $amr=$amrOff; }
*
*
* 
$grid
*
   compressible Navier Stokes (Godunov)
  exit
*
*** turn off graphics
*
  turn off twilight
  final time $tFinal
  times to plot $tPlot 
*  plot and always wait
  no plotting
***
  show file options
    compressed
    * specify the max number of parallel hdf sub-files: 
      OBPSF:maximum number of parallel sub-files 8
     open
       $show
    frequency to flush
       2
    exit
* -- specify which variables will appear in the show file:
    showfile options...
    OBPSF:show variable: rho 1
    OBPSF:show variable: u 0
    OBPSF:show variable: v 0
    OBPSF:show variable: w 0
    OBPSF:show variable: T 0
    OBPSF:show variable: Mach Number 0
    OBPSF:show variable: p 1
    close show file options
* 
***
* -----------------------------------------------------------------------
**  turn on memory checking
* -----------------------------------------------------------------------
* 
* 
  pde parameters
      mu
      0.
      kThermal
      0.
  done
  OBPDE:Godunov order of accuracy 2
*
* =================================================================
*   order of extrapolation for interpolation neighbours
*     1
*   order of extrapolation for second ghost line
*     1
* 
  reduce interpolation width
    2
  boundary conditions
   all=slipWall
   bcNumber4=superSonicInflow uniform(r=2.6667,v=-1.25,e=10.119)
   bcNumber1=superSonicOutflow
   bcNumber2=superSonicOutflow
  done
* 
  cfl
   $cfl
  dtMax $dtMax
*
**  check for floating point errors
*
  $amr
* 
  order of AMR interpolation
      2
  error threshold
     $tol 
  regrid frequency
     $regrid=$nbz*$ratio;
     $regrid
  change error estimator parameters
    set scale factors
      2 1 1 1 1 1 1 
    done
    weight for first difference
    1.
    weight for second difference
    1.
    exit
    truncation error coefficient
    1.
**show amr error function
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $nrl
    number of buffer zones
       $nbz
    grid efficiency
      .7
    turn on load balancer
    change load balancer
      * KernighanLin
      * sequential assignment
      * random assignment
      * all to all
    exit
  exit
  debug $debug
* 
  initial conditions
   step function
      y=$y0 
      r=1. v=0. e=1.786
      r=2.6667 v=-1.25 e=10.119
    continue
   continue
*
*   contour
*     pick to delete contour planes
*     delete contour plane 2
*     delete contour plane 1
*     delete contour plane 0
*     pick to add contour plane z
*     add contour plane  0.00000e+00  0.00000e+00  1.00000e+00   0. 0. 1.e-3
*     pick to add contour plane y
*     add contour plane  0.00000e+00  1.00000e+00  0.00000e+00   0. 0. 1.e-3
*     exit
* * 
* * 
*   grid
*     plot shaded surfaces (3D) 0
*     toggle grid 0 0
*     toggle shaded surfaces 1 0
*     toggle shaded surfaces 2 0
*     set view:0 0 0 0 1 0.831533 0.16566 -0.530197 0.0313766 0.938962 0.342588 0.554588 -0.301509 0.77558
*     plot grid lines 0
*   exit this menu
*
$go



movie mode
finish


  grid
    plot shaded surfaces (3D) 0
    toggle grid 0 0
    toggle shaded surfaces 1 0
    toggle shaded surfaces 2 0
    set view:0 0 0 0 1 0.831533 0.16566 -0.530197 0.0313766 0.938962 0.342588 0.554588 -0.301509 0.77558
    plot grid lines 0
  exit this menu


  continue



  grid
    toggle grid 0 0
    plot block boundaries 0
    exit this menu
*


movie mode
finish






* 
***  check for floating point errors
*
****** for a restart
   initial conditions
     read from a show file
      p2.show
       -1
   exit
   continue
*
*****


* 
  initial conditions
   step function
      x=$x0
*    smooth step function
**      y=.5
*       5.
*
      r=2.6667 u=1.25 e=10.119
      r=1. u=0. e=1.786
    continue
   continue







$tFinal=1.; $tPlot=.1; $show="";
$numberOfLevels=2;  $refinementRatio=4;  $regrid=2; 
* 
* box40
* box40-2-2
*
* $grid="/home/henshaw/Overture/ogen/box10-2-2"; 
$grid="box10"; $numberOfLevels=3;   $regrid=8; $tPlot=.01;  $show="shockTube3d.show";
*
$grid
*
*  compressible Navier Stokes (Jameson)  
  compressible Navier Stokes (Godunov)  
  exit
  turn off twilight
  final time (tf=)
   $tFinal
  times to plot (tp=)
    $tPlot
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
*   save a restart file 1
*
*  variable time step PC
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
  boundary conditions
    box=slipWall
    box(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
    box(1,0)=superSonicOutflow
    done
* 
  cfl
   1.
* 
 turn on adaptive grids
  order of AMR interpolation
      2
  error threshold
       .4
  regrid frequency
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
      $refinementRatio
    default number of refinement levels
      $numberOfLevels
  exit
*
  initial conditions
    * x=.5
    step function
      x=.25
      r=2.6667 u=1.25 e=10.119
      r=1. e=1.786
    continue
  debug 
   1 7 31
  continue




