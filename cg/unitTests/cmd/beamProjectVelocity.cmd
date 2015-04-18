#
# Test projecting the velocity on the beam model
#
# ----------------------------- get command line arguments ---------------------------------------
$degreex=2; $degreet=2; $orderOfProjection=2; 
$thick=.1; $numElem=10; 
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"numElem=i"=>\$numElem,"orderOfProjection=i"=>\$orderOfProjection, \
 "thick=f"=>\$thick,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,"ad2=i"=>\$ad2,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,"ad4=i"=>\$ad4,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"p0=f"=>\$p0,"addedMass=i"=>\$addedMass,"rhoBeam=f"=>\$rhoBeam,\
 "bdebug=i"=>\$bdebug,"E=f"=>\$E,"ampProjectVelocity=i"=>\$ampProjectVelocity,"dsBeam=f"=>\$dsBeam );
# -------------------------------------------------------------------------------------------------
standing wave
change beam parameters
  number of elements: $numElem
  thickness: $thick
  fluid on two sides 1
  order of Galerkin projection: $orderOfProjection
#
  bc left:clamped
  bc right:clamped
#
  twilight-zone 1
  Twilight-zone: polynomial
  degree in space: $degreex
  degree in time: $degreet
  exact solution...
    Exact solution:twilight zone
  exit 
  initial conditions...
    Initial conditions:exact solution
  exit 
# 
  show parameters
exit
check velocity projection
exit



  Exact solution:standing wave
  initial conditions...
    standing wave
    amplitude: .01
    standing wave t0: .25 
  exit
  show parameters
exit
check velocity projection