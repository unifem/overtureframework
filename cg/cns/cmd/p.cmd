*
* cgcns command file for various parallel test cases.
*
*  To compare results from different processors:
*        1. Set $debug=3 (optionally set $format to the output format)
*        2. Run with -np 1 --> results in the output file obNP1.debug     
*        3. Run with -np 2 --> results in the output file obNP2.debug     
*        4. Compare obNP1.debug to obNP2.debug
*
* mpirun -np 2 $cgcnsp -noplot p.cmd  >! twoBump2l2r4.out &
* mpirun -np 2 $cgcnsp -noplot p.cmd  >! twoBump4l3r2.out &
* mpirun-wdh -np 2 $cgcnsp -noplot p.cmd  >! twoBump2l2r4.out &
* 
* totalview srun -a -N8 -n8 -ppdebug $cgcnsp -noplot p.cmd 
* use valgrind: 
* srun -ppdebug -N8 -n8 memcheck_all $cgcnsp -noplot p.cmd
* 
$show = " ";  $format="%18.12e"; 
$cfl=1.; $debug=1; $tol=.05; $ratio=2; $x0=.5; $nrl=2; $dtMax=1.e10; $nbz=2; 
$backGround="square";
$amrOn="turn on adaptive grids";
$amrOff="turn off adaptive grids";
$amr=$amrOff; 
$load="KernighanLin";
* $load="sequential assignment"; 
* $load="random assignment"
* $load="all to all"; 
*
* $gridName ="square5.hdf"; $tFinal=.2; $tPlot=.05; 
* $gridName ="square5.hdf"; $tFinal=1; $tPlot=.01; $cfl=.1; $debug=1; 
* $gridName ="square10.hdf"; $tFinal=.2; $tPlot=.05; $x0=.25; $amr=$amrOn; $nrl=3; $show="pr.show";
* $gridName ="square16.hdf"; $tFinal=.2; $tPlot=.05; $x0=.25; $amr=$amrOn; $nrl=2;
* $gridName ="square32.hdf"; $tFinal=.2; $tPlot=.05; $x0=.25; $amr=$amrOn; $nrl=3;
* $gridName ="sise.hdf"; $tFinal=.9; $tPlot=.1; $x0=-.8; $amr=$amrOn; $nrl=2; $debug=0; $show="p.show";
* $gridName ="sis3e.hdf"; $tFinal=.9; $tPlot=.1; $x0=-.1; $amr=$amrOn; $nrl=3; $backGround="outer-square"; 
* $gridName ="rsise.hdf"; $tFinal=.9; $tPlot=.1; $x0=-.8; $amr=$amrOn; $nrl=3; $backGround="outer-square"; 
* $gridName ="rsis2e.hdf"; $tFinal=.2; $tPlot=.1; $x0=-.8; $amr=$amrOn; $nrl=3; $backGround="outer-square"; 
* $gridName ="rsis4e.hdf"; $tFinal=.9; $tPlot=.1; $x0=-.8; $amr=$amrOn; $ratio=4; $nrl=3; $backGround="outer-square"; 
* 
* $gridName ="stire.hdf"; $tFinal=.9; $tPlot=.05; $x0=-.35; $amr=$amrOn; $nrl=2; $backGround="backGround"; 
* 
* $gridName ="rotatedSquare10.hdf"; $tFinal=.9; $tPlot=.05; $x0=.5; $amr=$amrOff; $nrl=2; $backGround="square"; 
* $gridName ="quarterAnnulus0.hdf"; $tFinal=.9; $tPlot=.05; $x0=-2.; $amr=$amrOff; $nrl=2; $backGround="quarter"; 
* $gridName ="halfAnnulus1e.hdf"; $tFinal=.4; $tPlot=.2; $x0=-.5; $amr=$amrOn; $nrl=2; $debug=3; $format="%16.10e";
* $gridName ="halfAnnulus2e.hdf"; $tFinal=.4; $tPlot=.05; $x0=0.; $amr=$amrOn; $nrl=3; $show="p2.show";
* $gridName ="halfAnnulus2e.hdf"; $tFinal=.4; $tPlot=.05; $x0=0.; $amr=$amrOn; $nrl=3; $show=" ";
* $gridName ="halfAnnulus8e.hdf"; $tFinal=.4; $tPlot=.05; $x0=0.; $amr=$amrOn; $ratio=4; $nbz=4; $nrl=2; $show=" ";
* $gridName ="halfAnnulus16e.hdf"; $tFinal=.4; $tPlot=.05; $x0=0.; $amr=$amrOff; $nrl=2; $show=" ";
* $gridName ="sbse.hdf"; $tFinal=.2; $tPlot=.2; $cfl=.1;
* $gridName ="sbse1.hdf"; $tFinal=.2; $tPlot=.05;
* sbse.hdf
* sis3e.hdf
* $gridName ="twoBumpe.hdf"; $tFinal=1.4; $tPlot=.2; $x0=-.5; $backGround="channel"; $amr=$amrOn; $nrl=2; $debug=0
* $gridName ="twoBumpe2.order2.hdf"; $tFinal=1.4; $tPlot=.2; $x0=-.5; $backGround="channel"; $amr=$amrOn; $nrl=2;
* $gridName ="twoBumpe.hdf"; $tFinal=1.; $tPlot=.1; $x0=-.5; $backGround="channel"; $amr=$amrOn; $nrl=2; $debug=3;
* $gridName ="twoBumpe2.order2.hdf"; $tFinal=1.4; $tPlot=.2; $x0=-.5; $backGround="channel"; $amr=$amrOn; $ratio=4; $nrl=2; $show="twoBump2l2r4.show";
* $gridName ="twoBumpe2.order2.hdf"; $tFinal=1.4; $tPlot=.2; $x0=-.5; $backGround="channel"; $amr=$amrOn; $ratio=2; $nrl=3;  $show="twoBump2l3r2.show"
* $gridName ="twoBumpe4.order2.hdf"; $tFinal=1.4; $tPlot=.2; $x0=-.5; $backGround="channel"; $amr=$amrOn; $ratio=4; $nrl=2;  $show="twoBump4l2r2.show";
* for figure:
** $gridName ="cice.hdf"; $tFinal=2.; $tPlot=.1; $x0=-.8; $amr=$amrOn; $nrl=2; $show=""; $nbz=0; 
* $gridName ="cice.hdf"; $tFinal=2.; $tPlot=.1; $x0=-.5; $amr=$amrOn; $nrl=2; $show="p2.show";
* $gridName ="cice.hdf"; $tFinal=2.; $tPlot=.1; $x0=-1.6; $amr=$amrOn; $nrl=2; $show="";
$gridName ="cice2.hdf"; $tFinal=1.2; $tPlot=.1; $x0=-1.4; $amr=$amrOn; $nrl=3; $debug=1; 
* $gridName ="cic3e.hdf"; $tFinal=1.5; $tPlot=.1; $x0=-1.6; $amr=$amrOn; $nrl=2; $debug=0; $nbz=4;
* $gridName ="cic3e.hdf"; $tFinal=1.4; $tPlot=.1; $x0=-1.6; $amr=$amrOn; $nrl=2; $debug=0; $ratio=4; $nbz=3; 
* $gridName ="cic3e.hdf"; $tFinal=1.4; $tPlot=.1; $x0=-1.6; $amr=$amrOn; $nrl=3; $debug=0; $nbz=4; $show="";
* $gridName ="cic4e.hdf"; $tFinal=1.; $tPlot=.1;
* $gridName ="cicE.hdf"; $tFinal=1.; $tPlot=.1;
**** 280,000 pts:
* $gridName ="cic5e.hdf";  $tFinal=1.4; $tPlot=.1; $x0=-1.6; $amr=$amrOn; $nrl=2; $debug=0; $nbz=4; $show="";
***** 1.1M points:
* $gridName ="cic6e.hdf"; $tFinal=2.; $tPlot=.2; $show="cic6e.show"; 
* -- 4.3M pts
* $gridName ="cic7e.hdf"; $tFinal=1.2; $tPlot=.2; $show="cic7e.show"; 
* -- 17.M pts
* $gridName ="cic8e.hdf"; $tFinal=.0125; $tPlot=.0125;
*
* == 3D ==
*  $gridName ="box20.hdf"; $debug=0; $tFinal=.5; $tPlot=.1; $x0=.25; $amr=$amrOn; $nrl=2; $backGround="Box"; $format="%16.10e";
* $gridName ="bibe.hdf"; $debug=0; $tFinal=.2; $tPlot=.1; $x0=-.5; $amr=$amrOn; $nrl=2; $backGround="outer-box";
* $gridName ="nonBib2e.hdf"; $debug=0; $tFinal=.2; $tPlot=.02; $x0=-.5; $amr=$amrOn; $nrl=3; $backGround="outer-box";  $format="%20.14e";
* $gridName ="boxsbse.hdf"; $debug=3; $tFinal=.03; $tPlot=.01; $x0=-.5; $amr=$amrOff; $nrl=3; $backGround="left-box";  $format="%20.14e";
* $gridName ="rbibe.hdf"; $debug=0; $tFinal=1.; $tPlot=.1; $x0=-.5; $amr=$amrOn; $nrl=3; $backGround="outer-box"; $format="%16.10e"; 
* $gridName ="quarterSphere0e.hdf"; $tFinal=.5; $tPlot=.1; $x0=-2.; $amr=$amrOn; $nrl=2; $backGround="channel"; 
* $gridName ="quarterSphere1e.hdf"; $tFinal=.5; $tPlot=.1; $x0=-2.; $amr=$amrOn; $nrl=2; $backGround="channel"; 
*
* $gridName ="cylinderInAShortChannel1.hdf"; $tFinal=.5; $tPlot=.02; $x0=-.5; $amr=$amrOn; $nrl=2; $backGround="box";
* 
* 44,000 pts: (from ogen sibArg.cmd)
* $gridName ="sibe1.order2.hdf"; $tFinal=.5; $tPlot=.1; $x0=-1.25; $amr=$amrOn; $nrl=2; $backGround="box"; $format="%16.10e";
* $gridName ="sib2e.hdf"; $tFinal=.5; $tPlot=.1; $x0=-1.25; $amr=$amrOff; $nrl=2; $backGround="box"; $format="%16.10e";
*  78,000 pts:
* $gridName ="sibe2.order2.hdf"; $tFinal=.5; $tPlot=.1; $x0=-1.25; $amr=$amrOn; $nrl=2; $backGround="box";
* 242,000 pts:
* $gridName ="sibe3.order2.hdf"; $tFinal=.5; $tPlot=.1; $x0=-1.25; $amr=$amrOn; $nrl=2; $backGround="box";
* 554,000 pts:
* $gridName ="sibe4.order2.hdf"; $tFinal=1.; $tPlot=.1; $x0=-1.25; $ratio=4; $amr=$amrOn; $nrl=2; $show="p4.show"; $backGround="box";
* 
$gridName
***
** compressible Navier Stokes (Jameson)  
   compressible Navier Stokes (Godunov)
  exit
