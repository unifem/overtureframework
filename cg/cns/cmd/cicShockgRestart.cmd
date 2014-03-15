*
*  cgcns command file: shock hitting a cylinder
*     Solve the Euler equations with Godunov's method and AMR
* 
*      *** Demonstrate the reading of a restart file ***
* 
*  (1) First run the command file cicShockg.cmd saving the show file "cicShockg.show"
*          cgcns cicShockg
*  (2) Now run this command file -- it will read the initial conditions from the
*      show file created in step 1
*          cgcns cicShockgRestart
* 
* Background colour:0 black
* Foreground colour:0 white
*
$tPlot=.1; $tFinal=.4; $show=" "; $xStep="x=-1.5"; $debug=3; 
* 
$grid="cic2.hdf"; $tFinal=2.; $show="cicShockgRestart.show";
*
$grid 
*
   compressible Navier Stokes (Godunov)  
*
*   one step
*   add extra variables
*     1
  exit
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time $tFinal
  times to plot $tPlot
* no plotting
  plot and always wait
  show file options
    compressed
    open
     $show
    frequency to flush
      2
    exit
* -- specify which variables will appear in the show file:
*     showfile options...
*     OBPSF:show variable: rho 1
*     OBPSF:show variable: u 0
*     OBPSF:show variable: v 0
*     OBPSF:show variable: w 0
*     OBPSF:show variable: T 0
*     OBPSF:show variable: Mach Number 0
*     OBPSF:show variable: p 1
*     close show file options
* 
  * no plotting
  reduce interpolation width
    2
  boundary conditions
    * all=noSlipWall uniform(T=.3572)
    all=slipWall 
    square(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
    square(1,0)=superSonicOutflow
    square(0,1)=slipWall
    square(1,1)=slipWall
    done
*
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
*  .7 ok, .8 not ok 
*  cfl .8 .7  .85  .75
***
  * debug = 1 + 2 + 4 + 8
  debug $debug
****
*
  turn on adaptive grids
*   save error function to the show file
  show amr error function
  order of AMR interpolation
      2
  error threshold
     .2 .1
  regrid frequency
    4 8 4 8
  change error estimator parameters
    default number of smooths
      1
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      2 4 2 4 2 
    default number of refinement levels
      3 2 3 
    number of buffer zones
      2
    grid efficiency
      .7 .5 
  exit
*
*************************
  initial conditions
*   Here is where we read the initial conditions from a
*   previously created showfile 
   read from a show file
    cicShockg.show
     -1
   exit
* 
*************************
continue
*

movie mode
finish

