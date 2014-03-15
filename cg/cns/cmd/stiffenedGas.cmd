#
# Test of the Stiffened Gas EOS 
#    cgcns [-noplot] stiffenedGas -g=<name> -testCase=[ideal|stiff1|stiff2] -userDefinedEOS=[0|1]
#
# NOTE: these examples define STEADY shock profiles for an ideal and stiffened gas.
#
# Examples:
#    cgcns stiffenedGas -g=channelShort -testCase=stiff2
#    cgcns stiffenedGas -g=channelShort -testCase=stiff2 -userDefinedEOS=1
# -- 3D:
#    cgcns stiffenedGas -g=box40.order2 -testCase=stiff2 -userDefinedEOS=1
#
$mu=0.; $kThermal=0.; $Prandtl=.72; $cfl=.9; $go="halt"; 
$tFinal=5.; $tPlot=.1; 
$grid="channelShort.hdf";
$testCase="stiff2"; $userDefinedEOS=0; 
$show = "stiffenedGas.show";
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"testCase=s"=> \$testCase, "tf=f"=>\$tFinal,"debug=i"=> \$debug, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "cnsVariation=s"=>\$cnsVariation,"userDefinedEOS=i"=> \$userDefinedEOS );
* -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# Left and right states are from Veronica Eliasson ( cg/cns/doc/stiffenedGas.mv)
#
#
# -- ideal gas ! works 3/11
if( $testCase eq "ideal" ){ \
 $v0=0.; $gamma=1.4;$US=2.; $gammaStiff=1.4; $pStiff=0.0; $r1=1.; $u1=-$US; $p1=0.7; \
 $r2=2.6966;  $u2=1.25833-$US;  $p2=3.21666;  $T1=$p1/$r1; $T2=$p2/$r2; }
#
# -- Tamman Test 1 -OK-
if( $testCase eq "stiff1" ){ \
  $v0=0.; $gamma=1.4; $US=2.; $gammaStiff=1.4; $pStiff=0.1; $r1=1.; $u1=-$US; $p1=0.7; \
  $r2=2.500;  $u2=1.2-$US; $p2=3.1; }
# 
if( $testCase eq "stiff2" ){ \
  $v0=0.; $gamma=7.15; $US=1.5; $gammaStiff=7.15; $pStiff=0.1111111111; $r1=1.; $u1=-$US;\
  $p1=0.00003703703704; $r2=1.188669013;  $u2=0.238084372-$US;   $p2=0.357163595; }
#
# -- define T
 $T1=$p1/$r1; $T2=$p2/$r2;
# 
$mu=.0; $kThermal=$mu/$Prandtl; 
#
$grid
#* channelFine
# channel.hdf
## channelShort
# channelShortCoarse
#  
  compressible Navier Stokes (Godunov)  
  stiffened gas equation of state
  #  -- test user defined EOS
  $cmd="#"; 
  if( $userDefinedEOS ne 0 ){ $cmd="user defined equation of state\n stiffened gas\n $gammaStiff $pStiff\n done"; }
  $cmd
  # ideal gase case: over-ride above
  if( $testCase eq "ideal" ){ $cmd ="ideal gas law"; }else{ $cmd="#"; }
   $cmd
#
#*  one step
#
# -- currently the stiffened gas just uses the MieGruneisen variables -- fix this ---
    define real parameter alphaMG  $gammaStiff
    define real parameter betaMG   $pStiff
# 
  exit
  turn off twilight
  final time $tFinal 
  times to plot $tPlot 
  plot and always wait
 # no plotting
  show file options
    compressed
    open
      $show
    frequency to flush
      2
    exit
#
 reduce interpolation width
   2
#
#  variable time step PC
#
  OBPDE:gamma $gamma
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
#    Mach number
#      1.
#    conservative Godunov
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
  boundary conditions
    all=slipWall
#    rectangle(0,0)=superSonicInflow uniform(r=$r2,u=$u2,T=$T2)
#    rectangle(1,0)=superSonicOutflow
#
    bcNumber2=superSonicInflow uniform(r=$r1,u=$u1,T=$T1)
#    rectangle(0,0)=superSonicInflow uniform(r=$r2,u=$u2,T=$T2)
     bcNumber1=superSonicOutflow
   done
#
    cfl $cfl
#
#  debug
#    3
#**  turn on adaptive grids
#
#*  turn on user defined error estimator
#
  order of AMR interpolation
      2
  regrid frequency
    8 4 8
  error threshold
      .1 
  change error estimator parameters
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      4
    default number of refinement levels
      2 3 4 3 2 3 2 3 4 
    number of buffer zones
      2
  exit
#
#  allow user defined output 1
#
  initial conditions
 # x=.5
    step function
      x=.5
      r=$r2 u=$u2 T=$T2
      r=$r1 u=$u1 T=$T1
    continue
#**************
  debug
    0 
#**************
   continue
$go





