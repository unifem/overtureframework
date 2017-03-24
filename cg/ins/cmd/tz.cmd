#
# cgins -- test the twilightzone
#
echo to terminal 0
# Usage:
#   
#  cgins [-noplot] tz -g=<name> -tz=<poly/trig> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> -debug=<> ...
#         -ogesDebug=<> -iv=[viscous|full|oldViscous]  ...
#        -solver=[yale|best] -rtol=<> -atol=<>  
#        -psolver=[yale|best] -rtolp=<> -atolp=<> 
#        -order=[-1|2|4|6|8] -model=[ins|boussinesq|tp|vp] ...
#        -ts=[pc|pc4|im|fe|bdf|imex] -debug=<num> -bg=<backGround> -fullSystem -imp=<val> ...
#        -newts[0|1] -go=[run/halt/og] -do=[fd|compact] -move=[0|shift|rotate] -checkErrOnGhost=[0|1|2] ...
#        -bc[1|2|3|4]=[noSlip|slip|inflow|outflow|pinflow] -varMat=[0|1] -orderInTime=[]
# 
#  -model=bp : Boussinesq + passive scalar
#  -bc[1|2|3|4]= set the boundary conditions for grids with a face with bc=1,2,..,
#  -solver, rotl, atol : implicit solver and convergence tolerances
#  -psolver, rtolp, atolp : pressure solver and convergence tolerances
#  -newts : =1 : use the new advanceSteps time stepping routines 
#  -varMat : 1=variable material properties, 0=constant
#  -go : run, halt, og=open graphics
# 
# Examples:
# 
#  cgins noplot tz -g=square10 -degreex=2 -degreet=2 -go=go               [exact
#  cgins noplot tz -g=square10 -degreex=2 -degreet=2  -bcn=noSlip -go=go  [exact
#  cgins tz -g=square5 -degreex=2 -degreet=2 -ts=implicit
#  cgins noplot tz -g=nonSquare8 -degreex=2 -degreet=2  -go=go            [exact
#  cgins noplot tz -g=sise -degreex=2 -degreet=2  -go=go                  [exact
#  cgins noplot tz -g=sise -degreex=2 -degreet=2 -ts=implicit -go=go
#  cgins noplot tz -g=cice -degreex=2 -degreet=2  -go=go
#  cgins tz -g=cic4 -tz=trig  
# 
#  cgins noplot tz -g=sisHypee2.order2 -degreex=2 -degreet=2 -tf=.1 -tp=.1 -go=go  [exact
#  cgins noplot tz -g=rsisHypee2.order2 -degreex=2 -degreet=2 -tf=.5 -tp=.1 -go=go  [exact
#  cgins noplot tz -g=rsisHypee2.order2 -tz=trig -tf=.5 -tp=.1 -go=go
#      0.500 1.07e-03 1.33e-03 9.56e-04 4.86e-03  1.33e-03 4.35e-03 1.19e+00
#  cgins noplot tz -g=rsisHypee4.order2 -tz=trig -tf=.5 -tp=.1 -go=go
#      0.500 2.65e-04 3.19e-04 2.21e-04 1.33e-03  3.19e-04 1.12e-03 7.89e+00
# 
#  cgins tz -g=square10 -degreex=2 -degreet=1 -ts=fe -newts=1 -go=halt
#
# -- fourth-order (for explicit 4th order do NOT use the new implicit method to compute the RHS)
#  cgins -noplot tz -g=box10.order4 -degreex=4 -degreet=3 -ts=pc4 -useNewImp=0 -bcn=noSlip -tp=.05 -tf=.1 -go=go [exact
#  cgins -noplot tz -g=box10.order4 -degreex=4 -degreet=1 -ts=im -iv=oldViscous -bcn=noSlip -tp=.05 -tf=.1 -go=go [*NOT* exact  -- why?
#  cgins -noplot tz -g=square10.order4 -degreex=4 -degreet=3 -ts=pc4 -useNewImp=0 -bcn=noSlip -tp=.05 -tf=.1 -go=go [exact
#  cgins -noplot tz -g=square10.order4 -degreex=4 -degreet=1 -ts=im -iv=oldViscous -bcn=d -tp=.05 -tf=.1 -go=go [exact
#  cgins -noplot tz -g=nonSquare10.order4 -degreex=4 -degreet=1 -ts=im -iv=oldViscous -bcn=d -tp=.05 -tf=.1 -go=go [exact
#  cgins -noplot tz -g=square40.order4 -degreex=4 -degreet=2 -ts=im -iv=oldViscous -bcn=d -tp=.05 -tf=.1 -go=go [err=9e-10
#  cgins -noplot tz -g=square40.order4 -degreex=4 -degreet=0 -ts=im -iv=oldViscous -bcn=noSlip -tp=.05 -tf=.1 -go=go [exact
#  cgins -noplot tz -g=square40.order4 -degreex=4 -degreet=1 -ts=im -iv=oldViscous -bcn=noSlip -tp=.05 -tf=.1 -go=go [err(p)=2.09e-05 -- FIX ME : noslip wall problem I guess
#  mpirun -np 2 $cginsp -noplot tz -g=square20.order4 -degreex=4 -degreet=3 -ts=pc4 -useNewImp=0 -tp=.05 -tf=.1 -psolver=best -solver=best -go=go  [exact]
#
# -- fourth-order + pressure-inflow (new 2012/09/14)
#  cgins -noplot tz -g=square10.order4 -bc1=pinflow -degreex=4 -degreet=2 -ts=pc -useNewImp=0 -bcn=d -tp=.05 -tf=.1 -go=go  [exact
#  cgins -noplot tz -g=box10.order4 -bc1=pinflow -degreex=4 -degreet=2 -ts=pc -useNewImp=0 -bcn=d -tp=.05 -tf=.1 -go=go  [exact
# -- moving:
#
# -- simple test case for a deforming body 
# cgins tz -g=plugDeform1 -gridToMove="plug" -degreex=1 -degreet=0 -ts=pc -move=deform -tp=.01 -tf=1. -go=halt [exact
# 
# -- variable material Boussinesq:
#   cgins tz -g=square10.order2 -degreex=1 -degreet=1 -ts=pc -model=boussinesq -varMat=1
#
# =========== BAD CASE:
#
#
# mpirun -np 1 $cginsp tz -g=sise2.order2.ml3 -gridToMove="inner-square" -degreex=1 -degreet=1 -ts=implicit -iv=viscous -move=rotate -psolver=mg -solver=best -tp=.02 -freqFullUpdate=1 -rtolp=1.e-6 -ogesDebug=3
#
#
#  cgins tz -g=square16.order2 -degreex=1 -degreet=1 -move=rotate [OK]
#  cgins tz -g=square16.order2 -degreex=1 -degreet=1  -ts=implicit -iv=viscous -move=rotate [OK]
#  cgins tz -g=square16.order2 -degreex=1 -degreet=1  -ts=implicit -iv=viscous -move=rotate -psolver=mg -solver=mg  [OK]
#  mpirun -np 1 $cginsp tz -g=square16.order2 -degreex=1 -degreet=1  -ts=implicit -iv=viscous -move=rotate -psolver=mg -solver=mg [OK]
#  mpirun -np 1 $cginsp tz -g=sise1.order2.ml3 -gridToMove="inner-square" -degreex=1 -degreet=1  -ts=implicit -iv=viscous -move=rotate -tp=.02 -psolver=best -solver=best -freqFullUpdate=1  [OK
#  mpirun -np 1 $cginsp tz -g=sise1.order2.ml3 -gridToMove="inner-square" -degreex=1 -degreet=1  -ts=implicit -iv=viscous -move=rotate -psolver=mg -solver=mg -tp=.02 -freqFullUpdate=1  [OK n=1, OK n=2 to t=1
#  mpirun -np 1 $cginsp tz -g=sise2.order2.ml3 -gridToMove="inner-square" -degreex=1 -degreet=1  -ts=implicit -iv=viscous -move=rotate -psolver=mg -solver=mg -tp=.02 -freqFullUpdate=1 -rtolp=1.e-5 -ogesDebug=3 [OK
#  srun -N1 -n4 -ppdebug $cginsp tz -g=sise2.order2.ml3 -gridToMove="inner-square" -degreex=1 -degreet=1  -ts=implicit -iv=viscous -move=rotate -psolver=mg -solver=mg -tp=.02 -freqFullUpdate=1 -rtolp=1.e-5 -ogesDebug=3 [TROUBLE
# 
# -- generate a grid on the fly using a command file from $Overture/sampleGrids (--N is a command line option in square.cmd) 
#  cgins tz -g=ogen -gf=$Overture/sampleGrids/square.cmd --N=11 -degreex=2 -degreet=2 
#
# -- curved pipe
#  cgins tz -g=curvedPipei1.order2 -tz=trig -solver=best -tf=.1 -tp=.01 -cdv=0. -nu=.05 -rtol=1.e-5 -atol=1.e-7 -go=halt
#  cgins tz -g=solidCurvedPipe90e1.order2 -nu=0.025 -tf=0.05 -tz=trig -solver=best -rtol=1e-06 -atol=1e-09 -cdv=1 -fx=.5 -fy=.5 -fz=.5 -ft=.5 -tp=.005 -go=halt
# 
#  cgins noplot tz -g=box10 -degreex=2 -degreet=2 -solver=best
#  cgins noplot tz -g=nonBox5 -degreex=2 -degreet=2 -solver=best
#  cgins noplot tz -g=bibe -degreex=2 -degreet=2 -solver=best
#
#  cgins tz -g=sibe2.order2 -tz=trig -nu=.1 -fx=.5 -fy=.5 -fz=.5 -ft=.5 -solver=best -rtol=1.e-8 -atol=1.e-9 -go=halt
# 
# -- test new implicit:
#   cgins tz -g=square5 -degreex=2 -degreet=1 -ts=implicit -iv=viscous [exact]
#   cgins tz -g=square5 -degreex=2 -degreet=1 -ts=implicit -iv=viscous -ad2=1  [not exact]
#   cgins tz -g=square5 -degreex=2 -degreet=1 -ts=implicit -iv=viscous -model=boussinesq   [exact]
#   cgins tz -g=square5 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -model=boussinesq -ad2=1 [exact]
#   cgins tz -g=square5 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -model=boussinesq -ad2=1 -bcn="square(0,0)=slipWall\nsquare(0,1)=slipWall" [exact]
# -- track down the bug with MG and ad2: 
#   cgins -noplot tz -g=square8 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -ad2=1 -solver=mg -go=og -bcn="square(0,0)=noSlipWall" [ok]
#   cgins -noplot tz -g=square8 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -ad2=1 -solver=mg -go=og -bcn="square(0,0)=inflowWithVelocityGiven, uniform(p=1.)" [ok]
#   cgins -noplot tz -g=square8 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -ad2=1 -solver=mg -go=og -bcn="square(1,0)=outflow , pressure(1.*p+1.*p.n=0.)" [ok, but outflow-extrap replace by neumann -> *fix me*]
#
# -- test implicit and pressure inflow BC:
#   (rectangular uses 2 scalar systems and predefined equations)
#  cgins tz -g=square5 -degreex=2 -degreet=1 -ts=implicit -iv=viscous -bcn="square(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)" -debug=1 -tp=.01   [exact]
#  cgins tz -g=box5 -degreex=2 -degreet=1 -ts=implicit -iv=viscous -bcn="box(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)" -debug=1 -tp=.01   [exact]
#   non-rectangular uses system and insImp: 
#  cgins tz -g=nonSquare5 -degreex=2 -degreet=1 -ts=implicit -iv=viscous -bcn="square(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)" -debug=1 -tp=.01      [exact]
#  cgins tz -g=nonBox5 -degreex=2 -degreet=1 -ts=implicit -iv=viscous -bcn="box(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)" -debug=1 -tp=.01       [exact]
# 
# -- test approximate factorization scheme:
# cgins tz --boxorder=4 -grid_order=4 -order=4 -g=ogen -gf=/home/chand/scratch.64/ov_build/dev64/sampleGrids/square.cmd --N=21 --nx=N -degreex=1 -degreet=0 -ts=afs -tz=trig -newts=1 -debug=0 --boxbc=d --squarebc= -bcn=\# -fx=2 -fy=2 -fz=2 -ft=2 -advectionCoefficient=1 -nu=0.1 -tf=.5 -go= -tp=.1 -do=compact
# cgins tz --boxorder=4 -grid_order=4 -order=4 -g=ogen -gf=/home/chand/scratch.64/ov_build/dev64/sampleGrids/box.cmd --N=21 --nx=21 -degreex=1 -degreet=0 -ts=afs -tz=trig -newts=1 -debug=0 --boxbc=d --squarebc= -bcn=\# -fx=2 -fy=2 -fz=2 -ft=2 -advectionCoefficient=1 -nu=0.1 -tf=.5 -go= -tp=.1 -do=compact
# cgins tz --boxorder=4 -grid_order=4 -order=4 -g=ogen -gf=/home/chand/scratch.64/ov_build/dev64/sampleGrids/box.cmd --N=21 --nx=21 -degreex=1 -degreet=0 -ts=afs -tz=trig -newts=1 -debug=0 --boxbc=p --squarebc=periodic -bcn=\# -fx=2 -fy=2 -fz=2 -ft=2 -advectionCoefficient=1 -nu=0.1 -tf=.5 -go= -tp=.1 -do=compact
#
# -- Boussinesq:
#  
#  cgins tz -g=square10 -degreex=2 -degreet=2 -model=boussinesq -gravity="-10. 0 0." -tp=.1 -tf=5. [exact]
#  cgins tz -g=square5 -degreex=2 -degreet=1 -ts=implicit -model=boussinesq -gravity="-10. 0 0." -rf=5 -dtMax=.05 -tp=.1 -tf=2. -debug=1 [exact until refactor]
#  cgins tz -g=box5 -degreex=2 -degreet=1 -ts=implicit -model=boussinesq -gravity="-10. 0 0." -rf=5 -dtMax=.05 -tp=.1 -tf=2. -debug=1 [exact until refactor]
#    -- order=4: (for explicit 4th order do NOT use the new implicit method to compute the RHS)
#  cgins -noplot tz -g=square5.order4 -order=4 -degreex=2 -degreet=2 -model=boussinesq -ts=pc4 -useNewImp=0 -gravity="0 -1. 0." -tp=.1 -tf=.2 -go=go [exact 
# 
# Two-phase flow examples: (see also twoPhase.out)
#   cgins noplot tz -g=square5 -degreex=2 -degreet=1 -model=tp -tp=.005 -tf=.01 -dtMax=.005 -go=go -debug=1 -gravity="0. -10. 0."  [exact]
# -- test with rho varying in time (constant in space)
#   cgins tz -g=square5 -degreex=2 -degreet=1 -model=tp -tp=.005 -tf=.1 -dtMax=.005 -gravity="0. -1. 0." -cfl=.9 -go=halt -rhot=1. [exact]
# 
#   cgins tz -g=square20 -degreex=1 -degreet=0 -model=tp -tp=.005 -tf=.1 -dtMax=.005 -gravity="0. -1. 0." -cfl=.25 -go=halt -mu1=.1 -mu2=.2 [exact]
#
# -- convergence test 
#   cgins noplot tz -g=square10 -degreex=2 -degreet=2 -model=tp -tp=.1 -tf=.1 -dtMax=.05 -gravity="0. -1. 0." -cfl=.9 -go=go -mu1=.1 -mu2=.2 -rho1=2. -rho2=1.
#   cgins tz -g=square40 -degreex=2 -degreet=2 -model=tp -tp=.1 -tf=.1 -dtMax=.05 -gravity="0. -1. 0." -cfl=.9 -go=go -mu1=.1 -mu2=.2 -rho1=2. -rho2=1. -go=halt
# 
#  -- implicit:
# cgins noplot tz -g=square5 -degreex=2 -degreet=0 -model=tp -tp=.1 -tf=.1 -dtMax=.1 -gravity="0. -1. 0." -cfl=.9 -go=go -ts=implicit -imp=1. -fullSystem=1 -av1=1. -av2=1. -kThermal=1. [exact]
# 
# cgins noplot tz -g=square5 -degreex=2 -degreet=1 -model=tp -tp=.1 -tf=.4 -dtMax=.1 -gravity="0. -1. 0." -cfl=.9 -go=go -ts=implicit -imp=.5 -fullSystem=1 -av1=1. -av2=1. -kThermal=1. -debug=1 [exact]
# 
#  cgins noplot tz -g=square5 -degreex=2 -degreet=0 -model=tp -tp=.1 -tf=.1 -dtMax=.1 -gravity="0. 0. 0." -cfl=.9 -go=go -ts=implicit -imp=1. -iv=viscous -fullSystem=1 -av1=1. -av2=1. -kThermal=1. -debug=15 >! junk
# 
#  cgins tz -g=square5 -degreex=2 -degreet=1 -model=tp -tp=.005 -tf=.5 -dtMax=.05 -gravity="0. -1. 0." -cfl=.9 -go=halt -rhot=1. -ts=implicit
# 
# parallel examples:
#
# srun -N1 -n2 -ppdebug $cginsp noplot tz -g=square8 -degreex=2 -degreet=2 -ts=implicit -iv=viscous -tp=.01 -tf=.1 -solver=best -psolver=best -go=go [ok
# totalview srun -a -N1 -n2 -ppdebug $cginsp noplot tz -g=square8 -degreex=2 -degreet=2 -ts=implicit -model=boussinesq -tp=.01 -tf=.1 -solver=best -psolver=best -go=go
#  srun -N1 -n2 -ppdebug $cginsp noplot tz -g=square5 -degreex=2 -degreet=2 -ts=implicit -model=boussinesq -tp=.01 -tf=.5 -solver=best -psolver=best -go=og [trouble zeus new version
# 
# mpirun -np 2 $cginsp noplot tz -g=square10 -degreex=2 -degreet=2 -solver=best -psolver=best -tp=.05 -tf=1. -go=og
# mpirun -np 2 $cginsp noplot tz -g=square10 -degreex=2 -degreet=2 -ts=implicit -solver=best -tp=.01 -tf=.02 -go=og
# mpirun -np 1 $cginsp noplot tz -g=square5 -degreex=2 -degreet=2 -ts=implicit -model=boussinesq -solver=best -tp=.01 -tf=.02
# -- parallel + mg 
# mpirun -np 2 $cginsp -noplot tz -g=square16 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -ad2=0 -psolver=mg -solver=mg -rtol=1.e-10 -rtolp=1.e-10 -debug=3 -tf=.2 -go=go [ok "exact" ]
# srun -N1 -n2 -ppdebug $cginsp -noplot tz -g=square16 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -ad2=1 -psolver=mg -solver=mg -rtol=1.e-6 -rtolp=1.e-6 -debug=3 -go=go [ok]
# srun -N1 -n4 -ppdebug $cginsp -noplot tz -g=square64 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -ad2=0 -psolver=mg -solver=mg -rtol=1.e-6 -rtolp=1.e-6 -tf=.5 -go=go [ok, n=1,2,4,8,16
# srun -N1 -n4 -ppdebug $cginsp -noplot tz -g=square64 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -ad2=1 -psolver=mg -solver=mg -rtol=1.e-6 -rtolp=1.e-6 -tf=.5 -go=go [-n1,2 ok, -n4 BAD
# srun -N1 -n4 -ppdebug $cginsp -noplot tz -g=cice2.order2.ml2 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -ad2=0 -psolver=mg -solver=mg -rtol=1.e-6 -rtolp=1.e-6 -tf=.5 -go=go [ok, n=4,8
#  -- inflow-outflow: 
# srun -N1 -n4 -ppdebug $cginsp -noplot tz -g=square64 -degreex=1 -degreet=1 -ts=implicit -iv=viscous -ad2=0 -psolver=mg -solver=mg -rtol=1.e-6 -rtolp=1.e-6 -tf=.5 -bcn=sio -go=go [ok, but errors bad: FIX ogmg outflow BC
#
# 
# === Boussinesq + passive scalar
#  cgins tz -g=square10 -degreex=2 -degreet=2 -model=bp -tp=.1 -tf=5. [exact]
# 
# -- LES
#  cgins -noplot tz -tm=les -ts=pc -g=square10 -degreex=2 -degreet=2 -tp=.01 -tf=.1 -go=go [exact
#  cgins -noplot tz -tm=les -ts=pc -g=square10 -degreex=2 -degreet=2 -tp=.01 -tf=.1 -lesOption=1 -go=go [exact
#  cgins -noplot tz -tm=les -ts=pc4 -order=4 -g=square10.order4 -degreex=4 -degreet=3 -tp=.01 -tf=.02 -debug=1 -go=go [exact
#  -- LES + boussinesq
#  cgins -noplot tz -tm=les -ts=pc -g=square10 -degreex=2 -degreet=2 -tp=.01 -tf=.1 -model=boussinesq -lesOption=1 -go=go 
#   -- implicit: 
#  cgins -noplot tz -tm=les -ts=im -g=square10 -degreex=2 -degreet=2 -tp=.1 -tf=1. -dtMax=.1 -model=boussinesq -go=go [FINISH
#  cgins -noplot tz -tm=les -ts=im -g=square10 -degreex=2 -degreet=2 -tp=.1 -tf=1. -dtMax=.1 -go=go [FINISH
# 
# --- set default values for parameters ---
# 
echo to terminal 1
$model="ins"; $noplot=""; $backGround="square"; $show=" "; $newts=0; $project=0;  $varMat=0;
$ts="adams PC"; $implicitVariation="viscous"; $implicitFactor=.5; $cdv=1.; $refactorFrequency=100;
$numberOfCorrections=1; $orderInTime=-1;
$cp0=.1;  # coeff of p in the mxied BC for outflow
$debug = 0;  $tPlot=.1; $maxIterations=100; 
$rtol=1.e-16; $atol=1.e-16; $rtolp=1.e-16; $atolp=1.e-16; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$ad2=0; $ad21=1; $ad22=1;  $ad4=0; $ad41=2.; $ad42=2.; 
$order = -1;  # -1 : base order of accuracy on grid
$fullSystem=0; $go="halt"; 
$bcn="#"; $bc1="#"; $bc2="#"; $bc3="#"; $bc4="#"; $bc5="#"; $bc6="#"; 
$mbpbc=0; $mbpbcc=1.; 
$psolver="yale"; $solver="yale"; $ogesDebug=0; $tFinal=1.; $cfl=.9; $nu=.1;  $kThermal=.1; $thermalExpansivity=.1;
$thermalConductivity=.05;
$gravity=" 0. -1. 0."; $rho1=1.; $rho2=1.; $mu1=.1; $mu2=.1; $av1=.1; $av2=.1; $rhot=0.;
$advectionCoefficient = 1.;
$dop="fd"; $twoPhaseFlow=0; $lesOption=0; $lesPar1=.01; 
$move=0;  $gridToMove="square"; $rate=.5; $gridToMove2 = "#";
$freqFullUpdate=10; # frequency for using full ogen update in moving grids
$tm = "#"; # turbulence model
$checkErrOnGhost=0; # check errors on this many ghost lines
$useNewImp=1; # use the new implicit method 
$xshift=1.; $yshift=0.; $zshift=0.; # for moving grids, shift option
# 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
#
$aftol = 1e-10; $afit = 20
$gridCmdFileName = "";
$uplot="p";
$ogmgMaxIts = 50;
# $ksp="gmres"; 
#
# ----------------------------- get command line arguments ---------------------------------------
Getopt::Long::Configure("prefix_pattern=(--tz|--|-)");
GetOptions( "g=s"=>\$grid,"gf=s"=>\$gridCmdFileName,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"gravity=s"=>\$gravity,"rho1=f"=>\$rho1,"rho2=f"=>\$rho2,"useNewImp=i"=>\$useNewImp,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"newts=i"=>\$newts,"fx=f"=>\$fx,"fy=f"=>\$fy,"fz=f"=>\$fz,"ft=f"=>\$ft,\
 "mu1=f"=>\$mu1,"mu2=f"=>\$mu2,"av1=f"=>\$av1,"av2=f"=>\$av2,"rhot=f"=>\$rhot,"iv=s"=>\$implicitVariation,\
 "rf=s"=>\$refactorFrequency,"ogesDebug=i"=>\$ogesDebug,"imp=f"=>\$implicitFactor,"kThermal=f"=>\$kThermal,\
  "cdv=f"=>\$cdv,"show=s"=>\$show ,"thermalExpansivity=f"=>\$thermalExpansivity,"ad2=f"=>\$ad2,"bcn=s"=>\$bcn,\
  "advectionCoefficient=f"=>\$advectionCoefficient,"do=s"=>\$dop,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,\
  "psolver=s"=>\$psolver,"move=s"=>\$move,"gridToMove=s"=>\$gridToMove,"gridToMove2=s"=>\$gridToMove2,"freqFullUpdate=i"=>\$freqFullUpdate,\
  "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42, "rate=f"=>\$rate,"tm=s"=>\$tm,"lesOption=i"=>\$lesOption,\
  "lesPar1=f"=>\$lesPar1,"bc1=s"=>\$bc1,"bc2=s"=>\$bc2,"bc3=s"=>\$bc3,"bc4=s"=>\$bc4,"bc5=s"=>\$bc5,"bc6=s"=>\$bc6,\
  "checkErrOnGhost=i"=>\$checkErrOnGhost,"mbpbc=i"=>\$mbpbc,"mbpbcc=f"=>\$mbpbcc,"nc=i"=>\$numberOfCorrections,\
  "aftol=f"=>\$aftol, "afit=i"=>\$afit,"project=i"=>\$project,"cp0=f"=>\$cp0,"varMat=i"=>\$varMat,\
  "thermalConductivity=i"=>\$thermalConductivity,"xshift=f"=>\$xshift,"yshift=f"=>\$yshift,"zshift=f"=>\$zshift,\
  "uplot=s"=>\$uplot, "orderInTime=i"=>\$orderInTime );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }else{ $tz="turn on trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }\
