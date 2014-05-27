# -*- mode: text; -*-
#
# This file is included by other command files to define a Cgcns domain
#
# The following parameters should be set before including this file: 
#   $domainName : name of the domain to assign
#   $solverName : name given to the domain (e.g. "fluid")
#   $cnsVariation : jameson, godunov, nonconservative
#   $cnsEOS : equation of state, one of "ideal", "jwl", "mg", "user", "stiffened", "tait"
#   $cnsGammaStiff, $cnsPStiff : for the stiffened EOS
#   $tz, $degreeSpace, $degreeTime, $tzCmds : 
#   $errorNorm : "maximum norm", "l1 norm", "l2 norm"
#   $ts
#   $mu, $kThermal, $ktc
#   $ic : specify initial condition commands
#   $bc : specify boundary condition commands
#   $cnsGodunovOrder : 1 or 2
#   $cnsSaveAugmented : if 1, save all augmented variables (those plotted interactively) to the show file.
#   $slopeLimiter : 0=off, 1=on (default)
#   $orderOfExtrapForOutflow :
#   $orderOfExtrapForGhost2 :
#   $orderOfExtrapForInterpNeighbours :
#   $gravity : 
#   $adCns : linear artificial diffusion coefficient for the Godunov method
#   $moveCmds : commands for moving grids
#   $extraCmds : extra commands
#   $reduceInterpWidth : 2= reduce interpolation to 2, otherwise leave as is.
#   $densityLowerBound : 
#   $pressureLowerBound  :
#   $velocityLimiterEps  :
#   
if( $cfl eq "" ){ $cfl=0.9; }
if( $mu eq "" ){ $mu=0.; }
if( $kThermal eq "" ){ $kThermal=0.; }
if( $ktc eq "" ){ $ktc=0.; }
if( $adCns eq "" ){ $adCns=0.; }
if( $boundaryPressureOffset eq "" ){ $boundaryPressureOffset=0.; }
if( $tz eq "" ){ $tz="turn off twilight zone"; }
if( $tzCmds eq "" ){ $tzCmds="#"; }
if( $errorNorm eq "" ){ $errorNorm="#"; }
if( $moveCmds eq "" ){ $moveCmds="#"; }
if( $extraCmds eq "" ){ $extraCmds="#"; }
if( $cnsGodunovOrder eq "" ){ $cnsGodunovOrder=2;}
if( $cnsSaveAugmented eq "" ){ $cnsSaveAugmented=0;}
if( $degreex eq "" ){ $degreex=2;}
if( $degreet eq "" ){ $degreet=2;}
if( $cnsVariation eq "" ){ $cnsVariation="godunov";}  #
if( $cnsEOS eq "" ){ $cnsEOS="ideal";}  # changed below 
if( $cnsGammaStiff eq "" ){ $cnsGammaStiff=1.4;} 
if( $cnsPStiff eq "" ){ $cnsPStiff=0.;} 
if( $bcOption eq "" ){ $bcOption=0; }
if( $slopeLimiter eq "" ){ $slopeLimiter=1; }
if( $reduceInterpWidth eq "" ){ $reduceInterpWidth=2; }
if( $applyInterfaceConditions eq "" ){ $applyInterfaceConditions=1; }
if( $orderOfExtrapForOutflow eq "" ){ $orderOfExtrapForOutflow=-1;  }  # -1 = default 
if( $orderOfExtrapForGhost2 eq "" ){  $orderOfExtrapForGhost2=1; }  
if( $orderOfExtrapForInterpNeighbours eq "" ){ $orderOfExtrapForInterpNeighbours=1;  }  
if( $checkForWallHeating eq "" ){ $checkForWallHeating=0; }
if( $trigTzScaleFactor eq "" ){ $trigTzScaleFactor=1.; }
# ------- start new domain ----------
setup $domainName
 set solver Cgcns
 solver name $solverName
 solver parameters
