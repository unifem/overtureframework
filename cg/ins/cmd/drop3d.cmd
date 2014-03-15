#====================================================================================================================
# cgins: A falling sphere in a rectangular channel
#
# Usage:
#     cgins [-noplot] drop3d -g=<grid> -nu=<> -vIn=<> -ts=[pc|pc4|im|afs]   -project=[0|1] -freqFullUpdate=<>  ...
#           -go=[go|halt|og]
#
# Examples: *NOTE* gravity is in cm/s^2 to match Pan et.al.
#    ogen -noplot drop3d -factor=1 -ml=1
#    cgins drop3d -g=drop3di1.order2.ml1 -nu=.05 -vIn=7.5 -tf=2. -tp=.1 -ts=pc -solver=mg -psolver=mg  [OK
#    cgins drop3d -g=drop3di1.order2.ml1 -nu=.05 -vIn=7.5 -tf=2. -tp=.1 -ts=im -solver=mg -psolver=mg  [OK 
#    cgins drop3d -g=drop3di1.order2.ml1 -nu=.05 -vIn=7.5 -tf=2. -tp=.1 -ts=afs -solver=mg -psolver=mg [trouble *fix me*
#
#    ogen -noplot drop3d -interp=e -factor=2 -ml=1
#    cgins drop3d -g=drop3de2.order2.ml1 -nu=.025 -vIn=7.5 -tf=2. -tp=.01 -ts=im -solver=mg -psolver=mg
#
# -- parallel:
#  mpirun -np 2 $cginsp drop3d -g=drop3de2.order2.ml1 -nu=.025 -vIn=7.5 -tf=2. -tp=.01 -ts=im -solver=mg -psolver=mg -freqFullUpdate=1 [*fix me*
#
#=========================================================================================================================
$tFinal=3.; $vIn=7.5; $tFinal=2.; $nu=.05; $tPlot=.1; $ts="pc"; $density=1.1; $debug=1; $cfl=.9; 
$dtMax=.05; $newts=0; $project=1; $restart=""; $go="halt";
$radius=.25;  # radius of the sphere
* $hybrid = "use hybrid grid for surface integrals";
$hybrid = "do not use hybrid grid for surface integrals";
$g = -981.; # cm/s^2   -9.81 acceleration due to gravity standard value: 9.80665 m/s^2.
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; $ad2=1; $ad21=1; $ad22=1;  $ad4=0; $ad41=2.; $ad42=2.; 
$psolver="best"; $solver="best"; 
$iluLevels=1; $ogesDebug=0; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
# -- for Kyle's AF scheme:
$afit = 20;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$pi = 4.*atan2(1.,1.);
*
# old: 
* $gridName = "drop3d.hdf"; $show="drop3d.show"; $nu=.05; 
# $gridName = "drop3d1.hdf"; $show="drop3d.show"; $vIn=7.5; $tFinal=2.; $nu=.05; $tPrint=.2; $tolerance = "1.e-6";
* $gridName = "drop3d2.hdf"; $show="drop3d2.show"; $vIn=7.5; $tFinal=2.; $nu=.025; $tolerance = "1.e-6";
* $gridName = "drop3d2.hdf"; $show="drop3d2Restart.show"; $vIn=7.5; $tFinal=2.; $tPrint=.01; $nu=.025; $tolerance = "1.e-6";
*
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"imp=f"=>\$implicitFactor,"vIn=f"=>\$vIn,"radius=f"=>\$radius,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "amp=f"=>\$amp,"freq=f"=>\$freq, "solver=s"=>\$solver, "psolver=s"=>\$psolver,\
  "xshift=f"=>\$xshift,"yshift=f"=>\$yshift,"zshift=f"=>\$zshift,"density=f"=>\$density,\
  "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42 );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $ts eq "pc4" ){ $ts="adams PC order 4"; $useNewImp=0; } # NOTE: turn off new implicit for fourth order
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;  $useNewImp=0;}
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
  $grid
  incompressible Navier Stokes
  exit
*
  show file options
  * un-comment the next two lines to save a show file
   open
      $show
    frequency to flush
      2
  exit  
  turn off twilight zone
*
  recompute dt every 10 *  1 * 2 * 5 * 10 * 1 
*
****
  turn on moving grids
*   detect collisions 1
****
  $volume = 4./3.*$pi*$radius**3; $mass=$volume*$density;
  specify grids to move
      $hybrid
      rigid body
        density
          $density
        mass
           $mass
         moments of inertia
           $momentOfInertia=2./5.*$mass*$radius**2;
           $momentOfInertia $momentOfInertia $momentOfInertia
           * 1. 1. 1. 
         initial centre of mass
            0. 0. 0.   -.25 -.25 -.75
         * initial velocity
         done
 	# sphere1-north-pole
	# sphere1-south-pole
        choose grids by share flag
          1
      done
  done
**********
# 
  frequency for full grid gen update $freqFullUpdate
#
  implicit factor $impFactor
  dtMax $dtMax
# 
# use full implicit system 1
# use implicit time stepping
  $ts
  $newts
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
# 
  choose grids for implicit
   all=implicit
##   channel=explicit
  done
# -----------------------------------
  pde parameters
    nu  $nu 
*   -- specify the fluid density
   fluid density
     1.
*  turn on gravity
    gravity
       0. $g 0.
   #  turn on 2nd-order AD here:
   OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad21, $ad22
   OBPDE:fourth-order artificial diffusion $ad4
   OBPDE:ad41,ad42 $ad41, $ad42
#
   OBPDE:expect inflow at outflow
   done
#---------------------------------------
  boundary conditions
    all=noSlipWall, uniform(v=$vIn)
    # channel(0,1)=inflowWithVelocityGiven,  uniform(p=1,v=$vIn)
    # channel(1,1)=outflow , pressure(1.*p+0.0*p.n=0.)
    bcNumber3=inflowWithVelocityGiven,  uniform(p=1,v=$vIn)
    bcNumber4=outflow , pressure(1.*p+0.0*p.n=0.)
    done
*
  initial conditions
    uniform flow
     p=1., v=$vIn
*** ---
*   read from a show file
*    drop3d2.show
*      -1 6  -1
  exit
*
$project
*
  final time $tFinal 
  times to plot $tPlot
*
  plot and always wait
**  no plotting
*
*  initial conditions
*     read from a show file
*      twoDropNew2.show
*       24 48  -1 35 -1 1 4 -1 10 18  -1
*  exit
*
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
cfl $cfl
debug $debug
#
  continue
$go

*
* ----------- plot solution of sphere surface
    contour
      plot contours on grid boundaries
      (1,0,2) = (transform[grid=1],side,axis) (off)
      (2,0,2) = (transform[grid=2],side,axis) (off)
      exit
*      set view:0 0 0 0 1 0.827404 -0.160413 0.538211 -0.114096 0.89034 0.440768 -0.549896 -0.426101 0.718368
      exit
*-----------------------
*
$go

