#
# cgins command file for flow past a cylinder
#
# Usage:
#   cgins [-noplot] cic -g=<name> -tf=<> -tp=<> -nu=<> -ts=[pc|im|afs] -iv=[viscous|full] -tm=[les] ...
#          -solver=[best|yale|mg] -project=[0|1] -model=[ins|boussinesq] ...
#         -psolver=[best|yale|mg] -pc=[ilu|lu] -inflow=[uniform|parabolic|control|pressure|radial] -oscillate=[0|1] ...
#         -wall=[noSlip|slip] -cyl=[noSlip|slip] -restart=<showFile> -cgSolver=[best|hypre] ...
#
# Options:
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -iv : implicit variation: viscous=viscous terms implicit. full=all terms implicit. 
#  -psolver : pressure solver
#  -inflow : inflow boundary condition
#       = radial : specify inflow on the annulus using the user defined normal component boundary values option
#  -oscillate : =1 to use an oscillating inflow 
#  -wall : boundary condition for the top and bottom walls 
#  -cyl : boundary condition for the cylinder
#
# Examples:
#   cgins cic -g=cice2.order2 
#   cgins cic -g="cice2.order2.ml2.hdf" -solver=mg
#   cgins cic -g=cice4.order2 -inflow=parabolic -wall=noSlip -ad2=1 [ok 
#   cgins cic -g=cice4.order2 -inflow=parabolic -wall=slip   -ad2=1 [ok 
#   cgins cic -g=cice4.order2 -inflow=parabolic -wall=slip -ad2=1 -ad22=.001 [OK
#   cgins cic -g=cice4.order2 -inflow=parabolic -wall=noSlip -ad2=0 [OK
#   cgins cic -g=cice4.order2 -inflow=parabolic -wall=slip -cyl=slip -ad2=1 [*BAD* -> wrong number of imp solvers
#   cgins cic -g=cice4.order2 -order=2 -wall=slip -tp=.05 -nu=1.e-5 -ad2=1 -ad22=2. -ts=im [OK. note nu is "too" small
# -- restart example:
#    Step 1: cgins cic -g=cice4.order2 -inflow=parabolic -wall=noSlip -ad2=1 -tf=1. -tp=.5 -show=cic.show
#    Step 2: cgins cic -g=cice4.order2 -inflow=parabolic -wall=noSlip -ad2=1 -restart=cic.show -show=cic2.show
# -- oscillating inflow 
#   cgins cic -g=cice4.order2 -inflow=parabolic -oscillate=1 -tp=.2 -wall=noSlip -ad2=1 [ok 
#   cgins cic -g=cice4.order2 -inflow=uniform -oscillate=1 -tp=.2 -wall=noSlip -ad2=1 [ok 
# -- pressure given at inflow
#   cgins cic -g=cice4.order2 -inflow=pressure -tp=.05 -ad2=1 -ts=pc [OK
#   cgins cic -g=cice2.order2.ml2 -inflow=pressure -tp=.05 -ad2=1 -ts=afs [OK!
#   cgins cic -g=cice4.order2 -inflow=pressure -tp=.05 -ad2=1 -ts=im [ FINISH ME - scalar systems
#   cgins cic -g=cice4.order2 -inflow=pressure -tp=.05 -ad2=1 -ts=im -iv=full [OK 
#   -- pressure-inflow, order=4 now works: (note: turn off project-initial-conditions for now)
#   cgins cic -g=cice2.order4.ml2 -nu=.01 -ts=im -debug=3 -inflow=pressure -ad4=1 -ad2=0 -project=0 
#   cgins cic -g=cice4.order4.ml3 -nu=.005 -ts=pc -debug=3 -inflow=pressure -ad4=1 -ad2=0 -project=0
# -- radial inflow in the annulus (test user defined normal component boundary values option)
#   cgins cic -g=cice2.order2 -inflow=radial -uInflow=0
# -- 2nd-order + fourth-order diss:
#   cgins cic -g=cice4.order2 -order=2 -wall=slip -tp=.01 -nu=1.e-5 -ad2=0 -ad4=1 -ad42=2. -project=0 -ts=pc [OK 
#   cgins cic -g=cice4.order2 -order=2 -wall=slip -tp=.01 -nu=1.e-5 -ad2=0 -ad4=1 -ad42=2. -project=0 -ts=im [BAD... why ?? 
# -- Full implicit method: (good for low Reynolds number)
#   cgins cic -g=cice2.order2 -inflow=parabolic -wall=noSlip -nu=1000. -tf=1000. -tp=10. -dtMax=.1 -iv=full
# -- Fourth-order
#   cgins cic -g=cice2.order4 -wall=slip -tp=.1 -ad2=0 -nu=.01 [ok
#   cgins cic -g=cice4.order4 -wall=slip -tp=.05 -ad2=0 -nu=.005 [ok
#   cgins cic -g=cice2.order4 -wall=slip -tp=.01 -ad2=0 -nu=.001 -ad4=1 -ad42=2. -ts=pc [ok
#   cgins cic -g=cice4.order4 -inflow=parabolic -wall=noSlip -ad2=0 -nu=.005
# 
# -- Approximate factored scheme:
#  cgins cic -g=cice2.order2.ml2 -inflow=parabolic -solver=mg -psolver=mg -ts=afs
#  cgins cic -g=cice4.order4.ml3 -inflow=parabolic -solver=mg -psolver=mg -ts=afs
# -- Test slow start:
#  cgins cic -g=cice4.order2.ml3 -inflow=parabolic -solver=mg -psolver=mg -ts=afs -cfl=5. -slowStartCFL=1. -slowStartSteps=100 -slowStartRecomputeDt=10 -tp=.5 -tf=5.
# -- Test parabolic inflow
#   cgins cic -g=inletChannele2.order2 -inflow=parabolic -model=boussinesq
# -- LES
#   cgins cic -g=cice2.order2 -ts=pc -tm=les -lesOption=0 -go=halt
#   cgins cic -g=cice2.order2 -ts=pc -tm=les -nu=.05 -lesOption=1 -lesPar1=.001 -go=halt
#     -- les+implicit : finish me...
#   cgins cic -g=cice2.order2 -ts=im -tm=les -lesOption=0 -nu=.05 -go=halt
#
# --- Parallel
# mpirun -np 2 $cginsp cic -g=cice2.order2.ml2 -nu=.01 -solver=best -psolver=best -recomputeDt=200 -debug=3 [ok]
#
# mpirun -np 2 $cginsp cic -g=cicLongChannele2 -nu=.01 -solver=best -psolver=best -recomputeDt=2 -debug=3 [ok]
# mpirun -np 2 $cginsp noplot cic -g=cice2.order2.ml2 -nu=.01 -psolver=best -recomputeDt=200 -debug=3 -go=og [ok
# srun -N1 -n2 -ppdebug memcheck_all $cginsp cic -g=cice2.order2 -nu=.01 -recomputeDt=200 -debug=3 -go=halt
# srun -N1 -n2 -ppdebug $cginsp -noplot cic -g=cicLongChannele2 -nu=.01 -recomputeDt=200 -debug=3 -tf=.2 -go=go
# srun -N1 -n4 -ppdebug $cginsp -noplot cic -g=cicLongChannele4 -nu=1.e-3 -recomputeDt=200 -debug=3 -tf=.2 -go=go
#
# mpirun -np 2 $cginsp cic -g=cicLongChannele2 -nu=.01 -recomputeDt=2 -debug=3 -pc=lu
# mpirun -np 2 $cginsp noplot cic -g=cicLongChannele2 -nu=.01 -recomputeDt=2 -debug=3 -solver=mg -go=og
# mpirun -np 2 $cginsp noplot cic -g=cice2.order2.ml2 -nu=.01 -recomputeDt=2 -debug=3 -solver=mg -go=og
# srun -N1 -n2 -ppdebug $cginsp -noplot cic -g=cicLongChannele2 -nu=.01 -solver=mg -debug=3 -tf=.1 -go=go
#
# -- test parallel + implicit interp
#  Grid: ogen -noplot cilcArg -interp=i -factor=2
#  mpirun -np 2 $cginsp cic -g=cilci2.order2 -nu=.01 -solver=best -psolver=best -recomputeDt=200 -debug=3 
# 
$tFinal=10.; $tPlot=.05; $nu=.1; $cfl=.9; $debug=1; $show=" ";  $go="halt"; $project=1; $newts = 0; $dtMax=.1; 
$solver="yale";  $rtoli=1.e-5; $atoli=1.e-6; $idebug=0; 
$psolver="yale"; $rtolp=1.e-5; $atolp=1.e-6; $pdebug=0;
$recomputeDt=200; $refactorFrequency=1000; $userDefinedOutput=0; 
$slowStartSteps=-1; $slowStartCFL=.5; $slowStartRecomputeDt=100; 
$model="ins";  $restart="";
$grid="cice2.order2.hdf"; $outflowOption="neumann";
$order = -1;  # use default order of accuracy based on the grid
$ad2=1; $ad21=.5; $ad22=.5;   $ad4=0; $ad41=1.;  $ad42=1.; 
$pc="ilu"; 
$ts="im";  # time stepping
$iv=""; # implicit variation 
$fullImplicitSystem=0; # 1=do not use multiple scalar systems for implicit solves, even if possible
$tm = "#"; # turbulence model
$lesOption=0; $lesPar1=.01; 
$inflow = "uniform"; $wall="noSlip"; $cyl="noSlip"; $oscillate=0; $uInflow=1.; 
$Prandtl=.72; $thermalExpansivity=.1; $Tin=1.;
#
$append=0; # set to "1" to append to an existing show file 
#
$cgSolver="best"; # coarse grid solver for MG 
# -- for Kyle's AF scheme:
$afit = 10;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
$Tset=1.; $Kp=0.; $Ki=1.; $Kd=0.; # for inflow control
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"nu=f"=>\$nu,"recomputeDt=i"=>\$recomputeDt,\
            "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"order=i"=>\$order,"show=s"=>\$show,\
            "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"project=i"=>\$project,"dtMax=f"=>\$dtMax,\
            "go=s"=>\$go,"tm=s"=>\$tm,"ts=s"=>\$ts,"lesOption=i"=>\$lesOption,"lesPar1=f"=>\$lesPar1,\
            "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
            "inflow=s"=>\$inflow,"wall=s"=>\$wall,"cyl=s"=>\$cyl,"outflowOption=s"=>\$outflowOption,\
            "iv=s"=>\$iv,"newts=i"=>\$newts,"cfl=f"=>\$cfl, "model=s"=>\$model,"oscillate=i"=>\$oscillate,\
            "restart=s"=>\$restart,"Kp=f"=>\$Kp,"Ki=f"=>\$Ki,"Kd=f"=>\$Kd,"uInflow=f"=>\$uInflow,\
            "fullImplicitSystem=i"=>\$fullImplicitSystem,"cgSolver=s"=>\$cgSolver,"append=i"=>\$append,\
            "slowStartCFL=f"=>\$slowStartCFL,"slowStartSteps=i"=>\$slowStartSteps,"slowStartRecomputeDt=i"=>\$slowStartRecomputeDt );
