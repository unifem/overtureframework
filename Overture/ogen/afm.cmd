#**************************************************************************
#
#  Build grids for an interface based on AFM data
#
# usage: ogen [noplot] afm -factor=<num> -order=[2/4/6/8] -interp=[e/i] -curve=[1bump|3bump|afm1|flat]
# 
# Examples:
#   3 bumps: 
#     ogen noplot afm -interp=e -order=4 -factor=1
#     ogen noplot afm -interp=e -order=4 -factor=2
#     ogen noplot afm -interp=e -order=4 -factor=4
#     ogen noplot afm -interp=e -order=4 -factor=8
#
#     ogen noplot afm -interp=e -order=2 -factor=1
# 
#   1-bump
#     ogen noplot afm -interp=e -order=2 -curve=1bump -xa=-.5 -xb=.5 -ya=-.25-yb=.35 -factor=1
#     ogen noplot afm -interp=e -order=4 -curve=1bump -xa=-.5 -xb=.5 -ya=-.25-yb=.35 -factor=1
# 
#   afm profile 1 : 
#     ogen noplot afm -interp=e -order=4 -curve=afm1 -ya=-.25 -yb=.25 -factor=2 
#     ogen noplot afm -interp=e -order=4 -curve=afm1 -ya=-.25 -yb=.25 -factor=4
#     ogen noplot afm -interp=e -order=4 -curve=afm1 -ya=-.25 -yb=.25 -factor=8
#     ogen noplot afm -interp=e -order=4 -curve=afm1 -ya=-.25 -yb=.25 -factor=16
#     ogen noplot afm -interp=e -order=4 -curve=afm1 -ya=-.25 -yb=.25 -factor=32
#
#   flat profile for testing:
#     ogen noplot afm -interp=e -order=4 -curve=flat -xa=-.125 -xb=.125 -ya=-.25 -yb=.25 -factor=2 
#     ogen noplot afm -interp=e -order=4 -curve=flat -xa=-.125 -xb=.125 -ya=-.25 -yb=.25 -factor=4 
# 
#     ogen noplot afm -interp=e -order=4 -curve=flat -xa=-.05 -xb=.05 -ya=-.25 -yb=.25 -factor=64
# 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $extra=0; $curve="3bump"; 
$xa=-1.; $xb=1.; $ya=-.5; $yb=.5; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"curve=s"=> \$name,"curve=s"=> \$curve);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $extra=$orderOfAccuracy+2; }
if( $factor > 4 ){ $extra=$extra+8; }  # make interface grids a bit wider for higher resolution cases
# 
$suffix = ".order$order"; 
$curveName = ""; if( $curve eq "afm1" ){ $curveName="One"; }elsif( $curve eq "flat" ){ $curveName="Flat"; }elsif( $curve eq "1bump" ){ $curveName="1Bump"; }
if( $name eq "" ){ $name = "afm$curveName" . "$interp$factor" . $suffix . ".hdf";}
# 
# domain parameters:  
$ds = .01/$factor; # target grid spacing
#
#
$bcInterface=100;  # bc for interfaces
$ishare=100;
# 
#
#  bump(x, amp,x0,w0) 
#    w0 = width 
sub bump\
{ local($x,$amp,$x0,$w0)=@_; \
  $xs=($x-$x0)/$w0;\
  if( $xs < -.5 || $xs > .5 ){ return 0.; }else{ return $amp*(-1.-cos(2.*$pi*$xs));}\
}
#
create mappings 
#
#* -- nurbs --
# 
$pi =4.*atan2(1.,1.); $cmd="";
$n=401; $a=$xa; $b=$xb; $h=($b-$a)/($n-1); $amp=.1; 
$amp1=-.10; $x1=0.; $w1=.7; 
$amp2=-.05; $x2=-.4; $w2=.4; 
$amp3=-.04; $x3=+.5; $w3=.5; 
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
if( $curve eq "1bump" || $curve eq "3bump" || $curve eq "flat" ){ $cmds="nurbs (curve)\n enter points\n $n $degree\n $cmd mappingName\n interfaceCurve\n exit";}\
  else{ $cmds="*"; }
$cmds
#
if( $curve eq "afm1" ){ $cmds = "include afm1.cmd"; }else{ $cmds="*"; }
$cmds 
# 
  mapping from normals
    extend normals from which mapping?
    interfaceCurve
    $nr=7+$extra; 
    normal distance
      $dist=($nr-2)*$ds; 
      $dist 
    lines
      $length=($xb-$xa) + $amp*2.; 
      $nx = int( $length/$ds + 1.5 );
      $nx $nr 
    boundary conditions
      1 2 $bcInterface 0
    share
      1 2 $ishare 0
    mappingName
     lowerInterface
# 
  exit
# 
  mapping from normals
    extend normals from which mapping?
    interfaceCurve
    normal distance
      -$dist 
    lines
      $nx $nr
    boundary conditions
      1 2 $bcInterface 0
    share
      1 2 $ishare 0
    mappingName
     upperInterface
  exit
#
  $xar=$xa; $xbr=$xb; $yar=$ya; $ybr=$yMax; 
  rectangle 
    mappingName
      lower
    set corners
     $xar $xbr $yar $ybr
    lines
      $nx=int( ($xbr-$xar)/$ds+1.5 );
      $ny=int( ($ybr-$yar)/$ds+1.5 );
      $nx $ny
    boundary conditions
      1 2 3 0
    share
      1 2 0 0 
    exit 
#
  $xar=$xa; $xbr=$xb; $yar=$yMin; $ybr=$yb;
  rectangle 
    mappingName
      upper
    set corners
     $xar $xbr $yar $ybr 
    lines
      $nx=int( ($xbr-$xar)/$ds+1.5 );
      $ny=int( ($ybr-$yar)/$ds+1.5 );
      $nx $ny
    boundary conditions
      1 2 0 4
    share
      1 2 0 0 
    exit 
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
  compute overlap
# 
  exit
#
save an overlapping grid
$name
afm
exit
