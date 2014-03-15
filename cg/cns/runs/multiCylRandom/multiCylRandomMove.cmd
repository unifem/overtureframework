#================================================================================
# cgcns example: shock hitting a collection of cylinders of random size
#
# Usage:  cgcns multiCylRandomMove -g=<gridName> -tp=<real> -tf=<real> -show=<name> -restart=<name> -l=<int> -r=<int>
#
# -l = number of refinement levels (1=none)
# -r = refinement ratio (2 or 4)
# -tol= error tolerance for AMR
# 
# Examples:
#
# -- Grids:
#   ogen -noplot multiCylRandomGrid -factor=2 -name=multiCylRandom2.order2.hdf
#   ogen -noplot multiCylRandomGrid -factor=4 -name=multiCylRandom4.order2.hdf
#   ogen -noplot multiCylRandomGrid -factor=8 -name=multiCylRandom8.order2.hdf
#
# -- Runs: 
#  cgcns -noplot multiCylRandomMove -g=multiCylRandom2.order2.hdf -tf=4. -tp=.05 -show="multiCylRandom2.show" -go=og
#  cgcns -noplot multiCylRandomMove -g=multiCylRandom4.order2.hdf -tf=4. -tp=.05 -show="multiCylRandom4.show" -go=og
#
# -- Restart:
#  cgcns -noplot multiCylRandomMove -g=multiCylRandom2.order2.hdf -tf=4. -tp=.05 -restart="multiCylRandom2.show" -go=og
#
#=============================================================
#
* --- set default values for parameters ---
$grid="multiCylRandom4.order2.hdf"; $show = " "; $backGround="square"; $cnsVariation="godunov"; 
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tFinal=2.; $tPlot=.1; $cfl=1.; $debug=1; $tol=.05; $x0=.5; $dtMax=1.e10; $nbz=2; 
$xStep="x=-1.5"; $go="halt"; $restart=""; 
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal,"debug=i"=> \$debug, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "cnsVariation=s"=>\$cnsVariation,"restart=s"=>\$restart,"tol=f"=>\$tol );
# -------------------------------------------------------------------------------------------------
if( $cnsVariation eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $cnsVariation eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $cnsVariation eq "nonconservative" ){ $pdeVariation="compressible Navier Stokes (non-conservative)";}  #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* Here is the overlapping grid to use:
$grid 
#
  $pdeVariation
#   one step
  exit
  turn off twilight
#
#  do not use iterative implicit interpolation
#
 final time $tFinal
 times to plot $tPlot
 no plotting
#
  show file options
    compressed
    open
      $show
    frequency to flush
      2
    exit
 # no plotting
#****************************
 # There can be trouble if the grid moves too fast
  turn on moving grids
#
  detect collisions 1
  minimum separation for collisions 3.
#
  $density = 2.;  $pi=3.141592653;
#
  specify grids to move
      rigid body
        $radius=.125; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass
        moments of inertia
          1.
        done
        annulus1
       done
#
      rigid body
        $radius=.1; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass 
        moments of inertia
          1.
        done
        annulus2
       done
#
      rigid body
        $radius=.0625; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass 
        moments of inertia
          1.
        done
        annulus3
       done
#
      rigid body
        $radius=.25; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass
        moments of inertia
          1.
        done
        annulus4
       done
#
      rigid body
        $radius=.2; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass 
        moments of inertia
          1.
        done
        annulus5
       done
#
      rigid body
        $radius=.0625; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass 
        moments of inertia
          1.
        done
        annulus6
       done
#
      rigid body
        $radius=.175; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass 
        moments of inertia
          1.
        done
        annulus7
       done
#
      rigid body
        $radius=.15; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass 
        moments of inertia
          1.
        done
        annulus8
       done
#
      rigid body
        $radius=.125; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass 
        moments of inertia
          1.
        done
        annulus9
       done
#
      rigid body
        $radius=.13; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass 
        moments of inertia
          1.
        done
        annulus10
       done
#
      rigid body
        $radius=.19; $mass=$density*$radius*$radius*$pi; 
        mass
          $mass 
        moments of inertia
          1.
        done
        annulus11
       done
  done
# -- Euler equations only need 2-pt interpolation for 2nd-order accuracy:
  reduce interpolation width
    2
  maximum number of iterations for implicit interpolation
    10
#
  turn on adaptive grids
*   save error function to the show file
**  show amr error function 1
  order of AMR interpolation
      2
  error threshold
     $tol 
  regrid frequency
    $regrid=$nbz*$ratio;
    $regrid
  change error estimator parameters
    default number of smooths
      1
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $nrl
    number of buffer zones
      $nbz
*    width of proper nesting 
*      1
    grid efficiency
      .7
  exit
#-
#-  turn on adaptive grids
#-  order of AMR interpolation
#-      2
#-  error threshold
#-      .0005
#-  regrid frequency
#-      8
#-  change error estimator parameters
#-    set scale factors
#-      1 10000 10000 10000 10000
#-    done
#-    weight for first difference
#-    0.
#-    weight for second difference
#-    .03
#-    exit
#-    truncation error coefficient
#-    1.
#-    show amr error function
#-  change adaptive grid parameters
#-    refinement ratio
#-      4
#-    default number of refinement levels
#-      2
#-    number of buffer zones
#-      2
#-    grid efficiency
#-      .7
#-  exit
#****
  boundary conditions
    all=slipWall
    backGround(0,0)=superSonicInflow uniform(r=2.6069,T=.943011,u=0.694444,v=0.,s=0.0)
    done
#
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
#**************
#  cfl
#   .5 
#    .95
#  OBPDE:exact Riemann solver
# OBPDE:Roe Riemann solver
# OBPDE:HLL Riemann solver
#  OBPDE:Godunov order of accuracy 2
#*****************
#  debug
#    1
#
if( $restart eq "" ){ $cmds = "step function\n x=-1.5\n T=.943011, u=.694444, v=0., r=2.6069\n T=.714286, u=0., v=0., r=1.4"; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
   # step function
   # x=-1.5
   # T=.943011, u=.694444, v=0., r=2.6069
   # T=.714286, u=0., v=0., r=1.4
   $cmds
  continue
continue
  contour
    vertical scale factor 0.
    exit
  grid
    plot grid lines 0
    plot non-physical boundaries 1
    colour boundaries by refinement level number
  exit this menu
#
$go
