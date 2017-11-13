# Ogen command file: Make a 3D grid for a free surface 
#
# Usage: ogen [-noplot] freeSurfaceGrid3d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> -bc=[d|p]
# 
#
# Examples:
#  ogen -noplot freeSurfaceGrid3d -interp=e -factor=1
#  ogen -noplot freeSurfaceGrid3d -interp=e -factor=2
#
# Multigrid: (probably best to only increase the number of levels slowly)
#  ogen -noplot freeSurfaceGrid3d -interp=e -factor=2 -ml=1
#
#  Parameters: 
$xa=0.; $xb=1.; $ya=0.; $yb=1.; $za=-1.; $zb=0.; 
$bc = "d"; 
$amp=.10; # amplitude of the surface bump
#---------------------------------------------------------------------------------------------
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; 
$freeSurfaceShare=100; # share value for free surface
#-----------------------------------------------------------------
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "za=f"=> \$za,"interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml, "amp=f"=>\$amp,"bc=s"=> \$bc );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix=""; 
if( $bc eq "p" ){ $suffix .= "p"; }
$suffix .= ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = "freeSurfaceGrid3d" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds0=.05;
$ds=$ds0/$factor;
# 
$dw = $order+1; $iw=$order+1; 
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$pi = 4.*atan2(1.,1.);
#
$dw = $order+1; $iw=$order+1;
# parallel ghost lines: for ogen we need at least:
# #       .5*( iw -1 )   : implicit interpolation 
# #       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; }
minimum number of distributed ghost lines
$parallelGhost
#
#
create mappings
#
 x-r 90
 set home
#
#  bump(x,y, amp,x0,y0,w0) 
#    amp : amplitude of the bump 
#    x0,y0 : center of the bump
#    w0 = width of the bump
sub bump\
{ local($x,$y,$amp,$x0,$y0,$w0)=@_; \
  $rr= (($x-$x0)/$w0)**2 + (($y-$y0)/$w0)**2 ;\
  return $amp*exp(-$rr);\
}
#
create mappings 
#
# --- generate points on the free surface ---
#
$cmd="";
$n=11; $a=$xa; $b=$xb; $h=($b-$a)/($n-1); $amp=.0; 
$x1=.5; $y1=.5; $w1=.2; 
for( $j=0; $j<$n; $j++){for( $i=0; $i<$n; $i++){ $x=$xa + $h*$i; $y=$ya + $h*$j; \
$z = $zb + bump($x,$y, $amp,$x1,$y1,$w1); \
$cmd=$cmd . "$x $y $z\n"; }}
create mappings 
 # 
nurbs (surface)
  enter points
    $degree=3; 
    $n $n $degree
    $cmd
  mappingName
    interfaceCurve
  # open graphics
exit
#
  builder
    add surface grid
      interfaceCurve
    # target grid spacing $ds $ds (tang,norm)((<0 : use default)
    create volume grid...
     BC: left fix x, float y and z
     BC: right fix x, float y and z
     BC: bottom fix y, float x and z
     BC: top fix y, float x and z
     # -- GHOST points go bad at lower-right corner, this seems to help:
     apply boundary conditions to start curve 1
     # 
     $terrainFactor=1.25; # account for extra domain size that includes the terrain
     $nx = intmg( $terrainFactor*($xb-$xa)/$ds + 1.5 );
     $ny = intmg( $terrainFactor*($yb-$ya)/$ds + 1.5 );
     # points on initial curve 183, 145
     points on initial curve $nx, $ny
     if( $bc eq "d" ){ $bcCmd ="1 2 3 4 6 0"; }else{ $bcCmd ="-1 -1 -1 -1 6 0" }; 
     boundary conditions
       $bcCmd
       # 1 2 3 4 6 0
     share
       1 2 3 4 $freeSurfaceShare 0
     # We cannot use the boundary offset to shift the ghost points
     # since the resulting boundary faces will not flat. 
     boundary offset 0 0 0 0 0 1 (l r b t b f)
     # ---New 2011/10/03
     # FIX ME FOR PERIODIC: 
     if( $bc eq "d" ){ $cmd="normal blending 7, 7, 7, 7 (lines, left,right,bottom,top)"; }else{ $cmd="#"; }
     $cmd
     # I think we need to increase the number of volume smooths as we make the grid finer
     # ---New 2011/10/03
     $volSmooths=100*$factor; 
     volume smooths $volSmooths
     ## volume smooths 200
     $linesToMarch = intmg( 9 )-1 +1;
     lines to march $linesToMarch
     # factor of .75 -- make spacing a bit finer in the normal direction
     $dist = .75*$linesToMarch*$ds; 
     distance to march $dist
     backward
     ## evaluate as nurbs 1
     generate     
     # open graphics
     mappingName freeSurface
   # pause
   exit
#
  exit
#
#  Background grid 
#
$xac=$xa; $xbc=$xb; $yac=$ya; $ybc=$yb; 
$zac=$za; 
$zbc=$zb+.25*($zb-$za);    # extend grid in z to allow for motion of the free surface
box
  set corners
    $xac $xbc $yac $ybc $zac $zbc 
  lines
    $nx = intmg( ($xbc-$xac)/$ds +1.5 ); 
    $ny = intmg( ($ybc-$yac)/$ds +1.5 ); 
    $nz = intmg( ($zbc-$zac)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    if( $bc eq "d" ){ $bcCmd ="1 2 3 4 5 0"; }else{ $bcCmd ="-1 -1 -1 -1 5 0" }; 
    $bcCmd
    # 1 2 3 4 5 0
  share
    1 2 3 4 5 0 
  mappingName
    backGround
exit
#
 exit this menu
#
generate an overlapping grid
  backGround
  freeSurface
  done choosing mappings
#
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
  # open graphics
  #
  compute overlap
  #*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
freeSurfaceGrid3d
exit
