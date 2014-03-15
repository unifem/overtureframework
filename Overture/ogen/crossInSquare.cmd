#
# Overlapping cross in a square for testing grids that overlap on boundaries.
#
#
# usage: ogen [noplot] crossInSquare -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> ...
#                             -xa=<> -xb=<> -ya=<> -yb=<> -gt=[c|nc]
# 
#  -ml = number of (extra) multigrid levels to support
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -gt : build grids as Cartesian if gt=c or non-Cartesian for -gt=nc (for testing curvilinear grids)
# 
# examples:
#     ogen noplot crossInSquare -order=2 -factor=1
#     ogen noplot crossInSquare -order=2 -factor=2
#     ogen noplot crossInSquare -order=2 -factor=2 -ml=2	
#     ogen noplot crossInSquare -order=2 -factor=2 -ml=2 -gt=nc	 [makes nonCrossInSquarei2.order2.ml2.hdf
# - order 4: 
#     ogen noplot crossInSquare -order=4 -factor=1
#     ogen noplot crossInSquare -order=4 -factor=2
#     ogen noplot crossInSquare -order=4 -factor=2 -ml=2
# -- non-Cartesian versions (builds "nonCrossInSquare..." )
#     ogen noplot crossInSquare -gt=nc -order=4 -factor=2 -ml=2
# 
#
$prefix="crossInSquare"; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=0.; $xb=1.; $ya=0.; $yb=1.; 
$gt="c"; # Cartesian grid by default
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"prefix=s"=> \$prefix,"gt=s"=>\$gt);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $gt eq "nc" ){ $prefix = "nonCrossInSquare"; }
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
# 
$dw = $order+1; $iw=$order+1; 
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
create mappings
#
 rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 4
  share
    1 2 3 4 
  mappingName
    if( $gt eq "c" ){ $square="square"; $nonSquare="nsquare"; }else{ $square="csquare"; $nonSquare="square"; }
    $square
 exit
 rotate/scale/shift
   mappingName
    $nonSquare
 exit
# -- horizontal bar --
 rectangle
  set corners
    $xac = $xa;                 $xbc= $xb;
    $yac = $ya + .30*($yb-$ya); $ybc= $ya +.70*($yb-$ya); 
    $xac $xbc $yac $ybc
  lines
    $nx = intmg( ($xbc-$xac)/$ds +1.5 ); 
    $ny = intmg( ($ybc-$yac)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 0 0
  share
    1 2 0 0 
  mappingName
    if( $gt eq "c" ){ $hbar="horizontalBar"; $nonHbar="nhbar"; }else{ $hbar="chbar"; $nonHbar="horizontalBar"; }
    $hbar
 exit
 rotate/scale/shift
   mappingName
    $nonHbar
 exit
# -- vertical bar --
 rectangle
  set corners
    $xac = $xa + .30*($xb-$xa); $xbc= $xa+ .70*($xb-$xa);
    $yac = $ya;                 $ybc= $yb;
    $xac $xbc $yac $ybc
  lines
    $nx = intmg( ($xbc-$xac)/$ds +1.5 ); 
    $ny = intmg( ($ybc-$yac)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 3 4
  share
    0 0 3 4 
  mappingName
    if( $gt eq "c" ){ $vbar="verticalBar"; $nonVbar="nvbar"; }else{ $vbar="cvbar"; $nonVbar="verticalBar"; }
    $vbar
 exit
 rotate/scale/shift
   mappingName
    $nonVbar
 exit
# 
exit
generate an overlapping grid
    square
    horizontalBar
    verticalBar
  done
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
#plot
#open graphics 
#
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
crossInSquare
exit

