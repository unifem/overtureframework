#
# This file is included by other command files to define a Cgad domain
#
# The following parameters should be set before including this file: 
#   $domainName : name of the domain to assign
#   $solverName : name given to the domain (e.g. "solid")
#   $tz, $degreeSpace, $degreeTime, $fx, $fy, $fz, $ft
#   $ts : forward Euler, implicit, adams PC
#   $kappa, 
#   $ktc    : thermal conductivity
#   $T0     : initial condition given to the temperature
#   $ic     : specify initial condition commands
#   $bc     : specify boundary condition commands
#   $debug  : 
#   $implicitFactor : for implicit time-stepping: .5=CN, 1.=BE, 0.=FE
#   $commands : additional commands
#   $axisymmetric : if non-null turn on the axisymmetric option
#   $amr, $amrTol, $amrRatio, $amrLevels, $amrBufferZones, $showAmrError
#   $xPulse,$yPulse,$zPulse : initial location of the TZ pulse 
#   $vxPulse,$vyPulse,$vzPulse : velocity of the TZ pulse 
#   $pulsevxPulse,$vyPulse,$vzPulse : velocity of the TZ pulse 
#   $pulseAmplitude $pulseExponent $pulsePower : TZ pulse parameters
#
if( $tz eq "" ){ $tz="turn off twilight zone"; }
#
if( $dtMax eq "" ){ $dtMax=1.e20; }
if( $fx eq "" ){ $fx=1.; }
if( $fy eq "" ){ $fy=1.; }
if( $fz eq "" ){ $fz=1.; }
if( $ft eq "" ){ $ft=1.; }
if( $implicitFactor eq "" ){ $implicitFactor=.5; }
if( $rtoli eq "" ){ $rtoli=1.e-5; }
if( $atoli eq "" ){ $atoli=1.e-7; }
if( $debugi eq "" ){ $debugi=0; }
if( $T0 eq "" ){ $T0=0.; }
if( $amr eq "" ){ $amr = "turn off adaptive grids"; }
if( $amrTol eq "" ){ $amrTol=1.e-3; }
if( $amrRatio eq "" ){ $amrRatio=2; }
if( $amrLevels eq "" ){ $amrLevels=2; }
if( $amrBufferZones eq "" ){ $amrBufferZones=2; }
if( $showAmrError eq "" ){ $showAmrError=0; }
if( $xPulse eq "" ){ $xPulse=0.; }
if( $yPulse eq "" ){ $yPulse=0.; }
if( $zPulse eq "" ){ $zPulse=0.; }
if( $vxPulse eq "" ){ $vxPulse=1.; }
if( $vyPulse eq "" ){ $vyPulse=0.; }
if( $vzPulse eq "" ){ $vzPulse=0.; }
if( $pulseAmplitude eq "" ){ $pulseAmplitude=1.; }
if( $pulseExponent eq "" ){ $pulseExponent=40.; }
if( $pulsePower eq "" ){ $pulsePower=1; }
# ------- start new domain ----------
#  Cgad solid
setup $domainName
 set solver Cgad
 solver name $solverName
 solver parameters
# 
  convection diffusion
  continue
# 
  $ts
  implicit factor $implicitFactor 
# 
  pde parameters
    kappa $kappa
    thermal conductivity $ktc 
    a 0. 
    b 0. 
    c 0. 
  done
  $setAxi = $axisymmetric ? "turn on axisymmetric flow" : "*";
  $setAxi
  $setAxi="";
#
  if( $commands eq "" ){ $commands_ad="debug $debug";}else{$commands_ad=$commands; } 
  $commands_ad
  $commands_ad=""; 
  dtMax $dtMax
  debug $debug 
#
# 
  implicit time step solver options
     $solver
 # these tolerances are chosen for PETSc
     relative tolerance
      $rtoli
     absolute tolerance
      $atoli
     debug 
       $debugi
    exit
# 
  boundary conditions
   $bc 
    done
#
  $tz
  degree in space $degreeSpace
  degree in time $degreeTime
  OBTZ:frequencies (x,y,z,t) $fx, $fy, $fz, $ft
#
  OBTZ:pulse center $xPulse $yPulse $zPulse
  OBTZ:pulse velocity $vxPulse $vyPulse $vzPulse
  OBTZ:pulse amplitude, exponent, power $pulseAmplitude $pulseExponent $pulsePower
# 
  initial conditions
  if( $tz eq "turn off twilight zone" && $ic eq "" ){ $ic="uniform flow\n" . "T=$T0"; }elsif( $ic eq "" ){ $ic="*";}
    $ic
  continue
#****  Here we optionally turn on AMR *******
  $amr
  order of AMR interpolation
     3
  error threshold
     $amrTol
  regrid frequency
    $regrid=$amrRatio*$amrBufferZones;
    $regrid
  change error estimator parameters
    set scale factors
      1 1 1 1 1 1 
    done
    weight for first difference
      1.
    weight for second difference
      1.
    exit
    truncation error coefficient
      1.
    show amr error function $showAmrError
  change adaptive grid parameters
    refinement ratio
      $amrRatio
    default number of refinement levels
      $amrLevels
    number of buffer zones
      $amrBufferZones
    grid efficiency
      .7
  exit
#****
## kkc 080328 reset cmds in case it is used in other includes or command files
    $cmds = ""; 
# 
  continue
done
# 
