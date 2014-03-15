#
# Grid for a 2d channel with a bump
#
#
# usage: ogen [noplot] channelWithBump2dGrid -factor=<num> -interp=[e|i] -order=[2/4/6/8] -ml=<>
# 
#  -ml = number of (extra) multigrid levels to support
# 
#
#         yb + ------------------------------------------+
#            |                                           |
#            |                                           |
#            |    coarse BG                              |
#            |                                           |
#            |                                           |
#        yfb |            .....................          |
#            |            . fine BG           .          |
#            |            .                   .          |
#            |------------.         ----      .----------|
#            |            .       /      \    .          |
#            |coarse BL   .-------    -   ----. coarse BL|
#            |            .fine BL  /   \     .          |
#         ya +------------.--------/  .  \----.----------+
#           xa           xfa          x0     xfb        xb
#
#
# examples:
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=5
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=10
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=20
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=40
#
# multigrid:
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=5 -ml=2
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=10 -ml=3
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=20 -ml=3
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=40 -ml=4
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=80 -ml=5 
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=160 -ml=5
#     ogen -noplot channelWithBump2dGrid -interp=e -factor=320 -ml=6
#
# Fourth-order
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=4  -ml=2
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=5  -ml=2
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=10 -ml=3
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=20 -ml=3
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=40 -ml=4
# 
# Long fetch, 4th order: 
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=5  -ml=2 -xa=-10. -name=channelWithBump2dGridFetch10pe5.order4.ml2.hdf
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=5  -ml=2 -xa=-20. -xb=10. -name=channelWithBump2dGridFetch20pe5.order4.ml2.hdf
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=8  -ml=2 -xa=-20. -xb=10. -yb=6. -name=channelWithBump2dGridFetch20e8.order4.ml2.hdf
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=10  -ml=2 -xa=-20. -xb=10. -yb=6. -name=channelWithBump2dGridFetch20e10.order4.ml2.hdf
#     ogen -noplot channelWithBump2dGrid -interp=e -order=4 -factor=16  -ml=2 -xa=-20. -xb=10. -yb=6. -name=channelWithBump2dGridFetch20e16.order4.ml2.hdf
# 
$curve="1bump"; 
$order=2; $factor=1; $interp="i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
# 
$xa=-3.; $xb=3.; $ya=0.; $yb=3.; 
$yfb=1.5; # height of fine backGround grid 
$ycb=1.; # height of coarse boundary-layer grid 
$xfa=-1.; $xfb=2.0; 
$blFactor=5.;  # boundary layer spacing is this many times finer
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=>\$factor,"ml=i"=>\$ml,"xa=f"=>\$xa,"xb=f"=>\$xb,"yb=f"=>\$yb,"interp=s"=> \$interp,\
            "name=s"=> \$name,"blFactor=f"=>\$blFactor);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = "channelWithBump2dGrid" . "$interp$factor" . $suffix . ".hdf"; }
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
$n=201; $a=$xfa; $b=$xfb; $h=($b-$a)/($n-1);
$amp1=.5; $x1=0.; $w1=.25; 
# $amp2=-.05; $x2=.6; $w2=.4; 
# $amp3=-.04; $x3=1.5; $w3=.5; 
# if( $curve eq "1bump" ){ $amp2=0.; $amp3=0.; }
$yMin=100.; $yMax=-$yMin; \
for( $i=0; $i<$n; $i++){ $x=$a + $h*$i; \
$y = bump($x, $amp1,$x1,$w1); \
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
# 
  mapping from normals
    extend normals from which mapping?
     surfaceCurve
    $nrStretch=7;  # extra points for stretching 
    $nr=intmg(7+$extra+$nrStretch); 
    normal distance
      $dist=-($nr-$nrStretch)*$ds; 
      $dist 
    lines
      $length=($xfb-$xfa) + $amp*2.; 
      $nx = intmg( $length/$ds + 1.5 );
      $nx $nr 
    boundary conditions
      0 4 1 0
    share
      0 4 1  0
    mappingName
     hillUnstretched
#  open graphics
# pause
# 
  exit
# 
#  -- stretch hill grid ---
#
  stretch coordinates 
    STRT:multigrid levels $ml
    Stretch r2:exp
    $dsBL= $ds/$blFactor; 
    Stretch r2:exp to linear
    STP:stretch r2 expl: min dx, max dx $dsBL $ds
    STRT:name hill
 exit
#
# Boundary layer grid in the fetch: 
#
rectangle
  set corners
    $dsc=$ds*2.; 
    $xaf=$xa; $xbf = $xfa+3.1*$dsc; $yaf=$ya; $ybf=$ya-$dist*2.; 
    $xaf $xbf $yaf $ybf
  lines
    $nxf = intmg( ($xbf-$xaf)/$dsc +1.5 );
    $nyf = $nr; 
    $nxf $nyf
  boundary conditions
    3 0 1 0
  share
    3 0 1 0 
  mappingName
   fetchBoundaryLayerUnstretched
exit
# 
#  -- stretch fetch grid ---
#
  stretch coordinates 
    STRT:multigrid levels $ml
    Stretch r2:exp
    $dsBLc= $dsc/$blFactor; 
    Stretch r2:exp to linear
    STP:stretch r2 expl: min dx, max dx $dsBLc $dsc
    STRT:name fetchBoundaryLayer
 exit
#
# Boundary layer grid in the downWind
#
rectangle
  set corners
    $xaf=$xfb-3.6*$dsc; $xbf = $xb; $yaf=$ya; $ybf=$ya-$dist*2.; 
    $xaf $xbf $yaf $ybf
  lines
    $nxf = intmg( ($xbf-$xaf)/$dsc +1.5 );
    $nyf = $nr; 
    $nxf $nyf
  boundary conditions
    0 4 1 0
  share
    0 4 1 0 
  mappingName
   downWindBoundaryLayerUnstretched
exit
# 
#  -- stretch downwind BL grid ---
#
  stretch coordinates 
    STRT:multigrid levels $ml
    Stretch r2:exp
    $dsBLc= $dsc/$blFactor; 
    Stretch r2:exp to linear
    STP:stretch r2 expl: min dx, max dx $dsBLc $dsc
    STRT:name downWindBoundaryLayer
 exit
#
# fine grid background near the hill
#
rectangle
  set corners
    $yfa=$ya; 
    $xfa $xfb $yfa $yfb
  lines
    $nx = intmg( ($xfb-$xfa)/$ds +1.5 );
    $ny = intmg( ($yfb-$yfa)/$ds +1.5 );
    $nx $ny
  boundary conditions
    0 0 0 0
  share
    0 0 0 0 
  mappingName
   backGround
exit
#
# -- coarse backGround
#
$dsc=2.*$ds; 
rectangle
  set corners
    $yac=$ybf-3.*$dsc; $ybc=$yb; 
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
    fetchBoundaryLayer
    downWindBoundaryLayer
    hill
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
channelWithBump2dGrid
exit

