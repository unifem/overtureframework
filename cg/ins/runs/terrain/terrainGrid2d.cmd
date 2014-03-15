# Ogen command file: Make a 2D grid over terrain.
#
# Usage: ogen [-noplot] terrainGrid2d -site=<s> -prefix=<> -topLevel=<f> -midLevel=<f> ...
#                      -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> -useNurbs=[0|1]
#
#  -site : name of the data file from readTerrain.m
#  -prefix : name for the grid.
#  -midLevel : height of finer background grid above the surface
#  -topLevel : height of coarser background grid above the surface
#
#  NOTE: The input data file for the terrain curve comes from the matlab file: readTerrain.m 
#  NOTE: The input data file for the terrain curve holds the max and min values of the terrain.
#
# examples:
#   ogen -noplot terrainGrid2d -site=AltamontPassy50.dat -prefix=AltamontPass2d -interp=e -factor=1 
#
# Multigrid:
#   ogen -noplot terrainGrid2d -site=AltamontPassy50.dat -prefix=AltamontPass2d -interp=e -factor=1 -ml=1
#   ogen -noplot terrainGrid2d -site=AltamontPassy50.dat -prefix=AltamontPass2d -interp=e -factor=2 -ml=2
#
# -- Fourth-order:
#   ogen -noplot terrainGrid2d -site=AltamontPassy50.dat -prefix=AltamontPass2d -interp=e -order=4 -factor=1 -ml=1
#   ogen -noplot terrainGrid2d -site=AltamontPassy50.dat -prefix=AltamontPass2d -interp=e -order=4 -factor=2 -ml=2
# 
#----------------------------------------------------------------------------------------------
#  Parameters: 
$topLevel=700.;    # Here is the upper bound for the atmosphere grid -- this can be changed
$midLevel=300;      # Here is the top of the finer background grid near the surface
#---------------------------------------------------------------------------------------------
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name="";
$useNurbs = 1;   # 1 : build Nurbs mappings for wing grids, 0: use hyperbolic grids
$site = "AltamontPassy50.dat"; 
$prefix = "AltamontPass2d"; 
# NOTE: the bounds on the terrain are in the Nurbs data file.
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"topLevel=f"=> \$topLevel,"midLevel=f"=> \$midLevel,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"useNurbs=i"=>\$useNurbs,\
            "site=s"=>\$site, "prefix=s"=>\$prefix );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
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
create mappings
#
$degree=3; 
nurbs
  set domain dimension
    1
  set range dimension
    2
  enter points
   # Include the file created by the matlab script readTerrain.m
   include $site
 # We could scale the geometry here 
 #  scale
 #   $scale $scale $scale
 # -- make the actual domain a bit smaller so that ghost points will lie on
 #    the original surface.
 # restrict the domain
 #  $delta=.025;
 #  $ra=$delta; $rb=1.-$delta;
 #  $ra $rb 
 mappingName
  surface
exit
# NOTE: the bounds $xMin, $xMax, $yMix, $yMax on the terrain are defined in the Nurbs data file above.
  $xa=$xMin; $xb=$xMax; $ya=$yMin; 
#
  hyperbolic
     backward
     BC: left fix x, float y and z
     BC: right fix x, float y and z
     # -- GHOST points go bad at lower-right corner, this seems to help:
     apply boundary conditions to start curve 1
     $terrainFactor=1.25; # account for extra domain length that includes the terrain
     $nx = intmg( $terrainFactor*($xb-$xa)/$ds + 1.5 );
     points on initial curve $nx
     boundary conditions
       1 2 3 0
     share
       1 2 3 0
     # We cannot use the boundary offset to shift the ghost points
     # since the resulting boundary faces will not flat. 
     boundary offset 0 0 0 1 0 0  (l r b t b f)
     # I think we need to increase the number of volume smooths as we make the grid finer
     $volSmooths=50*$factor; 
     # volume smooths $volSmooths
     volume smooths 50 
     $linesToMarch = intmg( 17 )-1 +1;
     lines to march $linesToMarch
     # make the grid spacing finer in the normal direction -- we will later adjust this
     # when we stretch the grid
     $dist = .5*$linesToMarch*$ds; 
     distance to march $dist
     generate     
     mappingName terrain-unstretched
  exit
# -- convert to a nurbs 
   nurbs (surface)
     interpolate from mapping with options
     terrain-unstretched
     parameterize by index (uniform)
     choose degree
       2
     done
     mappingName
     terrain-nurbs
  exit
#
#
# -- stretch the grid lines in the normal direction.
#
  stretch coordinates
    transform which mapping?
    if( $useNurbs eq 0 ){ $mapToStretch="terrain-unstretched"; }else{ $mapToStretch="terrain-nurbs"; }
    $mapToStretch
    Stretch r2:exp to linear
    STRT:multigrid levels $ml
    # Transition the grid spacing to the outer box spacing:
    STP:stretch r2 expl: min dx, max dx $dsNormal $ds
    # stretch grid
    STRT:name terrain
  exit
#
#  Finer refinement patch near the surface
#
# $midLevel : the refinement level goes to this height above the lowest point on the terrain
# How about: $midLevel = $yMax + ($yMax-$yMin); 
$xas=$xa; $xbs=$xb; 
$yas=$ya + $dist - $ng*$ds;
$ybs=$midLevel+$yMin;
rectangle
  set corners
    $xa $xb $yas $ybs
  lines
    $nx = intmg( ($xbs-$xas)/$ds +1.5 ); 
    $ny = intmg( ($ybs-$yas)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 0 0 
  share
    1 2 0 0 
  mappingName
    nearSurfaceBackGround
exit
#
#  Coarser background grid 
#
$dsCoarse=2.*$ds;  # coarsen grid by this amount
$xac=$xa; $xbc=$xb;
$yac=$ybs -2.*$dsCoarse;
$ybc=$topLevel+$yMin; 
rectangle
  set corners
    $xa $xb $yac $ybc
  lines
    $nx = intmg( ($xbc-$xac)/$dsCoarse +1.5 ); 
    $ny = intmg( ($ybc-$yac)/$dsCoarse +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 0 4 
  share
    1 2 0 0 
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
#  open graphics
#
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
terrainGrid2d
exit
