#
#  Hyperbolic grid for a cylinder -- test high-order accurate grids
#  with multiple ghost points on the surface
#
# Examples:
# -- order=2: 
#     ogen -noplot hypeCyl -interp=e -factor=1 
# 
#
# -- order=4: 
#     ogen -noplot hypeCyl -interp=e -order=4 -factor=1 
#     ogen -noplot hypeCyl -interp=e -order=4 -factor=2
# 
# -- order=6:
#     ogen -noplot hypeCyl -interp=e -order=6 -factor=1  [OK 
#     ogen -noplot hypeCyl -interp=e -order=6 -factor=2  [OK 
#
# -- order=8:
#     ogen -noplot hypeCyl -interp=e -order=8 -factor=2  [OK 
#     ogen -noplot hypeCyl -interp=e -order=8 -factor=4  [OK 
#
#
$xa=-2.5; $xb=2.5; $ya=-2.5; $yb=2.5; $za=-1.; $zb=1.; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=3; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=4; }
if( $interp eq "i" ){ $interpType = "implicit for all grids"; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
$name = "hypeCyl" . "$factor" . $suffix . ".hdf";
# 
$ds=.1/$factor; 
$pi=4.*atan2(1.,1.);
$numGhost=$order/2; # number of ghost points we need (boundary offset)
$extraLines=$numGhost+1; # extra lines needed on surface grids so they overlap enough
# add extra overlap between grids for explicit interp: (is this the correct number to add?)
if( $interp eq "e" ){ $extraLines=$extraLines+$numGhost; }
#
$nr=9 + $order; # lines to march in the normal directio
# 
create mappings
# 
  Cylinder 
    surface or volume (toggle) 
    bounds on the axial variable
      $za $zb
    lines 
      201 11 
    exit 
  builder 
#   -- surface grid 1 : left side --
    create surface grid... 
      target grid spacing $ds $ds (tang,normal, <0 : use default)
      surface grid options... 
      initial curve:coordinate line 0 
      choose point on surface 0 0. -1.  -.25 .75 .5
      done 
      # -- do NOT set the boundary offset on surface grids --
      boundary offset 0 0 0 0  (l r b t)
      $dist = $pi;
      # march an extra numGhost in both directions:
      $lines = int( $dist/$ds + $extraLines + 1.5 );
      forward and backward
      lines to march $lines $extraLines 
      generate
      name cylSurf1
    exit
#   -- volume grid 1 : left side --
    create volume grid...
      BC: left fix z, float x and y
      BC: right fix z, float x and y
      BC: top outward splay
      BC: bottom outward splay
      target grid spacing $ds $ds (tang,normal, <0 : use default)
      backward
      outward splay .1 .1 .1 .1 (left,right,bottom,top for outward splay BC)
      lines to march $nr 
      generate
      # -- DO set the boundary offset on volume grids --
      boundary offset 0, 0, $numGhost,  $numGhost, 0, 0 (l r b t b f)
      boundary conditions
        5 6 0 0 7 0 
      share
        5 6 0 0 7 0
      # ghost lines to plot: 2
        name cylVol1a
      exit
#    -- surface grid 2 : right side ---
    create surface grid... 
      target grid spacing $ds $ds (tang,normal, <0 : use default)
      surface grid options... 
      initial curve:coordinate line 0 
      choose point on surface 0 0. -1.  -.25 .75 .5
      done 
      forward and backward
      # -- do NOT set the boundary offset on surface grids --
      boundary offset 0 0 0 0   (l r b t)
      $dist = $pi;
      # march an extra numGhost in both directions:
      $lines = int( $dist/$ds + $extraLines + 1.5 );
      lines to march $extraLines $lines
      generate
      name cylSurf2
    exit
#   -- volume grid 2 : right side --
    create volume grid...
      BC: left fix z, float x and y
      BC: right fix z, float x and y
      BC: top outward splay
      BC: bottom outward splay
      target grid spacing $ds $ds (tang,normal, <0 : use default)
      backward
      outward splay .1 .1 .1 .1 (left,right,bottom,top for outward splay BC)
      lines to march $nr 
      generate
      # -- DO set the boundary offset on volume grids --
      boundary offset 0, 0, $numGhost,  $numGhost, 0, 0 (l r b t b f)
      boundary conditions
        5 6 0 0 7 0 
      share
        5 6 0 0 7 0
      # ghost lines to plot: 2
        name cylVol2a
      exit
    exit
#
# Here is the back ground grid 
#
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    0 0 0 0 5 6
  mappingName
    backGround
  exit
#
# Define a perl subroutine to convert a Mapping to a Nurbs Mapping
# NOTES:
#   - the nurbs mapping will provide more accurate interpolation coefficients when ogen computes them 
#   - include the ghost points so that these remain on the surface
# 
# degree: degree of the Nurbs. We may to increase this for 6th order (but may be dangerous for 
#   surfaces with sharp corners since there may be some wiggles introduced.)
$degree=3; 
# $degree=1; $numGhost=0; # test 
#
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . \
              "number of ghost points to include\n $numGhost \n choose degree\n $degree\n done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
# 
convertToNurbs("cylVol1a","cylVol1",0.);
$commands
convertToNurbs("cylVol2a","cylVol2",0.);
$commands
# 
exit
#
  generate an overlapping grid
    backGround
    cylVol1
    cylVol2
  done
  change parameters
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng
    order of accuracy
     $orderOfAccuracy  
  exit
#  open graphics
  compute overlap
# 
  exit
#
save an overlapping grid
$name
hypeCyl
exit












  Cylinder
    surface or volume (toggle)
    lines
      21 5 
    exit
  hyperbolic
    lines to march
      3
    generate

