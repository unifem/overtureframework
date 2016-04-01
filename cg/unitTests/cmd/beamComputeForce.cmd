#
#  check the force computation in the beam model
#
$option="externalForce"; 
$numElem=10; 
$orderOfProjection=2;
$forceDegreeX=1; 
$degreeX=2; $degreeT=1; 
$rhoBeam=10.; $thickness=.1; $I=.1; $E=1.; $tension=0.; $k0=0.; $Kt=0.; $Kxxt=0.; 
# ------------------------------------------------------------------------------------------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreeX=i"=>\$degreeX, "degreeT=i"=>\$degreeT,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"numElem=i"=>\$numElem,"orderOfProjection=i"=>\$orderOfProjection, \
 "thick=f"=>\$thick,"forceDegreeX=f"=>\$forceDegreeX,"cfl=f"=>\$cfl,"noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,"ad2=i"=>\$ad2,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,"ad4=i"=>\$ad4,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"p0=f"=>\$p0,"addedMass=i"=>\$addedMass,"rhoBeam=f"=>\$rhoBeam,\
 "bdebug=i"=>\$bdebug,"ampProjectVelocity=i"=>\$ampProjectVelocity,"dsBeam=f"=>\$dsBeam,\
 "option=s"=>\$option,"E=f"=>\$E,"Kt=f"=>\$Kt,"K0=f"=>\$K0,"Kxxt=f"=>\$Kxxt,"tension=f"=>\$tension,\
 "thickness=f"=>\$thickness );
# ------------------------------------------------------------------------------------------------------------------------
#
force polynomial degree x: $forceDegreeX
#
#Longfei 20160116: new options added: FEM or FD:
Finite Element
change beam parameters
  #
  bc left:clamped
  bc right:clamped
  debug: 0
  #
  density: $rhoBeam
  thickness: $thickness
  area moment of inertia: $I
  elastic modulus: $E
  tension: $tension
  K0: $K0
  Kt: $Kt
  Kxxt: $Kxxt
  number of elements: $numElem
  order of Galerkin projection: $orderOfProjection
  Twilight-zone: polynomial
  degree in space: $degreeX
  degree in time: $degreeT
  use exact solution 1
  twilight-zone 1
  exact solution...
    Exact solution:twilight zone
  exit
  initial conditions...
    Initial conditions:exact solution
  exit
exit
if( $option eq "externalForce" ){ $cmd="check force"; }else{ $cmd="check internal force"; }
 $cmd
exit
