#
# Test of the multifluid solver -- Riemann problem (example from Section 4.2 of the cavity collapse paper)
#
#   cgcns [-noplot] multifluidChannel -tf=<f> -tp=<f> -amr=[0|1] -ratio=[2|4] -show=<s> -go=[halt|go|og]
#
#
$tFinal=.06; $tPlot=.004; $show=" "; $debug=0; $ratio=4; $buffer=2; $errTol=.0025; $efficiency=.7;
#
$grid="multifluidChannelGrid.hdf"; $levels=3; $show=" "; $debug=0; 
$xStep="x=-1.5"; $go="halt"; 
$amr=0; 
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"amr=i"=>\$amr,"l=i"=>\$nrl,"r=i"=>\$ratio,"tf=f"=>\$tFinal,"debug=i"=>\$debug, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep,"show=s"=>\$show,"go=s"=>\$go );
* -------------------------------------------------------------------------------------------------
if( $amr eq "0" ){ $amr="turn off adaptive grids"; }else{ $amr="turn on adaptive grids"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
#
$grid
#
  compressible Navier Stokes (multi-fluid)
  stiffened gas equation of state
  exit
#
  turn off twilight
#
  final time $tFinal
  times to plot $tPlot
  cfl .8
#  dtMax 7.e-5
 debug
   $debug
# no plotting
  plot and always wait
  show file options
    compressed
    open
      $show
    frequency to flush
      1
    exit
  reduce interpolation width
    2
#
  initial conditions
    OBIC:step function...
    OBIC:state ahead r=1.4, u=0, v=0, T=0.714286, mu1=2.5, mu2=0
    OBIC:state behind r=3063., u=1.932, v=0., T=11.19817, mu1=0.25, mu2=8571.25
    OBIC:step: a*x+b*y+c*z=d 1, 0, 0, 0.5, (a,b,c,d)
    OBIC:assign step function
    close step function
    exit
#
#  initial conditions
#    user defined
#      planar interface with shock
#      exit
#    exit
#
  pde options
    OBPDE:Godunov order of accuracy 1
    OBPDE:artificial viscosity 0.
#    OBPDE:artificial diffusion 2. 2. 2. 2. 2. 2.
#    OBPDE:artificial diffusion .5 .5 .5 .5 .5 .5
    OBPDE:artificial diffusion 0. 0. 0. 0. 0. 0.
    OBPDE:mu 0.
    OBPDE:kThermal 0.
    OBPDE:Rg (gas constant) 1.
    OBPDE:heat release 0.
  close pde options
#
  boundary conditions
    all=superSonicOutflow
# (side,axis)
    square(0,1)=slipWall
    square(1,1)=slipWall
  done
#
    adaptive grid options...
    use user defined error estimator 1
    close adaptive grid options
#
  $amr
  order of AMR interpolation
      2
  error threshold
     $errTol
  regrid frequency
     $regrid=2*$ratio;
     $regrid
  change error estimator parameters
    set scale factors
      1 1 1 1 1 1
    done
    weight for first difference
      0. 1.
    weight for second difference
      0. 1.
    exit
#    truncation error coefficient
#      1.
    show amr error function
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $levels
    number of buffer zones
      $buffer
    grid efficiency
      $efficiency
  exit
continue
#
$go