#
$kThermal=$nu/$Prandtl;
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }\
  else{ $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $pc eq "ilu" ){ $pc = "incomplete LU preconditioner"; }elsif( $pc eq "lu" ){ $pc = "lu preconditioner"; }else{ $pc="#"; }
if( $tm eq "les" ){ $tm ="LargeEddySimulation"; }
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "pc4" ){ $ts="adams PC order 4"; }
if( $ts eq "mid"){ $ts="midpoint"; }  
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = 1;}
if( $iv eq "full" ){ $iv = "useNewImplicitMethod\n use full implicit system 1\n implicitFullLinearized"; }else{ $iv="#"; }
if( $order eq "2" ){ $order = "second order accurate"; }elsif( $order eq "4" ){ $order = "fourth order accurate"; }else{ $order="#"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
# 
# specify the overlapping grid to use:
$grid
# Specify the equations we solve:
  $model
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
#
  $tm
  # Define LES parameters that are accessed by getLargeEddySimulationViscosity.bf 
  define integer parameter lesOption $lesOption
  define real parameter lesPar1 $lesPar1
  exit
# Set the order of accuracy
$order
#
# Next specify the file to save the results in. 
# This file can be viewed with Overture/bin/plotStuff.
  show file options
    if( $append eq 0 ){ $cmd="OBPSF:create new show file"; }else{ $cmd="OBPSF:append to old show file"; }
    $cmd
    compressed
      open
       $show
    frequency to flush
      2
    exit
#   display parameters
  turn off twilight zone 
#
# choose the time stepping:
  $ts
  $newts
  # fullImplicitSystem=1: do not use multiple scalar systems for implicit solves, even if possible
  use full implicit system $fullImplicitSystem
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
#
# -- choose the implicit variation (implicitViscous is default)
$iv
# ---------
#
# but integrate the square explicitly:
  choose grids for implicit
    all=implicit
##    square=explicit
    done
  dtMax $dtMax
#
  final time $tFinal
  times to plot $tPlot
#  Here is how to specify particular output times:
#  printArray(0)=0.2
#  printArray(1)=0.4
#  printArray(2)=1.
#  printArray(3)=1.1
#  
  plot and always wait
#*  no plotting
#
  recompute dt every $recomputeDt
  refactor frequency $refactorFrequency
# 
  pde parameters
    nu $nu
    kThermal $kThermal
#  turn on 2nd-order AD here:
   OBPDE:second-order artificial diffusion $ad2
     OBPDE:ad21,ad22 $ad21 $ad22
#  turn on 4th-order AD here: 
   OBPDE:fourth-order artificial diffusion $ad4
     OBPDE:ad41,ad42 $ad41,$ad42
   OBPDE:expect inflow at outflow
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
   $cmd
#
   done
#
#  OBPSF:show variable: vorticity 1
#  OBPSF:show variable: divergence 1
# 
  cfl $cfl
# 
  slow start cfl $slowStartCFL
  slow start steps $slowStartSteps
  slow start recompute dt $slowStartRecomputeDt
#
# -- define controls:
  if( $inflow eq "control" ){ $cmd = "controls...\nPID\n targetComponent: 3\n Kp, Ki, Kd: $Kp,$Ki,$Kd (PID gains)\n Set point: $Tset\n exit"; }else{ $cmd="#"; }
  $cmd
#
  maximum number of iterations for implicit interpolation
    10
# 
# -- Here we assume that the grid was made with boundary conditions:
#   1=inflow, 2=outflow, 5=bodies
  boundary conditions
   if( $wall eq "noSlip" ){ $cmd="all=noSlipWall"; }else{ $cmd="all=slipWall"; }
   $cmd
   if( $inflow eq "uniform" ){ $cmd = "bcNumber1=inflowWithVelocityGiven, uniform(p=1.,u=1.,T=$Tin)"; }\
   elsif( $inflow eq "parabolic" ){ $cmd = "bcNumber1=inflowWithVelocityGiven , parabolic(d=.5,p=1.,u=1.,T=$Tin)"; }\
   elsif( $inflow eq "control" ){ $cmd = "bcNumber1=inflowWithVelocityGiven, userDefinedBoundaryData\n" . \
               "inflow with control\n 1. 0. 0.\n done";}\
   elsif( $inflow eq "pressure" ){ $cmd = "bcNumber1=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)"; }\
   else{ $cmd = "bcNumber1=inflowWithVelocityGiven, uniform(p=1.,u=1.,T=$Tin)"; } 
   $cmd
#*    square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.), oscillate(a0=.5,a1=.5,t0=.0,omega=.5)
##   square(0,0)=inflowWithVelocityGiven , parabolic(d=.2,p=1.,u=1.), oscillate(a0=1.,t0=.0,omega=.5)
#*  square(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)
#*     square(1,0)=outflow,  pressure(1.*p+1.*p.n=0.)
#*    square(1,0)=outflow,  pressure(1.*p+0.*p.n=1.)
# 
   bcNumber2=outflow ,  pressure(1.*p+1.*p.n=0.)
   if( $cyl eq "noSlip" ){ $cmd="bcNumber5=noSlipWall"; }else{ $cmd="bcNumber5=slipWall"; }
   $cmd
   # radial inflow option: (this over-rides previous BC options)
   if( $inflow eq "radial" ){ $cmd="all=outflow,  pressure(1.*p+1.*p.n=0.)\n bcNumber5=inflowWithVelocityGiven, userDefinedBoundaryData\n" . \
             "normal component of velocity\n 1. 0.\n  done"; }else{ $cmd="#"; }
   $cmd
  done
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesPC=$pc; $ogesDebug=$pdebug;
   # ******* TEMP: 
   # NOTE: reduce the relative tol for AMG 
   if( $cgSolver eq "hypre" ){ $ogmgCoarseGridSolver="algebraic multigrid"; $ogmgCoarseGridMaxIterations=100; $ogmgRtolcg=1.e-5; $ogmgAutoChoose=0; }
   # ******* END TEMP 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtoli; $ogesAtol=$atoli; $ogesPC=$pc; $ogesDebug=$idebug;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=$uInflow, v=0., p=1., T=0."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
# 
#  plot residuals 1
  allow user defined output $userDefinedOutput
  if( $project eq "1"  && $restart eq "" ){ $cmd="project initial conditions"; }else{ $cmd="#"; }
  $cmd
  debug $debug 
  continue
#
$go


