#
#   Make a grid for site300 terrain
#
#  NOTE: The input data file for terrain curve comes from the matlab file: readSite300.m 
#  NOTE: The input data file for terrain curve holds the max and min values of the terrain.
#
# usage: ogen [noplot] site300Grid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> -useNurbs=[0|1] ...
#             -saveSurface=[0|1] -readSurface=[0|1] -surfaceFileName=<fileName> 
# 
# Parameters:
#   -saveSurface : 1 = compute and save the surface grid to a data-base file
#   -readSurface : 1 = read the surface grid from a data-base file $surface
#   -surfaceFileName : name of the surface grid data-base file. This name is given a default value
#                      based on the current options.
#
# examples:
#     ogen -noplot site300Grid -interp=e -factor=1 [ Pts=241K
#     ogen -noplot site300Grid -interp=e -factor=2 [ Pts=1.5M
#     ogen -noplot site300Grid -interp=e -factor=4 [ Pts=10M
#     ogen -noplot site300Grid -interp=e -factor=8 [ Pts=102M  ?
#
# Multigrid: (probably best to only increase the number of levels slowly)
#     ogen -noplot site300Grid -interp=e -factor=1 -ml=1 [
#     ogen -noplot site300Grid -interp=e -factor=2 -ml=1 [
#     ogen -noplot site300Grid -interp=e -factor=4 -ml=2 [
#     ogen -noplot site300Grid -interp=e -factor=8 -ml=3 [Pts=79M
#
# -- Fourth-order:
#   ogen -noplot site300Grid -interp=e -order=4 -factor=2 -ml=1  [1.2M pts
#   ogen -noplot site300Grid -interp=e -order=4 -factor=3 -ml=1 
#   ogen -noplot site300Grid -interp=e -order=4 -factor=4 -ml=1  [11M
#   ogen -noplot site300Grid -interp=e -order=4 -factor=4 -ml=2
#   ogen -noplot site300Grid -interp=e -order=4 -factor=8 -ml=3
# 
#   -- save surface grid to a file, then read back in and compute in parallel (hype is not parallel)
#    ogen -noplot site300Grid -saveSurface=1 -interp=e -order=4 -factor=8 -ml=3
#    srun -N4 -n16 -ppdebug $ogenp -noplot site300Grid -readSurface=1 -interp=e -order=4 -factor=8 -ml=3
# 
# === smaller central patch ===
#   ogen -noplot site300Grid -site="site300CentralPatch.dat" -prefix="site300CentralPatch" -interp=e -factor=1  [OK
#   ogen -noplot site300Grid -site="site300CentralPatch.dat" -prefix="site300CentralPatch" -interp=e -factor=2 -ml=1 
#   ogen -noplot site300Grid -site="site300CentralPatch.dat" -prefix="site300CentralPatch" -interp=e -factor=4 -ml=2
#
#  -- fourth-order:
#   ogen -noplot site300Grid -site="site300CentralPatch.dat" -prefix="site300CentralPatch" -interp=e -order=4 -factor=2 -ml=1
#   ogen -noplot site300Grid -site="site300CentralPatch.dat" -prefix="site300CentralPatch" -interp=e -order=4 -factor=4 -ml=2
#   ogen -noplot site300Grid -site="site300CentralPatch.dat" -prefix="site300CentralPatch" -interp=e -order=4 -factor=8 -ml=3 [4.2M
#   ogen -noplot site300Grid -site="site300CentralPatch.dat" -prefix="site300CentralPatch" -interp=e -order=4 -factor=16 -ml=4 [
#
# ==== lower left quadrant: 
#   ogen -noplot site300Grid -site="site300Quadrant11.dat" -prefix="site300Quadrant11" -interp=e -factor=2 -ml=1
#   ogen -noplot site300Grid -site="site300Quadrant11.dat" -prefix="site300Quadrant11" -interp=e -factor=4 -ml=2
#   ogen -noplot site300Grid -site="site300Quadrant11.dat" -prefix="site300Quadrant11" -interp=e -factor=8 -ml=3
# 
#   ogen -noplot site300Grid -site="site300Quadrant11.dat" -prefix="site300Quadrant11" -interp=e -order=4 -factor=2 -ml=1
#   ogen -noplot site300Grid -site="site300Quadrant11.dat" -prefix="site300Quadrant11" -interp=e -order=4 -factor=4 -ml=2 [3M pts
#
#   -- save surface grid to a file, then read back in and compute in parallel (hype is not parallel)
#    ogen -noplot site300Grid -site="site300Quadrant11.dat" -saveSurface=1 -interp=e -order=4 -factor=2 -ml=1 
#    mpirun -np 2 $ogenp -noplot site300Grid -site="site300Quadrant11.dat" -readSurface=1 -interp=e -order=4 -factor=2 -ml=1
#
#----------------------------------------------------------------------------------------------
#  Parameters: 
$topLevel=1000.;    # Here is the upper bound for the atmosphere grid -- this can be changed
$site="site300Full.dat"; # name of data file with surface
$prefix="site300"; 
$readSurface=0;
$saveSurface=0;
$surfaceFileName="site300SurfaceGrid"; 
#---------------------------------------------------------------------------------------------
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; 
$useNurbs = 1;   # 1 : build Nurbs mappings for wing grids, 0: use hyperbolic grids
#-----------------------------------------------------------------
$lengthScale=1000.; # geometry has a size on the order of 10^3 m
#-----------------------------------------------------------------
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"topLevel=f"=>\$topLevel,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"useNurbs=i"=>\$useNurbs,\
            "site=s"=>\$site, "prefix=s"=>\$prefix,"readSurface=i"=>\$readSurface,\
             "saveSurface=i"=>\$saveSurface,"surfaceFileName=s"=> \$surfaceFileName );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $useNurbs ne 1 ){ $suffix .= ".hype"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "Grid" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds0=20.;  # grid spacing for factor=1 
$ds=$ds0/$factor;
$dsNormal=$ds*.1; # make grid spacing in normal direction this amount
# 
$dw = $order+1; $iw=$order+1; 
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$pi = 4.*atan2(1.,1.);
#
$dw = $order+1; $iw=$order+1;
# parallel ghost lines: for ogen we need at least:
# #       .5*( iw -1 )   : implicit interpolation 
# #       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; }
minimum number of distributed ghost lines
$parallelGhost
#
# $scale=.001; 
$scale=1.; 
#
## require("geo_s300.p");
create mappings
#
 x-r 90
 set home
#
# NOTE: we always need to read the terrain data file since we need the bounds
#
$degree=2; # degree of the nurbs
nurbs (surface)
  set domain dimension
    2
  set range dimension
    3
  enter points
   # Include the file created by the matlab script readSite300.m
   # NOTE: this file holds the bounds for the background grid
   #
   include $ENV{CGWIND}/runs/site300/$site
   #
   # include $ENV{CGWIND}/runs/site300/site300Full.dat
 # We could scale the geometry here 
 #  scale
 #   $scale $scale $scale
 # -- make the actual domain a bit smaller so that ghost points will lie on
 #    the original surface.
 # restrict the domain
 #  $delta=.025;
 #  $ra=$delta; $rb=1.-$delta; $sa=$delta; $sb=1.-$delta;
 #  $ra $rb $sa $sb
 mappingName
  surface
  # pause
exit
# NOTE: the bounds $xMin, $xMax, $yMix, $yMax, $zMin, $zMax are defined in the Nurbs data file above.
  $xa=$xMin; $xb=$xMax; $ya=$yMin; $yb=$yMax; $za=$zMin; 
# 
if( $readSurface eq "0" ){ $cmds="include $ENV{Overture}/sampleGrids/site300SurfaceGrid.h"; }else{ $cmds="#"; }
$cmds 
#
# -- optionally save the surface grid in a data-base --
#   This allows us to read this grid back in and run 
#   ogen in parallel (The hype grid generator does not work in parallel yet).
$saveGridCmds="open a data-base\n site300SurfaceGrid.hdf\n open a new file\n put to the data-base\n terrain\n close the data-base"; 
if( $saveSurface eq "1" ){ $cmds = $saveGridCmds; }else{ $cmds="#"; }
$cmds
# 
# -- optionally read the surface grid from a file --
#
$readGridCmds ="open a data-base\n site300SurfaceGrid.hdf\n open an old file read-only\n get all mappings from the data-base\n close the data-base"; 
if( $readSurface eq "1" ){ $cmds = $readGridCmds; }else{ $cmds="#"; }
$cmds
#
#
#  Finer refinement patch near the surface
#
# $midLevel : the refinement level goes to this height
# How about: $midLevel = $yMax + ($yMax-$yMin); # doubel height of mountains
$midLevel=600; 
$zb=$midLevel; 
box
  set corners
    $xa $xb $ya $yb $za $zb 
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nz = intmg( ($zb-$za)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 0 0
  share
    1 2 3 4 0 0 
  mappingName
    nearSurfaceBackGround
exit
#
#  Coarser background grid 
#
$dsCoarse=2.*$ds;  # coarsen grid by this amount
# --- FIX ME ---
$za=$midLevel-2.*$dsCoarse;
$zb=$topLevel; 
box
  set corners
    $xa $xb $ya $yb $za $zb 
  lines
    $nx = intmg( ($xb-$xa)/$dsCoarse +1.5 ); 
    $ny = intmg( ($yb-$ya)/$dsCoarse +1.5 ); 
    $nz = intmg( ($zb-$za)/$dsCoarse +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 0 6 
  share
    1 2 3 4 0 0 
  mappingName
    backGround
exit
#
 exit this menu
#
generate an overlapping grid
  backGround
  nearSurfaceBackGround
  terrain
**  terrain-unstretched
  done choosing mappings
#
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#  display intermediate results
# open graphics
#
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
site300Terrain
exit
