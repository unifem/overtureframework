#
# Here are options for Oges. This file can be included in cg command scripts
#
# $ogesSolver :  "yale", "use best iterative solver", ...
# $ogesCmd : define any extra Oges commands here
# $ogesRtol   :  relative tolerance
# $ogesAtol   :  absolute tolerance
# $ogesEtol   :  error tolerance (for MG)
# $ogesDtol   :  maximum allowable increase in residual 
# $ogesMaxIterations   :  maximum number of iterations.
# $ogesPc : preconditioner: ilu, lu, 
# $ogesIluLevels : number of levels for ILU preconditioner
# $ogesDebug : debug parameter
#    -- multigrid: 
# $ogmgCmd : define any extra Ogmg commands here 
# $ogmgMaxIterations : max. number of iterations (default=20)
# $ogmgMaxExtraLevels : max. number of extra levels taht will be used (default=10)
# $ogmgCycle : multigrid cycle 1=V, 2=W, .. #
# $ogmgDebug :  debug parameter for Ogmg
# $ogmgOpav : operator averaging option: 0=none, 1=all, 2=Cartesian grids
# $ogmgSsr : 1=show smoothing rates
# $ogmgCoarseGridSolver : multigrid coarse grid solver
# $ogmgCoarseGridMaxIterations   :  maximum number of iterations for Ogmg coarse grid solver
# $ogmgRtolcg :  relative tolerance for multigrid Coarse Grid solve
# $ogmgAtolcg :  absolute tolerance for multigrid Coarse Grid solve
# $ogmgDebugcg : debug for coarse grid solvers
# $ogmgIlucgLevels : number of levels for ILU preconditioner
# $ogmgIlucgFill : expected fillin factor for the ILU preconditioner for the multigrid Coarse Grid solve
# $ogmgAutoChoose : automatically choose MG parameters: ogmgAutoChoose : 0=OFF, 1=ON, 2=robust
# $ogmgSaveGrid=<gridName> : optionally save the MG grid with coarser levels for use with a future run
# $ogmgReadGrid=<gridName> : optionally read the MG grid with coarser levels (saved from a previous run)
# -- defaults --
if( $ogesSolver eq "" ){ $ogesSolver="yale"; } #
if( $ogesCmd eq "" ){ $ogesCmd="#"; } #
if( $ogesRtol eq "" ){ $ogesRtol=1.e-6; } #
if( $ogesAtol eq "" ){ $ogesAtol=1.e-8; } #
if( $ogesEtol eq "" ){ $ogesEtol=1.e-3; } #
if( $ogesDtol eq "" ){ $ogesDtol=1.e5; } #
if( $ogesMaxIterations eq "" ){ $ogesMaxIterations=-1; } #
if( $ogesPc eq "ilu" ){ $ogesPc = "incomplete LU preconditioner"; }elsif( $ogesPc eq "lu" ){ $ogesPc = "lu preconditioner"; }else{ $ogesPc="#"; }
if( $ogesIluLevels eq "" ){ $ogesIluLevels=3; } #
if( $ogesDebug eq "" ){ $ogesDebug=0; } #
#
if( $ogmgMaxIterations eq "" ){ $ogmgMaxIterations=20; } #
if( $ogmgCycle eq "" ){ $ogmgCycle=1; } #
if( $ogmgCmd eq "" ){ $ogmgCmd="#"; } #
if( $ogmgDebug eq "" ){ $ogmgDebug=0; } #
if( $ogmgRtolcg eq "" ){ $ogmgRtolcg=1.e-3; } # coarse grid solve 
if( $ogmgAtolcg eq "" ){ $ogmgAtolcg=1.e-6; } #
if( $ogmgIlucgFill eq "" ){ $ogmgIlucgFill=3.; }  #
if( $ogmgOpav eq "" ){ $ogmgOpav =1; } #
if( $ogmgOpav eq "0" ){ $ogmgOpav = "do not average coarse grid equations"; }\
  elsif( $ogmgOpav eq "2" ){ $ogmgOpav ="do not average coarse curvilinear grid equations"; }else{ $ogmgOpav = "#"; }
