#**************************************************************************
#
#  Build grids for an interface with one or more bumps (for cgmx)
#
# usage: ogen [noplot] interfaceBumpHype -factor=<num> -order=[2/4/6/8] -interp=[e/i] -curve=[3bump|afm1|flat]
#              -saveSurfaceGrids=[0|1]
# 
# examples:
#
#  --  3d afm profile 3 :  Surface from Isaac Bass
#  ogen -noplot interfaceBumpHype -interp=i -order=2 -curve=afm3d3 -factor=1 -ds0=.4 -za=-14. -zb=6
#  ogen -noplot interfaceBumpHype -interp=e -order=2 -curve=afm3d3 -factor=2 -ds0=.4 -za=-14. -zb=6
#  ogen -noplot interfaceBumpHype -interp=e -order=2 -curve=afm3d3 -factor=4 -ds0=.4 -za=-14. -zb=6
#
#  -- fourth order:
#  ogen -noplot interfaceBumpHype -interp=e -order=4 -curve=afm3d3 -factor=2 -ds0=.4 -za=-14. -zb=6
#       lambda=.355  -> .355/1.5=.237   -> 2-4pts/wave-length, 17M pts
#  ogen -noplot interfaceBumpHype -interp=e -order=4 -curve=afm3d3 -factor=4 -ds0=.4 -za=-14. -zb=6
# 
#  ogen -noplot interfaceBumpHype -interp=e -order=4 -curve=afm3d3 -factor=8 -ds0=.4 -za=-14. -zb=6 [ 132M ]
#  ogen -noplot interfaceBumpHype -interp=e -order=4 -curve=afm3d3 -factor=16 -ds0=.4 -za=-14. -zb=6
# 
# -- save surface grids for interfaceBumpNurbs.cmd: (so we can build very fine grids in parallel
#  ogen -noplot interfaceBumpHype -interp=e -order=4 -curve=afm3d3 -factor=4 -ds0=.4 -za=-14. -zb=6 -saveSurfaceGrids=1
#
# Bigger central patch: m588MidPlus
#  ogen -noplot interfaceBumpHype -interp=i -order=2 -curve=m588MidPlus -factor=1 -ds0=.4 -za=-18. -zb=8
#  ogen -noplot interfaceBumpHype -interp=i -order=4 -curve=m588MidPlus -factor=2 -ds0=.4 -za=-18. -zb=8
#  ogen -noplot interfaceBumpHype -interp=e -order=4 -curve=m588MidPlus -factor=4 -ds0=.4 -za=-18. -zb=8 -saveSurfaceGrids=1
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $extra=0; $curve="1bump"; $saveSurfaceGrids=0;
$xa=-1.; $xb=1.; $ya=-1.; $yb=1.; $za=-.5; $zb=.5; $ds0=.05; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
        "za=f"=> \$za,"zb=f"=> \$zb,"interp=s"=> \$interp,"name=s"=> \$name,"curve=s"=> \$curve,\
        "extra=i"=>\$extra,"ds0=f"=> \$ds0,"saveSurfaceGrids=i"=>\$saveSurfaceGrids );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; $extra=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; $extra=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; $extra=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $extra=$extra+$order; }
# *wdh* 090409 if( $factor > 4 ){ $extra=$extra+8; }  # make interface grids a bit wider for higher resolution cases
# 
$suffix = ".order$order"; 
$curveName = ""; 
if( $curve eq "afm1" ){ $curveName="One"; }elsif( $curve eq "flat" ){ $curveName="Flat"; }else{ $curveName=$curve; }
if( $name eq "" ){ $name = "interfaceBumpHype$curveName" . "$interp$factor" . $suffix . ".hdf";}
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
# Middle section of surface from Isaac Bass:
if( $curve eq "afm3d3" ){ $amp=0.; $afm3dSurface="afm.M588Mid.dat"; }
# Bigger middle section:
if( $curve eq "m588MidPlus" ){ $amp=0.; $afm3dSurface="afm.M588MidPlus.dat"; $cmds = "include afm3d.cmd"; }
if( $curve eq "afm3d1" || $curve eq "afm3d2" || $curve eq "afm3d3" ){ $cmds = "include afm3d.cmd"; }
$cmds 
# 
  # 
  builder
    $nr=5+$extra; 
    target grid spacing $ds $ds (tang,norm)((<0 : use default)
    add surface grid
    interfaceCurve
#
    # estimate the number of lines on the initial curve -- this should be automatic 
    $curveFactor=1.1; # increase points from a flat surface
    $nx = int( $curveFactor*($xb-$xa)/$ds +1.5);
    $ny = int( $curveFactor*($yb-$ya)/$ds +1.5);
    create volume grid...
      backward
      points on initial curve $nx, $ny
      lines to march $nr
      BC: left fix x, float y and z
      BC: right fix x, float y and z
      BC: bottom fix y, float x and z
      BC: top fix y, float x and z
      uniform dissipation 0.2
      volume smooths 500
      generate
      fourth order
      boundary conditions
        1 2 3 4 $bcInterface 0 
      share
        1 2 3 4 $ishare 0 
      name lowerInterfaceDPM
      exit
#
    create volume grid...
      forward
      points on initial curve $nx, $ny
      lines to march $nr
      BC: left fix x, float y and z
      BC: right fix x, float y and z
      BC: bottom fix y, float x and z
      BC: top fix y, float x and z
      uniform dissipation 0.2
      volume smooths 500
      generate
      fourth order
      boundary conditions
        1 2 3 4 $bcInterface 0 
      share
        1 2 3 4 $ishare 0 
      name upperInterfaceDPM
      # open graphics
      exit
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
# --------------------------------------------------------------------
# ---- Define a subroutine to convert a Mapping to a Nurbs Mapping ---
# --------------------------------------------------------------------
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
#
# End
  convertToNurbs(lowerInterfaceDPM,lowerInterface,0.);
  $commands
  convertToNurbs(upperInterfaceDPM,upperInterface,0.);
  $commands
#
# -- save Nurbs surfaces to a file
if( $curve eq afm3d3 ){ $surfGridsFileName ="afm3d3SurfaceGrids.hdf"; }\
   else{ $surfGridsFileName = $curveName . "SurfaceGrids.hdf"; }
$cmds="open a data-base\n" .\
    "$surfGridsFileName\n" .\
  "open a new file\n" .\
  "put to the data-base\n" .\
  "  lowerInterface\n" .\
  "put to the data-base\n" .\
  "  upperInterface\n" .\
  "close the data-base\n";
if( $saveSurfaceGrids eq 0 ){ $cmds="#"; }
$cmds
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
# open graphics
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
