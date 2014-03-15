#
# Grid for a 2d channel with a bump
#
#
# usage: ogen [noplot] channelWithBump -factor=<num> -interp=[e|i] -order=[2/4/6/8] -ml=<>
# 
#  -ml = number of (extra) multigrid levels to support
# 
# examples:
#     ogen noplot channelWithBump -interp=e -factor=5
#     ogen noplot channelWithBump -interp=e -factor=10
#     ogen noplot channelWithBump -interp=e -factor=20
#     ogen noplot channelWithBump -interp=e -factor=40
#
# multigrid:
#     ogen noplot channelWithBump -interp=e -factor=5 -ml=2
#     ogen noplot channelWithBump -interp=e -factor=10 -ml=3
#     ogen noplot channelWithBump -interp=e -factor=20 -ml=3
#     ogen noplot channelWithBump -interp=e -factor=40 -ml=4
#     ogen noplot channelWithBump -interp=e -factor=80 -ml=5   [9M pts]
#     ogen noplot channelWithBump -interp=e -factor=160 -ml=5  [36M pts]
#     ogen noplot channelWithBump -interp=e -factor=320 -ml=6  [144M pts]
#
# Fourth-order
#     ogen noplot channelWithBump -interp=e -order=4 -factor=5  -ml=2
#     ogen noplot channelWithBump -interp=e -order=4 -factor=10 -ml=3
#     ogen noplot channelWithBump -interp=e -order=4 -factor=20 -ml=3
#     ogen noplot channelWithBump -interp=e -order=4 -factor=40 -ml=4
# 
# Long fetch, 4th order: 
#     ogen noplot channelWithBump -interp=e -order=4 -factor=5  -ml=2 -xa=-10. -name=channelWithBumpFetch10pe5.order4.ml2.hdf
#     ogen noplot channelWithBump -interp=e -order=4 -factor=5  -ml=2 -xa=-20. -xb=10. -name=channelWithBumpFetch20pe5.order4.ml2.hdf
# 
$curve="1bump"; 
$order=2; $factor=1; $interp="i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
# 
$xa=-1.; $xb=6.;
$ya=0.; $yb=2.; 
$ybc=4;  
$bStretch=7.; $nyFactor=2.5; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=>\$factor,"ml=i"=>\$ml,"xa=f"=>\$xa,"xb=f"=>\$xb,"interp=s"=> \$interp,\
            "name=s"=> \$name);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = "channelWithBump" . "$interp$factor" . $suffix . ".hdf"; }
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
# 
$pi=4.*atan2(1.,1.);
$ds=.1/$factor;
#
# 
create mappings
#
#
#  bump(x, amp,x0,w0) 
#    w0 = width 
#  if( $xs < -.5 || $xs > .5 ){ return 0.; }else{ return $amp*(-1.-cos(2.*$pi*$xs));}
sub bump\
{ local($x,$amp,$x0,$w0)=@_; \
  $xs=($x-$x0)/$w0;\
  return $amp*exp(-$xs*$xs);\
}
#
create mappings 
#
# -- nurbs --
# 
$cmd="";
$n=401; $a=$xa; $b=$xb; $h=($b-$a)/($n-1); $amp=.1; 
$amp1=.5; $x1=1.; $w1=.25; 
$amp2=-.05; $x2=.6; $w2=.4; 
$amp3=-.04; $x3=1.5; $w3=.5; 
if( $curve eq "1bump" ){ $amp2=0.; $amp3=0.; }
$yMin=100.; $yMax=-$yMin; \
for( $i=0; $i<$n; $i++){ $x=$a + $h*$i; \
$y = bump($x, $amp1,$x1,$w1); \
$y = $y+ bump($x, $amp2,$x2,$w2); \
$y = $y + bump($x, $amp3,$x3,$w3); \
if( $y<$yMin ){ $yMin=$y; } if( $y>$yMax ){ $yMax=$y; }\
$cmd=$cmd . "$x $y\n"; }
if( $curve eq "flat" ){ $n=11; $h=($b-$a)/($n-1); $amp=0.; $cmd=""; for( $i=0; $i<$n; $i++){ $x=$a + $h*$i; $y=0; $cmd=$cmd . "$x $y\n"; $yMin=0.; $yMax=0.; } }
create mappings 
 # 
$degree=5;
if( $curve eq "1bump" || $curve eq "3bump" || $curve eq "flat" ){ $cmds="nurbs (curve)\n enter points\n $n $degree\n $cmd mappingName\n surfaceCurve\n exit";}\
  else{ $cmds="*"; }
$cmds
#
if( $curve eq "afm1" ){ $cmds = "include afm1.cmd"; }else{ $cmds="*"; }
$cmds 
# 
  mapping from normals
    extend normals from which mapping?
     surfaceCurve
    $nrStretch=5;  # extra points for stretching 
    $nr=intmg(7+$extra+$nrStretch); 
    normal distance
      $dist=-($nr-$nrStretch)*$ds; 
      $dist 
    lines
      $length=($xb-$xa) + $amp*2.; 
      $nx = intmg( $length/$ds + 1.5 );
      $nx $nr 
    boundary conditions
      3 4 1 0
    share
      3 4 0  0
    mappingName
     surface-unstretched
#  open graphics
# pause
# 
  exit
# 
  stretch coordinates
    Stretch r2:itanh
    #  cluster points near the surface: 
    STP:stretch r2 itanh: layer 0 1 $bStretch 0 (id>=0,weight,exponent,position)
    # stretch in the tangential direction at the tip of the bump 
    Stretch r1:itanh
    $rb1=($x1-$xa)/($xb-$xa);  # location in parameter space of the bump 
    STP:stretch r1 itanh: layer 0 0.25 10. $rb1 (id>=0,weight,exponent,position)
    stretch grid
  mappingName
   surface
  exit
#
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 );
    $ny = intmg( ($yb-$ya)/$ds +1.5 );
    $nx $ny
  boundary conditions
    3 4 0 0
  share
    3 4 0 0 
  mappingName
   backGround
exit
# -- coarse backGround
$dsc=2.*$ds; 
rectangle
  set corners
    $yac=$yb-$dsc;
    $xa $xb $yac $ybc
  lines
    $nx = intmg( ($xb-$xa)/$dsc +1.5 );
    $ny = intmg( ($ybc-$yac)/$dsc +1.5 );
    $nx $ny
  boundary conditions
    3 4 0 2
  share
    3 4 0 0 
  mappingName
   upperBackGround
exit
# 
#
exit
generate an overlapping grid
    upperBackGround
    backGround
    surface
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
# open graphics
  compute overlap
  exit
#
save a grid (compressed)
$name
channelWithBump
exit

