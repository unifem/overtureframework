#**************************************************************************
#
#  Build grids for an interface with one or more bumps (for cgmx)
#
# usage: ogen [noplot] interfaceBump3d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -curve=[3bump|afm1|flat]
# 
# examples:
#   -- one bump: 
#     ogen noplot interfaceBump3d -interp=i -order=2 -curve=1bump -factor=1 -za=-1. -zb=1.
#     ogen noplot interfaceBump3d -interp=e -order=2 -curve=1bump -factor=2 -za=-1. -zb=1.
#    -- fourth-order: 
#     ogen noplot interfaceBump3d -interp=e -order=4 -curve=1bump -factor=2 -za=-1. -zb=1.
#     ogen noplot interfaceBump3d -interp=e -order=4 -curve=1bump -factor=4 -za=-1. -zb=1.
#     ogen noplot interfaceBump3d -interp=e -order=4 -curve=1bump -factor=8 -za=-1. -zb=1.  (41M)
#     srun -N2 -n8 -ppdebug $ogenp noplot interfaceBump3d -interp=e -order=4 -curve=1bump -factor=16 -za=-1. -zb=1.
# 
# parallel:
#    srun -N1 -n2 -ppdebug $ogenp noplot interfaceBump3d -interp=e -order=2 -curve=1bump -factor=1
#
#   3d afm profile 1 :  (small patch for testing)
#     ogen noplot interfaceBump3d -interp=e -order=2 -curve=afm3d1 -factor=4 -za=-.5 -zb=.5 
#    -- order=4
#     ogen noplot interfaceBump3d -interp=i -order=4 -curve=afm3d1 -factor=4 -za=-.25 -zb=.25  -- small cells
#     ogen noplot interfaceBump3d -interp=i -order=4 -curve=afm3d1 -factor=8 -za=-.25 -zb=.25 
#     ogen noplot interfaceBump3d -interp=e -order=4 -curve=afm3d1 -factor=8 -za=-.25 -zb=.25    (1M)
#     ogen noplot interfaceBump3d -interp=e -order=4 -curve=afm3d1 -factor=16 -za=-.25 -zb=.25   (9M)
# 
#   3d afm profile 2 :  (upper middle right [.5,.75]x[.5,.75])
#     ogen noplot interfaceBump3d -interp=i -order=2 -curve=afm3d2 -factor=4 -za=-.5 -zb=.5 
#     ogen noplot interfaceBump3d -interp=e -order=2 -curve=afm3d2 -factor=4 -za=-.5 -zb=.5 -- small cells
#     ogen noplot interfaceBump3d -interp=e -order=2 -curve=afm3d2 -factor=8 -za=-.5 -zb=.5 
#     ogen noplot interfaceBump3d -interp=e -order=2 -curve=afm3d2 -factor=16 -za=-.5 -zb=.5  (60M pts)
#
#   3d afm profile 3 :  Surface from Isaac Bass
#    ogen -noplot interfaceBump3d -interp=i -order=2 -curve=afm3d3 -factor=1 -ds0=.4 -za=-14. -zb=6
#    ogen -noplot interfaceBump3d -interp=e -order=2 -curve=afm3d3 -factor=2 -ds0=.4 -za=-14. -zb=6
#       lambda=.355  -> .355/1.5=.237   -> 2-4pts/wave-length, 17M pts
#    ogen -noplot interfaceBump3d -interp=e -order=4 -curve=afm3d3 -factor=4 -ds0=.4 -za=-14. -zb=6
# 
# -- order4:
#     ogen noplot interfaceBump3d -interp=i -order=4 -curve=afm3d2 -factor=4 -za=-.5 -zb=.5 --small cells
#     ogen noplot interfaceBump3d -interp=e -order=4 -curve=afm3d2 -factor=4 -za=-.5 -zb=.5 
#     ogen noplot interfaceBump3d -interp=i -order=4 -curve=afm3d2 -factor=6 -za=-.5 -zb=.5   (3M pts)
#     ogen noplot interfaceBump3d -interp=e -order=4 -curve=afm3d2 -factor=6 -za=-.5 -zb=.5   (3M pts) --small cells
#     ogen noplot interfaceBump3d -interp=e -order=4 -curve=afm3d2 -factor=8 -za=-.5 -zb=.5   (7M)
# 
# -- works: 
#  srun -N1 -n4 -ppdebug $ogenp noplot interfaceBump3d -interp=e -order=2 -curve=afm3d2 -factor=8 -za=-.25 -zb=.25 
# 
#  srun -N2 -n8 -ppdebug $ogenp noplot interfaceBump3d -interp=e -order=2 -curve=afm3d2 -factor=32 -za=-.25 -zb=.25 
#  Flat interface for testing:
#   ogen noplot interfaceBump3d -interp=e -order=2 -curve=flat -factor=2 -za=-.5 -zb=.5
#   ogen noplot interfaceBump3d -interp=e -order=2 -curve=flat -factor=4 -za=-.5 -zb=.5 -xa=-.25 -xb=.25 -ya=-.25 -yb=.25
#   ogen noplot interfaceBump3d -interp=e -order=2 -curve=flat -factor=8 -za=-.5 -zb=.5 -xa=-.25 -xb=.25 -ya=-.25 -yb=.25
# 
#   ogen noplot interfaceBump3d -interp=e -order=4 -curve=flat -factor=2 -za=-.5 -zb=.5 -xa=-.25 -xb=.25 -ya=-.25 -yb=.25
#   ogen noplot interfaceBump3d -interp=e -order=4 -curve=flat -factor=4 -za=-.5 -zb=.5 -xa=-.25 -xb=.25 -ya=-.25 -yb=.25
#   ogen noplot interfaceBump3d -interp=e -order=4 -curve=flat -factor=8 -za=-.5 -zb=.5 -xa=-.25 -xb=.25 -ya=-.25 -yb=.25
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $extra=0; $curve="1bump"; 
$xa=-1.; $xb=1.; $ya=-1.; $yb=1.; $za=-.5; $zb=.5; $ds0=.05; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
        "za=f"=> \$za,"zb=f"=> \$zb,"interp=s"=> \$interp,"name=s"=> \$name,"curve=s"=> \$curve,\
        "extra=i"=>\$extra,"ds0=f"=> \$ds0 );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; $extra=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; $extra=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; $extra=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $extra=$extra+$orderOfAccuracy+2; }
