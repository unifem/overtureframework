#
# Three-dimensional rhomboid
#
# usage: ogen [noplot] rhomboid -factor=<num> -order=[2/4/6/8] -ml=<> ...
#                             -ar= -as= -at= -br= -bs= -bt= -cr= -cs= -ct=
# 
#  ml = number of (extra) multigrid levels to support
#  ar, as, at, br, ... : defines rhomboid mapping (see below)
# 
# Here is the equation for the rhomboid mapping: x(r,s,t) : 
#   x=ar*r+as*s+at*t
#   y=br*r+bs*s+bt*t
#   z=cr*r+cs*s+ct*t
#
# examples:
#     ogen noplot rhomboid -order=2 -factor=1
#     ogen noplot rhomboid -order=2 -factor=2
# 
#     ogen noplot rhomboid -order=4 -factor=1
#     ogen noplot rhomboid -order=4 -factor=2
#
#   -- explicitly set the number of lines and name:
#     ogen noplot rhomboid -order=4 -n=9 -name="rhomboidEight.order4.hdf"
#     ogen noplot rhomboid -order=4 -n=5 -name="rhomboidFour.order4.hdf"
#
$prefix="rhomboid"; 
$order=2; $factor=1;  $ml=0; $nn=-1; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$ar=1.; $as=.25; $at=.05;
$br=.1; $bs=1.0; $bt=.15;
$cr=.2; $cs=.25; $ct=1.0;
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"n=i"=>\$nn,\
            "name=s"=> \$name,"ml=i"=>\$ml,"prefix=s"=> \$prefix,\
            "ar=f"=>\$ar,"as=f"=>\$as,"at=f"=>\$at,\
            "br=f"=>\$br,"bs=f"=>\$bs,"bt=f"=>\$bt,\
            "cr=f"=>\$cr,"cs=f"=>\$cs,"ct=f"=>\$ct );
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
  plane or rhombus
    specify plane or rhombus by three points
#
#      $ar= 1.; $as=.00; $at=.00;
#      $br=.0; $bs=1.0; $bt=.00;
#      $cr=.0; $cs=.00; $ct=1.0;
# 
      $r=0.; $s=0.; $t=0.;
        $xa=$ar*$r+$as*$s+$at*$t; 
        $ya=$br*$r+$bs*$s+$bt*$t; 
        $za=$cr*$r+$cs*$s+$ct*$t; 
      $r=1.; $s=0.; $t=0.; 
        $xb=$ar*$r+$as*$s+$at*$t; 
        $yb=$br*$r+$bs*$s+$bt*$t; 
        $zb=$cr*$r+$cs*$s+$ct*$t; 
      $r=0.; $s=1.; $t=0.; 
        $xc=$ar*$r+$as*$s+$at*$t; 
        $yc=$br*$r+$bs*$s+$bt*$t; 
        $zc=$cr*$r+$cs*$s+$ct*$t; 
      $xa $ya $za $xb $yb $zb $xc $yc $zc
    mappingName
     bottom
    exit
  plane or rhombus
    specify plane or rhombus by three points
      $r=0.; $s=0.; $t=1.;
        $xa=$ar*$r+$as*$s+$at*$t; 
        $ya=$br*$r+$bs*$s+$bt*$t; 
        $za=$cr*$r+$cs*$s+$ct*$t; 
      $r=1.; $s=0.; $t=1.; 
        $xb=$ar*$r+$as*$s+$at*$t; 
        $yb=$br*$r+$bs*$s+$bt*$t; 
        $zb=$cr*$r+$cs*$s+$ct*$t; 
      $r=0.; $s=1.; $t=1.; 
        $xc=$ar*$r+$as*$s+$at*$t; 
        $yc=$br*$r+$bs*$s+$bt*$t; 
        $zc=$cr*$r+$cs*$s+$ct*$t; 
      $xa $ya $za $xb $yb $zb $xc $yc $zc
    mappingName
      top
    exit
  tfi
    choose bottom curve (r_2=0)
      bottom
    choose top curve    (r_2=1)
      top
    lines
      $n = intmg( 1./$ds + 1.5 );
      if( $nn ne "-1" ){ $n = $nn; }
      $n $n $n
    boundary conditions
     1 2 3 4 5 6 
    mappingName
      rhomboid
    exit
  exit this menu
generate an overlapping grid
  rhomboid
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
rhomboid
exit

