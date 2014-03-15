#
# 2D "star-fish" with different numbers of legs.
#
#
# usage: ogen [-noplot] starFishGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>  ...
#                 -ra=<f> -rb=<f> -nArms=<i> -alpha0=<f>
#                 -xa=<> -xb=<> -ya=<> -yb=<> -numGhost=<i>
# 
#  -ml = number of (extra) multigrid levels to support
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
# 
# Examples:
#   ogen -noplot starFishGrid -order=2 -interp=e -factor=8
#   ogen -noplot starFishGrid -order=2 -interp=e -factor=16
#   ogen -noplot starFishGrid -order=2 -interp=e -factor=32
#
# -- four short arms:
#   ogen -noplot starFishGrid -order=2 -interp=e -factor=8 -nArms=4 -ra=.7 -rb=.2 -alpha0=.5 
#
#
$ra=.4;  # inner radius
$rb=.6; # half height of arms
$nArms=6;  # number of arms
$alpha0=1.; 
#
$prefix="starFishGrid";  $rgd="var"; $bcSquare="d"; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
$cx=0.; $cy=0.;  # center for the annulus
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "ra=f"=>\$ra,"rb=f"=>\$rb,"nArms=i"=>\$nArms,"alpha0=f"=>\$alpha0,\
            "rgd=s"=> \$rgd,"bcSquare=s"=>\$bcSquare,"numGhost=i"=>\$numGhost );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=3; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=4; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $bcSquare eq "p" ){ $prefix = $prefix . "p"; }
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $blf ne 1 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
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
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
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
    $sbc="1 2 3 4";
    if( $bcSquare eq "p" ){ $sbc = "-1 -1 3 4"; }
    $sbc
  mappingName
    backGround
exit
#
$ns=$nArms*100+1;  # number of spline points
$alpha=$alpha0*$pi/$nArms;  # alpha=pi/nArms : peaks are shifted to positions of troughs
$cmd="#";
for( $i=0; $i<$ns; $i++ ){ $s=2.*$pi*($i-1.)/($ns-1.); $y= (.5*(1.+sin($nArms*$s)) )**2; $x = $s + $alpha*$y**2; $r=$ra+$rb*$y; $xx=$r*cos($x); $yy=$r*sin($x); $cmd .= "\n $xx $yy"; }
# for( $i=0; $i<$ns; $i++ ){ $s=2.*$pi*($i-1.)/($ns-1.); $y=$s; $x = $s;  $cmd .= "\n $xx $yy"; }
#
spline
  #
  enter spline points
  # include c2.dat
  $ns
  $cmd
  lines
    $ns
    periodicity
      2
    mappingName
      starFishCurve
    exit
# 
# -- Make a fine grid hyperbolic grid --
#
  hyperbolic
    $nDist=.05; 
    distance to march $nDist
    lines to march 31  
    points on initial curve 4801
    uniform dissipation 0.05
    volume smooths 500
    equidistribution 0 (in [0,1])
    generate
    boundary conditions
      -1 -1 7 0 0 0
    share 
       0 0  7 0 0 0
    name starFishHype
  exit
#
# convert to a Nurbs and set the number of grid points
  nurbs (curve)
    interpolate from mapping with options
      starFishHype
    parameterize by chord length
    choose degree
      3
    done
  lines
    $arcLength= .5*$nArms*$pi*($ra+$rb);   # this is a guess
    $nTheta = intmg( $arcLength/$ds + 1.5 );
    $nr = intmg($nDist/$ds + 3.5); 
    $nTheta $nr 
  mappingName
   starFish
 exit
#
exit
generate an overlapping grid
    backGround
    starFish
  done
  change parameters
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      # $ngp = $ng+1;
      $ngp = $ng;
      $ng $ng $ng $ngp $ng $ng
  exit
#  display intermediate results
open graphics
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
cic
exit

