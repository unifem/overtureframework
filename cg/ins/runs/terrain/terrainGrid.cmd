# Ogen command file: Make a 3D grid over terrain.
#
# Usage: ogen [-noplot] terrainGrid -site=<s> -prefix=<> -topLevel=<f> -midLevel=<> ...
#                      -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> -useNurbs=[0|1]
# 
#  NOTE 1: The input data file for the terrain surface comes from the matlab file: readTerrain.m 
#  NOTE 2: The input data file for the terrain surface holds the max and min values of the terrain.
#
# Examples:
#  ogen -noplot terrainGrid -interp=e -site=AltamontPass.dat -prefix=AltamontPass -factor=1 [
#  ogen -noplot terrainGrid -interp=e -site=site300.dat -prefix=site300 -factor=1 [250K pts
#
# Multigrid: (probably best to only increase the number of levels slowly)
#  ogen -noplot terrainGrid -interp=e -site=site300.dat -prefix=site300 -factor=2 -ml=2 [1.7M  pts
#
# -- Fourth-order:
#  ogen -noplot terrainGrid -interp=e -site=site300.dat -prefix=site300 -order=4 -factor=2 -ml=2
#
#  Parameters: 
$topLevel=800.;    # Here is the upper bound for the atmosphere grid -- this can be changed
$midLevel=400;      # Here is the top of the finer background grid near the surface
# 
$site="site300.dat"; # name of data file with surface
$prefix="site300"; 
#---------------------------------------------------------------------------------------------
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; 
$useNurbs = 1;   # 1 : build Nurbs mappings for wing grids, 0: use hyperbolic grids
#-----------------------------------------------------------------
$lengthScale=1000.; # geometry has a size on the order of 10^3 m
#-----------------------------------------------------------------
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"topLevel=f"=>\$topLevel,"midLevel=f"=> \$midLevel,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"useNurbs=i"=>\$useNurbs,\
            "site=s"=>\$site, "prefix=s"=>\$prefix );
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
$degree=2; # degree of the nurbs
nurbs (surface)
  set domain dimension
    2
  set range dimension
    3
  enter points
   # Include the file created by the matlab script readTerrain.m
   # NOTE: this file holds the bounds for the background grid
   #
   include $site
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
  builder
    add surface grid
    surface
    # target grid spacing $ds $ds (tang,norm)((<0 : use default)
    create volume grid...
     BC: left fix x, float y and z
     BC: right fix x, float y and z
     BC: bottom fix y, float x and z
     BC: top fix y, float x and z
     # -- GHOST points go bad at lower-right corner, this seems to help:
     apply boundary conditions to start curve 1
     # 
     $terrainFactor=1.25; # account for extra domain size that includes the terrain
     $nx = intmg( $terrainFactor*($xb-$xa)/$ds + 1.5 );
     $ny = intmg( $terrainFactor*($yb-$ya)/$ds + 1.5 );
     # points on initial curve 183, 145
     points on initial curve $nx, $ny
     boundary conditions
       1 2 3 4 5 0
     share
       1 2 3 4 0 0
     # We cannot use the boundary offset to shift the ghost points
     # since the resulting boundary faces will not flat. 
     boundary offset 0 0 0 0 0 1 (l r b t b f)
     # ---New 2011/10/03
     normal blending 7, 7, 7, 7 (lines, left,right,bottom,top)
     # I think we need to increase the number of volume smooths as we make the grid finer
     # ---New 2011/10/03
     $volSmooths=100*$factor; 
     volume smooths $volSmooths
     ## volume smooths 200
     $linesToMarch = intmg( 17 )-1 +1;
     lines to march $linesToMarch
     # $dist = 75.*$scale;
     # make the grid spacing finer in the normal direction -- we will later adjust this
     # when we stretch the grid
     $dist = .5*$linesToMarch*$ds; 
     distance to march $dist
     generate     
     # ---New 2011/10/03
     ## fourth order
     mappingName terrain-unstretched
   # pause
   exit
#
  exit
# -- convert to a nurbs 
#-   nurbs (surface)
#-     interpolate from mapping with options
#-     terrain-unstretched
#-     parameterize by index (uniform)
#-     choose degree
#-       2
#-     done
#-     mappingName
#-     terrain-nurbs
#-  exit
#
# -- stretch the grid lines in the normal direction.
#
  stretch coordinates
    transform which mapping?
#
    terrain-unstretched
    #- terrain-nurbs
#
    Stretch r3:exp to linear
    STRT:multigrid levels $ml
    # Transition the grid spacing to the outer box spacing:
    STP:stretch r3 expl: min dx, max dx $dsNormal $ds
    # stretch grid
    STRT:name terrain
  #  pause
  exit
#
#  Finer refinement patch near the surface
#
# $midLevel : the refinement level goes to this height above the lowest point on the terrain
# How about: $midLevel = $yMax + ($yMax-$yMin); 
$xas=$xa; $xbs=$xb; $yas=$ya; $ybs=$yb;
$zas=$za + $dist - $ng*$ds;
$zbs=$midLevel+$zMin;
box
  set corners
    $xas $xbs $yas $ybs $zas $zbs 
  lines
    $nx = intmg( ($xbs-$xas)/$ds +1.5 ); 
    $ny = intmg( ($ybs-$yas)/$ds +1.5 ); 
    $nz = intmg( ($zbs-$zas)/$ds +1.5 ); 
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
$xac=$xa; $xbc=$xb; $yac=$ya; $ybc=$yb; 
$zac=$zbs -2.*$dsCoarse;  # coarse starts just below nearSurfaceBackGround
$zbc=$topLevel+$zMin;     # coarse grid reaches a height of topLevel above the loweset surface point
box
  set corners
    $xac $xbc $yac $ybc $zac $zbc 
  lines
    $nx = intmg( ($xbc-$xac)/$dsCoarse +1.5 ); 
    $ny = intmg( ($ybc-$yac)/$dsCoarse +1.5 ); 
    $nz = intmg( ($zbc-$zac)/$dsCoarse +1.5 ); 
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
terrain
exit
