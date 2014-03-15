#
# Grid for a 3d channel with a bump
#
#
# usage: ogen [noplot] channelWithBump3dGrid -factor=<num> -interp=[e|i] -order=[2/4/6/8] -ml=<>
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
#     ogen -noplot channelWithBump3dGrid -interp=e -factor=2
#     ogen -noplot channelWithBump3dGrid -interp=e -factor=4
#
# multigrid:
#     ogen -noplot channelWithBump3dGrid -interp=e -factor=4 -ml=2
#
# Fourth-order
#     ogen -noplot channelWithBump3dGrid -interp=e -order=4 -factor=4  -ml=2
#     ogen -noplot channelWithBump3dGrid -interp=e -order=4 -factor=5  -ml=2
# 
# Long fetch, 4th order: 
#     ogen -noplot channelWithBump3dGrid -interp=e -order=4 -factor=5  -ml=2 -xa=-10. -name=channelWithBump3dGridFetch10pe5.order4.ml2.hdf
# 
$curve="1bump"; 
$order=2; $factor=1; $interp="i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
# 
$xa=-2.; $xb=3.; $ya=0.; $yb=2.; $za=-2.5; $zb=2.5; 
$yfa=$ya; $yfb=1.0; # height of fine backGround grid 
$ycb=1.; # height of coarse boundary-layer grid 
$xfa=-1.; $xfb=2.0;  # x-extent of hill grid
$zfa=-1.; $zfb=1.0;  # z-width of hill grid 
$blFactor=3.;  # boundary layer spacing is this many times finer
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=>\$factor,"ml=i"=>\$ml,"xa=f"=>\$xa,"xb=f"=>\$xb,"yb=f"=>\$yb,\
            "za=f"=>\$za,"zb=f"=>\$zb,"interp=s"=> \$interp,\
            "name=s"=> \$name,"blFactor=f"=>\$blFactor);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = "channelWithBump3dGrid" . "$interp$factor" . $suffix . ".hdf"; }
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
#  bump(x,z, amp,x0,z0,wx,wz) 
#    (x0,z0) = centre 
#    wx, wz = widths
#  if( $xs < -.5 || $xs > .5 ){ return 0.; }else{ return $amp*(-1.-cos(2.*$pi*$xs));}
sub bump\
{ local($x,$z,$amp, $x0,$z0,$wx,$wz)=@_; \
  $xs=($x-$x0)/$wx; $zs=($z-$z0)/$wz;\
  return $amp*exp(-$xs*$xs-$zs*$zs);\
}
#
create mappings 
#
# -- nurbs --
# 
$cmd="";
$n=51; $a=$xfa; $b=$xfb; $h=($b-$a)/($n-1);   $hz=($zfb-$zfa)/($n-1);
$amp1=.5; $x1=0.; $z1=0.; $wx1=.25;  $wz1=.25; 
$yMin=100.; $yMax=-$yMin; 
for( $j=0; $j<$n; $j++){ $z=$zfa + $hz*$j; for( $i=0; $i<$n; $i++){ $x=$a + $h*$i;  \
$y = bump($x,$z, $amp1,$x1,$z1,$wx1,$wz1); \
if( $y<$yMin ){ $yMin=$y; } if( $y>$yMax ){ $yMax=$y; }\
$cmd=$cmd . "$x $y $z\n"; }}
#
#
create mappings 
 # 
$degree=5;
if( $curve eq "1bump" || $curve eq "3bump" || $curve eq "flat" ){ $cmds="nurbs (surface)\n enter points\n $n $n $degree\n $cmd mappingName\n surfaceCurve\n exit";}\
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
      $lengthz=($zfb-$zfa) + $amp*2.; 
      $nz = intmg( $lengthz/$ds + 1.5 );
      $nx $nz $nr 
    boundary conditions
      0 0 0 0 1 0 
    share
      0 0 0 0 1 0
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
    $dsBL= $ds/$blFactor; 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    STRT:name hill
 exit
#
# -- upwind (fetch) boundary layer grid 
#    -> twice the height of the hill grid 
$dsc=2.*$ds; 
box
  set corners
    $xaf=$xa; $xbf=$xfa+2.*$ds; $yaf=$yfa; $ybf=$ya-$dist*2.; $zaf=$zfa; $zbf=$zfb; 
    $xaf $xbf $yaf $ybf $zaf $zbf
  lines
    $nxf = intmg( ($xbf-$xaf)/$dsc +1.5 );
    $nyf = $nr; 
    $nzf = intmg( ($zbf-$zaf)/$dsc +1.5 );
    $nxf $nyf $nzf 
  boundary conditions
    3 0 1 0 0 0 
  share
    3 0 1 0 0 0 
  mappingName
   fetchBoundaryLayerUnstretched
exit
# 
#  -- stretch upwind (fetch) boundary layer grid  ---
#
  stretch coordinates 
    STRT:multigrid levels $ml
    $dscBL= $dsc/$blFactor; 
    Stretch r2:exp to linear
    STP:stretch r2 expl: min dx, max dx $dscBL $ds
    STRT:name fetchBoundaryLayer
 exit
#
# fine grid background near the hill
#
box 
  set corners
    $yfa=$ya; 
    $xfa $xfb $yfa $yfb $zfa $zfb
  lines
    $nx = intmg( ($xfb-$xfa)/$ds +1.5 );
    $ny = intmg( ($yfb-$yfa)/$ds +1.5 );
    $nz = intmg( ($zfb-$zfa)/$ds +1.5 );
    $nx $ny $nz
  boundary conditions
    0 0 1 0 0 0 
  share
    0 0 1 0 0 0 
  mappingName
   backGround
exit
#
# -- coarse backGround
#
$dsc=2.*$ds; 
box
  set corners
    # $yac=$ybf-3.*$dsc; $ybc=$yb; 
    # $xa $xb $yac $ybc
    $xa $xb $ya $yb $za $zb
  lines
    $nx = intmg( ($xb-$xa)/$dsc +1.5 );
    # $ny = intmg( ($ybc-$yac)/$dsc +1.5 );
    $ny = intmg( ($yb-$ya)/$dsc +1.5 );
    $nz = intmg( ($zb-$za)/$dsc +1.5 );
    $nx $ny $nz 
  boundary conditions
    3 4 1 2 5 6 
  share
    3 4 1 0 5 6 
  mappingName
   upperBackGround
exit
# 
#
# Explicit hole cutter 
# Define an explicit hole cutter for the region below the hill 
# This is needed since the two back-ground grids have boundaries that overlap here
#
box 
  set corners
    $xha=-.6; $xhb=.6; $yha=-$ds; $yhb=2.*$ds; $zha=$xha; $zhb=$xhb;
    $xha $xhb $yha $yhb $zha $zhb
  lines
    $nx = intmg( ($xhb-$xha)/$ds +1.5 );
    $ny = intmg( ($yhb-$yha)/$ds +1.5 );
    $nz = intmg( ($zhb-$zha)/$ds +1.5 );
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6
  mappingName
   hillHoleCutter
exit
#
exit
generate an overlapping grid
    upperBackGround
    backGround
    fetchBoundaryLayer
    hill
  done
  change parameters 
    # define an explicit hole cutter for the region below the hill 
    # This is needed since the two back-ground grids have boundaries that overlap here
   create explicit hole cutter
      name: explicitHoleCutter
      Hole cutter:hillHoleCutter
      # hole cutter does not cut in the hill 
      prevent hole cutting
        hill
      done
    exit
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
channelWithBump3dGrid
exit

