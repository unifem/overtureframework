# ==================================================================
#
# Drops in a channel (falling cylinders in a channel)
# Usage:
#      ogen [-noplot] dropsGrid -order=[2|4] -interp=[e|1] -factor=<> -numDrops=[1|5] -xa=<> -xb=<> -ya=<> -yb=<> ...
#              -x1=<> -y1=<> -nr=<> -saveWeights=[0|1]
#
# ogen -noplot dropsGrid -order=2 -factor=1 
# ogen -noplot dropsGrid -order=2 -factor=2 -ml=2 
# ogen -noplot dropsGrid -order=2 -factor=4 -ml=3 
# ogen -noplot dropsGrid -order=2 -factor=8 -ml=3 
#
# ogen -noplot dropsGrid -order=4 -factor=2 -ml=2
# ogen -noplot dropsGrid -order=4 -factor=4 -ml=2 -saveWeights=0
# ogen -noplot dropsGrid -order=4 -factor=8 -ml=3 
# ogen -noplot dropsGrid -order=4 -factor=16 -ml=3 
# ogen -noplot dropsGrid -order=4 -factor=32 -ml=4 
# 
# Explicit interp:
#  ogen -noplot dropsGrid -interp=e -order=2 -factor=4 -ml=2 -saveWeights=0
#  ogen -noplot dropsGrid -interp=e -order=2 -factor=8 -ml=3 -saveWeights=0
#
# -- One drop:
#  ogen -noplot dropsGrid -order=2 -numDrops=1 -xa=-1. -xb=1. -ya=-2 -yb=1.5 -x1=0 -y1=0 -factor=2 -ml=1 -nr=7
#  ogen -noplot dropsGrid -order=2 -numDrops=1 -xa=-1. -xb=1. -ya=-1 -yb=2.5 -x1=0 -y1=0 -ml=1 -nr=7 -interp=e -factor=4 
# 
# old:
# $factor=1; $name = "drops0.hdf";   $ml=1;
# $factor=2; $name = "drops.hdf";   $ml=2;
# $factor=2; $name = "dropsi2.order4.hdf";   $ml=2; $order=4; 
#
#====================================================================
$ml=1;
$xa=-2.; $xb=2.; $ya=-4.; $yb=3.; $nrExtra=0; $loadBalance=0; $numDrops=5;
$x1=-1.25; $y1=.25;  # location of drop 1 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$saveWeights=1;   # 1 = save integration weights
$nr = 5;   # default number of points in the radial direction (altered depending on -ml)
$suffix=""; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=>\$factor,"nrExtra=i"=>\$nrExtra,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance,"nr=i"=> \$nr,"ml=i"=> \$ml,"numDrops=i"=> \$numDrops,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"x1=f"=>\$x1,"y1=f"=>\$y1,\
            "saveWeights=i"=>\$saveWeights,"suffix=s"=>\$suffix);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$prefix="dropsGrid";
if( $numDrops ne 5 ){ $prefix = "dropsGrid$numDrops"; }
$suffix .= ".order$order"; 
$name = $prefix . "$interp$factor" . $suffix . ".hdf";
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; $nrExtra=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; $nrExtra=3; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; $nrExtra=4; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
$pi=4.*atan2(1.,1.);
$ds0=.1;
$ds=$ds0/$factor;
#
#
create mappings
#
rectangle
  set corners
    # -2. 2. -4. 3. 
    $xa $xb $ya $yb 
  lines
    $nx = int(($xb-$xa)/$ds + 1.5);
    $ny = int(($yb-$ya)/$ds + 1.5);
    $nx $ny 
  boundary conditions
    1 2 3 4
  mappingName
   channel
exit
#
$innerRadius=.3; 
# $outerRadius=$innerRadius+.15; 
#
$nr = intmg( $nr + $nrExtra);
$outerRadius=$innerRadius+ ($nr-2)*$ds; 
$nTheta = intmg( 2.*$pi*.5*($innerRadius+$outerRadius)/$ds + 1.5 );
#
$dropShare=4; 
Annulus
  lines
    $nTheta $nr
  inner and outer radii
    $innerRadius $outerRadius
  centre for annulus
    $x1 $y1 
    # -1.25  .25
  boundary conditions
    -1 -1 1 0
  share
     $dropShare=$dropShare+1;
     0  0 $dropShare 0 
  mappingName
   drop1
exit
#
#
Annulus
  lines
    $nTheta $nr 
  inner and outer radii
    $innerRadius $outerRadius 
  centre for annulus
    -.5 -.35
  boundary conditions
    -1 -1 1 0
  share
     $dropShare=$dropShare+1;
     0  0 $dropShare 0 
  mappingName
   drop2
exit
#
#
Annulus
  lines
    $nTheta $nr 
  inner and outer radii
    $innerRadius $outerRadius 
  centre for annulus
    .4  .0 
  boundary conditions
    -1 -1 1 0
  share
     $dropShare=$dropShare+1;
     0  0 $dropShare 0 
  mappingName
   drop3
exit
#
Annulus
  lines
    $nTheta $nr
  inner and outer radii
    $innerRadius $outerRadius  
  centre for annulus
    1.25  .25   
  boundary conditions
    -1 -1 1 0
  share
    $dropShare=$dropShare+1;
     0  0 $dropShare 0 
  mappingName
   drop4
exit
#
Annulus
  lines
    $nTheta $nr
  inner and outer radii
    $innerRadius $outerRadius  
  centre for annulus
    1. -.75   * 1. -1.
  boundary conditions
    -1 -1 1 0
  share
    $dropShare=$dropShare+1;
     0  0 $dropShare 0 
  mappingName
   drop5
exit
#
exit
generate an overlapping grid
    channel
    if( $numDrops eq 1 ){ $grids = "drop1"; }\
    elsif( $numDrops eq 2 ){ $grids = "drop1\n drop2"; }\
    elsif( $numDrops eq 3 ){ $grids = "drop1\n drop2\n drop3"; }\
    elsif( $numDrops eq 4 ){ $grids = "drop1\n drop2\n drop3\n drop4"; }\
    else{ $grids = "drop1\n drop2\n drop3\n drop4\n drop5"; }
    $grids
  done
  change parameters
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#  display intermediate results
  compute overlap
#  pause
  exit
#
save integration weights $saveWeights
save an overlapping grid
$name
drops
exit