*
**  turn off graphics
* 
  turn off twilight
  final time $tFinal
  times to plot $tPlot 
*  plot and always wait
  no plotting
***
  show file options
    compressed
     open
       $show
    frequency to flush
     1
* choose variables to plot:
*    OBPSF:show variable: rho 1
*    OBPSF:show variable: u 0 
*    OBPSF:show variable: v 0 
*    OBPSF:show variable: w 0 
*    OBPSF:show variable: T 0 
*    OBPSF:show variable: p 0 
  exit
***
* -----------------------------------------------------------------------
** OBPDE:Godunov order of accuracy 1
* -----------------------------------------------------------------------
* 
  pde parameters
      mu
      0.
      kThermal
      0.
  done
*
  output format $format
*
  reduce interpolation width
    2
  boundary conditions
**   all=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
*   rectangle=slipWall
*   rectangle(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
*   rectangle(1,0)=superSonicOutflow
   all=slipWall
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
      1 1 1 1 1 1 1 
    done
    weight for first difference
    1.
    weight for second difference
    1.
    exit
    truncation error coefficient
    1.
*   show amr error function
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
      * 
      $load
    exit
  exit
  debug
    $debug
* 
  initial conditions
   step function
      x=$x0
      r=2.6667 u=1.25 e=10.119
      r=1. u=0. e=1.786
    continue
   continue
*

movie mode
finish

****** for a restart
   initial conditions
     read from a show file
      p.show
       -1
   exit
   continue
*
*****


  initial conditions
    OBIC:uniform state r=2.6667, u=0, v=0, w=0, e=10.119
    OBIC:assign uniform state
  continue
  continue


* 
**  check for floating point errors
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
