*
* cgcns command file for a shock hitting a sphere (one quadrant only)
*
*  usage: cgcns [-noplot] quarterSphere -g=<name> -l=<levels> -r=[ratio] -tf=<tFinal> -tp=<tPlot> ...
*               -x0=<num> -show=<name> -go=[run/halt]
*
* Examples:
*
*  cgcns -noplot quarterSphere -g=quarterSphere1e.hdf -l=2 -r=2 -tf=.5 -tp=.05 -x0=-1. -show=quarterSphere1el2r2.show
*  cgcns -noplot quarterSphere -g=quarterSphere1e.hdf -l=3 -r=2 -tf=1.5 -tp=.05 -x0=-1. -go=halt
* 
*  srun -N1 -n4 -ppdebug $cgcnsp -noplot quarterSphere -g=quarterSphere2e.hdf -l=2 -r=2 -tf=1.5 -tp=.25 -x0=-1 -show=qs2el2r2.show >! qs2el2r2.N1.n4.out &
* 
*  srun -N1 -n8 -ppdebug $cgcnsp -noplot quarterSphere -g=quarterSphere2e.hdf -l=1 -r=2 -tf=1.5 -tp=.25 -x0=-1 -show=qs2el2r2.show >! qs2el1r2.N1.n8.out &
* 
*  srun -N2 -n16 -ppdebug $cgcnsp -noplot quarterSphere -g=quarterSphere2e.hdf -l=2 -r=2 -tf=1.5 -tp=.25 -x0=-1 -show=qs2el2r2.show >! qs2el2r2.N2.n16.out &
* 
*  srun -N2 -n16 -ppdebug $cgcnsp -noplot quarterSphere -g=quarterSphere2e.hdf -l=3 -r=2 -tf=1. -tp=.25 -x0=-1 -show=qs2el3r2.show >! qs2el3r2.N2.n16.out &
*
*  srun -N4 -n32 -ppdebug $cgcnsp -noplot quarterSphere -g=quarterSphere2e.hdf -l=2 -r=4 -tf=1.5 -tp=.25 -x0=-1 -show=qs2el2r4.show >! qs2el2r4.N4.n32.out &
*
*  srun -N2 -n16 -ppdebug $cgcnsp -noplot quarterSphere -g=quarterSphere4e.hdf -l=2 -r=4 -tf=1. -tp=.25 -x0=-1 -show=qs4el2r4.show >! qs4el2r4.N2.n16.out &
*
*  srun -N2 -n16 -ppdebug memcheck_all $cgcnsp -noplot quarterSphere -g=quarterSphere2e.hdf -l=1 -r=2 -tf=.1 -tp=.05 -x0=-1 -show=qs2el2r2.show >! qs2el1r2.N2.n16.out &
* 
*  srun -N2 -n8 -ppdebug memcheck_all $cgcnsp -noplot quarterSphere -g=quarterSphere1e.hdf -l=1 -r=2 -tf=.1  -tp=.05 -x0=-1 -show=qs1e.show >! qs1e.N2.n8.out &
* 
*  srun -N2 -n8 -ppdebug memcheck_all $cgcnsp -noplot quarterSphere -g=quarterSphere2e.hdf -l=1 -r=2 -tf=.1  -tp=.05 -x0=-1 -show=qs2e.show >! qs2e.N2.n8.out &
* 
*  srun -N2 -n8 -ppdebug $cgcnsp -noplot quarterSphere -g=quarterSphere8e.hdf -l=2 -r=2 -tf=.02 -tp=.01 -x0=0. -show=qs8el2r2.show >! qs8el2r2.N2.n8.out &
* 
* mpirun -np 2 $cgcnsp noplot quarterSphere >! quarterSphere2el2r2.out &
* srun -N2 -n4 -ppdebug $cgcnsp noplot quarterSphere >! quarterSphere2el2r2.out &
* mpirun -np 2 $cgcnsp noplot quarterSphere >! test.out &
* mpirun-wdh -np 16 $cgcnsp noplot quarterSphere >! quarterSphere3el2r4.out &
* mpirun-wdh -np 4 $cgcnsp noplot quarterSphere >! quarterSphere1el2r2.out &
* mpirun-wdh -np 8 $cgcnsp noplot quarterSphere >! quarterSphere1el2r4.out &
*
* srun -N12 -n24 -ppdebug $cgcnsp -noplot quarterSphere >! quarterSphere2el2r2np24.out 
* srun -N16 -n32 -ppdebug $cgcnsp -noplot quarterSphere >! quarterSphere2el2r2np32.out 
* srun -N16 -n32 -ppdebug $cgcnsp -noplot quarterSphere >! quarterSphere2el2r4np32.out 
* totalview srun -a -N2 -n2 -ppdebug $cgcnsp p.cmd
* srun -ppdebug -N4 -n4 memcheck_all  $cgcnsp -noplot quarterSphere
*
* --- set default values for parameters ---
$go="run"; $show = " "; $noplot=""; 
$nrl=2;  # number of refinement levels
$cfl=1.; $debug=1; $tol=.2; $ratio=2; $x0=.5; $dtMax=1.e10; $nbz=2;
$tFinal=1.5; $tPlot=.1;
* 
$backGround="channel";
$amrOn="turn on adaptive grids"; $amrOff="turn off adaptive grids";
$amr=$amrOn; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal, \
            "tp=f"=>\$tPlot, "x0=f"=>\$x0, "show=s"=>\$show, "go=s"=>\$go, "noplot=s"=>\$noplot );
