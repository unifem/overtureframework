*
* cgmp: deforming grid multi-domain examples
* 
* Usage:
*    cgmp [-noplot] deform -g=<name> -nu=<num> -kappa=<num> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> ...
*                      -tz=[poly/trig/none] -bg=<backGroundGrid> -degreex=<num> -degreet=<num> -ts=<fe/be/im>
* 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
* 
* Examples: 
* 
* diskAblate: front half of the disk deforms: 
*  cgmp deform.cmd -g="diskAblatee1.hdf" -solver=yale -tz=poly
*  cgmp deform.cmd -g="diskAblatee2.hdf" -solver=yale -tz=none
*
* diskDeform: the whole disk deforms:
*  cgmp deform.cmd -g=diskDeforme1 -dt="ellipse deform" -dg1=innerInterface -dg2=outerInterface -solver=yale -tz=poly
*  cgmp deform.cmd -g=diskDeforme2 -dt="ellipse deform" -dg1=innerInterface -dg2=outerInterface -solver=yale -tz=poly
*
*  cgmp deform.cmd -g=diskDeforme1 -dt="ellipse deform" -dg1=innerInterface -dg2=outerInterface -solver=yale -tz=poly -show="deform.show" -tp=.05 
* 
* sphere deform: (to-do)
*  cgmp deform.cmd -g=solidSphereDeformi1.order2 -dt="sphere deform" -dg1="share=100" -dg2="share=100" -solver=best -tz=trig
* 
* --- set default values for parameters ---
* 
$grid="diskAblatei1.hdf";
$tFinal=2.; $tPlot=.01; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; 
$ts="pc"; 
$nu=.1; $prandtl=.72;  $thermalExpansivity=1.;  $kappa=.1; $ktc=.1; $pdebug=0; 
$T0=10.; $Twall=10.; # T0 = initial T in the solid 
$tz=none; # turn on tz here
$gravity = "0 -10. 0.";
$solver="best"; 
$backGround="outerSquare"; 
$deformFrequency=2.; $deformAmplitude=1.; $deformationType="ice deform"; 
$deformingGrid1="innerIce"; $deformingGrid2="outerIce";
* 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"kappa=f"=>\$kappa, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "show=s"=>\$show,"ts=s"=>\$ts,"noplot=s"=>\$noplot,\
  "dg1=s"=>\$deformingGrid1,"dg2=s"=>\$deformingGrid2,"dt=s"=>\$deformationType,"da=f"=>\$deformAmplitude );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC";       $tsd="adams PC";  }
* 
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $uMin=-1; $uMax=1.; }
* 
$kThermal=$nu/$prandtl;
* 
$grid
* 
$domain1="innerDomain"; $domain2="outerDomain";
* 
$iface100="bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\nbcNumber100=heatFluxInterface";
$iface101="bcNumber101=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\nbcNumber101=heatFluxInterface";
* 
sub deformCommands\
{ local $cmds; \
  $cmds= "turn on moving grids\n" . \
          "specify grids to move\n" .\
            "deforming body\n" .\
              "user defined deforming body\n" .\
                "$deformationType\n" .\
                "deformation frequency\n" .\
                  "$deformFrequency\n" .\
                "deformation amplitude\n" .\
                " $deformAmplitude\n" .\
              "done\n" .\
              "$deformingGrid\n" .\
            "done\n" .\
          "done\n"; \
 $cmds;\
}
* ------- Assign domains ----------
$domainName=$domain1; $solverName="solidA"; 
$deformingGrid=$deformingGrid1;
$commands=deformCommands();
$bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n$iface100\n$iface101";
include adDomain.h
*
$domainName=$domain2; $solverName="solidB"; $Twall=0.; $T0=0.; 
$bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n $iface100\n$iface101"; 
$deformingGrid=$deformingGrid2; 
$commands=deformCommands();
include adDomain.h
*
continue
* 
* -- set parameters for cgmp ---
* 
  final time $tFinal
  times to plot $tPlot
  $ts 
  cfl $cfl
  $tz
* 
  show file options
    compressed
      open
       $show
    frequency to flush
      2
    exit
  continue
*
continue


   plot:fluidA : T
   plot:solidA : T
   plot:solidB : T
   plot:solidC : T
   plot:solidD : T
   plot:fluidB : T