# *wdh* 090409 if( $factor > 4 ){ $extra=$extra+8; }  # make interface grids a bit wider for higher resolution cases
# 
$suffix = ".order$order"; 
$curveName = ""; 
if( $curve eq "afm1" ){ $curveName="One"; }elsif( $curve eq "flat" ){ $curveName="Flat"; }else{ $curveName=$curve; }
if( $name eq "" ){ $name = "interfaceBump3d$curveName" . "$interp$factor" . $suffix . ".hdf";}
# 
# domain parameters:  
$ds = $ds0/$factor; # target grid spacing
# 
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$dw = $order+1; $iw=$order+1;
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
#
#
$bcInterface=100;  # bc for interfaces
$ishare=100;
# 
#
#  bump(x,y, amp,x0,y0,w0) 
#    amp : amplitude of the bump 
#    x0,y0 : center of the bump
#    w0 = width of the bump
sub bump\
{ local($x,$y,$amp,$x0,$y0,$w0)=@_; \
  $rr= sqrt( (($x-$x0)/$w0)**2 + (($y-$y0)/$w0)**2 );\
  if( $rr < -.5 || $rr > .5 ){ return 0.; }else{ return $amp*(-1.-cos(2.*$pi*$rr));}\
}
#
create mappings 
#
#* -- nurbs --
# 
$pi =4.*atan2(1.,1.); $cmd="";
$n=41; $a=$xa; $b=$xb; $h=($b-$a)/($n-1); $amp=.1; 
$amp1=-.10; $x1=0.; $y1=0.; $w1=1.; 
$amp2=-.05; $x2=-.4; $w2=.4; 
$amp3=-.04; $x3=+.5; $w3=.5; 
if( $curve eq "flat" ){ $amp1=0.; $amp2=0.; $amp3=0.; }
$zMin=100.; $zMax=-$zMin; \
for( $j=0; $j<$n; $j++){for( $i=0; $i<$n; $i++){ $x=$a + $h*$i; $y=$a + $h*$j; \
$z = bump($x,$y, $amp1,$x1,$y1,$w1); \
if( $z<$zMin ){ $zMin=$z; } if( $z>$zMax ){ $zMax=$z; }\
$cmd=$cmd . "$x $y $z\n"; }}
create mappings 
 # 
