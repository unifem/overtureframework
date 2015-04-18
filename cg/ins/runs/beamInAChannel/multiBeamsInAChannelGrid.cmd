#
# Create the initial grid for a deforming beam in a channel
# Use this grid with cgins or cgmp for a fluid-structure example.
#
# Usage:
#         ogen [-noplot] multiBeamsInAChannelGrid [options]
# where options are
#     -factor=<num>     : grid spacing is .1 divided by this factor
#     -interp=[e/i]     : implicit or explicit interpolation
#     -name=<string>    : over-ride the default name  
#     -case=[inner|outer] : only build a grid for the inner or outer domain
#     -nExtra          : add extra lines in the normal direction on the boundary fitted grids
#     -ae=<>, -be=<>   : scale factors for major and minor axes (default 1 for a circle)
#
# Examples:
#
#      ogen -noplot multiBeamsInAChannelGrid -interp=e -factor=2 
#      ogen -noplot multiBeamsInAChannelGrid -interp=e -factor=4 
#      ogen -noplot multiBeamsInAChannelGrid -interp=e -factor=8
#      ogen -noplot multiBeamsInAChannelGrid -interp=e -factor=16 
#      ogen -noplot multiBeamsInAChannelGrid -interp=e -factor=32
#
$beamLength=1.; $beamThickness=.2; 
$factor=1; $name=""; $case=""; 
$factor2=-1;   # by default factor2=factor
$interp="i"; $interpType = "implicit for all grids"; 
$order=2; $orderOfAccuracy = "second order"; $ng=2; $ml=0;
$xa=-1.; $xb=6.; $ya=0; $yb=1.5; $nExtra=0; 
$refineInner=0; $refineOuter=0; $fixedRadius=-1; 
#
$xoffset2=2.;   # offset for beam 2
# 
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,"case=s"=> \$case,\
           "xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,"nExtra=i"=>\$nExtra,"factor2=f"=> \$factor2,\
           "refineInner=i"=>\$refineInner,"refineOuter=i"=>\$refineOuter,"fixedRadius=f"=>\$fixedRadius,\
           "beamLength=f"=>\$beamLength,"beamThickness=f"=>\$beamThickness,"ml=i"=>\$ml );
#
if( $factor2 < 0 ){ $factor2=$factor; }
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
$suffix = ".order$order"; 
$prefix = "multiBeamsInAChannelGrid"; 
if( $fixedRadius ne -1 ){ $prefix .= "Fixed"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
#
$bcInterface0=100;  # bc for interfaces
$bcInterface1=101;  
$shareInterface=100;        # share value for interfaces
#
$Pi=4.*atan2(1.,1.);
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
$ds0 = .1; 
# target grid spacing:
$ds = $ds0/$factor;
$ds2 = $ds0/$factor2;
#
# width of the hyperbolic grid (fixed width)
$fixedWidth=.25;
# 
create mappings
#
  rectangle
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    boundary conditions
      1 2 3 4
    share
      0 0 3 0 
    mappingName
      channel
    exit
#
# -------------- BEAM 1 -------------------
#
# Create a start curve for the interface
#
SmoothedPolygon
* start on a side so that the polygon is symmetric
  vertices 
    $w=$beamThickness*.5; # beam half thickness
    $y2=$beamLength*1.1;  # increase beam length a bit since smoothed polygon may shorten
    $y1=.5*($ya+$y2); 
    6
     $w $ya
     $w $y1
     $w $y2
    -$w $y2
    -$w $y1
    -$w $ya 
  $tStretch=2.; 
  t-stretch
    0. 1.
    0. 1.
    1. $tStretch
    1. $tStretch
    0. 1.
    0. 1.
  # set sharpness of corners
  $sharp=10.; 
  sharpness
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
  # 
  curve or area (toggle)
  mappingName
    beamBoundary1
exit
# 
  hyperbolic
 # add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $nr = intmg( 7 );  # number of lines in the radial direction
    $dist = ($nr-3)*$ds;
    $ns = int( (2.*$beamLength+$beamThickness)*$stretchFactor/$ds +1.5 );
    if( $fixedRadius ne -1 ){ $dist=$fixedWidth; $nr = int( $dist/$ds + 2.5 ); }
    points on initial curve $ns
    distance to march $dist 
    lines to march $nr
    # 
    BC: left fix y, float x and z
    BC: right fix y, float x and z
    # 
    spacing: geometric
    geometric stretch factor 1.05
    generate
    # 
    boundary conditions
       3  3  5 0 
    share
       3  3  100 0 
    # -- set the order of data point interpolation: 
    fourth order
    # second order
    name beam1
 # pause
    exit
#
#
# -------------- BEAM 2 -------------------
#
# Create a start curve for the interface
#
SmoothedPolygon
* start on a side so that the polygon is symmetric
  vertices 
    $w=$beamThickness*.5; # beam half thickness
    $x1=$xoffset2+$w;  $x2=$xoffset2-$w; 
    $y2=$beamLength*1.1;  # increase beam length a bit since smoothed polygon may shorten
    $y1=.5*($ya+$y2); 
    6
     $x1 $ya
     $x1 $y1
     $x1 $y2
     $x2 $y2
     $x2 $y1
     $x2 $ya 
  $tStretch=2.; 
  t-stretch
    0. 1.
    0. 1.
    1. $tStretch
    1. $tStretch
    0. 1.
    0. 1.
  # set sharpness of corners
  $sharp=10.; 
  sharpness
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
  # 
  curve or area (toggle)
  mappingName
    beamBoundary2
exit
# 
  hyperbolic
 # add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $nr = intmg( 7 );  # number of lines in the radial direction
    $dist = ($nr-3)*$ds;
    $ns = int( (2.*$beamLength+$beamThickness)*$stretchFactor/$ds +1.5 );
    if( $fixedRadius ne -1 ){ $dist=$fixedWidth; $nr = int( $dist/$ds + 2.5 ); }
    points on initial curve $ns
    distance to march $dist 
    lines to march $nr
    # 
    BC: left fix y, float x and z
    BC: right fix y, float x and z
    # 
    spacing: geometric
    geometric stretch factor 1.05
    generate
    # 
    boundary conditions
       3  3  5 0 
    share
       3  3  101 0 
    # -- set the order of data point interpolation: 
    fourth order
    # second order
    name beam2
 # pause
    exit
#
  exit this menu
#
generate an overlapping grid
  channel
  beam1
  beam2
  done choosing mappings
# 
  change parameters 
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit 
#
# open graphics
  compute overlap
#
exit
#
save an overlapping grid
  $name
  multiBeamsInAChannelGrid
exit