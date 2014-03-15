$grid="square40.hdf";
*
$tFinal=0.4;
$tPlot=0.1;
$cfl=0.8;
*
$showFile = ""; $acm = 2.0;
*
*$amr="turn on adaptive grids";
$amr="turn off adaptive grids";
$amrRatio=4;
$amrLevels=2;
*
$artificialViscosity=0.2;
*
***** set up parameters
$p0 = 1.0;
$gamAir = 1.4;  $cvAir = 0.72;  $muAir = 1.0; $lamAir = 1.0; $rhoAir = 1.0;   $tAir = $p0/$rhoAir;
$gamHe  = 1.67; $cvHe  = 3.11;  $muHe  = 1.0; $lamHe  = 0.0; $rhoHe  = 0.138; $tHe  = $p0/$rhoHe;
$gamR22 = 1.25; $cvR22 = 0.365; $muR22 = 0.0; $lamR22 = 1.0; $rhoR22 = 3.15;  $tR22 = $p0/$rhoR22;
$gamCon = 2.0;  $cvCon = 1.0;   $muCon = 0.0; $lamCon = 0.0; $rhoCon = 2.5;   $tCon = $p0/$rhoCon;
*****
*
***** set shock jump in the air
$M=1.22;
$gamma=$gamAir;
$rho1=$rhoAir;
$u1=0.0;
$v1=0.0;
$p1=$p0;
$t1=$p1/$rho1;
*
$rho2=$rho1*($gamma+1.)*$M*$M/(($gamma-1.)*$M*$M+2.0);
$u2=sqrt($gamma*$p1/$rho1)*2.*($M*$M-1.)/(($gamma+1.)*$M)+$u1;
$v2=0.0;
$p2=$p1*2.*$gamma*($M*$M-1.)/($gamma+1.)+$p1;
$t2=$p2/$rho2;
*****
*
$grid
*
  compressible Navier Stokes (multi-component)
  one step pressure law
*
  define real parameter gamma1      $gamAir   * mu = 1, lam = 1
  define real parameter cv1         $cvAir
  define real parameter gamma2      $gamHe    * mu = 1, lam = 0
  define real parameter cv2         $cvHe
  define real parameter gamma3      $gamR22   * mu = 0, lam = 1
  define real parameter cv3         $cvR22
  define real parameter gamma4      $gamCon   * mu = 0, lam = 0
  define real parameter cv4         $cvCon
*
  define integer parameter slope          1
  define integer parameter fix            1
  define integer parameter useDon         1
  define integer parameter fourComp       1
  define integer parameter acousticSwitch 1
  define real parameter acmConst          $acm
  exit
*
  turn off twilight 
*
  final time (tf=)
    $tFinal
  times to plot (tp=)
    $tPlot
  cfl 
    $cfl
*
* dtMax
* .0025
*
  OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
  OBPDE:Godunov order of accuracy 2
  OBPDE:interpolate primitive and pressure
* plot and always wait
  no plotting
* debug
* 31
*
  show file options
    compressed
     open
     $showFile
    frequency to flush
    1
    exit
*
  pde options
  OBPDE:mu 0.
  OBPDE:kThermal 0.
*  OBPDE:Rg (gas constant) 1.
*  OBPDE:gamma 2.0
  OBPDE:heat release 0.0
  OBPDE:1/(activation Energy) 1.0
  OBPDE:rate constant 10.0
  OBPDE:artificial viscosity $artificialViscosity
  close pde options
*************************
  initial conditions
    user defined
      bubbles with shock
      3  * number of bubbles
      r=$rhoAir u=$u1 v=$v1 T=$tAir lambda=$muAir s=$lamAir   * background state
      0.15 0.5 0.75                                           * radius and center bub 1
      r=$rhoHe  u=$u1 v=$v1 T=$tHe  lambda=$muHe  s=$lamHe    * bub 1 state
      0.2 0.5 0.25                                            * radius and center bub 2
      r=$rhoR22 u=$u1 v=$v1 T=$tR22 lambda=$muR22 s=$lamR22   * bub 2 state
      0.2 0.75 0.5                                            * radius and center bub 3
      r=$rhoCon u=$u1 v=$v1 T=$tCon lambda=$muCon s=$lamCon   * bub 3 state
      0.2                                                     * shock location
      r=$rho2   u=$u2 v=$v2 T=$t2   lambda=$muAir s=$lamAir   * shock state
    exit
  done
*************************
*
* turn on axisymmetric flow
*
  boundary conditions
    all=superSonicOutflow
*    all=slipWall
*    square(0,0)=slipWall
    done
*
  reduce interpolation width
  2
*
* turn on user defined output
*
  $amr
  order of AMR interpolation
      2
  error threshold
      .0005
  regrid frequency
      8
  change error estimator parameters
    set scale factors
      1 1 1 1 1 1 
    done
    weight for first difference
    0.
    weight for second difference
    1.    .03
    exit
    truncation error coefficient
    1.
    show amr error function
  change adaptive grid parameters
    refinement ratio
      $amrRatio
    default number of refinement levels
      $amrLevels
    number of buffer zones
      2
    grid efficiency
      .7
  exit
continue
movie mode
finish