* -------------------------------------------------------------------------------------------------
if( $go eq "run" ){ $go = "movie mode\n finish"; }else{ $go="break"; }
*
*
* == 3D ==
* $grid ="rbibe.hdf"; $tFinal=.5; $tPlot=.1; $x0=-.5; $amr=$amrOn; $nrl=3; $backGround="outer-box"; 
* $grid ="sib2e.hdf";  $ratio=2; $tFinal=1.; $tPlot=.05; $x0=-1.5; $amr=$amrOff; $nrl=2; $backGround="box"; $show="qs.show"; 
* $grid ="sib2e.hdf";  $ratio=2; $tFinal=1.; $tPlot=.05; $x0=-1.5; $amr=$amrOn; $nrl=2; $backGround="box"; 
*
* --- quarter-sphere ---
*   
*  srun -N1 -n4 -ppdebug $cgcnsp -noplot quarterSphere >! quarterSphere2el2r2.N1.n4.out &
* $grid ="quarterSphere2e.hdf";  $tFinal=1.5; $tPlot=.25; $x0=-1.; $amr=$amrOn; $ratio=2; $nrl=2; $backGround="channel"; $show="qs2el2r2.show";
*  srun -N2 -n16 -ppdebug $cgcnsp -noplot quarterSphere >! quarterSphere2el3r2.N2.n16.out &
* $grid ="quarterSphere2e.hdf";  $tFinal=1.; $tPlot=.25; $x0=-1.; $amr=$amrOn; $ratio=2; $nrl=3; $backGround="channel"; $show="qs2el3r2.show";
*
* $grid ="quarterSphere1e.hdf";  $tFinal=1.8; $tPlot=.05; $x0=-2.; $amr=$amrOff; $ratio=2; $nrl=2; $backGround="channel"; $show="qs.show";
* $grid ="quarterSphere2e.hdf";  $tFinal=1.5; $tPlot=.1; $x0=-1.; $amr=$amrOff; $ratio=2; $nrl=2; $backGround="channel"; $show="qs2w3.show";
* $grid ="quarterSphere2i.hdf";  $tFinal=1.; $tPlot=.1; $x0=-.5; $amr=$amrOff; $ratio=2; $nrl=2; $backGround="channel"; $show="qs2i.show";
* $grid ="quarterSphere2e.hdf";  $tFinal=1.; $tPlot=.1; $x0=-.5; $amr=$amrOff; $ratio=2; $nrl=2; $backGround="channel"; $show="qs2w3.show";
* $grid ="quarterSphere2e.hdf";  $tFinal=1.; $tPlot=.1; $x0=-.5; $amr=$amrOff; $ratio=2; $nrl=2; $backGround="channel"; $show="qs2g1.show";
* $grid ="quarterSphere2e.hdf";  $tFinal=1.; $tPlot=.1; $x0=-.5; $amr=$amrOff; $ratio=2; $nrl=2; $backGround="channel"; $show="qs2ex2.show";
* $grid ="quarterSphere3e.hdf";  $tFinal=1.; $tPlot=.1; $x0=-.5; $amr=$amrOff; $ratio=2; $nrl=2; $backGround="channel"; $show="qs3ex2.show";
* $grid ="quarterSphere4e.hdf";  $tFinal=1.; $tPlot=.1; $x0=-.5; $amr=$amrOff; $ratio=2; $nrl=2; $backGround="channel"; $show="qs4ex2.show";
*
* $grid ="quarterSphere0e.hdf";  $ratio=2; $tFinal=1.; $tPlot=.1; $x0=-2.; $amr=$amrOn; $nrl=2; $backGround="channel";
* $grid ="quarterSphere1e.hdf";  $ratio=4; $tFinal=1.5; $tPlot=.1; $x0=-1.5; $amr=$amrOn; $nrl=2; $backGround="channel"; $show="test.show";
* $grid ="quarterSphere1e.hdf";  $ratio=2; $tFinal=1.; $tPlot=.05; $x0=-2.; $amr=$amrOn; $nrl=2; $backGround="channel"; $show="quarterSphere1el2r2.show"
* $grid ="quarterSphere1e.hdf";  $ratio=2; $tFinal=1.; $tPlot=.02; $x0=-2.; $amr=$amrOn; $nrl=2; $backGround="channel";
* $grid ="quarterSphere2e.hdf";  $tFinal=1.5; $tPlot=.1; $x0=-1.5; $amr=$amrOff;  $ratio=4; $nrl=2; $backGround="channel"; $show="qs2el2p32.show";
* $grid ="quarterSphere2e.hdf";  $tFinal=1.6; $tPlot=.1; $x0=.25; $amr=$amrOn; $ratio=4; $nrl=2; $backGround="channel"; $show="quarterSphere2el2r2.show"; $tol=.2;
* $grid ="quarterSphere2e.hdf";  $tFinal=1.6; $tPlot=.05; $x0=-2.; $amr=$amrOn; $ratio=2; $nrl=2; $backGround="channel";
* $grid ="quarterSphere2e.hdf";  $ratio=4; $tFinal=1.5; $tPlot=.1; $x0=-1.5; $amr=$amrOn; $nrl=2; $backGround="channel"; $show="test.show";
* $grid ="quarterSphere3e.hdf";  $tFinal=1.6; $tPlot=.05; $x0=-2.; $amr=$amrOn; $ratio=4; $nrl=2; $backGround="channel"; $show="quarterSphere3el2r4.show"; 
* $grid ="quarterSphere4e.hdf"; $tFinal=2.; $tPlot=.2;  $x0=-2.; $amr=$amrOff; $nrl=2; $backGround="channel"; $show="quarterSphere.show"; 
* 
$grid
***
** compressible Navier Stokes (Jameson)  
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
  turn on memory checking
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
**   all=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
*   rectangle=slipWall
*   rectangle(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
*   rectangle(1,0)=superSonicOutflow
   all=slipWall
**   all=noSlipWall
 $backGround(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
 $backGround(1,0)=superSonicOutflow
* outer-square(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
* outer-square(1,0)=superSonicOutflow
*  left-square(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
*  right-square(1,0)=superSonicOutflow
  done
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
      x=$x0
*    smooth step function
**      y=.5
*       5.
*
      r=2.6667 u=1.25 e=10.119
      r=1. u=0. e=1.786
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
