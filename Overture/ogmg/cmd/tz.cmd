# ===========================================================================
#   TZ tests for ogmg:
# Usage:
#   
#  ogmgt tz -g=<name> -maxit=<num> -sm=[rb|rbj|lz1|lz2|lz3|j|alj|alz] -ibs=[0|1] -nibs=<> -ils=<> -iml=<> 
#           -ins=<> -bls=<> -bli= -bll= -fmg=[0|1] -opav=[0|1|2] -autoSmooth=[0|1]
#           -option=[solve|sm|fc|cf|bc|cg] -lb=[kh|all] -levels=<num> -ssr=[0|1] -mii=<> -cycle=[V|W|F] -nsm="n m"
#           -eqn=[lap|dsg|heat] -predefined=[0|1] -autoChoose=[0|1|2] -tz=[poly|trig] -ic=[0|1] -scg=[0|1]
#           -cgSolver=[best|yale] -bcOrder4=[eqn|extrap] -matlab=[0|1] -omegaz=<> -rtol=<> -etol=<> -rtolcg=<> 
#           -conv=[residual|errEst|old] -rb=[new|old] -ilu=[0|1|2...] -iluFill=<> -save=<name> -read=<name>
#
#   -nibs = IBS: number of global smoothing iterations
#   -ils = IBS: number of interpolation layers to smooth
#   -iml = IBS: number of MG levels to apply IBS smoothing
#   -ins = IBS: number of smooth iterations
#   -bls, -bli, -bll = boundary smoothing: number of boundary layers to smooth, number of iterations, number of levels
#   -lb = load balancer
#   -levels = maximum number of levels
#   -ssr = show smoothing rates
#   -mii = max iterations for interpolation
#   -cycle = V, W or F 
#   -nsm = number of pre and post smooths, e.g. -nsm="2 1"
#   -opav : operator averaging, 0=none, 1=all grids, 2=only average Cartesian grids (for 4th-order)
#   -autoSmooth : 0= turn off auto sub smooth determination, i.e. fix number of sub-smooths
#   -option : test different parts of the MG solver, sm=smoother, fc=fine-to-coarse, cf=coarse-to-fine, 
#   -autoChoose : 0=OFF, 1= automatically choose good parameters (smoothers, etc.), 2=choose more robust params
#   -ic : 1=start with exact initial conditions
#   -scg : 1 = solve coarse grid equations with the smoother
#   -cgsi : number of iterations to use when smoothing the coarse grid equations
#   -matlab=1 : save a matlab file for plotting convergence rates
#   -rtolcg : coarse grid convergence tolerance
#   -ilucg : coarse grid solve ILU levels
#   -iluFill : expected fill in for coarse grid ILU pre-conditionner.
#   -bcOrder4 : 4th order BC's : use-equation or extrapolate to get extra conditions
#   -conv=[residual|errEst|old] : set the convergence criteria
#   -read=gridName : read a MG grid with all coarser levels
#   -save=gridName : save the MG grid with all coarser levels
#
# Examples (See also tz.tests for a more extensive list of examples)
#  ogmgt tz -noplot -g=square16.order2 -debug=3  [ ECR=0.419
#  ogmgt tz -noplot -g=cic.bbmg -debug=3  [ ECR=0.667
#  ogmgt tz -g=cic.bbmg4 -maxit=8
#  ogmgt tz -noplot -g=cic.bbmg -debug=7 
#  ogmgt tz -noplot -g=twoSpheresInAChannele2.order2.ml2.hdf -debug=3 [ECR=.569
#  ogmgt tz -noplot -g=twoSpheresInAChannele2.order2.ml2.hdf -autoChoose=1 -debug=3 [ECR=.608
#
#
# ===========================================================================
#
# Set default parameters: 
#
$grid="cic.bbmg6.hdf"; $maxit=10; $debug=3; $sm="rb"; $fmg=0; $opav=1; $option="solve"; $cfw=2; $omega=-1.; $omegaz=-1.; $ic=0; 
$ibs=1;$nibs=2; $ils=0; $iml=1; $ins=2; 
$bls=0; $bli=5; $bll=1; $save=""; $read=""; 
$bsmooth="smoother(0)=rb"; $bsm=""; $autoSmooth=1; $conv="residual";  $rb="new";
$maxInterpIts=3; $lb="kh"; $ssr=0; $cycle="V"; $nsm="1 1"; $autoChoose=0; $matlab=0;
$rtol=1.e-14; $atol=1.e-10; $etol=1.e-12; 
$rtolcg=1.e-3; $atolcg=1.e-14;  $ilucg=1; $iluFill=3.; 
$predefined=1;  $cgSolver="best"; $ogesDebug=0;
$eqn=lap; $tz="trig"; $bcOrder4=""; $fx=1.; $fy=1.; $fz=1.; 
$bc1=""; $bc2=""; $bc3=""; $bc4="";  $bc5=""; $bc6=""; $bc7=""; $bc8=""; 
$solveCoarseGridBySmoother=0; $cgsi=10; $projectSingular=1; $adjustSingularEquations=0; 
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"maxit=i"=>\$maxit,"debug=i"=>\$debug,"sm=s"=>\$sm,"bsm=s"=>\$bsm,"ic=i"=>\$ic,\
            "fmg=i"=>\$fmg,"opav=i"=>\$opav,"rtol=f"=>\$rtol,"atol=f"=>\$atol,"option=s"=>\$option,"lb=s"=>\$lb,\
            "ssr=i"=>\$ssr,"mii=i"=>\$maxInterpIts,"cycle=s"=>\$cycle,"nsm=s"=>\$nsm,"bc=s"=>\$bc,"conv=s"=>\$conv,\
            "ibs=i"=>\$ibs,"nibs=i"=>\$nibs,"ils=i"=>\$ils,"iml=i"=>\$iml,"ins=i"=>\$ins,"cfw=i"=>\$cfw,\
            "bls=i"=>\$bls,"bli=i"=>\$bli,"bll=i"=>\$bll,"autoSmooth=i"=>\$autoSmooth,"omega=f"=>\$omega,"omegaz=f"=>\$omegaz,\
            "eqn=s"=>\$eqn,"predefined=i"=>\$predefined,"tz=s"=>\$tz,"cgSolver=s"=>\$cgSolver,"etol=f"=>\$etol,\
            "ogesDebug=i"=>\$ogesDebug,"autoChoose=i"=>\$autoChoose,"bcOrder4=s"=>\$bcOrder4,"matlab=i"=>\$matlab,\
            "fx=f"=>\$fx,"fy=f"=>\$fy,"fz=f"=>\$fz,"rtolcg=f"=>\$rtolcg,"atolcg=f"=>\$atolcg,"scg=i"=>\$solveCoarseGridBySmoother,\
            "bc1=s"=>\$bc1,"bc2=s"=>\$bc2,"bc3=s"=>\$bc3,"bc4=s"=>\$bc4,"bc5=s"=>\$bc5,"bc6=s"=>\$bc6,\
            "bc7=s"=>\$bc7,"bc8=s"=>\$bc8,"rb=s"=>\$rb,"cgsi=i"=>\$cgsi,"projectSingular=i"=>\$projectSingular,\
            "adjustSingularEquations=i"=>\$adjustSingularEquations,"ilucg=i"=>\$ilucg,"iluFill=f"=>\$iluFill,\
            "save=s"=>\$save, "read=s"=>\$read );
