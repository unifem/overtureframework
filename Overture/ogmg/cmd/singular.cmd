#
#  ogmg: test singular problems 
#    Note: For a singular problem we compute the left null vector of the operator at each level.
#          The left null vector is saved to a file and can be read back in for subsequent solves.
#    NOTE: For now using predefined equations does not always work (null vector computation needs full matrix)
#
# Examples:
#   square8 with predefined=0 -- I think the coarse grid is too coarse and the equations are singular (corner extrap?)
#   ogmgt -noplot singular -g=square8 -fx=1. -fy=1. -sm=rbj -nullVector=square8NullVector.hdf -predefined=1 
#   ogmgt -noplot singular -g=square10 -fx=1. -fy=1. -sm=rbj -nullVector=square10NullVector.hdf -predefined=1 
#   ogmgt -noplot singular -g=channel2.order2.ml2 -fx=2. -fy=1. -sm=lz2 -nullVector=channel2.order2NullVector.hdf [ECR=.683
#   ogmgt -noplot singular -g=channel5.order2.ml3 -fx=2. -fy=1. -sm=lz2 -nullVector=channel5.order2NullVector.hdf
#   ogmgt -noplot singular -g=channel10.order2.ml3 -fx=2. -fy=1. -sm=lz2 -nullVector=channel10.order2NullVector.hdf [ECR=.623
#   ogmgt -noplot singular -g=channel10.order2.ml3 -fx=2. -fy=1. -sm=lz2 -nullVector=channel10.order2NullVector.hdf [ECR=.623
#
#   ogmgt -noplot singular -g=cic.bbmg4 -fx=2. -fy=1. -sm=lz2 -nullVector=cic.bbmg4NullVector.hdf [ECR=.768
#
# -- fourth-order
#   ogmgt -noplot singular -g=square20.order4.hdf -sm=rbj -nullVector=square20.order4NullVector.hdf -predefined=1 
# -- parallel
#   mpirun -np 2 $ogmgp -noplot singular -g=square8 -fx=1. -fy=1. -sm=rbj -nullVector=square8NullVector.hdf -predefined=1
#   mpirun -np 2 $ogmgp -noplot singular -g=square10 -sm=rbj -nullVector=square10NullVector.hdf -predefined=1
#   mpirun -np 2 $ogmgp -noplot singular -g=channel2.order2.ml2 -fx=2. -fy=1. -sm=lz2 -nullVector=channel2.order2NullVector.hdf -predefined=1
#   mpirun -np 2 $ogmgp -noplot singular -g=channel5.order2.ml3 -fx=2. -fy=1. -sm=lz2 -nullVector=channel5.order2NullVector.hdf -predefined=1 
#   mpirun -np 2 $ogmgp -noplot singular -g=channel10.order2.ml3 -fx=2. -fy=1. -sm=lz2 -nullVector=channel10.order2NullVector.hdf -predefined=1 
#   mpirun -np 2 $ogmgp -noplot singular -g=channel40.order2.ml4 -fx=2. -fy=1. -sm=lz2 -nullVector=channel40.order2NullVector.hdf -predefined=1 
#
$rtol=1.e-15; $etol=1.e-10; $maxit=10; $fx=2.; $fy=2.; $fz=2.; $debug=3; $eqn="lap"; $predefined=0; $ogesDebug=0; 
$sm="rb";
$nullVector="leftNullVector.hdf"; $rtolnv=1.e-12; $atolnv=1.e-14;
$nibs=0;  
$bls=0; 
$nvSolver="best"; $iluLevels=3;
* $nvSolver="yale";
*
$pi=4.*atan2(1.,1.);
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"maxit=i"=>\$maxit,"debug=i"=>\$debug,"sm=s"=>\$sm,"bsm=s"=>\$bsm,"ic=i"=>\$ic,\
            "fmg=i"=>\$fmg,"opav=i"=>\$opav,"rtol=f"=>\$rtol,"atol=f"=>\$atol,"option=s"=>\$option,"lb=s"=>\$lb,\
            "ssr=i"=>\$ssr,"mii=i"=>\$maxInterpIts,"cycle=s"=>\$cycle,"nsm=s"=>\$nsm,"bc=s"=>\$bc,\
            "ibs=i"=>\$ibs,"nibs=i"=>\$nibs,"ils=i"=>\$ils,"iml=i"=>\$iml,"ins=i"=>\$ins,"cfw=i"=>\$cfw,\
            "bls=i"=>\$bls,"bli=i"=>\$bli,"bll=i"=>\$bll,"autoSmooth=i"=>\$autoSmooth,"omega=f"=>\$omega,\
            "eqn=s"=>\$eqn,"predefined=i"=>\$predefined,"tz=s"=>\$tz,"cgSolver=s"=>\$cgSolver,\
            "ogesDebug=i"=>\$ogesDebug,"autoChoose=i"=>\$autoChoose,"bcOrder4=s"=>\$bcOrder4,\
            "fx=f"=>\$fx,"fy=f"=>\$fy,"fz=f"=>\$fz,"rtolcg=f"=>\$rtolcg,"atolcg=f"=>\$atolcg,\
            "nvSolver=s"=>\$nvSolver,"nullVector=s"=>\$nullVector );