elsif( $order eq "4" ){ $order = "fourth order accurate"; }\
elsif( $order eq "6" ){ $order = "sixth order accurate";}\
elsif( $order eq "8" ){ $order = "eighth order accurate";}else{ $order="#"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }\
elsif( $model eq "boussinesq" ){ $model = "incompressible Navier Stokes\n Boussinesq model"; }\
elsif( $model eq "tp" ){ $model = "incompressible Navier Stokes\n two-phase flow model"; $twoPhaseFlow=1; }\
elsif( $model eq "bp" ){ $model = "incompressible Navier Stokes\n Boussinesq model\n passive scalar advection"; }
if( $tm eq "les" ){ $tm ="LargeEddySimulation"; }
# 
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "bdf" ){ $ts="implicit BDF"; }
if( $ts eq "imex" ){ $ts="implicit explicit multistep"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "pc4" ){ $ts="adams PC order 4"; $useNewImp=0; } # NOTE: turn off new implicit for fourth order
if( $ts eq "mid"){ $ts="midpoint"; }  
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;  $implicitVariation="full"; }
#
if( $useNewImp eq 1 ){ $useNewImp ="useNewImplicitMethod"; }else{ $useNewImp="#"; }
#
if( $implicitVariation eq "viscous" ){ $implicitVariation = "$useNewImp\n implicitViscous"; }\
elsif( $implicitVariation eq "oldViscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "$useNewImp\n implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "$useNewImp\n implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
#
if( $bcn eq "sio" ){ $bcn="all=noSlipWall\n square(0,0)=inflowWithVelocityGiven\n square(1,0)=outflow"; }
if( $bcn eq "noSlip" ){ $bcn="all=noSlipWall"; }
if( $bcn eq "d" ){ $bcn="all=dirichletBoundaryCondition"; }
if ( $bcn eq "penaltySlip" ) { $bcn="all=penaltyBoundaryCondition, penaltySlipWallBC\ndone";}
# -- set faces with bc=1 : 
if( $bc1 eq "noSlip" ){ $bc1="bcNumber1=noSlipWall"; }\
 elsif( $bc1 eq "slip" ){ $bc1="bcNumber1=slipWall"; }\
 elsif( $bc1 eq "inflow" ){ $bc1="bcNumber1=inflowWithVelocityGiven"; }\
 elsif( $bc1 eq "pinflow" ){ $bc1="bcNumber1=inflowWithPressureAndTangentialVelocityGiven"; }\
 elsif( $bc1 eq "outflow"  ){ $bc1="bcNumber1=outflow , pressure($cp0*p+1.*p.n=0.)"; }\
 elsif( $bc1 eq "freeSurface" ){ $bc1="bcNumber1=freeSurfaceBoundaryCondition"; }\
 else{ }#$bc1="#"; }
# -- set faces with bc=2 : 
if( $bc2 eq "noSlip" ){ $bc2="bcNumber2=noSlipWall"; }\
 elsif( $bc2 eq "slip" ){ $bc2="bcNumber2=slipWall"; }\
 elsif( $bc2 eq "inflow" ){ $bc2="bcNumber2=inflowWithVelocityGiven"; }\
 elsif( $bc2 eq "pinflow" ){ $bc2="bcNumber2=inflowWithPressureAndTangentialVelocityGiven"; }\
 elsif( $bc2 eq "outflow"  ){ $bc2="bcNumber2=outflow , pressure($cp0*p+1.*p.n=0.)"; }\
 elsif( $bc2 eq "freeSurface" ){ $bc2="bcNumber2=freeSurfaceBoundaryCondition"; }\
 else{ }#$bc2="#"; }
# -- set faces with bc=3 : 
if( $bc3 eq "noSlip" ){ $bc3="bcNumber3=noSlipWall"; }\
 elsif( $bc3 eq "slip" ){ $bc3="bcNumber3=slipWall"; }\
 elsif( $bc3 eq "inflow" ){ $bc3="bcNumber3=inflowWithVelocityGiven"; }\
 elsif( $bc3 eq "pinflow" ){ $bc3="bcNumber3=inflowWithPressureAndTangentialVelocityGiven"; }\
 elsif( $bc3 eq "outflow"  ){ $bc3="bcNumber3=outflow , pressure($cp0*p+1.*p.n=0.)"; }\
 elsif( $bc3 eq "freeSurface" ){ $bc3="bcNumber3=freeSurfaceBoundaryCondition"; }\
 else{ $bc3="#"; }
# -- set faces with bc=4 : 
if( $bc4 eq "noSlip" ){ $bc4="bcNumber4=noSlipWall"; }\
 elsif( $bc4 eq "slip" ){ $bc4="bcNumber4=slipWall"; }\
 elsif( $bc4 eq "inflow" ){ $bc4="bcNumber4=inflowWithVelocityGiven"; }\
 elsif( $bc4 eq "pinflow" ){ $bc4="bcNumber4=inflowWithPressureAndTangentialVelocityGiven"; }\
 elsif( $bc4 eq "outflow"  ){ $bc4="bcNumber4=outflow , pressure($cp0*p+1.*p.n=0.)"; }\
 elsif( $bc4 eq "freeSurface" ){ $bc4="bcNumber4=freeSurfaceBoundaryCondition"; }\
 else{ $bc4="#"; }
# -- set faces with bc=5 : 
if( $bc5 eq "noSlip" ){ $bc5="bcNumber5=noSlipWall"; }\
 elsif( $bc5 eq "slip" ){ $bc5="bcNumber5=slipWall"; }\
 elsif( $bc5 eq "inflow" ){ $bc5="bcNumber5=inflowWithVelocityGiven"; }\
 elsif( $bc5 eq "pinflow" ){ $bc5="bcNumber5=inflowWithPressureAndTangentialVelocityGiven"; }\
 elsif( $bc5 eq "outflow"  ){ $bc5="bcNumber5=outflow , pressure($cp0*p+1.*p.n=0.)"; }\
 elsif( $bc5 eq "freeSurface" ){ $bc5="bcNumber5=freeSurfaceBoundaryCondition"; }\
 else{ $bc5="#"; }
# -- set faces with bc=6 : 
if( $bc6 eq "noSlip" ){ $bc6="bcNumber6=noSlipWall"; }\
 elsif( $bc6 eq "slip" ){ $bc6="bcNumber6=slipWall"; }\
 elsif( $bc6 eq "inflow" ){ $bc6="bcNumber6=inflowWithVelocityGiven"; }\
 elsif( $bc6 eq "pinflow" ){ $bc6="bcNumber6=inflowWithPressureAndTangentialVelocityGiven"; }\
 elsif( $bc6 eq "outflow"  ){ $bc6="bcNumber6=outflow , pressure($cp0*p+1.*p.n=0.)"; }\
 elsif( $bc6 eq "freeSurface" ){ $bc6="bcNumber6=freeSurfaceBoundaryCondition"; }\
 else{ $bc6="#"; }
# 
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
# 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $dop eq "fd" ) { $dop = "standard finite difference"; }\
elsif( $dop eq "compact" ) {$dop = "compact finite difference";}
$grid = ($grid eq "ogen") ? "ogen\n read command file\n $gridCmdFileName" : $grid;
#
# test mixed boundaries: 
# $grid="matchingSquares"; $tPlot=.01; $degreex=1; $degreet=0; $debug=1;
# 
$grid
#
  $model
  $tm
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
 # parameters for two-phase flow:
  define real parameter twoPhaseRho1 $rho1
  define real parameter twoPhaseRho2 $rho2
  define real parameter twoPhaseMu1  $mu1
  define real parameter twoPhaseMu2  $mu2
  define real parameter twoPhaseArtDisPsi $av1
  define real parameter twoPhaseArtDisPhi $av2
 # add an explicit time dependence to rho for testing: 
  define real parameter twoPhaseRhot $rhot
  # Define LES parameters that are accessed by getLargeEddySimulationViscosity.bf 
  define integer parameter lesOption $lesOption
  define real parameter lesPar1 $lesPar1
#
  variable material properties $varMat
#
  exit
# -- order of accuracy: 
$order 
# 
# Choose the time-stepping method:
  $ts
  if( $orderInTime eq 4 ){ $cmd="fourth order accurate in time\n BDF order 4"; }else{ $cmd="#"; }
  $cmd
#
# 
  show file options
    compressed
     open
      $show
    frequency to flush
      100
    exit
#**
# -- twilightzone options:
  $tz
  degree in space $degreex
  degree in time $degreet
  frequencies (x,y,z,t)   $fx $fy $fz $ft
# 
  final time $tFinal
  times to plot $tPlot
 # plot and always wait
  no plotting
  cfl $cfl
  dtMax $dtMax
# -- assign the coefficient of the advection terms: (1=NS, 0=Stokes)
  advectionCoefficient $advectionCoefficient
#
  $newts
#
  pde parameters
    nu $nu
    kThermal $kThermal
    thermal conductivity $thermalConductivity
    OBPDE:divergence damping  $cdv
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22  $ad21, $ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
   gravity
     $gravity
   # This will use a Neumann BC at outflow: 
   use Neumann BC at outflow
  done
# 
  OBPDE:use new fourth order boundary conditions 1
  OBPDE:use boundary dissipation in AF scheme 0
  OBPDE:stabilize high order boundary conditions 0
# **************************************
#  first order predictor
 number of PC corrections $numberOfCorrections
# 
max number of AF corrections $afit
AF correction relative tol $aftol
#
$cmd="#";
if( $move eq "rotate" ){ $cmd="turn on moving grids\n specify grids to move\n pause\nrotate\n 0. 0. 0 \n $rate 0.\n$gridToMove\n$gridToMove2\n done\n done"; }
if( $move eq "shift" ){ $cmd="turn on moving grids\n specify grids to move\n translate\n $xshift $yshift $zshift \n 1.\n$gridToMove\n$gridToMove2\n done\n done"; }
# deforming body linear motion is x(t) = a*t^p
$ap=-1.; $pp=1; $vg0=$ap; $vg1=0.; $vg2=0.; $ag0=0.; $ag1=0.; $ag2=0.; $gvOrder=2; $gaOrder=2;
if( $move eq "deform" ){ $cmd="turn on moving grids\n specify grids to move\n  deforming body\n" \
    . "user defined deforming body\n linear deform\n $ap $pp\n  boundary parameterization\n  1 \n" \
    . "grid evolution parameters...\n linear motion\n $ap $pp\n exit\n" \
    . "initial velocity\n $vg0 $vg1 $vg2\n initial acceleration\n $ag0 $ag1 $ag2\n" \
    . "velocity order of accuracy\n $gvOrder\n acceleration order of accuracy\n $gaOrder \n" \
    . "done\n choose grids by share flag\n 100\n done\n  done"; }
$cmd
#
  frequency for full grid gen update $freqFullUpdate
#
  $dop
  refactor frequency $refactorFrequency
  $implicitVariation
  implicit factor $implicitFactor 
# for testing, force use of the full implicit system:
  use full implicit system $fullSystem
#
  choose grids for implicit
    all=implicit
  done
#
#
echo to terminal 0
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgDebug=$ogesDebug; $ogmgCoarseGridSolver="best"; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp; $ogmgCmd = "maximum number of iterations\n$ogmgMaxIts"
   #$ogmgOpav=0;
   #$ogmgSsr=1; 
   #$ogesDtol=1.e6; 
   #$ogmgCoarseGridSolver="yale";
   $ogmgAutoChoose=0;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogmgOpav=0; $ogmgRtolcg=1.e-6;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
echo to terminal 1
#
  boundary conditions...
   order of extrap for outflow 3 (-1=default)
   #order of extrap for 2nd ghost line 3
   # for light moving bodies: (1=turn on for moving bodies, 2=turn on for all walls (for testing)
   moving body pressure BC $mbpbc
   moving body pressure BC coefficient $mbpbcc
  done
# 
  boundary conditions
    all=dirichletBoundaryCondition
    $bcn
    $bc1
    $bc2
    $bc3
    $bc4
    $bc5
    $bc6
#
#    bcNumber1=noSlipWall, mixedDerivative(.5*t+1.*t.n=1.)
#    bcNumber2=slipWall
##    bcNumber3=slipWall
#    bcNumber5=slipWall
#    bcNumber3=noSlipWall, mixedDerivative(.2*t+1.*t.n=1.)
# 
#*kkc all=noSlipWall
#*    all=noSlipWall, mixedDerivative(1.*t+0.*t.n=0.)
#    $backGround(0,0)=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)
#    $backGround(1,0)=outflow
#     all=noSlipWall, mixedDerivative(1.*t+0.*t.n=0.)
#     square(0,1)=dirichletBoundaryCondition
#
#     square(0,1)=slipWall
#   square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
#   square(1,0)=outflow 
#    square(1,0)=outflow , pressure($cp0*p+1.*p.n=0.)
#     square(0,0)=inflowWithVelocityGiven , parabolic(d=.2,p=1.,u=1.), oscillate(t0=.3,omega=2.5)
# 
   done
  debug $debug
  check error on ghost
   $checkErrOnGhost
  if( $project eq "1" ){ $cmd="project initial conditions"; }else{ $cmd="#"; }
  $cmd
 continue
# 
plot:$uplot
 $go

 movie mode
 finish