if( $ogmgSsr eq 1 ){ $ogmgSsr="show smoothing rates"; }else{ $ogmgSsr="#"; }
if( $ogmgCoarseGridSolver eq "" ){ $ogmgCoarseGridSolver="choose best iterative solver"; }
if( $ogmgCoarseGridSolver eq "best" ){ $ogmgCoarseGridSolver="choose best iterative solver"; }
if( $ogmgCoarseGridSolver eq "AMG" ){ $ogmgCoarseGridSolver="algebraic multigrid"; }
if( $ogmgDebugcg eq "" ){ $ogmgDebugcg=0; } #
if( $ogmgAutoChoose eq "" ){ $ogmgAutoChoose=1; } #
if( $ogmgMaxExtraLevels eq "" ){ $ogmgMaxExtraLevels=10; } #
#
  $ogesSolver
 # these tolerances are chosen for PETSc
  number of incomplete LU levels
    $ogesIluLevels
  relative tolerance
    $ogesRtol
  absolute tolerance
    $ogesAtol
  maximum allowable increase in the residual
    $ogesDtol
  maximum number of iterations
    $ogesMaxIterations
  # preconditionner:
    $ogesPc
  debug
    $ogesDebug
  # Perform any user supplied Oges commands: 
  $ogesCmd
  # --- start of multigrid parameters ---
  multigrid parameters
  # -- sparse/direct solver on coarse grid: (these may be over-ridden by ogmgAutoChoose)
   Oges parameters
    $ogmgCoarseGridSolver
     relative tolerance
       $ogmgRtolcg
     absolute tolerance
       $ogmgAtolcg
# the fillin for ILU can be hard to predict: anywhere from 2 to 20. 
     incomplete LU expected fill
       $ogmgIlucgFill
   exit
   # -- automatically choose MG parameters: ogmgAutoChoose : 0=OFF, 1=ON, 2=robust
   if( $ogesSolver eq "multigrid" ){ $cmd="choose good parameters: $ogmgAutoChoose"; }else{ $cmd="#"; }
   $cmd
   # --- now over-ride automatically chosen parameters *!* ---
   maximum number of iterations
    $ogmgMaxIterations
   # --- for parallel use: 
   use new red-black smoother
   # -- trouble with RB-jacobi (works in ogmg)
   #    red black jacobi
   # show smoothing rates? : 
   $ogmgSsr
   # cycles: 1=V, 2=W
   number of cycles
     $ogmgCycle
   residual tolerance $ogesRtol
   absolute tolerance $ogesAtol
   error tolerance $ogesEtol
   $ogmgOpav
   # turn off using local omega for line solves and variable coefficients - trouble in parallel *fix me*
   do not use locally optimal line omega
   # -- "direct" solver on coarse grid -- over-ride any pars from above
   Oges parameters
     # Do not overide coarse grid solver if autoChoose!=0  *wdh* 2013/09/15: 
     # NOTE: choosing "best" here will reset ILU levels etc from auto-choose
     if( $ogmgAutoChoose eq 0 ){ $ogmgOverideCoarseGridSolver=$ogmgCoarseGridSolver; }else{ $ogmgOverideCoarseGridSolver="#"; }
     $ogmgOverideCoarseGridSolver
     # TEST: 
     # $ogmgCoarseGridSolver
     #
     if( $ogmgCoarseGridMaxIterations ne "" ){ $cmd="maximum number of iterations\n $ogmgCoarseGridMaxIterations"; }else{ $cmd="#"; }
     $cmd
     # 
     maximum allowable increase in the residual
       $ogesDtol
     # optionally over-ride number of ilu levels for coarse grid
     if( $ogmgIlucgLevels eq "" ){ $ogmgIlucgLevelsCmd="#"; }else{ $ogmgIlucgLevelsCmd="number of incomplete LU levels\n$ogmgIlucgLevels"; }
     $ogmgIlucgLevelsCmd
     # number of incomplete LU levels
     # incomplete LU expected fill
     #  $ogmgIlucgFill
     #   $ogmgILUcg=5;
     #   $ogmgILUcg
     debug
      $ogmgDebugcg
     $ogesCmd
    exit
   maximum number of interpolation iterations
        3
   maximum number of extra levels
     $ogmgMaxExtraLevels
   #      maximum number of levels
   #        5 4 3 2
   #      maximum number of extra levels
   #        4 3 2 1
    debug
     $ogmgDebug
   # optionally save the MG grid with coarser levels for use with a future run
   if( $ogmgSaveGrid ne "" ){ $cmd="save the multigrid composite grid\n $ogmgSaveGrid"; }else{ $cmd="#"; }
   $cmd
   # optionally read the MG grid with coarser levels (saved from a previous run)
   if( $ogmgReadGrid ne "" ){ $cmd="read the multigrid composite grid\n $ogmgReadGrid"; }else{ $cmd="#"; }
   $cmd
   #
   $ogmgCmd
  exit
  # --- end of multigrid parameters ---