# -------------------------------------------------------------------------------------------------
$grid
if( $eqn eq "lap" && $predefined eq 1 ){ $eqn = "laplace (predefined)"; }
if( $eqn eq "dsg" && $predefined eq 1 ){ $eqn = "divScalarGrad (predefined)"; }
if( $eqn eq "heat" && $predefined eq 1 ){ $eqn = "heat equation (predefined)"; }
if( $eqn eq "lap" && $predefined eq 0 ){ $eqn = "laplace"; }
if( $eqn eq "dsg" && $predefined eq 0 ){ $eqn = "divScalarGrad"; }
if( $eqn eq "heat" && $predefined eq 0 ){ $eqn = "heat equation"; }
#
if( $cgSolver eq "best" ){ $cgSolver="choose best iterative solver"; }
if( $ssr eq 1 ){ $ssr="show smoothing rates"; }else{ $ssr="#"; }
if( $option eq "solve" ){ $option="solve"; }
if( $option eq "sm" ){ $option="test smoother"; $bsmooth="#"; }
if( $option eq "cf" ){ $option="test coarse to fine"; }
if( $option eq "fc" ){ $option="test fine to coarse"; }
if( $option eq "bc" ){ $option="test bc"; }
if( $option eq "cg" ){ $option="test coarse grid solver"; }
if( $sm eq "rb" ){ $smooth="red black"; }
if( $sm eq "rbj" ){ $smooth="red black jacobi"; $bsmooth="#"; }
if( $sm eq "j" ){ $smooth="jacobi"; }
if( $sm eq "lz1" ){ $smooth="line zebra direction 1"; }
if( $sm eq "lz2" ){ $smooth="line zebra direction 2"; }
if( $sm eq "lz3" ){ $smooth="line zebra direction 3"; }
if( $sm eq "lj1" ){ $smooth="line jacobi direction 1"; }
if( $sm eq "lj2" ){ $smooth="line jacobi direction 2"; }
if( $sm eq "lj3" ){ $smooth="line jacobi direction 3"; }
if( $sm eq "al" ){ $smooth="alternating"; }
if( $sm eq "alj" ){ $smooth="alternating jacobi"; }
if( $sm eq "alz" ){ $smooth="alternating zebra"; }
if( $sm eq "oges" ){ $smooth="oges smoother"; }
#
if( $bsm eq "rb" ){ $bsmooth="red black"; }
if( $bsm eq "rbj" ){ $bsmooth="red black jacobi"; }
if( $bsm eq "j" ){ $bsmooth="jacobi"; }
if( $bsm eq "lz1" ){ $bsmooth="line zebra direction 1"; }
if( $bsm eq "lz2" ){ $bsmooth="line zebra direction 2"; }
if( $bsm eq "lz3" ){ $bsmooth="line zebra direction 3"; }
if( $bsm eq "lj1" ){ $bsmooth="line jacobi direction 1"; }
if( $bsm eq "lj2" ){ $bsmooth="line jacobi direction 2"; }
if( $bsm eq "lj3" ){ $bsmooth="line jacobi direction 3"; }
if( $bsm eq "al" ){ $bsmooth="alternating"; }
if( $bsm eq "alj" ){ $bsmooth="alternating jacobi"; }
if( $bsm eq "alz" ){ $bsmooth="alternating zebra"; }
if( $bsm eq "oges" ){ $bsmooth="oges smoother"; }
if( $ibs eq 0 ){ $ibs="do not combine smooths with IBS"; }else{ $ibs="combine smooths with IBS"; }
#
$bcmd = "#"; 
if( $bc eq "nddd" ){ $bcmd = "bc(0,0,0)=2"; }
if( $bc eq "ddnd" ){ $bcmd = "bc(0,1,0)=2"; }
if( $bc eq "nndd" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=2"; }
if( $bc eq "ddnn" ){ $bcmd = "bc(0,1,0)=2\n bc(1,1,0)=2"; }
if( $bc eq "dndd" ){ $bcmd = "bc(0,0,0)=1\n bc(1,0,0)=2"; }
if( $bc eq "dnnd" ){ $bcmd = "bc(1,0,0)=2\n bc(0,1,0)=2"; }
if( $bc eq "ndnn" ){ $bcmd = "bc(0,0,0)=2\n bc(0,1,0)=2\n bc(1,1,0)=2"; }
if( $bc eq "nmnn" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=3\n bc(0,1,0)=2\n bc(1,1,0)=2"; }
if( $bc eq "nmnnn" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=3\n bc(0,1,0)=2\n bc(1,1,0)=2\n bc(0,1,1)=2"; }
if( $bc eq "nmnnnn" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=3\n bc(0,1,0)=2\n bc(1,1,0)=2\n bc(0,2,0)=2\n bc(1,2,0)=2\n bc(0,1,1)=2\n bc(0,1,2)=2"; }
if( $bc eq "ndnnn" ){ $bcmd = "bc(0,0,0)=2\n bc(0,1,0)=2\n bc(1,1,0)=2\n bc(0,1,1)=2"; }
if( $bc eq "dedd" ){ $bcmd = "bc(1,0,0)=5"; }  # 5=extrapolate
if( $bc eq "ndnnnn" ){ $bcmd = "bc(0,0,0)=2\n bc(0,1,0)=2\n bc(1,1,0)=2\n bc(0,2,0)=2\n bc(1,2,0)=2"; }
if( $bc eq "ndnnnd" ){ $bcmd = "bc(0,0,0)=2\n bc(0,1,0)=2\n bc(1,1,0)=2\n bc(0,2,0)=2\n bc(1,2,0)=1"; }
if( $bc eq "ndddnd" ){ $bcmd = "bc(0,0,0)=2\n bc(0,1,0)=1\n bc(1,1,0)=1\n bc(0,2,0)=2\n bc(1,2,0)=1"; }
if( $bc eq "nnddnd" ){ $bcmd = "bc(0,0,0)=2\n bc(0,1,0)=2\n bc(1,1,0)=1\n bc(0,2,0)=2\n bc(1,2,0)=1"; }
if( $bc eq "dnddnd" ){ $bcmd = "bc(0,0,0)=1\n bc(1,0,0)=2\n bc(0,1,0)=1\n bc(1,1,0)=1\n bc(0,2,0)=2\n bc(1,2,0)=1"; }
if( $bc eq "dndddn" ){ $bcmd = "bc(0,0,0)=1\n bc(1,0,0)=2\n bc(0,1,0)=1\n bc(1,1,0)=1\n bc(0,2,0)=1\n bc(1,2,0)=2"; }
if( $bc eq "dnddnn" ){ $bcmd = "bc(0,0,0)=1\n bc(1,0,0)=2\n bc(0,1,0)=1\n bc(1,1,0)=1\n bc(0,2,0)=2\n bc(1,2,0)=2"; }
if( $bc eq "nnnndd" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=2\n bc(0,1,0)=2\n bc(1,1,0)=2\n bc(0,2,0)=1\n bc(1,2,0)=1"; }
if( $bc eq "ddnnnn" ){ $bcmd = "bc(0,0,0)=1\n bc(1,0,0)=1\n bc(0,1,0)=2\n bc(1,1,0)=2\n bc(0,2,0)=2\n bc(1,2,0)=2"; }
if( $bc eq "nnddnn" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=2\n bc(0,1,0)=1\n bc(1,1,0)=1\n bc(0,2,0)=2\n bc(1,2,0)=2"; }
if( $bc eq "dnnnnn" ){ $bcmd = "bc(0,0,0)=1\n bc(1,0,0)=2\n bc(0,1,0)=2\n bc(1,1,0)=2\n bc(0,2,0)=2\n bc(1,2,0)=2"; }
if( $bc eq "nndnnn" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=2\n bc(0,1,0)=1\n bc(1,1,0)=2\n bc(0,2,0)=2\n bc(1,2,0)=2"; }
if( $bc eq "nnndnn" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=2\n bc(0,1,0)=2\n bc(1,1,0)=1\n bc(0,2,0)=2\n bc(1,2,0)=2"; }
if( $bc eq "nnnndn" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=2\n bc(0,1,0)=2\n bc(1,1,0)=2\n bc(0,2,0)=1\n bc(1,2,0)=2"; }
if( $bc eq "ndndnd" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=1\n bc(0,1,0)=2\n bc(1,1,0)=1\n bc(0,2,0)=2\n bc(1,2,0)=1"; }
if( $bc eq "ndnddn" ){ $bcmd = "bc(0,0,0)=2\n bc(1,0,0)=1\n bc(0,1,0)=2\n bc(1,1,0)=1\n bc(0,2,0)=1\n bc(1,2,0)=2"; }
if( $bc eq "ddddnd" ){ $bcmd = "bc(0,2,0)=2"; }
if( $bc eq "dddddn" ){ $bcmd = "bc(1,2,0)=2"; }
if( $bc eq "ddddnn" ){ $bcmd = "bc(0,2,0)=2\n bc(1,2,0)=2"; }
#
if( $fmg eq "1" ){ $fmg="use full multigrid"; }else{ $fmg="#"; }
if( $opav eq "0" ){ $opav = "do not average coarse grid equations"; }\
  elsif( $opav eq "2" ){ $opav ="do not average coarse curvilinear grid equations"; }else{ $opav = "#"; }
if( $lb eq "kh" ){ $lb="KernighanLin"; }
if( $lb eq "all" ){ $lb="all to all"; }
#
# pause
#
#
$option
# 
# divScalarGrad (predefined)
# laplace (predefined)
# heat equation (predefined)
$eqn
if( $adjustSingularEquations eq 1 ){ $cmd="adjust singular equations"; }else{ $cmd="#"; }
$cmd
#
# turn off twilight zone
# turn on trigonometric
if( $tz eq "poly" ){ $cmd="turn on polynomial"; }elsif( $tz eq trig ){ $cmd="turn on trigonometric"; }else{ $cmd = "turn off twilight zone"; }
$cmd
set trigonometric frequencies
  $fx $fy $fz 
if( $ic eq 1 ){ $cmd="set exact initial conditions"; }else{ $cmd="#"; }
$cmd
# dirichlet=1 neumann=2 mixed=3
$bcmd
# assign bc's of the form bcNumber<num>=[d|n|m|e]
$cmd="#"; 
if( $bc1 ne "" ){ $cmd.="\n bcNumber1=$bc1"; }
if( $bc2 ne "" ){ $cmd.="\n bcNumber2=$bc2"; }
if( $bc3 ne "" ){ $cmd.="\n bcNumber3=$bc3"; }
if( $bc4 ne "" ){ $cmd.="\n bcNumber4=$bc4"; }
if( $bc5 ne "" ){ $cmd.="\n bcNumber5=$bc5"; }
if( $bc6 ne "" ){ $cmd.="\n bcNumber6=$bc6"; }
if( $bc7 ne "" ){ $cmd.="\n bcNumber7=$bc7"; }
if( $bc8 ne "" ){ $cmd.="\n bcNumber8=$bc8"; }
$cmd
#
# bc(0,0,0)=2
# bc(1,0,0)=3
# bc(0,1,0)=2
# bc(1,1,0)=2
# bc(0,1,1)=2
#
#****************************************************
change parameters
#[residual|errEst|old]
if( $conv eq "residual" ){ $conv="Convergence criteria: residual converged"; }elsif($conv eq "errEst"){ $conv="Convergence criteria: error estimate converged"; }else{ $conv="Convergence criteria: residual converged old-way"; }
$conv
# do not use error estimate in convergence test
#
# operator averaging option: 
$opav
#
#* use new auto sub-smooth
#*
 $fmg
#
#* set load balancing options
#    KernighanLin
#    all to all
#*  $lb
#* exit
#ghost line averaging option
# 1=impose-extrapolation 3=partial-weighting (default), 6=impose-Neumann
#  1 6
#
if( $bcOrder4 eq "extrap" ){ $cmd="extrapolate fourth order boundary conditions"; }else{ $cmd="#"; }
$cmd
# do not use symmetry corner boundary condition
# For 4th-order + Neumann: solve for 2 ghost AND boundary point:
# solve equation with boundary conditions
******
# -- direct/sparse solver on coarse grid: 
  Oges parameters
    # choose best direct solver
    # choose best iterative solver
     $cgSolver
     number of incomplete LU levels
      $ilucg
     incomplete LU expected fill
       $iluFill
     relative tolerance
       $rtolcg
     absolute tolerance
       $atolcg
     debug 
      $ogesDebug 
  exit
# Solve the coarse grid equations using the smoother:
if( $solveCoarseGridBySmoother == 1 ){ $cmd = "iterate on coarse grid"; }else{ $cmd="#"; }
$cmd
number of iterations on coarse grid
  $cgsi
# The sub-smooth reference grid uses 1 sub-smooth, other grids can use more. 
# sub-smooth reference grid: 2 (-1=use default)
# 
******
#****
#* now the default: use new fine to coarse BC
#**********************************************
 number of boundary layers to smooth
   $bls
 number of boundary smooth iterations
   # 5 3 1 3  1 3 1 9 7 6 5 3 2 3 1 5 3
   $bli 
 number of levels for boundary smooths
   # 1 5 2 1
   $bll
# -- IBS: Interpolation boundary smoothing: 
 $ibs
 number of interpolation smooth global iterations
  $nibs
 number of interpolation layers to smooth
   # 4 3 2 2 1 2 1 3 0 2 1 3 2 1
   $ils
 number of interpolation smooth iterations
   # 2 2 3 5 2 1 3 1 3  1 3 1 9 7 6 5 3 2 3 1 5 3
   $ins
 number of levels for interpolation smooths
   # 1 2 1 5 2 1
   $iml
#*************************
#
# do not use locally optimal omega
# omega red-black
#   1.03 1.05 1.0 1.05 1.16 1.05 1.0 1.07 1.0 1.05 1.05 1.1 1.09  1.1 1.07
if( $omega > 0 ){ $cmd="do not use locally optimal omega\n omega red-black\n $omega"; }else{ $cmd="#"; }
$cmd
# Omega for the line smoother: 
if( $omegaz > 0 ){ $cmd="omega line-zebra\n $omegaz"; }else{ $cmd="#"; }
$cmd
# omega line-zebra
#   1.2 1.1
#**********
# do not use split step line solver
#**********
project right hand side for singular problems $projectSingular
adjust equations for singular problems $adjustSingularEquations
#
residual tolerance $rtol
absolute tolerance $atol
error tolerance $etol
# 
maximum number of iterations
  $maxit
maximum number of interpolation iterations
  $maxInterpIts
coarse to fine transfer width
  $cfw
#
# do not interpolate after smoothing
# --- for parallel use: 
# use new red-black smoother
if( $rb eq "old" ){ $cmd = "do not use new red-black smoother"; }else{ $cmd="#"; }
$cmd
#
# works better if we interpolate the defect!
# do not interpolate the defect
$ssr
if( $autoSmooth eq 0 ){ $cmd="do not use automatic sub-smooth determination"; }else{ $cmd="#"; }
$cmd
#
#   do not interpolate the defect
#  jacobi
# alternating zebra
# alternating 
# ld1
# line zebra direction 1
# line jacobi direction 2
$smooth
$bsmooth
#*********
#oges smoother
#  1
#done
#oges smoother parameters
# number of incomplete LU levels
#   0
# exit
#***************
# smoother(0)=jacobi
# number of cycles=2 : W cycle
#
$cmd="#"; 
if( $cycle eq "V" ){ $cmd = "number of cycles\n 1 1 1 1 1 "; }
if( $cycle eq "W" ){ $cmd = "number of cycles\n 2 2 2 2 2"; }
if( $cycle eq "F" ){ $cmd = "use an F cycle"; }
$cmd
# 
number of smooths
 $nsm
if( $matlab eq 1 ){ $cmd = "output a matlab file"; }else{ $cmd="#"; }
$cmd
# save the check file for coarse grid levels built:
#* save coarse grid check file
# do not use optimized version
#
# --------Automatically choose parameters -----------------------
# if( $autoChoose eq 1 ){ $cmd="choose good parameters"; }else{ $cmd="#"; }
# $cmd
choose good parameters: $autoChoose
# ---------------------------------------------------------------
# -- over-ride auto:
if( $ils ne 0 ){ $cmd = "number of interpolation layers to smooth\n $ils\n"; } else{ $cmd="#"; }
$cmd
#
#  -- save the MG CompositeGrid
if( $save ne "" ){ $cmd="save the multigrid composite grid\n $save"; }else{ $cmd="#"; }
$cmd
#  -- read an existing MG CompositeGrid with all the levels generated
if( $read ne "" ){ $cmd="read the multigrid composite grid\n $read"; }else{ $cmd="#"; }
$cmd
#
exit
debug
  $debug
exit






