# -*- mode: text; -*-
#
# This file is included by other command files to define a Cgsm domain
#
# The following parameters should be set before including this file: 
#   $domainName : name of the domain to assign
#   $solverName : name given to the domain (e.g. "fluid")
#   $tz, $degreeSpace, $degreeTime : 
#   $tsSM  : time-stepping method 
#   $errorNorm : "maximum norm", "l1 norm", "l2 norm"
#   $lambda, $mu : Lame constants for the solid
#   $rhoSolid : 
#   $smVariation : non-conservative, conservative, godunov, hemp
#   $smCheckErrors : 1=check and plot errors
#   $smPlotStress  : 1= plot Cauchy stress
#   $cfl :
#   $diss  : coeff. of artificial dissipation
#   $dissOrder : order of artificial dissipation [2|4]
#   $godunovType :  : 0=linear, 2=SVK
#   $stressRelaxation : 
#   $relaxAlpha : parameter for stress relaxtion 
#   $relaxDelta : parameter for stress relaxtion 
#   $slopeLimiter
#   $debug 
#   $bcCommands : commands to specify boundary conditions
#   $initialConditionCommands : commands to specify the initial conditions
#   $extraCmds : extra commands
# 
if( $extraCmds eq "" ){ $extra="*"; }
# 
# ------- start new domain ----------
if( $tsSM eq "" ){ $tsSM="modifiedEquationTimeStepping";}
# if( $cons eq "" ){ $cons=0;}
if( $diss eq "" ){ $diss=0.;}
if( $dissOrder eq "" ){ $dissOrder=2;}
if( $cfl eq "" ){ $cfl=.9;}
if( $lambda eq "" ){ $lambda=1.;}
if( $mu eq "" ){ $mu=1.;}
if( $rhoSolid eq "" ){ $rhoSolid=1.;}
if( $dissOrder eq "" ){ $dissOrder=2; }
if( $smVariation eq "" ){ $smVariation = "non-conservative"; $cons=0; }
if( $smVariation eq "conservative" ){ $cons=1; }
# if( $cons eq "1" ){ $smVariation = "conservative"; }else{ $smVariation = "non-conservative"; }
if( $smVariation eq "hemp" ){ $tsSM="improvedEuler"; }
if( $smCheckErrors eq "" ){ $smCheckErrors=0; }
if( $smPlotStress eq "" ){ $smPlotStress=0; }
if( $errorNorm eq "" ){ $errorNorm="*"; }
if( $applyInterfaceConditions eq "" ){ $applyInterfaceConditions=1; }
if( $slopeLimiter eq "" ){ $slopeLimiter=0; }
if( $stressRelaxation eq "" ){ $stressRelaxation=1; }
if( $tangentialStressDissipation eq "" ){ $tangentialStressDissipation=0.1; }
if( $tangentialStressDissipation1 eq "" ){ $tangentialStressDissipation1=0.; }
if( $displacementDissipation eq "" ){ $displacementDissipation=.5; }
if( $displacementDissipation1 eq "" ){ $displacementDissipation1=.0; }
if( $relaxAlpha eq "" ){ $relaxAlpha=.1; }
if( $relaxDelta eq "" ){ $relaxDelta=.1; }
#
if( $godunovOrder eq "" ){ $godunovOrder=2; }
if( $godunovType eq "" ){ $godunovType=0; }
# -hemp:
if( $hempRg eq "" ){ $hempRg=8.314/27.;}
if( $hempYield eq "" ){ $hempYield=1.e10; }
if( $hempBasePress eq "" ){ $hempBasePress=0.0; }
if( $hempC0 eq "" ){ $hempC0=1.0; }
if( $hempCl eq "" ){ $hempCl=2.0; }
if( $hempHgVisc eq "" ){ $hempHgVisc=4.e-2; }
if( $hempHgFlag eq "" ){ $hempHgFlag=2; }
if( $hempApr eq "" ){ $hempApr=0.0; }
if( $hempBpr eq "" ){ $hempBpr=0.0; }
if( $hempCpr eq "" ){ $hempCpr=0.0; }
if( $hempDpr eq "" ){ $hempDpr=0.0; }
if( $trigTzScaleFactor eq "" ){ $trigTzScaleFactor=1.; }
if( $tzCmds eq "" ){ $tzCmds="#"; }
# 
# ------- start new domain ----------
setup $domainName
 set solver Cgsm 
 solver name $solverName
 solver parameters