# -------------------------------------------------------------------------------------------------
## $fx=$fx/$pi; 
if( $eqn eq "lap" && $predefined eq 1 ){ $eqn = "laplace (predefined)"; }
if( $eqn eq "lap" && $predefined eq 0 ){ $eqn = "laplace"; }
if( $sm eq "rb" ){ $smooth="red black"; }
if( $sm eq "rbj" ){ $smooth="red black jacobi"; $bsmooth="#"; }
if( $sm eq "j" ){ $smooth="jacobi"; }
if( $sm eq "lz1" ){ $smooth="line zebra direction 1"; }
if( $sm eq "lz2" ){ $smooth="line zebra direction 2"; }
if( $sm eq "lz3" ){ $smooth="line zebra direction 3"; }
if( $sm eq "al" ){ $smooth="alternating"; }
if( $sm eq "alj" ){ $smooth="alternating jacobi"; }
if( $sm eq "alz" ){ $smooth="alternating zebra"; }
if( $nvSolver eq "best" ){ $nvSolver="choose best iterative solver"; }
* ---------- old way: (from sqn.cmd)
* $grid="square128";
* $grid="square256";
* $grid="square64";
* $grid="square32";
* $grid="square16";
* $grid="nonSquare128";
* $grid="nonSquare32";
* predefined equation fails for the next when we need to compute a null vector:
* $grid="square8";  $nullVector="square8NullVector.hdf"; 
* $grid="square8p";  $nullVector="square8pNullVector.hdf"; 
* $grid="nonSquare8";  $nullVector="nonSquare8NullVector.hdf"; 
*
* $grid="sbs.bbmg0.hdf"; $nullVector="sbs.bbmg0NullVector.hdf"; $maxit=1; 
* $grid="sbs.bbmg1.hdf"; $nullVector="sbs.bbmg1NullVector.hdf"; $nvSolver="yale";
* $grid="sbs.bbmg2.hdf"; $nullVector="sbs.bbmg2NullVector.hdf"; 
* predefined equation fails for the next when we need to compute a null vector:
* petsc/slap fail to solve null-vector for level>0 :  
* $grid="cic.bbmg3.hdf"; $smooth=$lz2; $nullVector="cic.bbmg3NullVector.hdf";
* $grid="cic.bbmg4.hdf"; $smooth="$lz2\n smoother(0)=rb"; $nullVector="cic.bbmg4NullVector.hdf"; 
* $grid="cic.bbmg5.hdf"; $smooth="$lz2\n smoother(0)=rb"; $nullVector="cic.bbmg5NullVector.hdf"; 
** $grid="cic.bbmg6.hdf"; $smooth="$lz2\n smoother(0)=rb"; $nullVector="cic.bbmg6NullVector.hdf"; 
* $grid="square4";
*
* use predefined equation for 
*   100602: these next few still work
* $grid="channel2.order2.ml2.hdf"; $fx=2./$pi; $fy=1.; $smooth=$lz2; $nullVector="channel2.order2NullVector.hdf";  
* $grid="channel10.order2.ml3.hdf"; $fx=2./$pi; $fy=1.; $smooth=$lz2; $nullVector="channel10.order2NullVector.hdf";  
* $grid="channel20.order2.ml3.hdf"; $fx=2./$pi; $fy=1.; $smooth=$lz2; $nullVector="channel20.order2NullVector.hdf";  
* $grid="channel40.order2.ml4.hdf"; $fx=2./$pi; $fy=1.; $smooth=$lz2; $nullVector="channel20.order2NullVector.hdf";  
* 
* $grid="channel2.order2.hdf"; $fx=2./$pi; $fy=1.; $smooth=$lz2; $nullVector="channel2.order2NullVector.hdf";  
* $grid="channel4.order2.hdf";  $fx=2./$pi; $fy=1.; $smooth=$lz2; $nullVector="channel4.order2NullVector.hdf"; 
* $grid="channel5.order2.hdf"; $fx=2./$pi; $fy=1.; $smooth=$lz2; $rtol=1.e-12; $etol=1.e-11; $nullVector="channel5.order2NullVector.hdf";
* $grid="channel10.order2.hdf"; $fx=2./$pi; $fy=1.; $smooth=$lz2; $rtol=1.e-12; $etol=1.e-11; $nullVector="channel10.order2NullVector.hdf";
* ------------------------
#
$grid
*
neumann
*
*
# laplace (predefined)
# laplace
$eqn
# 
* bc=2 : neumann
*bc(0,0)=2
*bc(1,0)=2
*bc(0,1)=2
*bc(1,1)=2
turn on trigonometric
set trigonometric frequencies
  $fx $fy $fz
