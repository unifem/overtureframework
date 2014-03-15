#
# Two-dimensional rhombus
#
# usage: ogen [noplot] rhombus -factor=<num> -order=[2/4/6/8] -ml=<> ...
#                             -ar= -as= -ars= -br= -bs= -brs=
# 
#  ml = number of (extra) multigrid levels to support
#  ar, as, br, bs : defines rhombus mapping (see below)
# 
# Here is the equation for the rhombus mapping: x(r,s) : 
#   x=ar*r+as*s + ars*r*s
#   y=br*r+bs*s + brs*r*s
#
# examples:
#     ogen noplot rhombus -order=2 -factor=1
#     ogen noplot rhombus -order=2 -factor=2
# 
#     ogen noplot rhombus -order=4 -factor=1
#     ogen noplot rhombus -order=4 -factor=2
#
$prefix="rhombus"; 
$order=2; $factor=1;  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$ar=1.; $as=.25; $ars=0.0; 
$br=.1; $bs=1.0; $brs=0.0; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,\
            "name=s"=> \$name,"ml=i"=>\$ml,"prefix=s"=> \$prefix,\
            "ar=f"=>\$ar,"as=f"=>\$as,"br=f"=>\$br,"bs=f"=>\$bs,"ars=f"=>\$ars,"brs=f"=>\$brs);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$factor" . $suffix . ".hdf";}
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
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
#
create mappings
#
 line (2D)
  set end points
   $r=0.; $s=0.; 
    $xa=$ar*$r+$as*$s+$ars*$r*$s; 
    $ya=$br*$r+$bs*$s+$brs*$r*$s; 
   $r=1.; $s=0.; 
    $xb=$ar*$r+$as*$s+$ars*$r*$s; 
    $yb=$br*$r+$bs*$s+$brs*$r*$s; 
    $xa $xb $ya $yb
  mappingName
   bottom
  exit
#
 line (2D)
   set end points
   $r=0.; $s=1.; 
    $xa=$ar*$r+$as*$s+$ars*$r*$s; 
    $ya=$br*$r+$bs*$s+$brs*$r*$s; 
   $r=1.; $s=1.; 
    $xb=$ar*$r+$as*$s+$ars*$r*$s; 
    $yb=$br*$r+$bs*$s+$brs*$r*$s; 
    $xa $xb $ya $yb
 mappingName
   top
 exit
#
 tfi
  choose bottom curve (r_2=0)
    bottom
  choose top curve    (r_2=1)
    top
  lines
    $n = intmg( 1./$ds + 1.5 );
    $n $n $n
  boundary conditions
   1 2 3 4 
  mappingName
    rhombus
 exit
exit this menu
generate an overlapping grid
  rhombus
  done choosing mappings
  change parameters
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
  compute overlap
# pause
  exit
#
save a grid (compressed)
$name
rhombus
exit















create mappings
  line (2D)
    set end points
      0 1  0 .25
*     0 1  0 0
*      0 1  .25 .5
*       0 1 0 .25
    mappingName
    bottom
    exit
  line (2D)
    set end points
      .25 1.25 1. 1.25
*     .5 1.5 1. 1.
*      .5 1.5 1. .9
*     0. 1. 1.25 1.5 
*       .25 1.25 1. 1.25
    mappingName
    top
    exit
  tfi
    choose bottom curve (r_2=0)
      bottom
    choose top curve    (r_2=1)
      top
    lines
    21 21
    mappingName
     rhombus
    exit
  exit this menu
generate an overlapping grid
  rhombus
  done choosing mappings
  compute overlap
  exit
save a grid
rhombus.hdf
rhombus
exit