$degree=$order+1;
if( $curve eq "1bump" || $curve eq "3bump" || $curve eq "flat" ){ $cmds="nurbs (surface)\n enter points\n $n $n $degree\n $cmd mappingName\n interfaceCurve\n exit";}\
  else{ $cmds="*"; }
#
if( $curve eq "afm3d1" ){ $amp=0.; $afm3dSurface="afm.smallMiddlePatch.dat"; }
# if( $curve eq "afm3d2" ){ $amp=0.; $afm3dSurface="/home/henshaw.0/cgDoc/nif/afm/afm.upperMiddleRight.dat"; }
if( $curve eq "afm3d2" ){ $amp=0.; $afm3dSurface="afm.upperMiddleRight.dat"; }
if( $curve eq "afm3d3" ){ $amp=0.; $afm3dSurface="afm.M588Mid.dat"; }
if( $curve eq "afm3d1" || $curve eq "afm3d2" || $curve eq "afm3d3" ){ $cmds = "include afm3d.cmd"; }
$cmds 
# 
  mapping from normals
    extend normals from which mapping?
    interfaceCurve
    $nr=5+$extra; 
    normal distance
      $dist=($nr-2)*$ds; 
      -$dist 
    lines
      $length=($xb-$xa) + $amp*2.; 
      $nx = int( $length/$ds + 1.5 );
      $ny=$nx; 
      $nx $ny $nr 
    boundary conditions
      1 2 3 4 $bcInterface 0 
    share
      1 2 3 4 $ishare 0 
    mappingName
     lowerInterface
# pause
# 
  exit
# 
  mapping from normals
    extend normals from which mapping?
    interfaceCurve
    normal distance
      $dist 
    lines
      $nx $ny $nr 
    boundary conditions
      1 2 3 4 $bcInterface 0 
    share
      1 2 3 4 $ishare 0 
    mappingName
     upperInterface
# pause
  exit
#
  $xar=$xa; $xbr=$xb; $yar=$ya; $ybr=$yb; $zar=$za; $zbr=$zMax; 
Box
    mappingName
      lower
  set corners
    $xar $xbr $yar $ybr $zar $zbr
  lines
    $nx = int( ($xbr-$xar)/$ds +1.5);
    $ny = int( ($ybr-$yar)/$ds +1.5);
    $nz = int( ($zbr-$zar)/$ds +1.5);
    $nx $ny $nz
    boundary conditions
      1 2 3 4 5 0 
    share
      1 2 3 4 0 0 
  exit
#
  $xar=$xa; $xbr=$xb; $yar=$ya; $ybr=$yb; $zar=$zMin; $zbr=$zb; 
Box
    mappingName
      upper
  set corners
    $xar $xbr $yar $ybr $zar $zbr
  lines
    $nx = int( ($xbr-$xar)/$ds +1.5);
    $ny = int( ($ybr-$yar)/$ds +1.5);
    $nz = int( ($zbr-$zar)/$ds +1.5);
    $nx $ny $nz
    boundary conditions
      1 2 3 4 0 6
    share
      1 2 3 4 0 0
  exit
#
exit
#
generate an overlapping grid 
  lower
  upper
  lowerInterface
  upperInterface
  done 
#
  change parameters
    specify a domain
 # domain name:
      lowerDomain 
 # grids in the domain:
      lower
      lowerInterface
      done
    specify a domain
 # domain name:
      upperDomain 
 # grids in the domain:
      upper
      upperInterface
      done
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit
# 
  compute overlap
# 
  exit
#
maximum number of parallel sub-files
  8
save an overlapping grid
$name
interfaceBump
exit