* turn on polynomial
* test bc
*
* -------------- start OgmgParameters ---------------------- 
change parameters
* 
  null vector option:readOrComputeAndSave
  null vector file name:$nullVector
  null vector solve options...
   $nvSolver
*      yale
    relative tolerance
      $rtolnv
    absolute tolerance
      $atolnv
*    define petscOption -ksp_type gmres
*    define petscOption -ksp_type richardson
*     define petscOption -ksp_type preonly
*     define petscOption -pc_type lu
    define petscOption -pc_factor_levels $iluLevels
#    define petscOption -ksp_monitor stdout
#    define petscOption -ksp_view
   exit
# -- direct solver on coarse grid: 
  Oges parameters
     debug 
      $ogesDebug 
  exit
* 
* project right hand side for singular problems 0
* set mean value for singular problems 0
* 
* do not average coarse grid equations
*  jacobi
*  maximum number of levels
*    2
* ghost line averaging option
*   3 3
residual tolerance
  $rtol
error tolerance
  $etol
maximum number of iterations
  $maxit
***********************************************
 number of boundary layers to smooth
   $bls
 number of boundary smooth iterations
   5 3 1 3  1 3 1 9 7 6 5 3 2 3 1 5 3
 number of levels for boundary smooths
   5 2 1
*************************
*************************
 number of interpolation layers to smooth
   $nibs
 number of interpolation smooth iterations
   2 
 number of levels for interpolation smooths
   5 1 
**************************
# --- for parallel use: 
use new red-black smoother
#
if( $debug > 3 ){ $ssr="show smoothing rates"; }else{ $ssr="#"; }
$ssr
$smooth
* alternating zebra
* alternating 
* ld1
* line zebra direction 1
* line zebra direction 2
* smoother(0)=rb
* line jacobi direction 2
exit
* ------------ end OgmgParameters ----------------------
debug
 $debug
exit