# 
if( $cnsVariation eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $cnsVariation eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $cnsVariation eq "nonconservative" ){ $pdeVariation="compressible Navier Stokes (non-conservative)";}  #
if( $cnsEOS eq "ideal" ){ $cnsEOS = "ideal gas law";}\
elsif( $cnsEOS eq "jwl" ){ $cnsEOS = "JWL equation of state";}\
elsif( $cnsEOS eq "mg" ){ $cnsEOS = "Mie-Gruneisen equation of state";}\
elsif( $cnsEOS eq "user" ){ $cnsEOS = "user defined equation of state";}\
elsif( $cnsEOS eq "stiffened" ){ $cnsEOS = "stiffened gas equation of state\n define real parameter alphaMG  $cnsGammaStiff\n define real parameter betaMG $cnsPStiff";}\
elsif( $cnsEOS eq "tait" ){ $cnsEOS = "tait equation of state";}\
else{ $cnsEOS = "ideal gas law";} # default EOS is ideal
  $pdeVariation
  $cnsEOS
#   compressible Navier Stokes (Godunov)  
#   compressible Navier Stokes (Jameson)
   define integer parameter SlopeLimiter $slopeLimiter
  exit
#
$cnsts = $ts;
if( $cnsVariation eq "godunov" ){ $cnsts="forward Euler"; } # godunov always used FE
$cnsts 
# 
$tz
degree in space $degreex
degree in time $degreet
OBTZ:trigonometric scale factor $trigTzScaleFactor
frequencies (x,y,z,t)   $fx $fy $fz $ft
$errorNorm
$tzCmds
# 
show file options
  save augmented variables $cnsSaveAugmented
# pause
exit
#
if( $reduceInterpWidth eq 2 ){ $cmd="reduce interpolation width\n 2"; }else{ $cmd="#"; }
$cmd
# 
  cfl $cfl
  pde parameters
    mu $mu
    kThermal $kThermal
    thermal conductivity $ktc
    gravity
      $gravity
    OBPDE:artificial diffusion $adCns $adCns $adCns $adCns $adCns $adCns $adCns
    # offset for the pressure when computing the force on the boundary 
    OBPDE:boundary pressure offset $boundaryPressureOffset
    OBPDE:Godunov order of accuracy $cnsGodunovOrder
    check for wall heating $checkForWallHeating
    if( $densityLowerBound ne "" ){ $cmd="OBPDE:density lower bound $densityLowerBound"; }else{ $cmd="#"; }
    $cmd
    if( $pressureLowerBound ne "" ){ $cmd="OBPDE:pressure lower bound $pressureLowerBound"; }else{ $cmd="#"; }
    $cmd
    if( $velocityLimiterEps ne "" ){ $cmd="OBPDE:velocity limiter epsilon $velocityLimiterEps"; }else{ $cmd="#"; }
    $cmd
    # pause
  done
###############
#   OBPDE:exact Riemann solver
# OBPDE:Roe Riemann solver
# OBPDE:HLL Riemann solver
##################
$setAxi = $axisymmetric ? "turn on axisymmetric flow" : "#";
$setAxi
$setAxi="";
# 
  $moveCmds
# 
#
  boundary conditions
    $bc 
#    Annulus(0,1)=noSlipWall uniform(u=.0,T=300.)
#    square(0,0)=subSonicInflow uniform(r=1.,u=$u0,T=300.)
#    square(1,0)=subSonicOutflow mixedDerivative(1.*t+1.*t.n=300.)
#    square(0,1)=slipWall
#    square(1,1)=slipWall
    done
# we sometimes turn off application of the interface conditions when they are done by cgmp:
boundary conditions...
  apply interface conditions $applyInterfaceConditions
   order of extrap for outflow $orderOfExtrapForOutflow (-1=default)
   order of extrap for 2nd ghost line $orderOfExtrapForGhost2
   order of extrap for interp neighbours $orderOfExtrapForInterpNeighbours
done
  OBPDE:slip wall boundary condition option $bcOption
  debug $debug
  initial conditions
  if( $tz eq "turn off twilight zone" && $ic eq "" ){ $ic="uniform flow\n" . "r=1. u=$u0 T=300."; }elsif( $ic eq "" ){ $ic="#";}
if( $restart eq "" ){ $icCmds = $ic; }\
  else{ $icCmds = "use grid from show file 1\n always interpolate from show file 1\n read from a show file\n $restart\n -1"; }
# new way:
#  else{ $icCmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n use grid from show file 1\n always interpolate from show file 1\n OBIC:assign solution from show file"; }
    $icCmds
#  pause
  continue
#
  $extraCmds
# 
 continue
done