#
linear elasticity
 $smVariation
 continue
# 
$tsSM
# 
#
final time $tFinal
times to plot $tPlot
# 
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:rho $rhoSolid 
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:slope limiting for Godunov $slopeLimiter
SMPDE:artificial diffusion $diss $diss $diss $diss $diss $diss $diss $diss $diss $diss $diss $diss $diss $diss $diss 
SMPDE:PDE type for Godunov $godunovType
SMPDE:stressRelaxation $stressRelaxation
SMPDE:relaxAlpha $relaxAlpha
SMPDE:relaxDelta $relaxDelta
SMPDE:tangential stress dissipation $tangentialStressDissipation  $tangentialStressDissipation1
SMPDE:displacement dissipation $displacementDissipation $displacementDissipation1
# --- start hemp parameters ---
SMPDE:Rg $hempRg
SMPDE:yield stress $hempYield
SMPDE:base pressure $hempBasePress
SMPDE:c0 viscosity $hempC0
SMPDE:cl viscosity $hempCl
SMPDE:hg viscosity $hempHgVisc
SMPDE:EOS polynomial $hempApr $hempBpr $hempCpr $hempDpr
SMPDE:hourglass control $hempHgFlag
# --- end hemp parameters ---
#
# ----- Twilight Zone. NOTE: TZ cmds should come after setting pde type etc.----
$tzsm=0; $tzsmType="polynomial";  # default TZ is off
if( $tz eq "turn on twilight zone\n turn on polynomial" ){ $tzsm=1; $tzsmType="polynomial"; $smCheckErrors=1; }
if( $tz eq "turn on twilight zone\n turn on trigonometric" ){ $tzsm=1; $tzsmType="trigonometric"; $smCheckErrors=1; }
OBTZ:$tzsmType
OBTZ:twilight zone flow $tzsm
OBTZ:degree in space $degreex
OBTZ:degree in time $degreet
OBTZ:trigonometric scale factor $trigTzScaleFactor
OBTZ:frequencies (x,y,z,t) $fx $fy $fz $ft
$errorNorm
$tzCmds
# -----------------------------
close forcing options
#
boundary conditions
  $bcCommands 
done  
# we sometimes turn off application of the interface conditions when they are done by cgmp:
boundary conditions...
  apply interface conditions $applyInterfaceConditions
done
#
# displacement scale factor 0.4
displacement scale factor 1.
dissipation $diss
order of dissipation $dissOrder
cfl $cfl
use conservative difference $cons
# 
plot divergence 1
plot vorticity 1
if( $smVariation eq "non-conservative" || $smVariation eq "conservative" ){ $plotCommands = "plot velocity 1\n plot stress 1"; }else{ $plotCommands="*"; }
$plotCommands
initial conditions options...
if( $restart eq "" ){ $icCmds = $initialConditionCommands; }\
 else{ $icCmds = "use grid from show file 1\n always interpolate from show file 1\n read from a show file\n $restart\n -1"; }
# new way: doesn't work yet: 
#  else{ $icCmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n use grid from show file 1\n always interpolate from show file 1\n OBIC:assign solution from show file"; }
 $icCmds
 # pause
close initial conditions options
check errors $smCheckErrors
plot errors $smCheckErrors
# plot Cauchy stress: 
plot stress $smPlotStress
#
debug $debug
#
#
  $extraCmds
#
continue
done
# --------------------------------- end cgsm commands ------------------------------------------
